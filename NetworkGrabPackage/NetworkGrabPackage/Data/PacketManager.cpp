#include "PacketManager.hpp"

using namespace NetworkGrabPackage;
void PacketManager::AddPacket(unique_ptr<Packet> packet)
{
    packets.push_back(move(packet));
}

void PacketManager::ParseAll()
{
    for (auto &packet : packets)
    {
        try
        {
            packet->Parse();
        }
        catch (const exception &e)
        {
            cerr << "Error parsing packet: " << e.what() << endl;
        }
    }
}

void PacketManager::displayAll() const
{
    for (const auto &packet : packets)
    {
        packet->display();
        cout << endl;
    }
}

void PacketManager::displaySummaries() const
{
    for (const auto &packet : packets)
    {
        cout << packet->getSummary() << endl;
    }
}
