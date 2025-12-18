#pragma once

#include "Packet.hpp"
class UDP : public Packet
{
private:
    uint16_t src_port;
    uint16_t dst_port;
    uint16_t length;

public:
    UDP(const std::vector<uint8_t> &data);
    void Parse() override;
    void display() const override;
    void showSummary(size_t index, tm localTm) const override;
    uint16_t getSrcPort() const;
    uint16_t getDstPort() const;
    uint16_t getLength() const;
};
