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

    pcap_t* localHandle = nullptr;

    
    while (!stopRequested)
    {
        std::lock_guard<std::mutex> lk(handleMutex);
        if (!handle) break;
        localHandle = handle;

        result = pcap_next_ex(localHandle, &header, &pktData);

        if (result == 0)
        {
            continue;
        }
        timeoutCount = 0;

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

        const time_t ts = header->ts.tv_sec;
        struct tm tmDest;
        errno_t erro = localtime_s(&tmDest, &ts);
        char timestr[16];
        strftime(timestr, sizeof(timestr), "%H-%M-%S", &tmDest);

        char name[100];

        snprintf(name, sizeof(name), "../Output/%s.pcap", timestr);

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
    
    requestStop();

    std::lock_guard<std::mutex> lk(handleMutex);
    if (handle)
    {
        // 可以安全地关闭 handle（因为 startCapture 检查 stopRequested 并会在超时后退出）
        pcap_close(handle);
        handle = nullptr;
        cout << "[Info]Packet capture closed." << endl;
    }
}

const PacketManager &PacketCapture::getPacketManager() const
{
    return packetManager;
}
