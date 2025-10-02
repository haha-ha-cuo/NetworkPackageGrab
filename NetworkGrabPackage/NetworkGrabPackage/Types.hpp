﻿#pragma once
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
typedef struct ipHeader {
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
}ipHeader;

/* UDP header*/
typedef struct udpHeader {
	u_short sport; // Source port
	u_short dport; // Destination port
	u_short len;   // Datagram length
	u_short crc;   // Checksum
}udpHeader;

typedef struct etherHeader {
	u_char ether_dhost[6];  // 目标 MAC 地址
	u_char ether_shost[6];  // 源 MAC 地址
	u_short ether_type;     // 上层协议类型（如 IPv4、ARP）
} etherHeader;


/* TCP header */
typedef struct tcpHeader {
	u_short sport; // Source port
	u_short dport; // Destination port
	u_int seqnum;  // Sequence number
	u_int acknum;  // Acknowledgment number
	u_char dataOffsetReserved; // Data offset (4 bits) + Reserved (4 bits)
	u_char flags;  // Flags
	u_short window; // Window size
	u_short checksum; // Checksum
	u_short urgptr;  // Urgent pointer
}tcpHeader;