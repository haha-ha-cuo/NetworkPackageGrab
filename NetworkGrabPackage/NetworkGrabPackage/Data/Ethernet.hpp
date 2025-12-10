#pragma once

#include "Packet.hpp"
class Ethernet : public Packet
{
public:
    Ethernet(const std::vector<uint8_t>& data)
        : Packet(data, PacketType::ETHERNET), ether_type(0) {}

    void Parse() override;
    void display() const override;
    std::string getSummary() const override;
    std::string getSrcMac() const { return src_mac; }
    std::string getDestMac() const { return dst_mac; }
    uint16_t getEtherType() const { return ether_type; }

private:
    std::string src_mac;
    std::string dst_mac;
    uint16_t ether_type;
};
