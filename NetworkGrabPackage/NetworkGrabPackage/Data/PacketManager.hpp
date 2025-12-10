#pragma once

#include "Packet.hpp"


class PacketManager
{
public:
    PacketManager() = default;
    ~PacketManager() = default;
    PacketManager(const PacketManager&) = delete;
    PacketManager& operator=(const PacketManager&) = delete;
    PacketManager(PacketManager&&) = default;
    PacketManager& operator=(PacketManager&&) = default;
    void AddPacket(std::unique_ptr<Packet> packet);
    void ParseAll();
    void displayAll() const;
    void displaySummaries() const;
    const std::vector<std::unique_ptr<Packet>>& GetPackets() const {
        return packets;
    }
    size_t GetPacketCount() const {
        return packets.size();
    }

private:
    std::vector<std::unique_ptr<Packet>> packets;

};
