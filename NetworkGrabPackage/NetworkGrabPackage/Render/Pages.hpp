#pragma once
#include <stack>
#include "PacketCapture.hpp"

class Pages {
protected:
	PacketCapture capturePtr;
	inline static stack<Pages*> pageStack;
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
