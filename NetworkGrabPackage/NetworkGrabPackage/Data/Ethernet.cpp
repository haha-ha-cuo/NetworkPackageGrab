#include "Ethernet.hpp"

using namespace std;

Ethernet::Ethernet(const std::vector<uint8_t> &data)
    : Packet(data, PacketType::ETHERNET), ether_type(0) {}

void Ethernet::Parse()
{
    if (raw_data.size() < 14)
    {
        throw runtime_error("Data too short to be an Ethernet frame");
    }
    dst_mac = "";
    for (int i = 0; i < 6; ++i)
    {
        dst_mac += (i == 0 ? "" : ":") + to_string(static_cast<int>(raw_data[i]));
    }
    src_mac = "";
    for (int i = 6; i < 12; ++i)
    {
        src_mac += (i == 6 ? "" : ":") + to_string(static_cast<int>(raw_data[i]));
    }
    ether_type = (static_cast<uint16_t>(raw_data[12]) << 8) | static_cast<uint16_t>(raw_data[13]);
}

void Ethernet::display() const
{
    cout << "Ethernet Frame:" << endl;
    cout << "  Source MAC: " << src_mac << endl;
    cout << "  Destination MAC: " << dst_mac << endl;
    cout << "  EtherType: 0x" << hex << ether_type << dec << endl;
    cout << "Pakcet Size: " << GetPacketSize() << " bytes" << endl;
}

void Ethernet::showSummary(size_t index, tm localTm) const
{
    clog << index << "\t" << put_time(&localTm, "%H:%M:%S") << "\t" << PacketTypeToString(packet_type) << "\t"
         << src_mac << "\t\t\t" << dst_mac << "\t\t\t" << GetPacketSize() << endl;
}

string Ethernet::getSrcMac()
{
    return src_mac;
}
string Ethernet::getDestMac()
{
    return dst_mac;
}
uint16_t Ethernet::getEtherType()
{
    return ether_type;
}
