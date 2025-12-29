#pragma once

#include <pcap.h>
#include <zlib.h>

#include <cstdint>
#include <string>
#include <vector>
#include <fstream>
#include <filesystem>
#include <sstream>
#include <iomanip>
#include <iostream>
#include <chrono>

namespace PacketFilePipeline
{
    inline std::uint16_t ReadBE16(const std::uint8_t *p)
    {
        return (static_cast<std::uint16_t>(p[0]) << 8) | static_cast<std::uint16_t>(p[1]);
    }

    inline std::uint32_t Crc32(const std::uint8_t *data, std::size_t len)
    {
        uLong crc = crc32(0L, Z_NULL, 0);
        crc = crc32(crc, reinterpret_cast<const Bytef *>(data), static_cast<uInt>(len));
        return static_cast<std::uint32_t>(crc);
    }

    inline void BytesToHex(const std::uint8_t *data, std::size_t len, std::string &out)
    {
        static const char *kHex = "0123456789ABCDEF";
        out.resize(len * 2);
        for (std::size_t i = 0; i < len; ++i)
        {
            const std::uint8_t b = data[i];
            out[i * 2 + 0] = kHex[(b >> 4) & 0x0F];
            out[i * 2 + 1] = kHex[b & 0x0F];
        }
    }

    inline int HexNibble(char c)
    {
        if (c >= '0' && c <= '9')
            return c - '0';
        if (c >= 'a' && c <= 'f')
            return 10 + (c - 'a');
        if (c >= 'A' && c <= 'F')
            return 10 + (c - 'A');
        return -1;
    }

    inline bool HexToBytes(const std::string &hex, std::vector<std::uint8_t> &out)
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

    inline std::string FormatTs(const timeval &tv)
    {
        std::ostringstream oss;
        oss << tv.tv_sec << "." << std::setw(6) << std::setfill('0') << tv.tv_usec;
        return oss.str();
    }

    inline bool AppendCapturedPacketLine(std::ofstream &out, const pcap_pkthdr &hdr, const u_char *data, std::string *err)
    {
        if (!out.is_open())
        {
            if (err)
                *err = "stream not open";
            return false;
        }
        if (!data || hdr.caplen == 0)
        {
            if (err)
                *err = "empty packet";
            return false;
        }

        const auto *bytes = reinterpret_cast<const std::uint8_t *>(data);
        const std::uint32_t crc = Crc32(bytes, hdr.caplen);

        std::string hex;
        BytesToHex(bytes, hdr.caplen, hex);

        out << FormatTs(hdr.ts) << "\t"
            << hdr.caplen << "\t"
            << std::hex << std::uppercase << std::setw(8) << std::setfill('0') << crc
            << std::dec << "\t"
            << hex << "\n";

        if (!out.good())
        {
            if (err)
                *err = "write failed";
            return false;
        }
        return true;
    }

    inline std::string IPv4ToString(const std::uint8_t *p)
    {
        std::ostringstream oss;
        oss << static_cast<int>(p[0]) << "."
            << static_cast<int>(p[1]) << "."
            << static_cast<int>(p[2]) << "."
            << static_cast<int>(p[3]);
        return oss.str();
    }

    inline std::uint16_t ReadBE16(const std::uint8_t *p, std::size_t offset)
    {
        return ReadBE16(p + offset);
    }

    inline std::string HexPreview(const std::vector<std::uint8_t> &buf, std::size_t offset, std::size_t maxBytes)
    {
        if (offset >= buf.size())
            return "";
        std::size_t n = buf.size() - offset;
        if (n > maxBytes)
            n = maxBytes;

        std::string hex;
        BytesToHex(buf.data() + offset, n, hex);
        return hex;
    }

    inline void WriteLog(std::ofstream &log, std::uint64_t lineNo, const std::string &msg)
    {
        log << "line\t" << lineNo << "\t" << msg << "\n";
    }

    inline std::string ToFullWidthDigits(std::uint64_t n)
    {
        std::string s = std::to_string(n);
        std::string out;
        out.reserve(s.size() * 3);

        for (char ch : s)
        {
            if (ch >= '0' && ch <= '9')
            {
                const char32_t fw = U'０' + (ch - '0');
                if (fw <= 0x7F)
                {
                    out.push_back(static_cast<char>(fw));
                }
                else
                {
                    char buf[4] = {0};
                    const std::uint32_t cp = static_cast<std::uint32_t>(fw);
                    buf[0] = static_cast<char>(0xE0 | ((cp >> 12) & 0x0F));
                    buf[1] = static_cast<char>(0x80 | ((cp >> 6) & 0x3F));
                    buf[2] = static_cast<char>(0x80 | (cp & 0x3F));
                    out.append(buf, buf + 3);
                }
            }
            else
            {
                out.push_back(ch);
            }
        }

        return out;
    }

    inline std::string PacketTitleZh(const std::string &proto)
    {
        if (proto == "TCP")
            return "TCP 数据包（传输层，面向连接）";
        if (proto == "UDP")
            return "UDP 数据包（传输层，无连接）";
        if (proto == "ICMP")
            return "ICMP 数据包（网络层控制消息）";
        if (proto == "NON_IPV4")
            return "非 IPv4 数据包（未解析 L3/L4）";
        if (proto == "BAD_IPV4")
            return "IPv4 数据包（头部不完整或异常）";
        if (proto.rfind("IP(", 0) == 0)
            return "IP 数据包（其他上层协议）";
        return "未知类型数据包";
    }

    inline std::string MakeSeparatorLine(std::uint64_t index)
    {
        return std::string("# ＝＝＝＝＝＝ 包分隔 ") + ToFullWidthDigits(index) + " ＝＝＝＝＝＝";
    }

    inline bool ParseInputFileToOutput(const std::string &inputPath,
                                       const std::string &outputPath,
                                       const std::string &logPath)
    {
        namespace fs = std::filesystem;

        try
        {
            if (fs::path(outputPath).has_parent_path())
                fs::create_directories(fs::path(outputPath).parent_path());
            if (fs::path(logPath).has_parent_path())
                fs::create_directories(fs::path(logPath).parent_path());
        }
        catch (...)
        {
        }

        std::ifstream in(inputPath, std::ios::in | std::ios::binary);
        if (!in.is_open())
        {
            std::cerr << "[Error]Open input failed: " << inputPath << std::endl;
            return false;
        }

        std::ofstream out(outputPath, std::ios::out | std::ios::binary | std::ios::trunc);
        if (!out.is_open())
        {
            std::cerr << "[Error]Open output failed: " << outputPath << std::endl;
            return false;
        }

        std::ofstream log(logPath, std::ios::out | std::ios::binary | std::ios::trunc);
        if (!log.is_open())
        {
            std::cerr << "[Error]Open log failed: " << logPath << std::endl;
            return false;
        }

        std::uintmax_t totalSize = 0;
        try
        {
            if (fs::exists(inputPath))
                totalSize = fs::file_size(inputPath);
        }
        catch (...)
        {
        }

        std::string line;
        std::uint64_t lineNo = 0;
        std::uint64_t ok = 0;
        std::uint64_t bad = 0;
        std::uint64_t packetIndex = 0;

        auto lastReport = std::chrono::steady_clock::now();

        while (std::getline(in, line))
        {
            ++lineNo;
            if (line.empty())
                continue;

            std::size_t p1 = line.find('\t');
            std::size_t p2 = (p1 == std::string::npos) ? std::string::npos : line.find('\t', p1 + 1);
            std::size_t p3 = (p2 == std::string::npos) ? std::string::npos : line.find('\t', p2 + 1);
            if (p1 == std::string::npos || p2 == std::string::npos || p3 == std::string::npos)
            {
                WriteLog(log, lineNo, "format_error");
                ++bad;
                continue;
            }

            const std::string ts = line.substr(0, p1);
            const std::string lenStr = line.substr(p1 + 1, p2 - (p1 + 1));
            const std::string crcStr = line.substr(p2 + 1, p3 - (p2 + 1));
            const std::string hex = line.substr(p3 + 1);

            std::size_t caplen = 0;
            try
            {
                caplen = static_cast<std::size_t>(std::stoull(lenStr));
            }
            catch (...)
            {
                WriteLog(log, lineNo, "len_parse_error");
                ++bad;
                continue;
            }

            std::uint32_t crcIn = 0;
            try
            {
                crcIn = static_cast<std::uint32_t>(std::stoul(crcStr, nullptr, 16));
            }
            catch (...)
            {
                WriteLog(log, lineNo, "crc_parse_error");
                ++bad;
                continue;
            }

            if (hex.size() != caplen * 2)
            {
                WriteLog(log, lineNo, "hex_len_mismatch");
                ++bad;
                continue;
            }

            std::vector<std::uint8_t> bytes;
            if (!HexToBytes(hex, bytes))
            {
                WriteLog(log, lineNo, "hex_decode_error");
                ++bad;
                continue;
            }

            const std::uint32_t crcCalc = Crc32(bytes.data(), bytes.size());
            if (crcCalc != crcIn)
            {
                WriteLog(log, lineNo, "crc_mismatch");
                ++bad;
                continue;
            }

            std::string proto = "UNKNOWN";
            std::string src = "-";
            std::string dst = "-";
            std::size_t payloadLen = 0;
            std::string extra;

            if (bytes.size() >= 14)
            {
                const std::uint16_t etherType = ReadBE16(bytes.data(), 12);

                if (etherType == 0x0800 && bytes.size() >= 14 + 20)
                {
                    const std::uint8_t *ip = bytes.data() + 14;
                    const std::size_t ipAvail = bytes.size() - 14;

                    const std::uint8_t version = (ip[0] >> 4) & 0x0F;
                    const std::size_t ihl = static_cast<std::size_t>(ip[0] & 0x0F) * 4;

                    if (version == 4 && ihl >= 20 && ihl <= ipAvail)
                    {
                        const std::uint8_t ipProto = ip[9];

                        const std::string srcIP = IPv4ToString(ip + 12);
                        const std::string dstIP = IPv4ToString(ip + 16);

                        const std::size_t l4 = 14 + ihl;

                        if (ipProto == 6)
                        {
                            proto = "TCP";
                            if (bytes.size() >= l4 + 20)
                            {
                                const std::uint8_t *tcp = bytes.data() + l4;
                                const std::uint16_t sp = ReadBE16(tcp, 0);
                                const std::uint16_t dp = ReadBE16(tcp, 2);
                                src = srcIP + ":" + std::to_string(sp);
                                dst = dstIP + ":" + std::to_string(dp);

                                const std::size_t doff = static_cast<std::size_t>((tcp[12] >> 4) & 0x0F) * 4;
                                const std::size_t payloadOff = l4 + doff;
                                if (doff >= 20 && payloadOff <= bytes.size())
                                {
                                    payloadLen = bytes.size() - payloadOff;
                                    extra = HexPreview(bytes, payloadOff, 64);
                                }
                            }
                        }
                        else if (ipProto == 17)
                        {
                            proto = "UDP";
                            if (bytes.size() >= l4 + 8)
                            {
                                const std::uint8_t *udp = bytes.data() + l4;
                                const std::uint16_t sp = ReadBE16(udp, 0);
                                const std::uint16_t dp = ReadBE16(udp, 2);
                                src = srcIP + ":" + std::to_string(sp);
                                dst = dstIP + ":" + std::to_string(dp);

                                const std::size_t payloadOff = l4 + 8;
                                if (payloadOff <= bytes.size())
                                {
                                    payloadLen = bytes.size() - payloadOff;
                                    extra = HexPreview(bytes, payloadOff, 64);
                                }
                            }
                        }
                        else if (ipProto == 1)
                        {
                            proto = "ICMP";
                            src = srcIP;
                            dst = dstIP;
                        }
                        else
                        {
                            proto = "IP(" + std::to_string(static_cast<int>(ipProto)) + ")";
                            src = srcIP;
                            dst = dstIP;
                        }
                    }
                    else
                    {
                        proto = "BAD_IPV4";
                    }
                }
                else
                {
                    proto = "NON_IPV4";
                }
            }

            ++packetIndex;

            out << "\n";
            out << MakeSeparatorLine(packetIndex) << "\n";
            out << "\n";
            out << "# 包名: " << PacketTitleZh(proto) << "\n";

            out << ts << "\t"
                << proto << "\t"
                << src << "\t"
                << dst << "\t"
                << bytes.size() << "\t"
                << payloadLen << "\t"
                << "OK"
                << "\t"
                << extra << "\n";

            ++ok;

            auto now = std::chrono::steady_clock::now();
            if (now - lastReport >= std::chrono::seconds(1))
            {
                lastReport = now;

                std::uintmax_t pos = 0;
                auto tell = in.tellg();
                if (tell != std::streampos(-1))
                    pos = static_cast<std::uintmax_t>(tell);

                if (totalSize > 0)
                {
                    int pct = static_cast<int>((pos * 100) / totalSize);
                    std::cout << "\r[Progress] " << pct << "%  packets=" << ok << "  errors=" << bad << std::flush;
                }
                else
                {
                    std::cout << "\r[Progress] packets=" << ok << "  errors=" << bad << std::flush;
                }
            }
        }

        std::cout << "\n[Info]Parse done. packets=" << ok << " errors=" << bad << "\n";
        return true;
    }
}
