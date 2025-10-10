#pragma once
//包解析器
#include "PacketCapture.hpp"
#include "FileStorage.hpp"
#include <vector>
#include <string>
class PacketParser : public PacketCapture
{
private:
	FileStorage fileStorage;
public:

	PacketParser();
	~PacketParser();

	void startParse(const char* deviceName); //开始解析
	bool saveToFile(const std::string& filename); //保存到文件
	bool loadAndParseFromJson(const std::string& filename); //从JSON文件加载并解析
	void parsePacket(const u_char* data, time_t timetamp); //解析单个包
};