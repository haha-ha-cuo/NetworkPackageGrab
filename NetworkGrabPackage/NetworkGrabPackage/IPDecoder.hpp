#pragma once

//IP协议解码
#include "ProtocolDecoder.hpp"
#include "Types.hpp"


class IPDecoder : public ProtocolDecoder
{
public:
	void packetHandle(const u_char* data_pkt) override;
	~IPDecoder() override {}

private:
	ipHeader* ih;

};