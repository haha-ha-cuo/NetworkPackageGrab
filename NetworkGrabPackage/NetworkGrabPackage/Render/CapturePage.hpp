#include "Pages.hpp"
#include <mutex>
#include "PacketCapture.hpp"

class CapturePage : public Pages
{
private:
    mutable std::mutex mtx;
    
    char* port;

public:
    explicit CapturePage(){}

    Pages* display() override;

    void show();
};
