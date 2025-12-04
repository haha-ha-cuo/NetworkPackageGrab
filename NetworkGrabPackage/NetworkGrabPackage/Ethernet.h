#pragma once
#include "Packet.h"
class Ethernet : public Packet
{
public:
	Ethernet(const vector<uint8_t>& data) :
		Packet(data, PacketType::ETHERNET) {}

	void Parse() override;
	void display() const override;
	string getSummary() const override;

	string getSrcMac() const {
		return src_mac;
	}
	string getDestMac() const {
		return dst_mac;
	}
	uint16_t getEtherType() const {
		return ether_type;
	}

private:
	string src_mac;
	string dst_mac;
	uint16_t ether_type;
};

