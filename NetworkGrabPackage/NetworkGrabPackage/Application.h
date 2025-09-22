#pragma once
// 应用程序主类
#include "NetworkInterface.h"
#include "PacketCapture.h"

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