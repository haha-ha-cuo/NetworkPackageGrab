#pragma once
//ץ�������ࣨ������ӿڣ�
#include <pcap.h>

class PacketCapture
{
private:

	pcap_t* handle; //ץ�����

	char errorbuf[PCAP_ERRBUF_SIZE]; //������Ϣ������

public:

	PacketCapture();
	~PacketCapture();

	void startCapture(const char* deviceName);//��ʼץ��

	void closeCapture();//�ر�ץ��

	pcap_t* getHandle(); //��ȡץ�����
};

