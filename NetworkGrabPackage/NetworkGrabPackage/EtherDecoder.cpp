#include "EthernetDecoder.hpp"

#include <iostream>

using namespace std;


void EthernetDecoder::packetHandle(const u_char* data_pkt)
{
	eth = (etherHeader*)data_pkt;
	cout << endl;
	cout << "[Info]------- Ethernet Header -------" << endl;
	cout << "[Info]Destination MAC: ";
	for (int i = 0; i < 6; i++) {
		printf("%02X", eth->ether_dhost[i]);
		if (i != 5) printf(":");
	}
	cout << endl;
	cout << "[Info]Source MAC: ";
	for (int i = 0; i < 6; i++) {
		printf("%02X", eth->ether_shost[i]);
		if (i != 5) printf(":");
	}
	cout << endl;
	cout << "[Info]EtherType: 0x" << hex << ntohs(eth->ether_type) << dec << endl;
	cout << "[Info]--------------------------------" << endl;
	cout << endl;
}