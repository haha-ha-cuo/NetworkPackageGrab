#pragma once
//公共数据类型定义
#include <pcap.h>

enum class PacketDirection
{
	Incoming, //入栈
	Outgoing, //出栈
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

// IPv4地址
typedef struct ip_address {
	u_char byte1;
	u_char byte2;
	u_char byte3;
	u_char byte4;
}ip_address;

/* IPv4 header */
typedef struct ip_header {
	u_char  ver_ihl; // Version (4 bits) + IP header length (4 bits)
	u_char  tos;     // Type of service 
	u_short tlen;    // Total length 
	u_short identification; // Identification
	u_short flags_fo; // Flags (3 bits) + Fragment offset (13 bits)
	u_char  ttl;      // Time to live
	u_char  proto;    // Protocol
	u_short crc;      // Header checksum
	ip_address  saddr; // Source address
	ip_address  daddr; // Destination address
	u_int  op_pad;     // Option + Padding
}ip_header;

/* UDP header*/
typedef struct udp_header {
	u_short sport; // Source port
	u_short dport; // Destination port
	u_short len;   // Datagram length
	u_short crc;   // Checksum
}udp_header;