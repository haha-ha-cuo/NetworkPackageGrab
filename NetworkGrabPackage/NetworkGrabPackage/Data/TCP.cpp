#include "TCP.hpp"

using namespace std;

void TCP::Parse()
{
    if (raw_data.size() < 20)
    {
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
    if (raw_data.size() > header_size)
    {
        payload.insert(payload.end(), raw_data.begin() + header_size, raw_data.end());
    }
}

void TCP::display() const
{
    cout << "TCP Segment:" << endl;
    cout << "  Source Port: " << src_port << endl;
    cout << "  Destination Port: " << dst_port << endl;
    cout << "  Sequence Number: " << sequence_number << endl;
    cout << "  Acknowledgment Number: " << acknowledgment_number << endl;
    cout << "  Data Offset: " << static_cast<int>(data_offset) << " ("
         << static_cast<int>(data_offset) * 4 << " bytes)" << endl;
    cout << "  Payload Size: " << payload.size() << " bytes" << endl;
    cout << "Packet Size: " << GetPacketSize() << " bytes" << endl;
}

string TCP::getSummary() const
{
    return "TCP: " + to_string(src_port) + " -> " + to_string(dst_port);
}
