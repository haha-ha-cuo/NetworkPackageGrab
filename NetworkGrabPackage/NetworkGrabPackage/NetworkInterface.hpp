#pragma once
//网络接口管理类
#include <cstring>
#include <Map>
#include <pcap.h>

using namespace std;

class NetworkInterface
{
private:

	pcap_if_t* alldevs; //所有网络接口(链表)

	map<const char*, pcap_if_t*> deviceMap; //名称到接口的映射

public:

	NetworkInterface();
	~NetworkInterface();

	//获取所有网络接口
	pcap_if_t* getAllDevices();

	//打印所有网络接口信息
	void printAllDevices();

	//根据名称查找网络接口
	pcap_if_t* findDeviceByName(const char* name);

	uint32_t getIPV4SubnetMask(const char* name);

	const char* getIPV6SubnetMask(const char* name);

};