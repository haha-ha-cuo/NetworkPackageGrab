#pragma once
// 应用程序主类
#include "NetworkInterface.hpp"
#include "PacketCapture.hpp"\
#include <vector>


class Application : public PacketCapture, public NetworkInterface
{
public:

	Application();

	~Application();

	void StartApplication();

	void printAllDevices();
private:
	char* deviceName;

	char* port;
};