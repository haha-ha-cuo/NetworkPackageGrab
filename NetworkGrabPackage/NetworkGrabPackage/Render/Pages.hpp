#pragma once
#include <stack>
#include <string>
#include "PacketCapture.hpp"

class Pages {
protected:
	inline static PacketCapture capturePtr;
	inline static stack<Pages*> pageStack;
	inline static char* deviceName;
	inline static string port;
public:
    virtual Pages* display() = 0;

	Pages* Back() {
		if (pageStack.empty()) {
			return nullptr;
		}
		Pages* prevPage = pageStack.top();
		pageStack.pop();
		return prevPage;
	};
};
