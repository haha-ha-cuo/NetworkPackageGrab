#include "Application.hpp"
#include "NetworkInterface.hpp"
#include "PacketCapture.hpp"
#include "PacketParser.hpp"
#include <thread>
#include <iostream>
#include <zlib.h>
using namespace std;
Application::Application() {

}

Application::~Application() {
	
}

void Application::StartApplication(){
	thread captureThread;
	printAllDevices();
	cout << "Choice:" ;
	int choice;
	cin >> choice;
	deviceName = devices[choice];
	string portStr;
	cout << "Port:";
	getchar();
	getline(cin,portStr);
	port = const_cast<char*>(portStr.c_str());
	try {

		captureThread = thread(&PacketCapture::startCapture, this, deviceName, port);
		
	}catch (const std::runtime_error& e) {

		cerr << "线程异常" << endl;
	}
	captureThread.join();
	
	
	//PacketParser parser;
	/*const char* deviceName = "\\Device\\NPF_{A5484AF4-D1D3-4914-A825-DC74FAAEE006}";
	parser.startParse(deviceName);
	parser.startParse("\\Device\\NPF_{DB3D8D4E-8126-44D3-AF82-DA2276ADAA30}");
	parser.saveToFile("captured_packets.json");
	parser.loadAndParseFromJson("captured_packets.json");*/

}

void Application::printAllDevices() {
	int i = 0;
	for(const char * deviceName : devicesDescription) {
		std::cout<<i++<<"->" << deviceName << std::endl;
	}
}

