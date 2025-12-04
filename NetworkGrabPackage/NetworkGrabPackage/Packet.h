#pragma once
#include <iostream>
#include<vector>
#include<memory>
#include<chrono>//Ê±¼ä¿â
#include<string>

using namespace std;
using namespace std::chrono;

enum class PacketType {
    ETHERNET,
    IP,
    TCP,
    UDP,
    HTTP,
    UNKNOWN
};

class Packet
{
public:
    Packet(const vector<uint8_t>& data, PacketType type) :
        raw_data(data), packet_type(type), packet_size(data.size()) {
        timestamp = system_clock::now();
    }
    virtual ~Packet() = default;

    virtual void Parse() = 0;
    virtual void display() const = 0;
    virtual string getSummary() const = 0;

    vector<uint8_t> GetRawData() const {
        return raw_data;
	}
    PacketType GetPacketType() const {
        return packet_type;
    }
    size_t GetPacketSize() const {
        return packet_size;
    }
    system_clock::time_point GetTimestamp() const {
        return timestamp;
	}

    static string typeToString(PacketType type) {
        switch (type) {
            case PacketType::ETHERNET:
				return "Ethernet";
			case PacketType::IP:
				return "IP";
			case PacketType::TCP:
				return "TCP";
			case PacketType::UDP:
				return "UDP";
			case PacketType::HTTP:
				return "HTTP";
			default:
				return "Unknown";
        }
    }

protected:
	vector<uint8_t> raw_data;
	system_clock::time_point timestamp;
	PacketType packet_type;
    size_t packet_size;
};

