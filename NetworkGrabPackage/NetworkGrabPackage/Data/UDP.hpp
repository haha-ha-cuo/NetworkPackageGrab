#pragma once

#include "Packet.hpp"
class UDP :
    public Packet
{
    public:
    UDP(const std::vector<uint8_t>& data)
        : Packet(data, PacketType::UDP), src_port(0), dst_port(0),
        length(0){}
    void Parse() override;
    void display() const override;
    std::string getSummary() const override;
    uint16_t getSrcPort() const {
        return src_port;
    }
    uint16_t getDstPort() const {
        return dst_port;
    }
    uint16_t getLength() const {
        return length;
    }
    private:
    uint16_t src_port;
    uint16_t dst_port;
    uint16_t length;
};
