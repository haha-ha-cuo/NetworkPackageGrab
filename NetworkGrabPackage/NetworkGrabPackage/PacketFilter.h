#pragma once
//包过滤器
#include<string>
#include<pcap.h>


using namespace std;

class PacketFilter
{
private:

	struct bpf_program* fcode;

public:
	PacketFilter();
	~PacketFilter();

	void setFilter(const char* filterExp,pcap_t* handle);//设置过滤器(name:ip ,tcp,arp,ethernet)

};