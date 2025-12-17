#pragma once

#include "Types.hpp"
#include <iostream>
#include <vector>
#include <memory>
#include <chrono>
#include <string>

#include <mutex>

class Packet
{
protected:
    std::vector<uint8_t> raw_data;                   // 原始数据
    std::chrono::system_clock::time_point timestamp; // 时间戳
    PacketType packet_type;                          // 类型
    size_t packet_size;                              // 大小

public:
    Packet(const std::vector<uint8_t> &data, PacketType type) : raw_data(data), packet_type(type), packet_size(data.size())
    {
        timestamp = std::chrono::system_clock::now();
    }
    virtual ~Packet() = default;

    virtual void Parse() = 0;
    virtual void display() const = 0;
    virtual std::string getSummary() const = 0;

    std::vector<uint8_t> GetRawData() const { return raw_data; }
    PacketType GetPacketType() const { return packet_type; }
    size_t GetPacketSize() const { return packet_size; }
    std::chrono::system_clock::time_point GetTimestamp() const { return timestamp; }
};
