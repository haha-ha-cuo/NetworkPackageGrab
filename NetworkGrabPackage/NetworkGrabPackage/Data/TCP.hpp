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
    TCP(const std::vector<uint8_t> &data);

    void Parse() override;
    void display() const override;
    void showSummary(size_t index, tm localTm) const override;

    uint16_t getSrcPort() const;
    uint16_t getDstPort() const;
    uint32_t getSequenceNumber() const;
    uint32_t getAcknowledgmentNumber() const;
    uint8_t getDataOffset() const;
    std::vector<uint8_t> getPayload() const;
};
