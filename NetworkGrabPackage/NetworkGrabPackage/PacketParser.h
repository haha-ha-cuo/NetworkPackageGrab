#pragma once
//��������
#include "PacketCapture.h"
class PacketParser: public PacketCapture
{
private:

	

public:

	PacketParser();
	~PacketParser();

	void startParse(const char* deviceName); //��ʼ����
};