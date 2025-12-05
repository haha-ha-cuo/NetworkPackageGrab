#include "DevicePage.hpp"
#include "CapturePage.hpp"
#include "HomePage.hpp"
#include<iostream>

void DevicePage::display() {
	system("cls");
	std::cout << "Device Page Display" << std::endl;
	int i = 0;
	for (const char* deviceName : devicesDescription) {
		std::cout << i++ << " : " << deviceName << std::endl;
	}
	std::cout << ">>1. next" << std::endl;
	std::cout << ">>2. Back to Home Page" << std::endl;
	std::cout << ">>";
}

Pages* DevicePage::chiocePage(int index) 
{
	return new CapturePage();
}

const char* DevicePage::getDeviceName(int i) {
	return devices[i];
}