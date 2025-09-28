#pragma once
//IP协议解码

#include "ProtocolDecoder.hpp"

class IPDecoder : public ProtocolDecoder
{
public:
	void paketHandle(u_char* param, const struct pcap_pkthdr* header, const u_char* pkt_data) override;
	~IPDecoder() override {}
};