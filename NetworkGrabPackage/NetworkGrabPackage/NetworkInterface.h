#pragma once
//����ӿڹ�����
#include<pcap.h>
#include <Map>
#include <cstring>
using namespace std;
class NetworkInterface
{
private:

	pcap_if_t* alldevs; //��������ӿ�(����)

	map<const char*, pcap_if_t*> deviceMap; //���Ƶ��ӿڵ�ӳ��

public:

	NetworkInterface();
	~NetworkInterface();

	//��ȡ��������ӿ�
	pcap_if_t* getAllDevices();

	//��ӡ��������ӿ���Ϣ
	void printAllDevices();

	//�������Ʋ�������ӿ�
	pcap_if_t* findDeviceByName(const char* name);

};