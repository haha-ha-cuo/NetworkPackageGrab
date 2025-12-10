#include "UDP.hpp"
void UDP::Parse() {
    if(raw_data.size() < 8) {
        throw std::runtime_error("Data too short to be a UDP datagram");
    }
    src_port = (static_cast<uint16_t>(raw_data[0]) << 8) | static_cast<uint16_t>(raw_data[1]);

    dst_port = (static_cast<uint16_t>(raw_data[2]) << 8) | static_cast<uint16_t>(raw_data[3]);

    length = (static_cast<uint16_t>(raw_data[4]) << 8) | static_cast<uint16_t>(raw_data[5]);
}

void UDP::display() const {
    std::cout << "UDP Datagram:" << std::endl;
    std::cout << "  Source Port: " << src_port << std::endl;
    std::cout << "  Destination Port: " << dst_port << std::endl;
    std::cout << "  Length: " << length << " bytes" << std::endl;
    std::cout << "Pakcet Size: " << GetPacketSize() << " bytes" << std::endl;
}

std::string UDP::getSummary() const {
    return "UDP: " + std::to_string(src_port) + " -> " + std::to_string(dst_port);
}
