#pragma once

//流量统计和分析
#include "PacketCapture.hpp"
#include "Statistics.hpp"


class TrafficStatistics : public PacketCapture
{
public:
    TrafficStatistics();
    ~TrafficStatistics();

    void analyzeTraffic();
    void generateReport();
};
