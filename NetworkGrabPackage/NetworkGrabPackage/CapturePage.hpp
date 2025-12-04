#include "Pages.hpp"
#include <mutex>
#include "PacketCapture.hpp"

class CapturePage : public Pages
{
private:
	mutable std::mutex mtx;
	PacketCapture* capturePtr = nullptr; // 指向正在抓包的实例（非拥有）
public:
	explicit CapturePage(PacketCapture* cap = nullptr) : capturePtr(cap) {}
	void setPacketCapture(PacketCapture* cap) override{ capturePtr = cap; }
	void display() override;
};