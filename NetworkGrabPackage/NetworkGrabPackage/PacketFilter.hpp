#pragma once

//包过滤器
#include "NetworkInterface.hpp"
#include <pcap.h>
#include <string>


class PacketFilter
{
private:
	struct bpf_program* fcode;
	NetworkInterface networkInterface;

public:
	PacketFilter();
	~PacketFilter();
	
	void setFilter(const char* filterExp, pcap_t* handle, const char* name);	//设置过滤器(name:ip ,tcp,arp,ethernet)
};