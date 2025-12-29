#include "Pages.hpp"
#include <mutex>
#include "PacketCapture.hpp"

class CapturePage : public Pages
{
private:
    mutable std::mutex mtx;
    char *port;

public:
    Pages *display() override;

    // void show();
};
