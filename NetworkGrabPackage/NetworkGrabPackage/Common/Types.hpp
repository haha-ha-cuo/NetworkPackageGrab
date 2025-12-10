#pragma once

#include <pcap.h>

enum class PacketDirection
{
    Incoming,
    Outgoing,
    Both
};

enum class StorageType
{
    Memory,
    File
};

enum class NetworkInterfaceType
{
    Ethernet,
    WiFi,
    Loopback,
    Other
};

enum class PacketType
{
    ETHERNET,
    IP,
    TCP,
    UDP,
    ARP,
    UNKNOWN
};

typedef struct ip_address
{
    u_char byte1;
    u_char byte2;
    u_char byte3;
    u_char byte4;
} ip_address;

typedef struct ipHeader
{
    u_char ver_ihl;
    u_char tos;
    u_short tlen;
    u_short identification;
    u_short flags_fo;
    u_char ttl;
    u_char proto;
    u_short crc;
    ip_address saddr;
    ip_address daddr;
    u_int op_pad;
} ipHeader;
