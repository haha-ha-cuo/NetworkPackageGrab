#pragma once

#include "Pages.hpp"
#include "TrafficStatistics.hpp"

class StatisticsPage :public Pages
{
private:
	TrafficStatistics ts;
public:
	Pages* display() override;
};