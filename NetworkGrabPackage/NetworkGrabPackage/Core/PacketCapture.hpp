#pragma once

// 抓包核心类
#include "Consts.hpp"
#include "PacketFilter.hpp"
#include "PacketManager.hpp"
#include "PacketFactory.hpp"
#include <pcap.h>
#include <vector>

class PacketCapture
{
private:
    struct pcap_pkthdr *header;
    const u_char *pktData;
    int result;
    pcap_t *handle;
    char errorbuf[PCAP_ERRBUF_SIZE];
    PacketManager packetManager;
    int Counter;
    PacketFilter packetFilter;
    pcap_dumper_t *dumper;
    struct dataPkg
    {
        char ts[16];
        u_short sPort;
        u_short dPort;
        bpf_u_int32 size;
    };
protected:
	vector<dataPkg> dataVector;

public:
    PacketCapture();
    ~PacketCapture();

    void startCapture(const char *deviceName, const char *port);
    void closeCapture();
    const PacketManager &getPacketManager() const;
};
