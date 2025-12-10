#include "Pages.hpp"
#include <mutex>
#include "PacketCapture.hpp"

class CapturePage : public Pages
{
private:
    mutable std::mutex mtx;
public:
    explicit CapturePage(){}

    Pages* display() override;
};
