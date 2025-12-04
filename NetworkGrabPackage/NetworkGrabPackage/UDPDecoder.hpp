#pragma once

//UDP协议解码
#include "ProtocolDecoder.hpp"
#include "Types.hpp"


class UDPDecoder : public ProtocolDecoder
{
public:
	void packetHandle(const u_char* data_pkt) override;
	~UDPDecoder() override {};

private:
	udpHeader* tH;

};