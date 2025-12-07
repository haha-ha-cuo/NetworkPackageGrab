#include "CapturePage.hpp"
#include <iostream>
#include <vector>
#include <thread>
#include <chrono>

void CapturePage::display() {
	while (1) {
		system("cls");
		std::clog << "Time\t\t" << "Source Port\t\t" << "Destination Port\t" << "Size" << std::endl;

		if (!capturePtr) {
			std::clog << "[Warn] PacketCapture instance not set. Use setPacketCapture()." << std::endl;
			std::this_thread::sleep_for(std::chrono::milliseconds(2000));
			continue;
		}

		// 使用 auto 避免在此处需要显式引用 PacketCapture::dataPkg 类型
		auto dataPkgList = capturePtr->getDataPkgList();
		for (const auto& pkg : dataPkgList) {
			std::clog << pkg.ts << "\t" << pkg.sPort << "\t\t\t" << pkg.dPort << "\t\t\t" << pkg.size << std::endl;
		}

		std::this_thread::sleep_for(std::chrono::milliseconds(2000));
	}
}

Pages* CapturePage::chiocePage(int index) {
	return nullptr;
}