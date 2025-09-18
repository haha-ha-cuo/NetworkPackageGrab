#include "Application.h"
#include "NetworkInterface.h"
#include "PacketCapture.h"
#include "PacketParser.h"
#include <iostream>
using namespace std;

Application::Application() {
	// 初始化网络接口管理器
	NetworkInterface* networkInterface = new NetworkInterface();
	networkInterface->printAllDevices();
	PacketParser* packetParser = new PacketParser();
	packetParser->startParse("\\Device\\NPF_{DB3D8D4E-8126-44D3-AF82-DA2276ADAA30}"); // 替换为实际的网络接口名称
}