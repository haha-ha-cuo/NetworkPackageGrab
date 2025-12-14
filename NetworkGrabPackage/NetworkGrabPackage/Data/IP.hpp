#pragma once

#include "Packet.hpp"

class IP : public Packet
{
private:
    std::string src_ip;
    std::string dst_ip;
    uint8_t protocol;
    uint16_t total_length;
    uint8_t ttl;

public:
    IP(const std::vector<uint8_t> &data) : Packet(data, PacketType::IP), protocol(0), total_length(0), ttl(0) {}

    void Parse() override;
    void display() const override;
    std::string getSummary() const override;

    std::string getSrcIP() const { return src_ip; }
    std::string getDestIP() const { return dst_ip; }
    uint8_t getProtocol() const { return protocol; }
    uint16_t getTotalLength() const { return total_length; }
    uint8_t getTTL() const { return ttl; }
};
