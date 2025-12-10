#pragma once
#include <stack>
#include "PacketCapture.hpp"

class Pages {
private:
    std::stack<Pages*> pageStack;
public:
    virtual void display() = 0;
    virtual void setPacketCapture(PacketCapture* cap) {}
};
