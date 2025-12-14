#pragma once
#include <string>
#include <unordered_map>
#include <vector>
#include <cstdint>

class HTTP
{
private:
    bool valid = false; // 是否是http包
    std::string method;
    std::string uri;
    std::string version;
    std::unordered_map<std::string, std::string> headers;
    std::string body;

    void parse(const std::string &data);

public:
    HTTP(const std::vector<uint8_t> &payload);
    ~HTTP() {}
    bool is_valid() const { return valid; }
    void display() const;
    std::string getSummary() const;
    std::string getFullUrl() const;
    std::string getMethod() const { return method; }
};