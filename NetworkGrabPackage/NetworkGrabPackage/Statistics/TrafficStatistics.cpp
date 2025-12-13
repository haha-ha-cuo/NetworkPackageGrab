#include "TrafficStatistics.hpp"

#include <pcap.h>
#include <filesystem>
#include <iostream>

namespace fs = std::filesystem;

namespace
{
    // 简单把协议号转成可读字符串
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

void TrafficStatistics::analyzeTraffic()
{
    records_.clear();

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

// 将 analyzeTraffic 收集到的记录逐条输出：源 IP、目的 IP、协议
void TrafficStatistics::generateReport()
{
    if (records_.empty())
    {
        std::cout << "[Info] No traffic records. Please call analyzeTraffic() first." << std::endl;
        return;
    }

    std::cout << "================ Traffic Report ================" << std::endl;
    std::cout << "Total records: " << records_.size() << std::endl;
    std::cout << "Idx\tSrc IP\t\tDest IP\t\tProtocol" << std::endl;
    std::cout << "------------------------------------------------" << std::endl;

    std::size_t index = 0;
    for (const auto& rec : records_)
    {
        std::cout << index++ << "\t"
            << rec.srcIP << "\t"
            << rec.dstIP << "\t"
            << protocolToString(rec.protocol)
            << std::endl;
    }

    std::cout << "================ End of Report =================" << std::endl;
}