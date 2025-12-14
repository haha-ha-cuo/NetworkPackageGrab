#include "TrafficStatistics.hpp"

#include <pcap.h>
#include <filesystem>
#include <iostream>
#include <map>

namespace fs = std::filesystem;

namespace
{
    /**
     * @brief 将协议号转换为可读字符串
     * @param proto 协议号
     * @return 协议名称字符串
     */
    std::string protocolToString(std::uint8_t proto)
    {
        switch (proto)
        {
        case 1:
            return "ICMP(1)";
        case 6:
            return "TCP(6)";
        case 17:
            return "UDP(17)";
        default:
            return "PROTO(" + std::to_string(static_cast<int>(proto)) + ")";
        }
    }
}

TrafficStatistics::TrafficStatistics()
{
}

TrafficStatistics::~TrafficStatistics()
{
}

/**
 * @brief 获取IP地址的类别
 * @param ip IP地址字符串
 * @return 返回IP类别字符 (A-E, L表示回环地址)
 */
char TrafficStatistics::getIPClass(const std::string& ip) const
{
    size_t firstDot = ip.find('.');
    if (firstDot == std::string::npos) return 'E'; // 无效的IP格式
    
    try {
        int firstOctet = std::stoi(ip.substr(0, firstDot));
        
        if (firstOctet >= 1 && firstOctet <= 126) return 'A';     // A类地址: 1.0.0.0 - 126.255.255.255
        if (firstOctet == 127) return 'L';                       // 回环地址: 127.0.0.0 - 127.255.255.255
        if (firstOctet >= 128 && firstOctet <= 191) return 'B';  // B类地址: 128.0.0.0 - 191.255.255.255
        if (firstOctet >= 192 && firstOctet <= 223) return 'C';  // C类地址: 192.0.0.0 - 223.255.255.255
        if (firstOctet >= 224 && firstOctet <= 239) return 'D';  // D类地址(组播): 224.0.0.0 - 239.255.255.255
        if (firstOctet >= 240 && firstOctet <= 255) return 'E';  // E类地址(保留): 240.0.0.0 - 255.255.255.255
    }
    catch (...) {
        // 解析错误处理
    }
    
    return 'E'; // 默认返回E类地址
}

/**
 * @brief 分析网络流量数据
 * 从Output目录下的所有pcap文件中读取网络数据包，并解析IP信息
 */
void TrafficStatistics::analyzeTraffic()
{
    records_.clear(); // 清空现有记录

    try
    {
        // 以当前源文件为基准，找到上一级目录的 Output 目录
        fs::path currentFile = __FILE__;
        fs::path parentDir = currentFile.parent_path().parent_path().parent_path();
        fs::path outputDir = parentDir / "Output";

        if (!fs::exists(outputDir))
        {
            std::cerr << "[Error] Output folder does not exist: " << outputDir << std::endl;
            return;
        }
        if (!fs::is_directory(outputDir))
        {
            std::cerr << "[Error] Output path is not a directory: " << outputDir << std::endl;
            return;
        }

        std::cout << "[Info] Analyze pcap files in: " << outputDir << std::endl;

        char errbuf[PCAP_ERRBUF_SIZE] = { 0 };

        // 遍历 Output 目录下的所有文件
        for (const auto& entry : fs::directory_iterator(outputDir))
        {
            if (!entry.is_regular_file())
                continue;

            if (entry.path().extension() != ".pcap")
                continue;

            std::string filename = entry.path().string();
            std::cout << "[Info] Open pcap file: " << filename << std::endl;

            pcap_t* handle = pcap_open_offline(filename.c_str(), errbuf);
            if (!handle)
            {
                std::cerr << "[Error] Failed to open pcap file: " << filename
                    << " , reason: " << errbuf << std::endl;
                continue;
            }

            struct pcap_pkthdr* header = nullptr;
            const u_char* data = nullptr;
            int res = 0;

            while ((res = pcap_next_ex(handle, &header, &data)) >= 0)
            {
                if (res == 0)
                {
                    // 读取超时（离线文件一般很少遇到），跳过
                    continue;
                }

                // 至少要有以太网头(14字节) + 最小IP头(20字节)
                if (!header || header->caplen < 14 + 20)
                {
                    continue;
                }

                // 简单认为是 Ethernet + IPv4，IP 头从 14 字节偏移处开始
                const u_char* ipData = data + 14;
                std::size_t   ipLen = header->caplen - 14;

                std::vector<std::uint8_t> ipVector(ipData, ipData + ipLen);

                try
                {
                    IP ipPacket(ipVector);
                    ipPacket.Parse();

                    TrafficRecord rec;
                    rec.srcIP = ipPacket.getSrcIP();
                    rec.dstIP = ipPacket.getDestIP();
                    rec.protocol = ipPacket.getProtocol();
                    rec.ipClass = getIPClass(rec.dstIP);

                    records_.push_back(rec);
                }
                catch (const std::exception& e)
                {
                    // 不是 IPv4 包或解析失败，直接忽略
                    // std::cerr << "[Warn] IP parse error: " << e.what() << std::endl;
                    continue;
                }
            }

            pcap_close(handle);
        }

        std::cout << "[Info] Traffic analysis completed. Parsed IP packets: "
            << records_.size() << std::endl;
    }
    catch (const std::exception& e)
    {
        std::cerr << "[Error] analyzeTraffic exception: " << e.what() << std::endl;
    }
}

/**
 * @brief 生成并显示流量统计报告
 * 显示每条流量的源IP、目的IP、目的IP类别和协议信息
 */
void TrafficStatistics::generateReport()
{
    if (records_.empty())
    {
        std::cout << "[Info] No traffic records found. Please call analyzeTraffic() first." << std::endl;
        return;
    }

    std::cout << "=================== Traffic Statistics Report =================" << std::endl;
    std::cout << "Total records: " << records_.size() << std::endl;
    std::cout << "ID\tSource IP\tDestination IP\tDest IP Class\tProtocol" << std::endl;
    std::cout << "---------------------------------------------------------------" << std::endl;

    std::size_t index = 0;
    for (const auto& rec : records_)
    {
        std::cout << index++ << "\t"
            << rec.srcIP << (rec.srcIP.length() < 8 ? "\t" : "") << "\t"
            << rec.dstIP << (rec.dstIP.length() < 8 ? "\t" : "") << "\t\t"
            << rec.ipClass << "\t"
            << protocolToString(rec.protocol)
            << std::endl;
    }

    // 显示IP类别统计信息
    printIPClassStatistics();
    
    std::cout << "======================== End of Report ========================" << std::endl;
}

/**
 * @brief 统计并显示各个IP类别的数量
 */
void TrafficStatistics::printIPClassStatistics() const
{
    std::map<char, int> classCounts = {
        {'A', 0}, {'B', 0}, {'C', 0}, {'D', 0}, {'E', 0}, {'L', 0}
    };

    // 统计每个类别的数量
    for (const auto& rec : records_) {
        classCounts[rec.ipClass]++;
    }

    // 显示统计结果
    std::cout << "\n============== IP Address Class Statistics ==============" << std::endl;
    std::cout << "Class A addresses: " << classCounts['A'] << std::endl;
    std::cout << "Class B addresses: " << classCounts['B'] << std::endl;
    std::cout << "Class C addresses: " << classCounts['C'] << std::endl;
    std::cout << "Class D (Multicast) addresses: " << classCounts['D'] << std::endl;
    std::cout << "Class E (Reserved) addresses: " << classCounts['E'] << std::endl;
    std::cout << "Loopback addresses: " << classCounts['L'] << std::endl;
}

// 将 analyzeTraffic 收集到的记录逐条输出：源 IP、目的 IP、协议
