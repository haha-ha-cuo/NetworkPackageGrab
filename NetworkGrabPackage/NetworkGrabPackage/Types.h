#pragma once
//公共数据类型定义

	enum class PacketDirection
	{
		Incoming, //入站
		Outgoing, //出站
		Both      //双向
	};
	//存储类型
	enum class StorageType
	{
		Memory, //内存存储
		File    //文件存储
	};
	//网络接口类型
	enum class NetworkInterfaceType
	{
		Ethernet, //以太网
		WiFi,     //无线网
		Loopback, //环回接口
		Other     //其他类型
	};

	enum MyEnum
	{
		ETHERNET,
		IP,
		TCP,
		UDP,
		ARP
	};
