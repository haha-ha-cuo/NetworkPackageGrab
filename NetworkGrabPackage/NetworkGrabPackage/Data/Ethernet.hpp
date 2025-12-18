#pragma once

#include "Packet.hpp"
class Ethernet : public Packet
{
private:
    std::string src_mac; // 源MAC地址（6字节） 
    std::string dst_mac; // 目的MAC地址（6字节） 
    uint16_t ether_type; // 以太网类型（2字节）

public:
    Ethernet(const std::vector<uint8_t> &data);

    void Parse() override;
    void display() const override;
    void showSummary(size_t index, tm localTm) const override;
    std::string getSrcMac();
    std::string getDestMac();
    uint16_t getEtherType();
};
