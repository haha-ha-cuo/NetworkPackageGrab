#include "CapturePage.hpp"
#include <iostream>
#include <vector>
#include <thread>
#include <chrono>
#include <iomanip>
#include "TCP.hpp"
#include "UDP.hpp"

Pages* CapturePage::display()
{
    while (1)
    {
        system("cls");
        std::clog << "Time\t\t" << "Source Port\t\t" << "Destination Port\t" << "Size" << std::endl;

        if (capturePtr.getPacketManager().GetPackets().empty())
        {
            std::clog << "[Warn] PacketCapture instance not set. Use setPacketCapture()." << std::endl;
            std::this_thread::sleep_for(std::chrono::milliseconds(2000));
            continue;
        }

        const auto &pkts = capturePtr.getPacketManager().GetPackets();
        size_t start = pkts.size() > 200 ? pkts.size() - 200 : 0;
        for (size_t i = start; i < pkts.size(); ++i)
        {
            const auto &p = pkts[i];
            switch (p->GetPacketType())
            {
            case PacketType::TCP:
            {
                const TCP *t = static_cast<const TCP *>(p.get());
                std::time_t tt = std::chrono::system_clock::to_time_t(p->GetTimestamp());
                tm localTm{};
                localtime_s(&localTm, &tt);
                std::clog << std::put_time(&localTm, "%H:%M:%S") << "\t" << t->getSrcPort() << "\t\t\t" << t->getDstPort() << "\t\t\t" << p->GetPacketSize() << std::endl;
                break;
            }
            case PacketType::UDP:
            {
                const UDP *u = static_cast<const UDP *>(p.get());
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

        std::this_thread::sleep_for(std::chrono::milliseconds(2000));
    }
}
