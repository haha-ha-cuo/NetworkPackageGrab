#pragma once
#include "Pages.hpp"
#include "NetworkInterface.hpp"

class DevicePage : public Pages, public NetworkInterface
{
public:
    DevicePage();
    ~DevicePage();
    void display() override;
    Pages* chiocePage(int index);
    const char* getDeviceName(int i) const;
};
