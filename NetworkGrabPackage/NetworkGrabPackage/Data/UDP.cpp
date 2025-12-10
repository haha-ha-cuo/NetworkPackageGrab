#include "UDP.hpp"

using namespace std;
void UDP::Parse()
{
    if (raw_data.size() < 8)
    {
        throw std::runtime_error("Data too short to be a UDP datagram");
    }
    src_port = (static_cast<uint16_t>(raw_data[0]) << 8) | static_cast<uint16_t>(raw_data[1]);

    dst_port = (static_cast<uint16_t>(raw_data[2]) << 8) | static_cast<uint16_t>(raw_data[3]);

    length = (static_cast<uint16_t>(raw_data[4]) << 8) | static_cast<uint16_t>(raw_data[5]);
}

void UDP::display() const
{
    cout << "UDP Datagram:" << endl;
    cout << "  Source Port: " << src_port << endl;
    cout << "  Destination Port: " << dst_port << endl;
    cout << "  Length: " << length << " bytes" << endl;
    cout << "Packet Size: " << GetPacketSize() << " bytes" << endl;
}

string UDP::getSummary() const
{
    return "UDP: " + to_string(src_port) + " -> " + to_string(dst_port);
}
