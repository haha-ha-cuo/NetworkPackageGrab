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

<<<<<<< Updated upstream
=======
	void pushPage(Pages* page) {
		pageStack.push(page);
	}

	Pages* popPage() {
		if (pageStack.empty()) {
			return nullptr;
		}
		Pages* topPage = pageStack.top();
		pageStack.pop();
		return topPage;
	}


>>>>>>> Stashed changes
private:
	const char* deviceName;
	char* port;
	Pages* currentPage;
	std::stack<Pages*> pageStack;
};