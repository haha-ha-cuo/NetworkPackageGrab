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
    template <typename Func>
    void forEach(Func &&f)
    {
        std::lock_guard<std::mutex> lg(packets_mutex);
        for (auto &p : packets)
        {
            f(*p);
        }
    }

    template <typename Func>
    void forEach(Func &&f) const
    {
        std::lock_guard<std::mutex> lg(packets_mutex);
        for (const auto &p : packets)
        {
            f(*p);
        }
    }
    void ParseAll();
    void displayAll() const;

    const std::vector<std::unique_ptr<Packet>> &GetPackets() const;
    size_t GetPacketCount() const;
};
