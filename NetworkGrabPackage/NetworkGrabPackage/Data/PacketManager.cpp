#include "PacketManager.hpp"

using namespace std;
void PacketManager::AddPacket(unique_ptr<Packet> packet)
{
    packets.push_back(move(packet));
}

void PacketManager::ParseAll()
{
    forEach([](Packet &p)
            {
                try
                {
                    p.Parse();
                }
                catch (const exception &e)
                {
                    cerr << "Error parsing packet: " << e.what() << endl;
                } });
}
void PacketManager::displayAll() const
{
    forEach([](const Packet &p)
            { p.display(); });
}
const vector<std::unique_ptr<Packet>> &PacketManager::GetPackets() const
{
    lock_guard<std::mutex> lg(packets_mutex);
    return packets;
}

size_t PacketManager::GetPacketCount() const
{
    return packets.size();
}