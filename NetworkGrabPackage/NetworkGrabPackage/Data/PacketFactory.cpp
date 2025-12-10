#include "PacketFactory.hpp"

using namespace std;
namespace PacketFactory
{
    PacketType identifyPacketType(const vector<uint8_t> &data)
    {
        if (data.size() < 14)
            return PacketType::UNKNOWN;
        uint16_t ether_type = (static_cast<uint16_t>(data[12]) << 8) | static_cast<uint16_t>(data[13]);
        if (ether_type == 0x0800)
        {
            if (data.size() >= 34)
            {
                uint8_t ip_protocol = data[23];
                if (ip_protocol == 6)
                {
                    return PacketType::TCP;
                }
                else if (ip_protocol == 17)
                {
                    return PacketType::UDP;
                }
            }
            return PacketType::IP;
        }
        if (data.size() >= 20)
        {
            uint8_t version = (data[0] >> 4) & 0x0F;
            if (version == 4 || version == 6)
            {
                uint8_t ip_protocol = data[9];
                if (ip_protocol == 6)
                {
                    return PacketType::TCP;
                }
                else if (ip_protocol == 17)
                {
                    return PacketType::UDP;
                }
                return PacketType::IP;
            }
        }
        if (data.size() >= 14)
            return PacketType::ETHERNET;
        return PacketType::UNKNOWN;
    }
    unique_ptr<Packet> createPacket(const std::vector<uint8_t> &data)
    {
        PacketType type = identifyPacketType(data);
        switch (type)
        {
        case PacketType::ETHERNET:
            return make_unique<Ethernet>(data);
        case PacketType::IP:
        {
            if (data.size() > 14)
            {
                vector<uint8_t> ip(data.begin() + 14, data.end());
                return make_unique<IP>(ip);
            }
            return nullptr;
        }
        case PacketType::TCP:
        {
            if (data.size() > 34)
            {
                size_t ipOffset = 14;
                uint8_t ihl = (data[ipOffset] & 0x0F) * 4;
                size_t tcpOffset = ipOffset + ihl;
                if (data.size() > tcpOffset)
                {
                    vector<uint8_t> tcp(data.begin() + tcpOffset, data.end());
                    return make_unique<TCP>(tcp);
                }
            }
            return nullptr;
        }
        case PacketType::UDP:
        {
            if (data.size() > 34)
            {
                size_t ipOffset = 14;
                uint8_t ihl = (data[ipOffset] & 0x0F) * 4;
                size_t udpOffset = ipOffset + ihl;
                if (data.size() > udpOffset)
                {
                    vector<uint8_t> udp(data.begin() + udpOffset, data.end());
                    return make_unique<UDP>(udp);
                }
            }
            return nullptr;
        }
        default:
            return nullptr;
        }
    }
}
