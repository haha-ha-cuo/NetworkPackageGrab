#include "IPDecoder.hpp"
#include "Types.hpp"

#include <iostream>

using namespace std;

void IPDecoder::packetHandle(const u_char* data_pkt) {
	 // IP header
	ih = (ipHeader*)(data_pkt + 14); //length of ethernet header
	u_int ip_len = (ih->ver_ihl & 0x0F) * 4; //计算IP头长度
	cout << "[Info]------- IP Header -------" << endl;
	cout << "[Info]Version: " << ((ih->ver_ihl >> 4) & 0x0F) << endl;
	cout << "[Info]Header Length: " << (unsigned int)ip_len << " bytes" << endl;
	cout << "[Info]Type of Service: " << (unsigned int)ih->tos << endl;
	cout << "[Info]Total Length: " << ntohs(ih->tlen) << " bytes" << endl;
	cout << "[Info]Identification: " << ntohs(ih->identification) << endl;
	cout << "[Info]Flags: " << ((ntohs(ih->flags_fo) >> 13) & 0x07) << endl;
	cout << "[Info]Fragment Offset: " << (ntohs(ih->flags_fo) & 0x1FFF) << endl;
	cout << "[Info]Time to Live: " << (unsigned int)ih->ttl << endl;
	cout << "[Info]Protocol: " << (unsigned int)ih->proto << endl;
	cout << "[Info]Header Checksum: " << ntohs(ih->crc) << endl;
	cout << "[Info]Source Address: " << (unsigned int)ih->saddr.byte1 << "." << (unsigned int)ih->saddr.byte2 << "." << (unsigned int)ih->saddr.byte3 << "." << (unsigned int)ih->saddr.byte4 << endl;
	cout << "[Info]Destination Address: " << (unsigned int)ih->daddr.byte1 << "." << (unsigned int)ih->daddr.byte2 << "." << (unsigned int)ih->daddr.byte3 << "." << (unsigned int)ih->daddr.byte4 << endl;
	cout << "[Info]------------------------" << endl;
}