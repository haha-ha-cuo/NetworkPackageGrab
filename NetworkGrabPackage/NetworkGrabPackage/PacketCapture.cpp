﻿#include "Contants.hpp"
#include "PacketCapture.hpp"
#include <iostream>
#include <pcap.h>
#include "IPDecoder.hpp"
#include "EthernetDecoder.hpp"
#include "TCPDecoder.hpp"

using namespace std;
using namespace Contants;

PacketCapture::PacketCapture() {
	handle = nullptr;
	header = nullptr;
	pktData = nullptr;
	result = 0;
	Counter = 200; //循环10次（之后用线程控制）
	protocolDecoderIPV4 = new IPDecoder(); //暂时只支持IP协议解码
	protocolDecoderEthernet = new EthernetDecoder();
	protocolDecoderTCP = new TCPDecoder();
	
};

PacketCapture::~PacketCapture() {
	closeCapture();
};

void PacketCapture::startCapture(const char* deviceName) {

	cout << "[Info]Starting packet capture on device: " << deviceName << endl;

	handle = pcap_open_live(
		deviceName,
		MAX_PACKET_SIZE,
		1,
		3000,
		errorbuf
	);

	if (handle == nullptr) {
		cerr << "[Warn]Could not open device " << deviceName << ": " << errorbuf << endl;//模拟日志，前端log
		return;
	}

	cout << "[Info]Packet capture started on device: " << deviceName << endl;

	packetFilter.setFilter("tcp port 80", handle, deviceName); //设置过滤器，只捕获ip包

	int capturedPackets = 0;
	int timeoutCount = 0;
	const int maxTimeouts = 20; // 最大超时次数

	

	while ((result = pcap_next_ex(handle, &header, &pktData)) >= 0&&capturedPackets<20) {
		if (result == 0) {
			cout << "[Warn]Timeout elapsed.\n\n" << endl;
			continue;
		}

		timeoutCount = 0;
		capturedPackets++;

		// 打印时间戳和长度
		cout << "[Info]Packet captured." << endl;
		const time_t ts = header->ts.tv_sec;
		struct tm tmDest;
		errno_t erro = localtime_s(&tmDest, &ts);
		char timestr[16];
		strftime(timestr, sizeof(timestr), "%H:%M:%S", &tmDest);
		printf("[Info]时间: %s.%06ld 长度: %d 字节\n", timestr, header->ts.tv_usec, header->len);

		//调用协议解码器
		protocolDecoderEthernet->packetHandle(pktData);
		protocolDecoderIPV4->packetHandle(pktData);
		protocolDecoderTCP->packetHandle(pktData);

		//数据存储
		//packetMap.insert(pair<const time_t, const u_char* >(ts, pktData));
		cout << "[Info]Packet stored. Total packets stored: " << packetMap.size() << endl;

		char timeName[16];
		strftime(timeName, sizeof(timeName), "%H-%M-%S", &tmDest);

		char name[100];
		snprintf(name, sizeof(name), "../Output/%s.pcap", timeName);

		//数据转储
		try {
			dumper = pcap_dump_open(handle, name);
			if (!dumper) {
				cerr << "[Error] Failed to open dump file: " << name<< endl;
				continue;
			}
			pcap_dump((u_char*)dumper, header, pktData);
			pcap_dump_close(dumper);
		}
		catch (...) {
			cerr << "[Error]Failed to save dump file: " << name<< endl;
			continue;
		}
		cout << "\n" << endl;

	}
	if (result == -1) {
		cerr << "[Error]Error reading the packets: " << pcap_geterr(handle) << endl;
	}
	else if (capturedPackets == 0) {
		cout << "[Warn]No packets captured after " << maxTimeouts << " timeouts." << endl;
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