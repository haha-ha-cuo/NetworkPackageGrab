#pragma once
// 应用程序主类
#include "NetworkInterface.hpp"
#include "PacketCapture.hpp"

class Application
{
public:
	Application();
	~Application();

	void StartApplication();
private:
	NetworkInterface networkInterface;
	PacketCapture packetCapture;
};