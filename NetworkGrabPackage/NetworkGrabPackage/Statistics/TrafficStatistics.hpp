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
};



class TrafficStatistics : public PacketCapture
{
public:
    TrafficStatistics();
    ~TrafficStatistics();

    void analyzeTraffic();
    void generateReport();

private:
    std::vector<TrafficRecord> records_; // 保存分析结果
};
