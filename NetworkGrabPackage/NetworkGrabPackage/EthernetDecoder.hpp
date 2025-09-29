#pragma once
//以太网协议解码
#include "ProtocolDecoder.hpp"
#include "Types.hpp"

class EthernetDecoder : public ProtocolDecoder
{
public:
	void packetHandle(const u_char* data_pkt) override;
	~EthernetDecoder() override {}
private:
	etherHeader* eth;
};