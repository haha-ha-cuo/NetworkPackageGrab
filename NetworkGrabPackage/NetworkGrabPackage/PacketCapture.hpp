#pragma once

//抓包核心类
#include "PacketFilter.hpp"
#include "ProtocolDecoder.hpp"
#include <unordered_map>
#include <pcap.h>
#include <vector>
#include <mutex>

class PacketCapture
{
private:
	struct pcap_pkthdr* header;							//包文头指针
	const u_char* pktData;								//包数据指针
	int result;											//抓包结果
	pcap_t* handle;										//抓包句柄
	char errorbuf[PCAP_ERRBUF_SIZE];					//错误信息缓冲区
	std::unordered_map<time_t, const u_char*> packetMap;		//使用包ID作为键，包数据作为值
	int Counter;										//包计数器
	PacketFilter packetFilter;							//包过滤器对象
	ProtocolDecoder* protocolDecoderEthernet;			//协议解码器对象
	ProtocolDecoder* protocolDecoderIPV4;				//协议解码器对象
	ProtocolDecoder* protocolDecoderTCP;				//协议解码器对象
	pcap_dumper_t* dumper;
	mutable std::mutex mtx;							//互斥锁，保护packetMap的线程安全
	
	
protected:

	struct dataPkg {
		char ts[16];
		u_short sPort;
		u_short dPort;
		bpf_u_int32 size;
	};
	
	std::vector<dataPkg> dataPkgList; //存储包的时间戳、源端口、目的端口和大小
	
public:
	PacketCapture();
	~PacketCapture();

	void startCapture(const char* deviceName, const char* port);	//开始抓包
	void closeCapture();											//关闭抓包
	std::unordered_map<time_t, const u_char*> getPacketMap();	//获取包数据

	std::vector<dataPkg> getDataPkgList() const {
		std::lock_guard<std::mutex> lock(mtx);
		return dataPkgList;
	}
};

