#include "NetworkInterface.h"
#include<cstring>
#include<stdexcept>
#include<iostream>
#include <ws2tcpip.h>
#include <winsock2.h>

using namespace std;

NetworkInterface::NetworkInterface()
{
	char errbuf[PCAP_ERRBUF_SIZE];

	if (pcap_findalldevs_ex(PCAP_SRC_IF_STRING, NULL, &alldevs, errbuf) == -1) {
		throw std::runtime_error(std::string("Error finding devices: ") + errbuf);
	}

	for (pcap_if_t* device = alldevs; device != nullptr; device = device->next) {
		deviceMap.insert(pair< const char*, pcap_if_t* >(device->name, device));
	}
}

NetworkInterface::~NetworkInterface()
{
	if (alldevs) {
		pcap_freealldevs(alldevs);
	}
}

pcap_if_t* NetworkInterface::getAllDevices()
{
	return alldevs;
}

pcap_if_t* NetworkInterface::findDeviceByName(const char* name)
{
	auto it = deviceMap.find(name);
	if (it != deviceMap.end()) {
		return it->second;
	}
	return nullptr;
}

void NetworkInterface::printAllDevices()
{
	for (pcap_if_t* device = alldevs; device != nullptr; device = device->next) {
		std::cout << device->name;
		if (device->description)
			std::cout << " (" << device->description << ")";
		std::cout << std::endl;
	}
}

uint32_t NetworkInterface::getIPV4SubnetMask(const char* name)
{
	//pcap_if_t* device = deviceMap[name];

	try{
		for(pcap_if_t* device = alldevs; device != nullptr; device = device->next) {
			if(strcmp(device->name,name)==0){
				for (pcap_addr_t* addr = device->addresses; addr != nullptr; addr = addr->next) {
					if (addr->addr && addr->addr->sa_family == AF_INET) {
						if (addr->netmask) {
							struct sockaddr_in* netmask = (struct sockaddr_in*)addr->netmask;
							struct in_addr* inaddr = &(netmask->sin_addr);
							char* buf = new char[INET_ADDRSTRLEN];
							cout << "[Info]Subnet Mask for device " << name << ": " << inet_ntop(AF_INET, inaddr, buf, INET_ADDRSTRLEN) << endl;
							return netmask->sin_addr.S_un.S_addr;
						}
					}
				}
			}
		}
		
		throw runtime_error("Device not found or no IPv4 address available.");
		return NULL;
	}
	catch (const std::runtime_error& e) {
		std::cerr << "Defualt：" << e.what() << std::endl;
	}
	
}

const char * NetworkInterface::getIPV6SubnetMask(const char* name)
{
	try {
		for (pcap_if_t* device = alldevs; device != nullptr; device = device->next) {
			if (strcmp(device->name, name) == 0) {
				for (pcap_addr_t* addr = device->addresses; addr != nullptr; addr = addr->next) {
					if (addr->addr && addr->addr->sa_family == AF_INET6) {
						if (addr->netmask) {
							struct sockaddr_in6* netmask = (struct sockaddr_in6*)addr->netmask;
							struct in6_addr* inaddr = &(netmask->sin6_addr);
							char* buf = new char[INET6_ADDRSTRLEN];
							cout << "[Info]Subnet Mask for device " << name << ": " << inet_ntop(AF_INET6, inaddr, buf, INET6_ADDRSTRLEN) << endl;
							return buf;
						}
					}
				}
			}
		}
		throw runtime_error("Device not found or no IPv6 address available.");
		return nullptr;
	}
	catch (const std::runtime_error& e) {
		std::cerr << "Defualt：" << e.what() << std::endl;
	}
}