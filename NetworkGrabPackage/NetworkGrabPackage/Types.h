#pragma once
//�����������Ͷ���

	enum class PacketDirection
	{
		Incoming, //��վ
		Outgoing, //��վ
		Both      //˫��
	};
	//�洢����
	enum class StorageType
	{
		Memory, //�ڴ�洢
		File    //�ļ��洢
	};
	//����ӿ�����
	enum class NetworkInterfaceType
	{
		Ethernet, //��̫��
		WiFi,     //������
		Loopback, //���ؽӿ�
		Other     //��������
	};

	enum MyEnum
	{
		ETHERNET,
		IP,
		TCP,
		UDP,
		ARP
	};
