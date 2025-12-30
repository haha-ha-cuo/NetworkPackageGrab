#include "HttpPage.hpp"
#include <algorithm>
#include <cctype>
#include <cstdint>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <limits>
#include <sstream>
#include <string>
#include <vector>

namespace fs = std::filesystem;
using namespace std;

namespace
{
    int HexNibble(char c)
    {
        if (c >= '0' && c <= '9')
            return c - '0';
        if (c >= 'a' && c <= 'f')
            return 10 + (c - 'a');
        if (c >= 'A' && c <= 'F')
            return 10 + (c - 'A');
        return -1;
    }

    bool HexToBytes(const std::string &hex, std::vector<std::uint8_t> &out)
    {
        if ((hex.size() % 2) != 0)
            return false;
        out.clear();
        out.reserve(hex.size() / 2);
        for (std::size_t i = 0; i < hex.size(); i += 2)
        {
            int hi = HexNibble(hex[i]);
            int lo = HexNibble(hex[i + 1]);
            if (hi < 0 || lo < 0)
                return false;
            out.push_back(static_cast<std::uint8_t>((hi << 4) | lo));
        }
        return true;
    }

    std::uint16_t ReadBE16(const std::uint8_t *p)
    {
        return (static_cast<std::uint16_t>(p[0]) << 8) | static_cast<std::uint16_t>(p[1]);
    }

    bool SplitTabs(const std::string &s, std::vector<std::string> &out)
    {
        out.clear();
        std::size_t start = 0;
        while (true)
        {
            std::size_t pos = s.find('\t', start);
            if (pos == std::string::npos)
            {
                out.push_back(s.substr(start));
                break;
            }
            out.push_back(s.substr(start, pos - start));
            start = pos + 1;
        }
        return !out.empty();
    }

    std::string ToUpperAscii(std::string s)
    {
        for (char &c : s)
            c = static_cast<char>(std::toupper(static_cast<unsigned char>(c)));
        return s;
    }

    bool StartsWithAny(const std::string &s, const std::vector<std::string> &prefixes)
    {
        for (const auto &p : prefixes)
        {
            if (s.size() >= p.size() && std::equal(p.begin(), p.end(), s.begin()))
                return true;
        }
        return false;
    }

    std::string FirstLine(const std::string &s)
    {
        std::size_t pos = s.find("\r\n");
        if (pos == std::string::npos)
            pos = s.find('\n');
        if (pos == std::string::npos)
            return s;
        return s.substr(0, pos);
    }

    std::string BytesToPrintableAscii(const std::vector<std::uint8_t> &buf)
    {
        std::string s;
        s.reserve(buf.size());
        for (std::uint8_t b : buf)
        {
            if (b == '\r' || b == '\n' || b == '\t')
            {
                s.push_back(static_cast<char>(b));
                continue;
            }
            if (b >= 32 && b <= 126)
                s.push_back(static_cast<char>(b));
            else
                s.push_back('.');
        }
        return s;
    }

    bool ExtractTcpPayload(const std::vector<std::uint8_t> &frame, std::vector<std::uint8_t> &payload)
    {
        payload.clear();
        if (frame.size() < 14)
            return false;
        const std::uint16_t etherType = ReadBE16(frame.data() + 12);
        if (etherType != 0x0800)
            return false;
        if (frame.size() < 14 + 20)
            return false;

        const std::uint8_t *ip = frame.data() + 14;
        const std::size_t ipAvail = frame.size() - 14;
        const std::uint8_t version = (ip[0] >> 4) & 0x0F;
        const std::size_t ihl = static_cast<std::size_t>(ip[0] & 0x0F) * 4;
        if (version != 4 || ihl < 20 || ihl > ipAvail)
            return false;
        const std::uint8_t ipProto = ip[9];
        if (ipProto != 6)
            return false;

        const std::size_t l4 = 14 + ihl;
        if (frame.size() < l4 + 20)
            return false;

        const std::uint8_t *tcp = frame.data() + l4;
        const std::size_t doff = static_cast<std::size_t>((tcp[12] >> 4) & 0x0F) * 4;
        if (doff < 20)
            return false;

        const std::size_t payloadOff = l4 + doff;
        if (payloadOff > frame.size())
            return false;

        payload.assign(frame.begin() + payloadOff, frame.end());
        return true;
    }
}

HttpPage::HttpPage() {}

HttpPage::~HttpPage() {}

Pages *HttpPage::display()
{
    while (true)
    {
        system("cls");
        cout << "=== HTTP Analysis Page ===" << endl;
        cout << "1: Analyze ../Output/input.txt + ../Output/output.txt" << endl;
        cout << "0: Return" << endl;
        cout << ">> ";

        int choice;
        cin >> choice;

        if (cin.fail())
        {
            cin.clear();
            cin.ignore((numeric_limits<streamsize>::max)(), '\n');
            continue;
        }

        if (choice == 0)
            return Pages::Back();

        if (choice == 1)
        {
            analyzeInputOutput("../Output/input.txt", "../Output/output.txt");
            cout << "\nAnalysis complete. Press Enter to continue..." << endl;
            cin.ignore((numeric_limits<streamsize>::max)(), '\n');
            cin.get();
        }
    }
}

void HttpPage::analyzeInputOutput(const std::string &inputPath, const std::string &outputPath)
{
    std::string inPath = inputPath;
    std::string outPath = outputPath;

    if (!fs::exists(outPath))
    {
        if (fs::exists("Output/output.txt"))
            outPath = "Output/output.txt";
    }

    if (!fs::exists(inPath))
    {
        if (fs::exists("Output/input.txt"))
            inPath = "Output/input.txt";
    }

    std::ifstream outFile(outPath, std::ios::in | std::ios::binary);
    if (!outFile.is_open())
    {
        cout << "[Warn] output.txt not found: " << outPath << endl;
        return;
    }

    std::ifstream inFile(inPath, std::ios::in | std::ios::binary);

    std::string outLine;
    std::string inLine;

    std::uint64_t foundHttp = 0;
    std::uint64_t foundTls = 0;

    while (std::getline(outFile, outLine))
    {
        if (outLine.empty())
            continue;
        if (!outLine.empty() && outLine[0] == '#')
            continue;

        std::vector<std::string> cols;
        SplitTabs(outLine, cols);
        if (cols.size() < 8)
            continue;

        const std::string &ts = cols[0];
        const std::string &proto = cols[1];
        const std::string &src = cols[2];
        const std::string &dst = cols[3];
        const std::string &extraHex = cols[7];

        if (proto != "TCP")
            continue;

        std::vector<std::uint8_t> payload;
        bool hasPayload = false;

        if (inFile.is_open())
        {
            while (std::getline(inFile, inLine))
            {
                if (inLine.empty())
                    continue;

                std::size_t p1 = inLine.find('\t');
                std::size_t p2 = (p1 == std::string::npos) ? std::string::npos : inLine.find('\t', p1 + 1);
                std::size_t p3 = (p2 == std::string::npos) ? std::string::npos : inLine.find('\t', p2 + 1);
                if (p1 == std::string::npos || p2 == std::string::npos || p3 == std::string::npos)
                    continue;

                const std::string hex = inLine.substr(p3 + 1);
                std::vector<std::uint8_t> frame;
                if (!HexToBytes(hex, frame))
                    continue;

                hasPayload = ExtractTcpPayload(frame, payload);
                break;
            }
        }

        if (!hasPayload)
        {
            std::vector<std::uint8_t> extraBytes;
            if (HexToBytes(extraHex, extraBytes))
            {
                payload.swap(extraBytes);
                hasPayload = !payload.empty();
            }
        }

        if (!hasPayload)
            continue;

        std::string ascii = BytesToPrintableAscii(payload);
        std::string upper = ToUpperAscii(ascii);

        const std::vector<std::string> httpPrefixes = {
            "GET ", "POST ", "HEAD ", "PUT ", "DELETE ", "OPTIONS ", "CONNECT ", "TRACE ", "HTTP/"};

        if (StartsWithAny(upper, httpPrefixes))
        {
            ++foundHttp;
            cout << "--------------------------------" << endl;
            cout << "Time: " << ts << endl;
            cout << "Flow: " << src << " -> " << dst << endl;
            cout << "HTTP: " << FirstLine(ascii) << endl;
            continue;
        }

        std::string sni = parseTLSClientHello(payload);
        if (!sni.empty())
        {
            ++foundTls;
            cout << "--------------------------------" << endl;
            cout << "Time: " << ts << endl;
            cout << "Flow: " << src << " -> " << dst << endl;
            cout << "TLS SNI: " << sni << endl;
            continue;
        }
    }

    if (foundHttp == 0 && foundTls == 0)
    {
        cout << "[Info] No HTTP/HTTPS records found in output.txt" << endl;
    }
    else
    {
        cout << "[Info] HTTP=" << foundHttp << " HTTPS(TLS SNI)=" << foundTls << endl;
    }
}

// 简单的 TLS Client Hello SNI 解析器
std::string HttpPage::parseTLSClientHello(const std::vector<uint8_t> &payload)
{
    if (payload.size() < 43)
        return ""; // 最小长度检查

    const uint8_t *data = payload.data();
    size_t len = payload.size();
    size_t pos = 0;

    // 1. TLS Record Layer Header
    // 握手类型
    if (data[pos] != 0x16)
        return "";
    pos += 1;
    // 协议版本 - Skip 2 bytes
    pos += 2;
    // 握手消息长度 - Skip 2 bytes
    pos += 2;

    if (pos >= len)
        return "";

    // 2. 握手协议
    // 握手类型: 客户端 Hello (1)
    if (data[pos] != 0x01)
        return "";
    pos += 1;
    // 握手消息长度 - Skip 3 bytes
    pos += 3;
    // Version协议版本 - Skip 2 bytes
    pos += 2;
    // 随机数 - Skip 32 bytes
    pos += 32;

    if (pos >= len)
        return "";

    // 会话 ID 长度
    uint8_t sessionIDLen = data[pos];
    pos += 1 + sessionIDLen;

    if (pos + 2 > len)
        return "";

    // 加密套件长度
    uint16_t cipherSuitesLen = (data[pos] << 8) | data[pos + 1];
    pos += 2 + cipherSuitesLen;

    if (pos + 1 > len)
        return "";

    // 压缩方法长度
    uint8_t compMethodsLen = data[pos];
    pos += 1 + compMethodsLen;

    if (pos + 2 > len)
        return "";

    // 扩展长度
    uint16_t extensionsLen = (data[pos] << 8) | data[pos + 1];
    pos += 2;

    size_t extensionsEnd = pos + extensionsLen;
    if (extensionsEnd > len)
        return "";

    // 遍历 Extensions
    while (pos + 4 <= extensionsEnd)
    {
        uint16_t extType = (data[pos] << 8) | data[pos + 1];
        uint16_t extLen = (data[pos + 2] << 8) | data[pos + 3];
        pos += 4;

        if (extType == 0x0000) // Server Name Indication (SNI)
        {
            if (pos + 2 > extensionsEnd)
                return "";
            // SNI List Length
            // uint16_t listLen = (data[pos] << 8) | data[pos + 1];
            pos += 2;

            if (pos + 3 > extensionsEnd)
                return "";
            // SNI Type (0 = host_name)
            uint8_t sniType = data[pos];
            pos += 1;

            if (sniType == 0)
            {
                uint16_t hostnameLen = (data[pos] << 8) | data[pos + 1];
                pos += 2;
                if (pos + hostnameLen <= extensionsEnd)
                {
                    return std::string((const char *)(data + pos), hostnameLen);
                }
            }
        }
        else
        {
            pos += extLen;
        }
    }

    return "";
}