#include "CapturePage.hpp"
#include <iostream>
#include <vector>
#include <thread>
#include <chrono>
#include <conio.h>
#include <iomanip>
#include "TCP.hpp"
#include "UDP.hpp"

Pages* CapturePage::display()
{
	system("cls"); 
    cout << "[Info]Starting packet capture..." << endl;
    thread threadCapture([this]() {
        capturePtr.startCapture(Pages::deviceName, const_cast<char *>(Pages::port.c_str()));
        });

    threadCapture.join();
    cout << "[Info]Stopping packet capture..." << endl;

    cout << "[Info]Parsing ../Output/input.txt -> ../Output/output.txt ..." << endl;
    if (capturePtr.parseInputFileToOutput())
    {
        cout << "[Info]Parse finished: ../Output/output.txt" << endl;
    }
    else
    {
        cout << "[Warn]Parse failed." << endl;
    }

    cout << "[Info]Press [Enter] to continue >>";
    getchar();
    return Pages::Back();
}

//道行太浅以后再用吧
/*
void CapturePage::show() {

    while (!Pages::captureRunning)
    {
        system("cls");
        std::clog << "Time\t\t" << "Source Port\t\t" << "Destination Port\t" << "Size" << std::endl;

        if (Pages::capturePtr.getPacketManager().GetPackets().empty())
        {
            std::clog << "[Warn] PacketCapture instance not set. Use setPacketCapture()." << std::endl;
            std::this_thread::sleep_for(std::chrono::milliseconds(2000));

            if (_kbhit()) {
                int ch = _getch();
                if (ch == 'q' || ch == 'Q' || ch == 27) {
                    Pages::capturePtr.closeCapture(); // 安全地请求并关闭
                    Pages::captureRunning.store(true);
                    break;
                }
            }
            cout << ">> Press 'q' or 'Esc' to stop capturing..." << endl;

            continue;
        }


        const auto& pkts = capturePtr.getPacketManager().GetPackets();

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
                Pages::capturePtr.closeCapture(); // 安全地请求并关闭
                Pages::captureRunning.store(true);
                break;
            }
        }
        cout << ">> Press 'q' or 'Esc' to stop capturing..." << endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(2000));
    }
}
*/
