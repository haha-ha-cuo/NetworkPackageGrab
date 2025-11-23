#pragma once
//TCP协议解码
#include "ProtocolDecoder.hpp"
#include "Types.hpp"

using namespace std;

class TCPDecoder : public ProtocolDecoder
{
public:
	void packetHandle(const u_char* data_pkt) override;
	~TCPDecoder() override {};

	void TcpPort(const u_char* data_pkt, pcap_pkthdr* header, char * timestr) override;
private:
	tcpHeader* tH;
};