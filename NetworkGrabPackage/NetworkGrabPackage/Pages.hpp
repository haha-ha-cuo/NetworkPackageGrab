#pragma once
#include <stack>
#include "PacketCapture.hpp"

class Pages {
protected:
	PacketCapture* capturePtr = nullptr; // 指向正在抓包的实例（非拥有）

public:
	virtual void display() = 0; //纯虚函数，必须由派生类实现
	void setPacketCapture(PacketCapture* cap) {} //虚函数，派生类可选择性实现
	virtual Pages* chiocePage(int index)=0; //虚函数，派生类可选择性实现
	virtual const char* getDeviceName(int i) {
		return NULL;
	}
	
};