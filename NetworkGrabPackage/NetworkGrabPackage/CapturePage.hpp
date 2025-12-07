#pragma once
#include "Pages.hpp"
#include <mutex>
#include "PacketCapture.hpp"

class CapturePage : public Pages
{
private:
	mutable std::mutex mtx;// 指向正在抓包的实例（非拥有）
public:
	Pages* chiocePage(int index) override;
	void display() override;
};