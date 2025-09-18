#pragma once
//抓包核心类（抽象类接口）
#include <pcap.h>
#include<Map>

using namespace std;

class PacketCapture
{
private:

	struct pcap_pkthdr* header;//包文头指针

	const u_char* pktData;//包数据指针

	int result;//抓包结果

	pcap_t* handle; //抓包句柄

	char errorbuf[PCAP_ERRBUF_SIZE]; //错误信息缓冲区

	map <const time_t, const u_char*> packetMap; //使用包ID作为键，包数据作为值

	int Counter;
public:

	PacketCapture();
	~PacketCapture();

	void startCapture(const char* deviceName);//开始抓包

	void closeCapture();//关闭抓包

	map <const time_t, const u_char*> getPacketMap();//获取包数据
};

