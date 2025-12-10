#pragma once

// 应用程序主类
#include "NetworkInterface.hpp"
#include "PacketCapture.hpp"
#include <vector>
#include "Pages.hpp"


class Application : public PacketCapture, public NetworkInterface
{
public:
    Application();
    ~Application();

    void StartApplication();
    void printAllDevices();

private:
    const char* deviceName;
    char* port;
    Pages* currentPage;
};
