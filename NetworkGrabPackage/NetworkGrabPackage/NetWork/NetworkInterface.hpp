#pragma once

// 网络接口管理类
#include <cstring>
#include <vector>
#include <pcap.h>

using namespace std;

class NetworkInterface
{
private:
    pcap_if_t *alldevs;

protected:
    vector<const char *> devices;
    vector<const char *> devicesDescription;

public:
    NetworkInterface();
    ~NetworkInterface();

    pcap_if_t *getAllDevices();
    void printAllDevices();
    vector<const char *> findDevices();
    uint32_t getIPV4SubnetMask(const char *name);
    const char *getIPV6SubnetMask(const char *name);
};
