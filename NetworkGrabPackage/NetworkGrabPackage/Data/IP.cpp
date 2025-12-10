#include "IP.hpp"

void IP::Parse() {
    if (raw_data.size() < 20) {
        throw::std::runtime_error("Data too short to be an IP packet");
    }

    uint8_t version = (raw_data[0] >> 4) & 0x0F;
    if (version != 4) {
        throw::std::runtime_error("Not an IPv4 packet");
    }

    total_length = (static_cast<uint16_t>(raw_data[2]) << 8) | static_cast<uint16_t>(raw_data[3]);

    ttl = raw_data[8];

    protocol = raw_data[9];

    src_ip = std::to_string(static_cast<int>(raw_data[12])) + "." +
        std::to_string(static_cast<int>(raw_data[13])) + "." +
        std::to_string(static_cast<int>(raw_data[14])) + "." +
        std::to_string(static_cast<int>(raw_data[15]));

    dst_ip = std::to_string(static_cast<int>(raw_data[16])) + "." +
        std::to_string(static_cast<int>(raw_data[17])) + "." +
        std::to_string(static_cast<int>(raw_data[18])) + "." +
        std::to_string(static_cast<int>(raw_data[19]));
}

void IP::display() const {
    std::cout << "IP Packet:" << std::endl;
    std::cout << "  Source IP: " << src_ip << std::endl;
    std::cout << "  Destination IP: " << dst_ip << std::endl;
    std::cout << "  Protocol: " << static_cast<int>(protocol) << std::endl;
    std::cout << "  Total Length: " << total_length << " bytes" << std::endl;
    std::cout << "  TTL: " << static_cast<int>(ttl) << std::endl;
    std::cout << "Pakcet Size: " << GetPacketSize() << " bytes" << std::endl;
}

std::string IP::getSummary() const{
    return "IP: " + src_ip + " -> " + dst_ip + " Proto:" + std::to_string(protocol);
}
