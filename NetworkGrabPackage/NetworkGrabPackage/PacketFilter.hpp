#pragma once
//包过滤器
#include "NetworkInterface.hpp"
#include <pcap.h>
#include <string>


using namespace std;

class PacketFilter
{
private:

	struct bpf_program* fcode;

	NetworkInterface networkInterface;

public:
	PacketFilter();
	~PacketFilter();

	//设置过滤器(name:ip ,tcp,arp,ethernet)
	void setFilter(const char* filterExp, pcap_t* handle, const char* name);
};