#pragma once
//��������
#include "PacketCapture.h"
class PacketParser: public PacketCapture
{
private:

	struct pcap_pkthdr* header;//����ͷָ��

	const u_char* pktData;//������ָ��

	int result;//ץ�����

public:

	PacketParser();
	~PacketParser();

	void startParse(const char* deviceName); //��ʼ����
};