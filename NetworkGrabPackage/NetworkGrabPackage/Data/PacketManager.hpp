#pragma once

#include "Packet.hpp"
#include <mutex>

class PacketManager
{
private:
    std::vector<std::unique_ptr<Packet>> packets;
    mutable std::mutex packets_mutex; // 新增：保护 packets

public:
    PacketManager() = default;
    ~PacketManager() = default;
    PacketManager(const PacketManager &) = delete;
    PacketManager &operator=(const PacketManager &) = delete;
    PacketManager(PacketManager &&) = default;
    PacketManager &operator=(PacketManager &&) = default;
    void AddPacket(std::unique_ptr<Packet> packet);
    void ParseAll();
    void displayAll() const;
    void displaySummaries() const;
    const std::vector<std::unique_ptr<Packet>> &GetPackets() const
    {
        std::lock_guard<std::mutex> lg(packets_mutex);
        return packets;
    }
   
    size_t GetPacketCount() const
    {
        return packets.size();
    }
};
