#pragma once
//包解析器
#include "PacketCapture.hpp"
class PacketParser : public PacketCapture
{
private:



public:

	PacketParser();
	~PacketParser();

	void startParse(const char* deviceName); //开始解析
};