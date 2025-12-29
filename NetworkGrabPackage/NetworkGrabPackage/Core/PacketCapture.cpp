#include "PacketCapture.hpp"
#include "Consts.hpp"
#include "Types.hpp"
#include "PacketFilePipeline.hpp"
#include <iostream>
#include <pcap.h>
#include <chrono>
#include <conio.h>
#include <Windows.h>
#include <iomanip>
#include <filesystem>
#include <fstream>

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

void PacketCapture::startCapture(const char *deviceName, const char *port, const char *inputPath)
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
    cout << ">> Press 'q' or 'Esc' to stop capturing..." << endl;

    if (!packetFilter.setFilter(port, handle, deviceName))
    {
        cerr << "[Error]Failed to apply filter. Stopping packet capture." << endl;
        pcap_close(handle);
        handle = nullptr;
        return;
    }

    namespace fs = std::filesystem;
    try
    {
        fs::create_directories("../Output");
    }
    catch (...)
    {
    }

    std::ofstream inputOut(inputPath, std::ios::out | std::ios::binary | std::ios::trunc);
    if (!inputOut.is_open())
    {
        std::cerr << "[Error]Failed to open input file for writing: " << inputPath << std::endl;
        pcap_close(handle);
        handle = nullptr;
        return;
    }

    std::this_thread::sleep_for(std::chrono::milliseconds(1000));

    int capturedPackets = 0;
    int timeoutCount = 0;
    const int maxTimeouts = 20;

    pcap_t *localHandle = nullptr;
    stopRequested.store(false, std::memory_order_relaxed);

    while (!stopRequested)
    {

        if (!handle)
            break;
        localHandle = handle;

        result = pcap_next_ex(localHandle, &header, &pktData);

        if (result == 0)
        {
            timeoutCount++;

            if (_kbhit())
            {
                int ch = _getch();
                if (ch == 'q' || ch == 'Q' || ch == 27)
                {
                    cout << "[Info]User requested to stop capturing." << endl;
                    break;
                }
            }

            if (timeoutCount >= maxTimeouts)
            {
                cout << "[Warn]No packets captured after " << timeoutCount << " timeouts." << endl;
                cout << ">> Press 'q' or 'Esc' to stop capturing, any other key to continue..." << endl;
                int ch = _getch();
                if (ch == 'q' || ch == 'Q' || ch == 27)
                {
                    break;
                }
                timeoutCount = 0;
            }

            std::this_thread::sleep_for(std::chrono::milliseconds(200));
            continue;
        }
        timeoutCount = 0;

        if (header && pktData && header->caplen > 0)
        {
            std::string err;
            if (PacketFilePipeline::AppendCapturedPacketLine(inputOut, *header, pktData, &err))
            {
                capturedPackets++;
            }
            else
            {
                std::cerr << "[Warn]Write input.txt failed: " << err << std::endl;
            }
        }

        vector<uint8_t> dataVec(pktData, pktData + header->caplen);

        unique_lock<std::mutex> lk(handleMutex);
        auto packet = PacketFactory::createPacket(dataVec);
        if (packet)
        {
            try
            {
                packet->Parse();
            }
            catch (const exception &)
            {
                continue;
            }
            packetManager.AddPacket(move(packet));
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
        cout << "\n"
             << endl;
        stopRequested = displayData();
    }
    if (result == -1)
    {
        cerr << "[Error]Error reading the packets: " << pcap_geterr(handle) << endl;
    }
    else
    {
        inputOut.flush();
        inputOut.close();

        cout << "[Info]Total packets captured: " << capturedPackets << endl;
        if (capturedPackets == 0)
        {
            cout << "[Warn]No packets captured." << endl;
        }
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

bool PacketCapture::parseInputFileToOutput(const std::string &inputPath,
                                          const std::string &outputPath,
                                          const std::string &logPath) const
{
    return PacketFilePipeline::ParseInputFileToOutput(inputPath, outputPath, logPath);
}

void PacketCapture::dumpPacket(const char *name)
{

    if (!handle)
    {
        return;
    }
    if (!header || !pktData)
    {
        return;
    }

    // 从 pcap header 获取秒与微秒，构造带毫秒的文件名
    time_t sec = header->ts.tv_sec;
    long usec = header->ts.tv_usec; // microseconds
    struct tm tmDest{};
    if (localtime_s(&tmDest, &sec) != 0)
    {
        // fallback: 仅使用秒数
        tmDest.tm_year = 70;
        tmDest.tm_mon = 0;
        tmDest.tm_mday = 1;
    }
    int ms = static_cast<int>(usec / 1000); // 毫秒部分

    char fname[128];
    // 格式: ../Output/YYYY-MM-DD_HH-MM-SS-ms.pcap
    snprintf(fname, sizeof(fname), "../Output/%04d-%02d-%02d_%02d-%02d-%02d-%03d.pcap",
             tmDest.tm_year + 1900, tmDest.tm_mon + 1, tmDest.tm_mday,
             tmDest.tm_hour, tmDest.tm_min, tmDest.tm_sec, ms);

    pcap_dumper_t *localDumper = pcap_dump_open(handle, fname);
    if (!localDumper)
    {
        cerr << "[Error] Failed to open dump file: " << fname << " : " << pcap_geterr(handle) << endl;
        return;
    }
    pcap_dump((u_char *)localDumper, header, pktData);
    pcap_dump_close(localDumper);
}

bool PacketCapture::displayData() const
{
    system("cls");
    std::clog << std::left
              << std::setw(6) << "Idx"
              << std::setw(10) << "Time"
              << std::setw(12) << "Type"
              << std::setw(22) << "Source"
              << std::setw(22) << "Destination"
              << std::setw(8) << "Size"
              << std::endl;

    const auto &pkts = getPacketManager().GetPackets();

    if (pkts.empty())
    {
        std::clog << "[Warn] PacketCapture instance not set. Use setPacketCapture()." << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));

        if (_kbhit())
        {
            int ch = _getch();
            if (ch == 'q' || ch == 'Q' || ch == 27)
            {
                return true;
            }
        }
        std::cout << ">> Press 'q' or 'Esc' to stop capturing..." << std::endl;
        return false;
    }

    size_t index = 1;

    for (const auto &p : pkts)
    {
        std::time_t tt = std::chrono::system_clock::to_time_t(p->GetTimestamp());
        tm localTm{};
        localtime_s(&localTm, &tt);
        p->showSummary(index, localTm);

        ++index;
    }

    if (_kbhit())
    {
        int ch = _getch();
        if (ch == 'q' || ch == 'Q' || ch == 27)
        {
            return true;
        }
        else if ((ch == 'd' || ch == 'D') && !pkts.empty())
        {
            std::cout << "\nEnter packet index [1 - " << pkts.size() << "]: ";
            size_t idx;
            if (!(std::cin >> idx))
            {
                std::cin.clear();
                std::cin.ignore(1024, '\n');
            }
            else if (idx <= pkts.size())
            {
                system("cls");
                std::cout << "[Info]Packet #" << idx << " detail:" << std::endl;
                pkts[idx - 1]->display();
                std::cout << "\n[Info]Press any key to return..." << std::endl;
                _getch();
            }
        }
    }
    std::cout << ">> Press 'q' or 'Esc' to stop capturing, 'd' to view packet detail..." << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    return false;
}