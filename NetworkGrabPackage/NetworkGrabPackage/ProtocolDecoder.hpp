#pragma once
//协议解码器基类
#include<pcap.h>
using namespace std;
class ProtocolDecoder
{
public:

	virtual void packetHandle(const u_char* data_pkt) = 0; //纯虚函数，必须由派生类实现

	virtual ~ProtocolDecoder() {} //虚析构函数，确保派生类的析构函数被调用


};