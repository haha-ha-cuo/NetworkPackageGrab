#pragma once
#include "Pages.hpp"
#include "NetworkInterface.hpp"

class DevicePage : public Pages, public NetworkInterface
{
public:
    DevicePage();
    ~DevicePage();
    Pages* display() override;
    Pages* choicePage(int index);
    const char* getDeviceName(int i) const;
};
