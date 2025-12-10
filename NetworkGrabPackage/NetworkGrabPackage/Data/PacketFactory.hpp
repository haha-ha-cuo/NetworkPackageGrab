#pragma once

#include "Packet.hpp"
#include "Ethernet.hpp"
#include "IP.hpp"
#include "TCP.hpp"
#include "UDP.hpp"

namespace PacketFactory
{
    std::unique_ptr<Packet> createPacket(const std::vector<uint8_t> &data);
    PacketType identifyPacketType(const std::vector<uint8_t> &data);
}
