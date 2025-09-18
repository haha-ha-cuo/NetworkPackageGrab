#pragma once
//ץ�������ࣨ������ӿڣ�
#include <pcap.h>
#include<Map>

using namespace std;

class PacketCapture
{
private:

	struct pcap_pkthdr* header;//����ͷָ��

	const u_char* pktData;//������ָ��

	int result;//ץ�����

	pcap_t* handle; //ץ�����

	char errorbuf[PCAP_ERRBUF_SIZE]; //������Ϣ������

	map <const time_t, const u_char*> packetMap; //ʹ�ð�ID��Ϊ������������Ϊֵ

	int Counter;
public:

	PacketCapture();
	~PacketCapture();

	void startCapture(const char* deviceName);//��ʼץ��

	void closeCapture();//�ر�ץ��

	map <const time_t, const u_char*> getPacketMap();//��ȡ������
};

