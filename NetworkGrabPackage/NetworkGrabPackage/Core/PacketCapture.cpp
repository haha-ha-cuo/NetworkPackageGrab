#include "PacketCapture.hpp"
#include "Consts.hpp"
#include <iostream>
#include <pcap.h>
#include <chrono>
#include <conio.h>
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

    std::this_thread::sleep_for(std::chrono::milliseconds(1000));

    int capturedPackets = 0;
    int timeoutCount = 0;
    const int maxTimeouts = 20;

    pcap_t* localHandle = nullptr;
	stopRequested.store(false, std::memory_order_relaxed);
    
    while (!stopRequested)
    {
        
        if (!handle) break;
        localHandle = handle;

        result = pcap_next_ex(localHandle, &header, &pktData);

        if (result == 0)
        {
            continue;
        }
        timeoutCount = 0;

        std::vector<uint8_t> dataVec(pktData, pktData + header->caplen);
        
        std::unique_lock<std::mutex> lk(handleMutex);
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

		lk.unlock();
        const time_t ts = header->ts.tv_sec;
        struct tm tmDest;
        errno_t erro = localtime_s(&tmDest, &ts);
        char timestr[16];
        strftime(timestr, sizeof(timestr), "%H-%M-%S", &tmDest);

        char name[100];

        snprintf(name, sizeof(name), "../Output/%s.pcap", timestr);

        try
        {
            dumpPacket(name);
        }
        catch (...)
        {
            cerr << "[Error]Failed to save dump file: " << name << endl;
            continue;
        }
        cout << "\n"<< endl;
        stopRequested = displayData();
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

void PacketCapture::dumpPacket(const char* name)
{
    
    if (!handle) {
        // handle 已被关闭或不可用
        return;
    }
    if (!header || !pktData) {
        return;
    }

    // 从 pcap header 获取秒与微秒，构造带毫秒的文件名
    time_t sec = header->ts.tv_sec;
    long usec = header->ts.tv_usec; // microseconds
    struct tm tmDest {};
    if (localtime_s(&tmDest, &sec) != 0) {
        // fallback: 仅使用秒数
        tmDest.tm_year = 70; tmDest.tm_mon = 0; tmDest.tm_mday = 1;
    }
    int ms = static_cast<int>(usec / 1000); // 毫秒部分

    char fname[128];
    // 格式: ../Output/YYYY-MM-DD_HH-MM-SS-ms.pcap
    snprintf(fname, sizeof(fname), "../Output/%04d-%02d-%02d_%02d-%02d-%02d-%03d.pcap",
        tmDest.tm_year + 1900, tmDest.tm_mon + 1, tmDest.tm_mday,
        tmDest.tm_hour, tmDest.tm_min, tmDest.tm_sec, ms);

    pcap_dumper_t* localDumper = pcap_dump_open(handle, fname);
    if (!localDumper)
    {
        cerr << "[Error] Failed to open dump file: " << fname << " : " << pcap_geterr(handle) << endl;
        return;
    }
    pcap_dump((u_char*)localDumper, header, pktData);
    pcap_dump_close(localDumper);
    
}


bool PacketCapture::displayData() const
{
    system("cls");
    std::clog << "Time\t\t" << "Source Port\t\t" << "Destination Port\t" << "Size" << std::endl;

    if (getPacketManager().GetPackets().empty())
    {
        std::clog << "[Warn] PacketCapture instance not set. Use setPacketCapture()." << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));

        if (_kbhit()) {
            int ch = _getch();
            if (ch == 'q' || ch == 'Q' || ch == 27) {
                return true;
            }
        }
        cout << ">> Press 'q' or 'Esc' to stop capturing..." << endl;

    
    }


    const auto& pkts = getPacketManager().GetPackets();

    for (const auto& p : pkts)
    {

        switch (p->GetPacketType())
        {
        case PacketType::TCP:
        {
            const TCP* t = static_cast<const TCP*>(p.get());
            std::time_t tt = std::chrono::system_clock::to_time_t(p->GetTimestamp());
            tm localTm{};
            localtime_s(&localTm, &tt);
            std::clog << std::put_time(&localTm, "%H:%M:%S") << "\t" << t->getSrcPort() << "\t\t\t" << t->getDstPort() << "\t\t\t" << p->GetPacketSize() << std::endl;
            break;
        }
        case PacketType::UDP:
        {
            const UDP* u = static_cast<const UDP*>(p.get());
            std::time_t tt = std::chrono::system_clock::to_time_t(p->GetTimestamp());
            tm localTm{};
            localtime_s(&localTm, &tt);
            std::clog << std::put_time(&localTm, "%H:%M:%S") << "\t" << u->getSrcPort() << "\t\t\t" << u->getDstPort() << "\t\t\t" << p->GetPacketSize() << std::endl;
            break;
        }
        default:
            break;
        }
    }

    if (_kbhit()) {
        int ch = _getch();
        if (ch == 'q' || ch == 'Q' || ch == 27) {
			return true;
        }
    }
    cout << ">> Press 'q' or 'Esc' to stop capturing..." << endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    return false;
}