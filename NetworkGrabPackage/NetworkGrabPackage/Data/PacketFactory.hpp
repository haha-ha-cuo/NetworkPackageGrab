#pragma once

#include "Packet.hpp"
#include "Ethernet.hpp"
#include "IP.hpp"
#include "TCP.hpp"
#include "UDP.hpp"

class PacketFactory {
public:
    static std::unique_ptr<Packet> createPacket(const std::vector<uint8_t>& data);

private:
    static PacketType identifyPacketType(const std::vector<uint8_t> &data);
};
