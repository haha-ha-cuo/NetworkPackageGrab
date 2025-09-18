#include "Application.h"
#include "NetworkInterface.h"
#include "PacketCapture.h"
#include "PacketParser.h"
#include <iostream>
using namespace std;

Application::Application() {
	// ��ʼ������ӿڹ�����
	NetworkInterface* networkInterface = new NetworkInterface();
	networkInterface->printAllDevices();
	PacketParser* packetParser = new PacketParser();
	packetParser->startParse("\\Device\\NPF_{DB3D8D4E-8126-44D3-AF82-DA2276ADAA30}"); // �滻Ϊʵ�ʵ�����ӿ�����
}