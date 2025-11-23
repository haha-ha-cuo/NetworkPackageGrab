#pragma once
//流量统计和分析
#include "PacketCapture.hpp"

class TrafficStatistics : public PacketCapture
{
	public:
	TrafficStatistics();
	~TrafficStatistics();
	void analyzeTraffic(); // 分析流量数据
	void generateReport();  // 生成流量报告
};