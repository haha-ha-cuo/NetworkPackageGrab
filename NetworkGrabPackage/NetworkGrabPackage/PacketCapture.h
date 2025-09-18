#pragma once
//抓包核心类（抽象类接口）
#include <pcap.h>

class PacketCapture
{
private:

	pcap_t* handle; //抓包句柄

	char errorbuf[PCAP_ERRBUF_SIZE]; //错误信息缓冲区

public:

	PacketCapture();
	~PacketCapture();

	void startCapture(const char* deviceName);//开始抓包

	void closeCapture();//关闭抓包

	pcap_t* getHandle(); //获取抓包句柄
};

