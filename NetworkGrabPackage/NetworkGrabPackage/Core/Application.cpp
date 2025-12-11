#include "Application.hpp"
#include "NetworkInterface.hpp"
#include "PacketCapture.hpp"
#include <thread>
#include <iostream>
#include <zlib.h>
#include "CapturePage.hpp"
#include "HomePage.hpp"

using namespace std;

Application::Application() {

}

Application::~Application() {
    
}

void Application::StartApplication(){
	currentPage = new HomePage();
    while (true)
    {
        if (!currentPage) {
            break;
        }
        Pages* nextPage = currentPage->display();
        if (!nextPage) {
           break;
        }
        currentPage = nextPage;
	}
}
