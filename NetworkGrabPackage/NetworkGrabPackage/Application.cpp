#include "Application.h"
#include "NetworkInterface.h"
#include "PacketCapture.h"
#include "PacketParser.h"
#include <iostream>
using namespace std;

Application::Application() {
	
}

void Application::StartApplication() {
	networkInterface.printAllDevices();
	packetCapture.startCapture("\\Device\\NPF_{DB3D8D4E-8126-44D3-AF82-DA2276ADAA30}");
}