#include "HTTP.hpp"
#include <sstream>
#include <iostream>
#include <algorithm>
#include <cctype>

using namespace std;

HTTP::HTTP(const vector<uint8_t> &payload)
{
    if (payload.empty())
    {
        valid = false;
        return;
    }
    // 将二进制负载转换为字符串处理
    string data(payload.begin(), payload.end());
    parse(data);
}

void HTTP::parse(const string &data)
{
    // 查找头部结束标记 (Double CRLF)
    size_t headerEnd = data.find("\r\n\r\n");
    if (headerEnd == string::npos)
    {
        valid = false;
        return;
    }

    string headerPart = data.substr(0, headerEnd);
    body = data.substr(headerEnd + 4);

    istringstream stream(headerPart);
    string line;

    // 1. 解析请求行/状态行
    if (getline(stream, line) && !line.empty())
    {
        if (!line.empty() && line.back() == '\r')
            line.pop_back();

        istringstream lineStream(line);
        string part1, part2, part3;
        lineStream >> part1 >> part2 >> part3;

        // 判断是请求还是响应
        if (part1.substr(0, 4) == "HTTP")
        {
            // Response: HTTP/1.1 200 OK
            version = part1;
            method = "";               // 响应报文没有 Method，这里留空区分
            uri = part2 + " " + part3; // 状态码 + 原因
        }
        else
        {
            // Request: GET /index.html HTTP/1.1
            method = part1;
            uri = part2;
            version = part3;
        }

        // 简单校验：方法名或版本号是否合法
        if (method != "GET" && method != "POST" && method != "PUT" &&
            method != "DELETE" && method != "HEAD" && method != "OPTIONS" &&
            part1.substr(0, 4) != "HTTP")
        {
            valid = false;
            return;
        }
    }
    else
    {
        valid = false;
        return;
    }

    // 2. 解析 Headers
    while (getline(stream, line) && !line.empty())
    {
        if (!line.empty() && line.back() == '\r')
            line.pop_back();
        if (line.empty())
            break;

        size_t colonPos = line.find(':');
        if (colonPos != string::npos)
        {
            string key = line.substr(0, colonPos);
            string value = line.substr(colonPos + 1);

            // 去除值的前导空格
            size_t first = value.find_first_not_of(" \t");
            if (string::npos != first)
            {
                value = value.substr(first);
            }
            headers[key] = value;
        }
    }

    valid = true;
}

void HTTP::display() const
{
    if (!valid)
        return;
    cout << "  [HTTP Protocol]" << endl;
    if (!method.empty())
    {
        cout << "    Method: " << method << endl;
        string fullUrl = getFullUrl();
        if (!fullUrl.empty())
        {
            cout << "    URL: " << fullUrl << endl;
        }
        else
        {
            cout << "    URI: " << uri << endl;
        }
    }
    else
    {
        cout << "    Response: " << version << " " << uri << endl;
    }
    if (!version.empty() && !method.empty())
        cout << "    Version: " << version << endl;

    cout << "    Headers: " << endl;
    for (const auto &pair : headers)
    {
        cout << "      " << pair.first << ": " << pair.second << endl;
    }
    if (!body.empty())
    {
        cout << "    Body Size: " << body.size() << " bytes" << endl;

        // Check headers for encoding
        string contentEncoding = "";
        for (const auto &pair : headers)
        {
            string key = pair.first;
            transform(key.begin(), key.end(), key.begin(), ::tolower);
            if (key == "content-encoding")
                contentEncoding = pair.second;
        }

        bool isCompressed = false;
        if (contentEncoding.find("gzip") != string::npos ||
            contentEncoding.find("deflate") != string::npos ||
            contentEncoding.find("br") != string::npos)
        {
            isCompressed = true;
        }

        if (isCompressed)
        {
            cout << "    Body Content: [Compressed Data (" << contentEncoding << ")]" << endl;
        }
        else
        {
            // Increase display limit to 2048 bytes
            if (body.size() < 2048)
            {
                string cleanBody = body;
                replace_if(cleanBody.begin(), cleanBody.end(), [](char c)
                           { return c < 32 && c != '\n' && c != '\r' && c != '\t'; }, '.');
                cout << "    Body Content:\n"
                     << cleanBody << endl;
            }
            else
            {
                cout << "    Body Content: (Too large to display, size > 2048 bytes)" << endl;
                string preview = body.substr(0, 1000);
                replace_if(preview.begin(), preview.end(), [](char c)
                           { return c < 32 && c != '\n' && c != '\r' && c != '\t'; }, '.');
                cout << "    Preview (first 1000 chars):\n"
                     << preview << "..." << endl;
            }
        }
    }
}

string HTTP::getFullUrl() const
{
    if (!valid || method.empty())
    {
        return "";
    }
    string host = "";
    for (const auto &pair : headers)
    {
        string key = pair.first;
        transform(key.begin(), key.end(), key.begin(), ::tolower);
        if (key == "host")
        {
            host = pair.second;
            break;
        }
    }

    if (host.empty())
    {
        return "";
    }

    if (!host.empty() && host.back() == '\r')
        host.pop_back();

    return "http://" + host + uri;
}

string HTTP::getSummary() const
{
    if (!valid)
        return "Invalid HTTP";
    if (!method.empty())
        return "HTTP " + method + " " + version;
    return "HTTP Response " + version;
}
