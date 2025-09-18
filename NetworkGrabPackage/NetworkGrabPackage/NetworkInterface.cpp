#include "NetworkInterface.h"
#include<cstring>
#include<stdexcept>
#include<iostream>
using namespace std;

NetworkInterface::NetworkInterface()
{
	char errbuf[PCAP_ERRBUF_SIZE];

	if (pcap_findalldevs_ex(PCAP_SRC_IF_STRING, NULL, &alldevs, errbuf) == -1) {
		throw std::runtime_error(std::string("Error finding devices: ") + errbuf);
	}

	for (pcap_if_t* device = alldevs; device != nullptr; device = device->next) {
		deviceMap.insert(pair< const char*, pcap_if_t* >( device->name, device ));
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