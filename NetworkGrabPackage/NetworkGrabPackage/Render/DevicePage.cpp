#include "DevicePage.hpp"
#include "CapturePage.hpp"
#include <iostream>
#include "Render.hpp"

DevicePage::DevicePage() {}

DevicePage::~DevicePage() {}

Pages* DevicePage::display()
{
    system("cls");
    const Render render;
    std::cout << "Device Page Display" << std::endl;
    std::cout << ">>1. next" << std::endl;
    std::cout << ">>2. Back to Home Page" << std::endl;
    std::cout << ">>";
    int n;
    cin >> n;
    /*for (const char* deviceName : devicesDescription) {
        std::cout << i++ << " : " << deviceName << std::endl;
    }*/
	
	
    if (choicePage(n - 1)) {
		system("cls");
        cout << ">>Input Port Number: ";
        getchar();
        getline(cin, port);

        system("cls");
        int i = 0;
        int index;
        //cout << ">>Select Device";
        index = render.Select(devicesDescription);
        deviceName = const_cast<char*>(getDeviceName(index-1));
        
        
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
