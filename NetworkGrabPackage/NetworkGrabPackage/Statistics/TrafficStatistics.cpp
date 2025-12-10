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

TrafficStatistics::~TrafficStatistics()
{
}
