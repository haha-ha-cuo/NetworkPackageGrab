#include "TCP.hpp"

void TCP::Parse() {
    if (raw_data.size() < 20) {
        throw std::runtime_error("Data too short to be a TCP segment");
    }

    src_port = (static_cast<uint16_t>(raw_data[0]) << 8) | static_cast<uint16_t>(raw_data[1]);
    dst_port = (static_cast<uint16_t>(raw_data[2]) << 8) | static_cast<uint16_t>(raw_data[3]);

    sequence_number = (static_cast<uint32_t>(raw_data[4]) << 24) |
        (static_cast<uint32_t>(raw_data[5]) << 16) |
        (static_cast<uint32_t>(raw_data[6]) << 8) |
        static_cast<uint32_t>(raw_data[7]);

    acknowledgment_number = (static_cast<uint32_t>(raw_data[8]) << 24) |
        (static_cast<uint32_t>(raw_data[9]) << 16) |
        (static_cast<uint32_t>(raw_data[10]) << 8) |
        static_cast<uint32_t>(raw_data[11]);

    data_offset = (raw_data[12] >> 4) & 0x0F;

    size_t header_size = data_offset * 4;
    if (raw_data.size() > header_size) {
        payload.insert(payload.end(), raw_data.begin() + header_size, raw_data.end());
    }
}

void TCP::display() const{
    std::cout << "TCP Segment:" << std::endl;
    std::cout << "  Source Port: " << src_port << std::endl;
    std::cout << "  Destination Port: " << dst_port << std::endl;
    std::cout << "  Sequence Number: " << sequence_number << std::endl;
    std::cout << "  Acknowledgment Number: " << acknowledgment_number << std::endl;
    std::cout << "  Data Offset: " << static_cast<int>(data_offset) << " ("
        << static_cast<int>(data_offset) * 4 << " bytes)" << std::endl;
    std::cout << "  Payload Size: " << payload.size() << " bytes" << std::endl;
    std::cout << "Pakcet Size: " << GetPacketSize() << " bytes" << std::endl;
}

std::string TCP::getSummary() const{
    return "TCP: " + std::to_string(src_port) + " -> " + std::to_string(dst_port);
}
