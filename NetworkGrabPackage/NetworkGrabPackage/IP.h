#pragma once

#include "Packet.h"


class IP : public Packet
{
public:
	IP(const vector<uint8_t>& data):
		Packet(data, PacketType::IP),protocol(0), total_length(0), ttl(0){}

	void Parse() override;
	void display() const override;
	string getSummary() const override;

	string getSrcIP() const { return src_ip; }
	string getDestIP() const { return dst_ip; }
	uint8_t getProtocol() const { return protocol; }
	uint16_t getTotalLength() const { return total_length; }
	uint8_t getTTL() const { return ttl; }

private:
	string src_ip;
	string dst_ip;
	uint8_t protocol;
	uint16_t total_length;
	uint8_t ttl;

};

