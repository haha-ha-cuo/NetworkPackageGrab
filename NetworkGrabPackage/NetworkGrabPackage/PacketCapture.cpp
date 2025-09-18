#include "PacketCapture.h"
#include <iostream>
#include "Contants.h"
#include <pcap.h>

using namespace std;
using namespace Contants;

PacketCapture::PacketCapture(){
	handle = nullptr;
	header = nullptr;
	pktData = nullptr;
	result = 0;
	Counter = 30; //ѭ��10�Σ�֮�����߳̿��ƣ�
};

PacketCapture::~PacketCapture() {
	closeCapture();
};

void PacketCapture::startCapture(const char* deviceName) {

	cout << "[Info]Starting packet capture on device: " << deviceName << endl;

	handle = pcap_open_live(deviceName, 
		MAX_PACKET_SIZE, 
		1, 
		DEFAULT_SESSION_TIMEOUT, 
		errorbuf
	);

	if (handle == nullptr) {
		cerr << "[Warn]Could not open device " << deviceName << ": " << errorbuf << endl;//ģ����־��ǰ��log
		return;
	}

	cout << "[Info]Packet capture started on device: " << deviceName << endl;

	while ((result = pcap_next_ex(handle, &header, &pktData)) >= 0 && Counter--) {
		if (result == 0) {
			cout << "[Info]Timeout elapsed." << endl;
			continue;
		}

		// ��ӡʱ����ͳ���
		cout << "[Info]Packet captured." << endl;
		const time_t ts = header->ts.tv_sec;
		struct tm tmDest;
		errno_t erro = localtime_s(&tmDest, &ts);
		char timestr[16];
		strftime(timestr, sizeof(timestr), "%H:%M:%S", &tmDest);
		printf("[Info]ʱ��: %s.%06ld ����: %d �ֽ�\n", timestr, header->ts.tv_usec, header->len);

		//���ݴ洢
		packetMap.insert( pair<const time_t, const u_char* >(ts, pktData));
		cout << "[Info]Packet stored. Total packets stored: " << packetMap.size() << endl;

	}
	if (result == -1) {
		cerr << "[Error]Error reading the packets: " << pcap_geterr(handle) << endl;
	}
}

void PacketCapture::closeCapture() {
	if (handle) {
		pcap_close(handle);
		cout << "[Info]Packet capture closed." << endl;
	}
}

map <const time_t, const u_char*> PacketCapture::getPacketMap() {
	return packetMap;
}