#include "Application.hpp"
#include "NetworkInterface.hpp"
#include "PacketCapture.hpp"
#include <thread>
#include <iostream>
#include <zlib.h>
#include "CapturePage.hpp"
#include "HomePage.hpp"

using namespace std;


Application::Application() {}

Application::~Application() {}

void Application::StartApplication(){
	currentPage = new HomePage();
    while (true)
    {
		Pages* nextPage = currentPage->display();
        if (nextPage == nullptr) {
            // 视为退出或没有可切换页面，结束循环
            delete currentPage;
            break;
        }
		currentPage = nextPage;
	}
}
