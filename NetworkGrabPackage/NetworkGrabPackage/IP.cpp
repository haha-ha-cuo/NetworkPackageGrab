#include "IP.h"

void IP::Parse() {
	if (raw_data.size() < 20) {
		throw::runtime_error("Data too short to be an IP packet");
	}

	uint8_t version = (raw_data[0] >> 4) & 0x0F;
	if (version != 4) {
		throw::runtime_error("Not an IPv4 packet");
	}//解析IP版本和头长度

	total_length = (static_cast<uint16_t>(raw_data[2]) << 8) | static_cast<uint16_t>(raw_data[3]);//总长度

	ttl = raw_data[8];//生存时间

	protocol = raw_data[9];//协议类型

	src_ip = to_string(static_cast<int>(raw_data[12])) + "." +
		to_string(static_cast<int>(raw_data[13])) + "." +
		to_string(static_cast<int>(raw_data[14])) + "." +
		to_string(static_cast<int>(raw_data[15]));//源IP地址

	dst_ip = to_string(static_cast<int>(raw_data[16])) + "." +
		to_string(static_cast<int>(raw_data[17])) + "." +
		to_string(static_cast<int>(raw_data[18])) + "." +
		to_string(static_cast<int>(raw_data[19]));//目的IP地址
}

void IP::display() const {
	cout << "IP Packet:" << endl;
	cout << "  Source IP: " << src_ip << endl;
	cout << "  Destination IP: " << dst_ip << endl;
	cout << "  Protocol: " << static_cast<int>(protocol) << endl;
	cout << "  Total Length: " << total_length << " bytes" << endl;
	cout << "  TTL: " << static_cast<int>(ttl) << endl;
	cout << "Pakcet Size: " << GetPacketSize() << " bytes" << endl;
}

string IP::getSummary() const{
	return "IP: " + src_ip + " -> " + dst_ip + " Proto:" + to_string(protocol);
}