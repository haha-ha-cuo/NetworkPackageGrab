#include "PacketFilter.h"
#include <iostream>

using namespace std;


PacketFilter::PacketFilter() {
}

PacketFilter::~PacketFilter() {
}

void PacketFilter::setFilter(const char* filterExp,pcap_t* handle,const char* name) {
	char rpcapName[100];

	snprintf(rpcapName, sizeof(rpcapName), "rpcap://%s", name);

	fcode = new bpf_program;
	if (pcap_compile(handle, fcode, filterExp, 1, networkInterface.getIPV4SubnetMask(rpcapName)) < 0) {
		cerr << "[Error]Could not compile filter " << filterExp << ": " << pcap_geterr(handle) << endl;
		delete fcode;
		return;
	}
	if (pcap_setfilter(handle, fcode) < 0) {
		cerr << "[Error]Could not set filter " << filterExp << ": " << pcap_geterr(handle) << endl;
		pcap_freecode(fcode);
		delete fcode;
		return;
	}
	cout << "[Info]Filter set successfully: " << filterExp << endl;
	pcap_freecode(fcode);
	delete fcode;
}