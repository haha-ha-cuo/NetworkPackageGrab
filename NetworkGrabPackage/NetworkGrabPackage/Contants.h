#pragma once
//常量定义头文件
namespace Contants {
	static const int MAX_PACKET_SIZE = 65536; //最大数据包大小
	static const int DEFAULT_BUFFER_SIZE = 1024 * 1024; //默认缓冲区大小1MB
	static const int DEFAULT_SESSION_TIMEOUT = 300; //默认会话超时时间300秒
	static const int MAX_SESSIONS = 10000; //最大会话数
	static const int TRAFFIC_STAT_INTERVAL = 60; //流量统计时间间隔60秒
}