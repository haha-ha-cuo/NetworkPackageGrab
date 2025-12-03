#include "Application.hpp"
#include "NetworkInterface.hpp"
#include "PacketCapture.hpp"
#include "PacketParser.hpp"
#include <iostream>
#include <zlib.h>
Application::Application() {

}

Application::~Application() {

}

void Application::StartApplication(){
	printAllDevices();
	cout << "Choice:" ;
	int choice;
	cin >> choice;
	deviceName = const_cast<char*>(devices[choice]);
	string portStr;
	cout << "Port:";
	getchar();
	getline(cin,portStr);
	port = const_cast<char*>(portStr.c_str());
	startCapture(deviceName,port);
	
	//PacketParser parser;
	/*const char* deviceName = "\\Device\\NPF_{A5484AF4-D1D3-4914-A825-DC74FAAEE006}";
	parser.startParse(deviceName);
	parser.startParse("\\Device\\NPF_{DB3D8D4E-8126-44D3-AF82-DA2276ADAA30}");
	parser.saveToFile("captured_packets.json");
	parser.loadAndParseFromJson("captured_packets.json");*/

}

void Application::printAllDevices() {
	int i = 0;
	for(const char * deviceName : devices) {
		std::cout<<i++<<"->" << deviceName << std::endl;
	}
}

