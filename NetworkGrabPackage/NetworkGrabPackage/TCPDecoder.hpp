#pragma once

//TCP协议解码
#include "ProtocolDecoder.hpp"
#include "Types.hpp"


class TCPDecoder : public ProtocolDecoder
{
public:
	void packetHandle(const u_char* data_pkt) override;
	~TCPDecoder() override {};
	TCPDecoder(const u_char* data_pkt);
	void TcpPort(const u_char* data_pkt, pcap_pkthdr* header, char * timestr) override;

	u_short getSourcePort() override ;

	u_short getDestinationPort() override ;

private:
	tcpHeader* tH;

};