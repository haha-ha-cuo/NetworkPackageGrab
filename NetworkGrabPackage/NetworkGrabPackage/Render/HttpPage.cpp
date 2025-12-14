#include "HttpPage.hpp"
#include "PacketFactory.hpp"
#include "TCP.hpp"
#include "HTTP.hpp"
#include <iostream>
#include <filesystem>
#include <pcap.h>
#include <algorithm>

namespace fs = std::filesystem;
using namespace std;

HttpPage::HttpPage() {}

HttpPage::~HttpPage() {}

Pages *HttpPage::display()
{
    while (true)
    {
        system("cls");
        cout << "=== HTTP Analysis Page ===" << endl;

        vector<string> files;
        listFiles(files);

        if (files.empty())
        {
            cout << "[Warn] No pcap files found in ../Output/" << endl;
            cout << "Press any key to return..." << endl;
            cin.get();
            cin.get();
            return Pages::Back();
        }

        cout << "Select a pcap file to analyze:" << endl;
        for (size_t i = 0; i < files.size(); ++i)
        {
            cout << (i + 1) << ": " << files[i] << endl;
        }
        cout << "0: Return" << endl;
        cout << ">> ";

        int choice;
        cin >> choice;

        // 处理输入错误
        if (cin.fail())
        {
            cin.clear();
            cin.ignore((numeric_limits<streamsize>::max)(), '\n');
            continue;
        }

        if (choice == 0)
            return Pages::Back();

        if (choice > 0 && choice <= static_cast<int>(files.size()))
        {
            string filepath = "../Output/" + files[choice - 1];
            analyzeFile(filepath);
            cout << "\nAnalysis complete. Press Enter to continue..." << endl;
            cin.ignore();
            cin.get();
        }
    }
}

void HttpPage::listFiles(vector<string> &files)
{
    string path = "../Output";
    // 检查目录是否存在
    if (!fs::exists(path))
    {
        if (fs::exists("Output"))
            path = "Output"; // 尝试当前目录
        else
            return;
    }

    for (const auto &entry : fs::directory_iterator(path))
    {
        if (entry.path().extension() == ".pcap")
        {
            files.push_back(entry.path().filename().string());
        }
    }
}

void HttpPage::analyzeFile(const string &filepath)
{
    char errbuf[PCAP_ERRBUF_SIZE];
    // 打开 pcap 文件
    pcap_t *handle = pcap_open_offline(filepath.c_str(), errbuf);

    if (handle == nullptr)
    {
        cerr << "[Error] Opening file: " << errbuf << endl;
        return;
    }

    struct pcap_pkthdr *header;
    const u_char *pktData;
    int res;
    int count = 0;

    cout << "\n[Info] Parsing " << filepath << "..." << endl;

    // 循环读取报文
    while ((res = pcap_next_ex(handle, &header, &pktData)) >= 0)
    {
        if (res == 0)
            continue;

        // 构造数据 Vector
        vector<uint8_t> dataVec(pktData, pktData + header->caplen);

        // 使用工厂创建报文对象
        auto packet = PacketFactory::createPacket(dataVec);

        if (packet)
        {
            try
            {
                packet->Parse();
            }
            catch (...)
            {
                continue;
            }

            // 仅处理 TCP 报文
            if (packet->GetPacketType() == PacketType::TCP)
            {
                TCP *tcpPacket = dynamic_cast<TCP *>(packet.get());
                if (tcpPacket)
                {
                    // 尝试作为 HTTP 解析 Payload
                    HTTP http(tcpPacket->getPayload());
                    if (http.is_valid())
                    {
                        count++;
                        cout << "--------------------------------" << endl;
                        // 显示时间戳
                        const time_t ts = header->ts.tv_sec;
                        struct tm tmDest;
                        localtime_s(&tmDest, &ts);
                        char timestr[32];
                        strftime(timestr, sizeof(timestr), "%Y-%m-%d %H:%M:%S", &tmDest);
                        cout << "Time: " << timestr << endl;

                        // 显示 TCP 和 HTTP 信息
                        tcpPacket->display();
                        http.display();
                    }
                    else
                    {
                        // 尝试解析 HTTPS (TLS Client Hello SNI)
                        string sni = parseTLSClientHello(tcpPacket->getPayload());
                        if (!sni.empty())
                        {
                            count++;
                            cout << "--------------------------------" << endl;
                            const time_t ts = header->ts.tv_sec;
                            struct tm tmDest;
                            localtime_s(&tmDest, &ts);
                            char timestr[32];
                            strftime(timestr, sizeof(timestr), "%Y-%m-%d %H:%M:%S", &tmDest);
                            cout << "Time: " << timestr << endl;

                            tcpPacket->display();
                            cout << "  [HTTPS Protocol (TLS)]" << endl;
                            cout << "    SNI (Domain): " << sni << endl;
                            cout << "    Note: Encrypted Traffic" << endl;
                        }
                    }
                }
            }
        }
    }

    if (count == 0)
    {
        cout << "[Info] No HTTP/HTTPS packets found in this file." << endl;
    }
    else
    {
        cout << "[Info] Found " << count << " HTTP/HTTPS packets." << endl;
    }

    pcap_close(handle);
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