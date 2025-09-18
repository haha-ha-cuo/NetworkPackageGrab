#include "PacketParser.h"
#include<iostream>
#include<time.h>

using namespace std;

PacketParser::PacketParser()
{
}

PacketParser::~PacketParser()
{
}

void PacketParser::startParse(const char* deviceName)
{
	startCapture(deviceName);                                              //调用基类的抓包方法
	pcap_t* handle = getHandle();                                          //获取抓包句柄
	if (handle == nullptr) {
		return;                                                            //如果句柄无效，直接返回
	}
	while ((result = pcap_next_ex(handle, &header, &pktData)) >= 0) {
		if (result == 0) {
			continue;     
		}
		// 打印时间戳和长度
		cout << "[Info]Packet captured." << endl;
		const time_t ts = header->ts.tv_sec;
		struct tm tmDest;
        errno_t erro = localtime_s( &tmDest , &ts);
		char timestr[16];
		strftime(timestr, sizeof(timestr), "%H:%M:%S", &tmDest);
		printf("[Info]时间: %s.%06ld 长度: %d 字节\n", timestr, header->ts.tv_usec, header->len);
	}
	if (result == -1) {
		cerr << "[Error]Error reading the packets: " << pcap_geterr(handle) << endl;
	}
	closeCapture(); //关闭抓包
}