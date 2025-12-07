#pragma once
#include "Pages.hpp"
#include "NetworkInterface.hpp"

class DevicePage : public Pages, public NetworkInterface
{
public:
	void display() override;

	Pages* chiocePage(int index) override;

	const char* getDeviceName(int i) override;

};