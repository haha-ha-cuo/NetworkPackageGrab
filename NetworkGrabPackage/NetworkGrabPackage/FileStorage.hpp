﻿#pragma once
//文件存储实现
#include <string>
#include <fstream>
#include <map>
#include <pcap.h>
#include <vector>

using namespace std;

class FileStorage {
public:
    FileStorage();
    ~FileStorage();

    // 将数据包写入JSON文件
    bool Packets2Json(const std::map<const time_t, const u_char*>& packetMap, const std::string& filename);
    
    // 从JSON文件读取数据包
    std::vector<std::pair<time_t, std::vector<u_char>>> readPacketsFromJson(const std::string& filename);

private:
    // 将二进制数据转换为十六进制字符串
    std::string bytesToHexString(const u_char* data, int length);
    
    // 将十六进制字符串转换回二进制数据
    std::vector<u_char> hexStringToBytes(const std::string& hexStr);
};