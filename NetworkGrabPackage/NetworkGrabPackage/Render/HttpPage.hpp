#pragma once
#include "Pages.hpp"
#include <vector>
#include <string>

class HttpPage : public Pages
{
public:
    HttpPage();
    ~HttpPage();
    Pages *display() override;

private:
    void listFiles(std::vector<std::string> &files);
    void analyzeFile(const std::string &filepath);
    std::string parseTLSClientHello(const std::vector<uint8_t> &payload);
};