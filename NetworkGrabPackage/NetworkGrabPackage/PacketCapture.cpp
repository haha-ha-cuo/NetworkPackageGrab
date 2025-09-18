#include "PacketCapture.h"
#include <iostream>
#include "Contants.h"
#include <pcap.h>

using namespace std;
using namespace Contants;

PacketCapture::PacketCapture(){};

PacketCapture::~PacketCapture() {};

void PacketCapture::startCapture(const char* deviceName) {

	cout << "[Info]Starting packet capture on device: " << deviceName << endl;

	handle = pcap_open_live(deviceName, 
		MAX_PACKET_SIZE, 
		1, 
		DEFAULT_SESSION_TIMEOUT, 
		errorbuf
	);

	if (handle == nullptr) {
		cerr << "[Warn]Could not open device " << deviceName << ": " << errorbuf << endl;//模拟日志，前端log
		return;
	}

	cout << "[Info]Packet capture started on device: " << deviceName << endl;
}

void PacketCapture::closeCapture() {
	if (handle) {
		pcap_close(handle);
		cout << "[Info]Packet capture closed." << endl;
	}
}

pcap* PacketCapture::getHandle() {
	return handle;
}