#pragma once

#include "Packet.hpp"

class TCP : public Packet
{
private:
    uint16_t src_port;
    uint16_t dst_port;
    uint32_t sequence_number;
    uint32_t acknowledgment_number;
    uint8_t data_offset;
    std::vector<uint8_t> payload;

public:
    TCP(const std::vector<uint8_t> &data) : Packet(data, PacketType::TCP), src_port(0), dst_port(0),
                                            sequence_number(0), acknowledgment_number(0), data_offset(0) {}

    void Parse() override;
    void display() const override;
    std::string getSummary() const override;

    uint16_t getSrcPort() const
    {
        return src_port;
    }
    uint16_t getDstPort() const
    {
        return dst_port;
    }
    uint32_t getSequenceNumber() const
    {
        return sequence_number;
    }
    uint32_t getAcknowledgmentNumber() const
    {
        return acknowledgment_number;
    }
    uint8_t getDataOffset() const
    {
        return data_offset;
    }
    std::vector<uint8_t> getPayload() const
    {
        return payload;
    }
};
