#pragma once
//包解析器
#include "PacketCapture.h"
class PacketParser: public PacketCapture
{
private:

	struct pcap_pkthdr* header;//包文头指针

	const u_char* pktData;//包数据指针

	int result;//抓包结果

public:

	PacketParser();
	~PacketParser();

	void startParse(const char* deviceName); //开始解析
};