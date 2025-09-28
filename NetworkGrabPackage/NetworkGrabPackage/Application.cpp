#include "Application.hpp"
#include "NetworkInterface.hpp"
#include "PacketCapture.hpp"
#include "PacketParser.hpp"
#include <iostream>

using namespace std;

Application::Application() {

}

Application::~Application() {

}

void Application::StartApplication() {
	packetCapture.startCapture("\\Device\\NPF_{DB3D8D4E-8126-44D3-AF82-DA2276ADAA30}");
}

