#pragma once

//流量统计和分析
#include "PacketCapture.hpp"
#include "Statistics.hpp"

#include <vector>
#include <string>
#include <cstdint>


struct TrafficRecord
{
    std::string srcIP;
    std::string dstIP;
    std::uint8_t protocol;
    char ipClass;  // A, B, C, D, or E
};



class TrafficStatistics : public PacketCapture
{
public:
    TrafficStatistics();
    ~TrafficStatistics();

    // 分析网络流量，从 output.txt 中提取 IP 信息
    void analyzeTraffic();
    
    // 生成并显示流量统计报告
    void generateReport();
    
    // 统计并显示各个IP类别的总数
    void printIPClassStatistics() const;

private:
    char getIPClass(const std::string& ip) const;
    std::vector<TrafficRecord> records_; // 保存分析结果
};
