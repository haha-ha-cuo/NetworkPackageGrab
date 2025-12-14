#include "Application.hpp"
#include "TrafficStatistics.hpp"
#include <iostream>
#include <Windows.h>
#include <pcap.h>

using namespace std;

int main()
{
    SetConsoleCP(65001);
    SetConsoleOutputCP(65001);
    Application application;
    application.StartApplication();
    return 0;
}
