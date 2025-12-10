#include "PacketCapture.hpp"
#include "Consts.hpp"
#include <iostream>
#include <pcap.h>
#include <Windows.h>

using namespace std;


PacketCapture::PacketCapture()
{
    header = nullptr;
    pktData = nullptr;
    handle = nullptr;
    result = 0;
    Counter = 200;

    

    dumper = nullptr;
    errorbuf[0] = '\0';
};

PacketCapture::~PacketCapture()
{
    closeCapture();
};

void PacketCapture::startCapture(const char *deviceName, const char *port)
{

    cout << "[Info]Starting packet capture on device: " << deviceName << endl;

    handle = pcap_open_live(
        deviceName,
        Consts::MAX_PACKET_SIZE,
        1,
        3000,
        errorbuf);

    if (handle == nullptr)
    {
        cerr << "[Warn]Could not open device " << deviceName << ": " << errorbuf << endl;
        return;
    }

    cout << "[Info]Packet capture started on device: " << deviceName << endl;

    packetFilter.setFilter(port, handle, deviceName);

    Sleep(2000);

    int capturedPackets = 0;
    int timeoutCount = 0;
    const int maxTimeouts = 20;

    while ((result = pcap_next_ex(handle, &header, &pktData)) >= 0 && capturedPackets < 20)
    {
        if (result == 0)
        {
            continue;
        }
        timeoutCount = 0;
        capturedPackets++;

        std::vector<uint8_t> dataVec(pktData, pktData + header->caplen);
        auto packet = PacketFactory::createPacket(dataVec);
        if (packet)
        {
            try {
                packet->Parse();
            } catch (const std::exception&) {
                continue;
            }
            packetManager.AddPacket(std::move(packet));
        }

        char timeName[16];
        char name[100];
        snprintf(name, sizeof(name), "../Output/%s.pcap", timeName);

        try
        {
            dumper = pcap_dump_open(handle, name);
            if (!dumper)
            {
                cerr << "[Error] Failed to open dump file: " << name << endl;
                continue;
            }
            pcap_dump((u_char *)dumper, header, pktData);
            pcap_dump_close(dumper);
        }
        catch (...)
        {
            cerr << "[Error]Failed to save dump file: " << name << endl;
            continue;
        }
        cout << "\n"
             << endl;
    }
    if (result == -1)
    {
        cerr << "[Error]Error reading the packets: " << pcap_geterr(handle) << endl;
    }
    else if (capturedPackets == 0)
    {
        cout << "[Warn]No packets captured after " << maxTimeouts << " timeouts." << endl;
    }
}

void PacketCapture::closeCapture()
{
    if (handle)
    {
        pcap_close(handle);
        cout << "[Info]Packet capture closed." << endl;
    }
}

const PacketManager &PacketCapture::getPacketManager() const
{
    return packetManager;
}
