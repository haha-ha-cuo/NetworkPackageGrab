#include "DevicePage.hpp"
#include "CapturePage.hpp"
#include <iostream>

DevicePage::DevicePage() {}

DevicePage::~DevicePage() {}

Pages* DevicePage::display()
{
    system("cls");
    std::cout << "Device Page Display" << std::endl;
    int i = 0;
    for (const char* deviceName : devicesDescription) {
        std::cout << i++ << " : " << deviceName << std::endl;
    }
    std::cout << ">>1. next" << std::endl;
    std::cout << ">>2. Back to Home Page" << std::endl;
    std::cout << ">>";

    int n;
    cin >> n;
	
    if (choicePage(n - 1)) {
        int i;
		cout << ">>Select Device Index: ";
        cin >> i;
		deviceName = const_cast<char*>(getDeviceName(i));
       
		cout << ">>Input Port Number: ";
        getchar();
		getline(cin, port);
        Pages::pageStack.push(this);
		return choicePage(n - 1);
    }
    else {
		return Pages::Back();
    }
}

Pages* DevicePage::choicePage(int index)
{
    if (!index)
        return new CapturePage();
    else
        return nullptr;
}

const char* DevicePage::getDeviceName(int i) const
{
    if (i < 0 || i >= static_cast<int>(devices.size())) return nullptr;
    return devices[i];
}
