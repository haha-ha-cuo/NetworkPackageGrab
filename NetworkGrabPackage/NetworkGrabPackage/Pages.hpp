#pragma once
#include <stack>
#include "PacketCapture.hpp"

class Pages {
private:
	std::stack<Pages*> pageStack;
public:
	virtual void display() = 0; //纯虚函数，必须由派生类实现
	virtual void setPacketCapture(PacketCapture* cap) {} //虚函数，派生类可选择性实现
};