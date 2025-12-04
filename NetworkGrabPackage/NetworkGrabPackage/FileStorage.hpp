#pragma once

//文件存储实现
#include <string>
#include <fstream>
#include <unordered_map>
#include <pcap.h>
#include <vector>


class FileStorage
{
public:
    FileStorage();
    ~FileStorage();

    bool Packets2Json(const std::unordered_map<time_t, const u_char*>& packetMap, const std::string& filename); // 将数据包写入JSON文件
    std::vector<std::pair<time_t, std::vector<u_char>>> readPacketsFromJson(const std::string& filename);       // 从JSON文件读取数据包
    void printOutputFiles();                                                                                    //从Output文件夹中读取文件名称及大小

private:
    std::string bytesToHexString(const u_char* data, int length);       // 将二进制数据转换为十六进制字符串
    std::vector<u_char> hexStringToBytes(const std::string& hexStr);    // 将十六进制字符串转换回二进制数据

};