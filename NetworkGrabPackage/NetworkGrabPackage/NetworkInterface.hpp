#pragma once

//网络接口管理类
#include <cstring>
#include <vector>
#include <pcap.h>


class NetworkInterface
{
private:
	pcap_if_t* alldevs;		//所有网络接口(链表)

protected:
	std::vector<const char*> devices;
	std::vector<const char*> devicesDescription;

public:
	NetworkInterface();
	~NetworkInterface();

	pcap_if_t* getAllDevices();				//获取所有网络接口
	void printAllDevices();					//打印所有网络接口信息
	std::vector<const char*> findDevices();		//根据名称查找网络接口
	uint32_t getIPV4SubnetMask(const char* name);
	const char* getIPV6SubnetMask(const char* name);

};