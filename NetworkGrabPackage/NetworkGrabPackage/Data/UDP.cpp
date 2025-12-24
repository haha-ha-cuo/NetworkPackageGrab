#include "UDP.hpp"
#include <iomanip>
#include <sstream>

using namespace std;

UDP::UDP(const std::vector<uint8_t> &data)
    : Packet(data, PacketType::UDP), src_port(0), dst_port(0),
      length(0) {}
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

void UDP::showSummary(size_t index, tm localTm) const
{
    ostringstream time_ss;
    time_ss << put_time(&localTm, "%H:%M:%S");

    clog << left
         << setw(6) << index
         << setw(10) << time_ss.str()
         << setw(12) << PacketTypeToString(packet_type)
         << setw(22) << to_string(src_port)
         << setw(22) << to_string(dst_port)
         << setw(8) << GetPacketSize()
         << endl;
}
uint16_t UDP::getSrcPort() const
{
    return src_port;
}
uint16_t UDP::getDstPort() const
{
    return dst_port;
}
uint16_t UDP::getLength() const
{
    return length;
}