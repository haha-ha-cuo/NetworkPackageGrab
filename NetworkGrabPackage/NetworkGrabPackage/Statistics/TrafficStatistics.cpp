#include "TrafficStatistics.hpp"

#include <filesystem>
#include <fstream>
#include <iostream>
#include <map>
#include <sstream>

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
    records_.clear();

    std::string path = "../Output/output.txt";
    if (!fs::exists(path))
    {
        if (fs::exists("Output/output.txt"))
            path = "Output/output.txt";
    }

    std::ifstream in(path, std::ios::in | std::ios::binary);
    if (!in.is_open())
    {
        std::cerr << "[Error] output.txt not found: " << path << std::endl;
        return;
    }

    std::string line;
    while (std::getline(in, line))
    {
        if (line.empty())
            continue;
        if (!line.empty() && line[0] == '#')
            continue;

        std::vector<std::string> cols;
        cols.reserve(8);

        std::size_t start = 0;
        while (true)
        {
            std::size_t pos = line.find('\t', start);
            if (pos == std::string::npos)
            {
                cols.push_back(line.substr(start));
                break;
            }
            cols.push_back(line.substr(start, pos - start));
            start = pos + 1;
        }

        if (cols.size() < 4)
            continue;

        const std::string &proto = cols[1];
        const std::string &src = cols[2];
        const std::string &dst = cols[3];

        auto stripPort = [](const std::string &s) -> std::string {
            std::size_t pos = s.find(':');
            if (pos == std::string::npos)
                return s;
            return s.substr(0, pos);
        };

        TrafficRecord rec;
        rec.srcIP = stripPort(src);
        rec.dstIP = stripPort(dst);

        if (proto == "ICMP")
            rec.protocol = 1;
        else if (proto == "TCP")
            rec.protocol = 6;
        else if (proto == "UDP")
            rec.protocol = 17;
        else if (proto.rfind("IP(", 0) == 0)
        {
            std::size_t l = proto.find('(');
            std::size_t r = proto.find(')', l + 1);
            if (l != std::string::npos && r != std::string::npos && r > l + 1)
            {
                try
                {
                    int v = std::stoi(proto.substr(l + 1, r - (l + 1)));
                    if (v >= 0 && v <= 255)
                        rec.protocol = static_cast<std::uint8_t>(v);
                    else
                        rec.protocol = 0;
                }
                catch (...)
                {
                    rec.protocol = 0;
                }
            }
            else
            {
                rec.protocol = 0;
            }
        }
        else
        {
            rec.protocol = 0;
        }

        rec.ipClass = getIPClass(rec.dstIP);

        if (!rec.srcIP.empty() && rec.srcIP != "-" && !rec.dstIP.empty() && rec.dstIP != "-")
            records_.push_back(rec);
    }

    std::cout << "[Info] Traffic analysis completed. Parsed records: " << records_.size() << std::endl;
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
