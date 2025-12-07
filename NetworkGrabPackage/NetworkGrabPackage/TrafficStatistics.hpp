#pragma once

//流量统计和分析
#include "PacketCapture.hpp"
#include "Statistics.hpp"

#include <vector>
#include <string>
#include <cstdint>

// 单个流量记录
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

    void analyzeTraffic(); // 分析流量数据（从上一级目录 Output 读取 pcap，填充 records）
    void generateReport(); // 生成流量报告（输出每个流量包的 IP 和协议）

private:
    std::vector<TrafficRecord> records_; // 保存分析结果
};