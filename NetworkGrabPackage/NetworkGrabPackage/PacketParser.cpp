#include "PacketParser.h"
#include<iostream>
#include<time.h>

using namespace std;

PacketParser::PacketParser()
{
}

PacketParser::~PacketParser()
{
}

void PacketParser::startParse(const char* deviceName)
{
	startCapture(deviceName);                                              //���û����ץ������
	pcap_t* handle = getHandle();                                          //��ȡץ�����
	if (handle == nullptr) {
		return;                                                            //��������Ч��ֱ�ӷ���
	}
	while ((result = pcap_next_ex(handle, &header, &pktData)) >= 0) {
		if (result == 0) {
			continue;     
		}
		// ��ӡʱ����ͳ���
		cout << "[Info]Packet captured." << endl;
		const time_t ts = header->ts.tv_sec;
		struct tm tmDest;
        errno_t erro = localtime_s( &tmDest , &ts);
		char timestr[16];
		strftime(timestr, sizeof(timestr), "%H:%M:%S", &tmDest);
		printf("[Info]ʱ��: %s.%06ld ����: %d �ֽ�\n", timestr, header->ts.tv_usec, header->len);
	}
	if (result == -1) {
		cerr << "[Error]Error reading the packets: " << pcap_geterr(handle) << endl;
	}
	closeCapture(); //�ر�ץ��
}