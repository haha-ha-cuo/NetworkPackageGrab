#pragma once

#include <pcap.h>
#include <string>

enum class PacketDirection
{
    Incoming,
    Outgoing,
    Both
};

enum class StorageType
{
    Memory,
    File
};

enum class NetworkInterfaceType
{
    Ethernet,
    WiFi,
    Loopback,
    Other
};

enum class PacketType
{
    ETHERNET,
    IP,
    TCP,
    UDP,
    ARP,
    UNKNOWN
};

inline std::string PacketTypeToString(PacketType type)
{
    switch (type)
    {
    case PacketType::ETHERNET:
        return "Ethernet";
    case PacketType::IP:
        return "IP";
    case PacketType::TCP:
        return "TCP";
    case PacketType::UDP:
        return "UDP";
    default:
        return "Unknown";
    }
}