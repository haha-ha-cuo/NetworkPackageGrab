#include "PacketParser.hpp"
#include "EthernetDecoder.hpp"
#include "IPDecoder.hpp"
#include "TCPDecoder.hpp"
#include<iostream>
#include<time.h>

using namespace std;

PacketParser::PacketParser(){}

PacketParser::~PacketParser(){}

void PacketParser::startParse(const char* deviceName) {
	startCapture(deviceName,"tcp port 80");
}

bool PacketParser::saveToFile(const std::string& filename) {
	return fileStorage.Packets2Json(getPacketMap(), filename);
}

bool PacketParser:: loadAndParseFromJson(const std::string& filename) {
	auto packets = fileStorage.readPacketsFromJson(filename);
	if(packets.empty()) {
		cerr << "No packets loaded from JSON file: " << filename << endl;
		return false;
	}
	EthernetDecoder ethernetDecoder;
	IPDecoder ipDecoder;
	TCPDecoder tcpDecoder;
	for (const auto& packet : packets) {
		time_t timetamp = packet.first;
		const u_char* data = packet.second.data();
		struct tm tmDest;
		localtime_s(&tmDest, &timetamp);
		char timestr[16];
		strftime(timestr, sizeof(timestr), "%H:%M:%S", &tmDest);
		printf("[Info]时间： %s 数据包 长度： %zu 字节\n", timestr, packet.second.size());
		ethernetDecoder.packetHandle(data);
		ipDecoder.packetHandle(data + sizeof(etherHeader));
		tcpDecoder.packetHandle(data + sizeof(etherHeader) + sizeof(ipHeader));
		cout << "\n" << endl;
	}
	cout << "[Info]解析完成" << endl;
	return true;
}

void PacketParser:: parsePacket(const u_char* data, time_t timetamp) {
	EthernetDecoder ethernetDecoder;
	IPDecoder ipDecoder;
	TCPDecoder tcpDecoder;

	struct tm tmDest;
	localtime_s(&tmDest, &timetamp);
	char timestr[16];
	strftime(timestr, sizeof(timestr), "%H:%M:%S", &tmDest);
	printf("[Info]时间： %s\n", timestr);
	ethernetDecoder.packetHandle(data);
	ipDecoder.packetHandle(data + sizeof(etherHeader));
	tcpDecoder.packetHandle(data + sizeof(etherHeader) + sizeof(ipHeader));
	cout << "\n" << endl;
}