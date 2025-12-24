#include "IP.hpp"
#include <iomanip>
#include <sstream>

using namespace std;

IP::IP(const std::vector<uint8_t> &data) : Packet(data, PacketType::IP), protocol(0), total_length(0), ttl(0) {}
IP::IP(const IP &other) : Packet(other), protocol(other.protocol), total_length(other.total_length), ttl(other.ttl) {}
void IP::Parse()
{
    if (raw_data.size() < 20)
    {
        throw runtime_error("Data too short to be an IP packet");
    }

    uint8_t version = (raw_data[0] >> 4) & 0x0F;
    if (version != 4)
    {
        throw runtime_error("Not an IPv4 packet");
    }

    total_length = (static_cast<uint16_t>(raw_data[2]) << 8) | static_cast<uint16_t>(raw_data[3]);

    ttl = raw_data[8];

    protocol = raw_data[9];

    src_ip = to_string(static_cast<int>(raw_data[12])) + "." +
             to_string(static_cast<int>(raw_data[13])) + "." +
             to_string(static_cast<int>(raw_data[14])) + "." +
             to_string(static_cast<int>(raw_data[15]));

    dst_ip = to_string(static_cast<int>(raw_data[16])) + "." +
             to_string(static_cast<int>(raw_data[17])) + "." +
             to_string(static_cast<int>(raw_data[18])) + "." +
             to_string(static_cast<int>(raw_data[19]));
}

void IP::display() const
{
    cout << "IP Packet:" << endl;
    cout << "  Source IP: " << src_ip << endl;
    cout << "  Destination IP: " << dst_ip << endl;
    cout << "  Protocol: " << static_cast<int>(protocol) << endl;
    cout << "  Total Length: " << total_length << " bytes" << endl;
    cout << "  TTL: " << static_cast<int>(ttl) << endl;
    cout << "Packet Size: " << GetPacketSize() << " bytes" << endl;
}

void IP::showSummary(size_t index, tm localTm) const
{
    ostringstream time_ss;
    time_ss << put_time(&localTm, "%H:%M:%S");

    clog << left
         << setw(6) << index
         << setw(10) << time_ss.str()
         << setw(12) << PacketTypeToString(packet_type)
         << setw(22) << src_ip
         << setw(22) << dst_ip
         << setw(8) << GetPacketSize()
         << endl;
}
string IP::getSrcIP() const { return src_ip; }
string IP::getDestIP() const { return dst_ip; }
uint8_t IP::getProtocol() const { return protocol; }
uint16_t IP::getTotalLength() const { return total_length; }
uint8_t IP::getTTL() const { return ttl; }
