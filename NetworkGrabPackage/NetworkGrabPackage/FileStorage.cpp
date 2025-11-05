#include "FileStorage.hpp"
#include <iostream>
#include <iomanip>
#include <sstream>
#include <json.hpp>

using json = nlohmann::json;
using namespace std;

FileStorage::FileStorage() {
}
FileStorage::~FileStorage() {
}

string FileStorage::bytesToHexString(const u_char* data, int length) {
    stringstream ss;
    ss << hex << setfill('0');
    for (int i = 0; i < length; ++i) {
        ss << setw(2) << static_cast<int>(data[i]);
    }
    return ss.str();
}

vector<u_char> FileStorage::hexStringToBytes(const string& hexStr) {
    vector<u_char> bytes;
    for (size_t i = 0; i < hexStr.length(); i += 2) {
        string byteString = hexStr.substr(i, 2);
        u_char byte = static_cast<u_char>(strtol(byteString.c_str(), nullptr, 16));
        bytes.push_back(byte);
    }
    return bytes;
}

bool FileStorage::Packets2Json(const unordered_map<time_t, const u_char*>& packetMap, const string& filename) {
    json packetsArray = json::array();

    if (packetMap.empty()) {
        cout << "[Info]No packets captured. Creating test packets for JSON file." << endl;
    }
    else {
        // 处理实际捕获的数据包
        for (const auto& pair : packetMap) {
            json packetObj;
            time_t timestamp = pair.first;
            const u_char* data = pair.second;

            packetObj["timestamp"] = timestamp;
            // 假设每个数据包的长度为100字节
            packetObj["data"] = bytesToHexString(data, 100);

            packetsArray.push_back(packetObj);
        }
    }

    // 写入文件
    ofstream file(filename);
    if (!file.is_open()) {
        cerr << "无法打开文件进行写入: " << filename << endl;
        return false;
    }

    file << setw(4) << packetsArray << endl;
    file.close();

    cout << "[Info]Successfully wrote " << packetsArray.size() << " packets to JSON file: " << filename << endl;
    return true;
}

std::vector<std::pair<time_t, std::vector<u_char>>> FileStorage::readPacketsFromJson(const std::string& filename) {
    std::vector<std::pair<time_t, std::vector<u_char>>> packets;

    try {
        std::ifstream file(filename);
        if (!file.is_open()) {
            std::cerr << "无法打开文件: " << filename << std::endl;
            return packets;
        }

        json packetsJson;
        file >> packetsJson;

        for (const auto& packetObj : packetsJson) {
            time_t timestamp = packetObj["timestamp"];
            std::string hexData = packetObj["data"];
            std::vector<u_char> data = hexStringToBytes(hexData);

            packets.emplace_back(timestamp, data);
        }

        std::cout << "成功从JSON文件读取 " << packets.size() << " 个数据包" << std::endl;
    }
    catch (const std::exception& e) {
        std::cerr << "读取JSON文件时发生错误: " << e.what() << std::endl;
    }

    return packets;
}


/*#include "FileStorage.hpp"
#include <iostream>
#include <iomanip>
#include <sstream>
#include <json.hpp>

using json = nlohmann::json;
using namespace std;

FileStorage::FileStorage() {
}
FileStorage::~FileStorage() {
}

string FileStorage::bytesToHexString(const u_char* data, int length) {
    stringstream ss;
    ss << hex << setfill('0');
    for (int i = 0; i < length; ++i) {
        ss << setw(2) << static_cast<int>(data[i]);
    }
    return ss.str();
}

vector<u_char> FileStorage::hexStringToBytes(const string& hexStr) {
    vector<u_char> bytes;
    for (size_t i = 0; i < hexStr.length(); i += 2) {
        string byteString = hexStr.substr(i, 2);
        u_char byte = static_cast<u_char>(strtol(byteString.c_str(), nullptr, 16));
        bytes.push_back(byte);
    }
    return bytes;
}

bool FileStorage::Packets2Json(const map<const time_t, const u_char*>& packetMap, const string& filename) {
    json j;
    for (const auto& pair : packetMap) {
        time_t timestamp = pair.first;
        const u_char* data = pair.second;
        // 假设每个数据包的长度是100字节,后期需要更改！！！
        string hexData = bytesToHexString(data,100);
        j[to_string(timestamp)] = hexData;
    }
    ofstream file(filename);
    if (!file.is_open()) {
        cerr << "无法打开文件进行写入: " << filename << endl;
        return false;
    }
    file << setw(4) << j << endl; // 美化输出
    file.close();
    return true;
}
std::vector<std::pair<time_t, std::vector<u_char>>> FileStorage::readPacketsFromJson(const std::string& filename) {
    std::vector<std::pair<time_t, std::vector<u_char>>> packets;

    try {
        std::ifstream file(filename);
        if (!file.is_open()) {
            std::cerr << "无法打开文件: " << filename << std::endl;
            return packets;
        }

        json packetsJson;
        file >> packetsJson;

        for (const auto& packetJson : packetsJson) {
            time_t timestamp = packetJson["timestamp"];
            std::string hexData = packetJson["data"];
            std::vector<u_char> data = hexStringToBytes(hexData);

            packets.emplace_back(timestamp, data);
        }

        std::cout << "成功从JSON文件读取 " << packets.size() << " 个数据包" << std::endl;
    }
    catch (const std::exception& e) {
        std::cerr << "读取JSON文件时发生错误: " << e.what() << std::endl;
    }

    return packets;
}*/