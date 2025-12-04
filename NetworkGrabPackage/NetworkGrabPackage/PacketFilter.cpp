#include "PacketFilter.hpp"
#include <iostream>

using namespace std;


PacketFilter::PacketFilter() {}

PacketFilter::~PacketFilter() {}

void PacketFilter::setFilter(const char* filterExp, pcap_t* handle, const char* name) {
	fcode = new bpf_program;
	if (pcap_compile(handle, fcode, filterExp, 1, networkInterface.getIPV4SubnetMask(name)) < 0) {
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