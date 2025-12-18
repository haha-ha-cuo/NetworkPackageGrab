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
    IP(const std::vector<uint8_t> &data);
    IP(const IP &other);
    void Parse() override;
    void display() const override;
    void showSummary(size_t index, tm localTm) const override;

    std::string getSrcIP() const;
    std::string getDestIP() const;
    uint8_t getProtocol() const;
    uint16_t getTotalLength() const;
    uint8_t getTTL() const;
};
