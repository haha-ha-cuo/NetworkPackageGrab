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
    void analyzeInputOutput(const std::string &inputPath, const std::string &outputPath);
    std::string parseTLSClientHello(const std::vector<uint8_t> &payload);
};