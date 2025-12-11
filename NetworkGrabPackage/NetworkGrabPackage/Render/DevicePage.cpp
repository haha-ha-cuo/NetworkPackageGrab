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
    std::vector<std::string> m{ "  1.next  ",
                                "  2.Back to Home Page  "};
    int s = Select(m);
    if (s >= 0) std::cout << "你选择了：" << s + 1 << endl;

    Pages* next = choicePage(s - 1);
    if (next) {
        Pages::pageStack.push(this);
        return next;
    }
    else {
        return Pages::Back();
    }
}

Pages* DevicePage::choicePage(int index)
{
    if (!index) {
        static CapturePage capture;
        return &capture;
    }
    else
        return nullptr;
}

const char* DevicePage::getDeviceName(int i) const
{
    if (i < 0 || i >= static_cast<int>(devices.size())) return nullptr;
    return devices[i];
}
