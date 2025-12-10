#include "Application.hpp"
#include "NetworkInterface.hpp"
#include "PacketCapture.hpp"
#include <thread>
#include <iostream>
#include <zlib.h>
#include "CapturePage.hpp"

using namespace std;

Application::Application() {

}

Application::~Application() {
    
}

void Application::StartApplication(){
    thread captureThread;
    thread displayThread;
    printAllDevices();
    cout << "Choice:" ;
    int choice;
    cin >> choice;
    deviceName = devices[choice];
    string portStr;
    cout << "Port:";
    getchar();
    getline(cin,portStr);
    port = const_cast<char*>(portStr.c_str());
    currentPage = new CapturePage();
    currentPage->setPacketCapture(this);
    try {

        captureThread = thread(&PacketCapture::startCapture, this, deviceName, port);

        displayThread = thread(&Pages::display, currentPage);

    } catch (const std::runtime_error& e) {

        cerr << "线程异常" << e.what() << endl;

    }
    captureThread.join();
    displayThread.join();
    
}

void Application::printAllDevices() {
    int i = 0;
    for(const char * deviceName : devicesDescription) {
        std::cout << i++ << " : " << deviceName << std::endl;
    }
}
