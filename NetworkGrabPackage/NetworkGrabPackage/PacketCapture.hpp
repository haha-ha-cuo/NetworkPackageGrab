#pragma once
//抓包核心类
#include "PacketFilter.hpp"
#include <unordered_map>
#include <pcap.h>
#include "ProtocolDecoder.hpp"

using namespace std;

class PacketCapture
{
private:

	struct pcap_pkthdr* header;//包文头指针

	const u_char* pktData;//包数据指针

	int result;//抓包结果

	pcap_t* handle; //抓包句柄

	char errorbuf[PCAP_ERRBUF_SIZE]; //错误信息缓冲区

	unordered_map <time_t, const u_char*> packetMap; //使用包ID作为键，包数据作为值

	int Counter;

	PacketFilter packetFilter;//包过滤器对象

	ProtocolDecoder* protocolDecoderEthernet;//协议解码器对象

	ProtocolDecoder* protocolDecoderIPV4;//协议解码器对象

	ProtocolDecoder* protocolDecoderTCP;//协议解码器对象

	pcap_dumper_t* dumper;
public:

	PacketCapture();
	~PacketCapture();

	void startCapture(const char* deviceName, const char* port);//开始抓包

	void closeCapture();//关闭抓包

	unordered_map <time_t, const u_char*> getPacketMap();//获取包数据
};

