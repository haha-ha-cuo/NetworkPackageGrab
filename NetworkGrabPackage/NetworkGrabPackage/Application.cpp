#include "Application.hpp"
#include "NetworkInterface.hpp"
#include "PacketCapture.hpp"
#include "PacketParser.hpp"
#include <iostream>
#include <zlib.h>
#include <cpprest/http_client.h>
#include <cpprest/filestream.h>

using namespace web;
using namespace web::http;
using namespace web::http::client;
using namespace utility;

Application::Application() {

}

Application::~Application() {

}

void Application::StartApplication() {
	//packetCapture.startCapture("\\Device\\NPF_{DB3D8D4E-8126-44D3-AF82-DA2276ADAA30}");
	//networkInterface.printAllDevices();
	//PacketParser parser;
	//const char* deviceName = "\\Device\\NPF_{A5484AF4-D1D3-4914-A825-DC74FAAEE006}";
	//parser.startParse(deviceName);
	//parser.startParse("\\Device\\NPF_{DB3D8D4E-8126-44D3-AF82-DA2276ADAA30}");
	//parser.saveToFile("captured_packets.json");
	//parser.loadAndParseFromJson("captured_packets.json");
    std::cout << "cpprestsdk installation test..." << std::endl;

    // 简单的 HTTP 客户端测试
    try
    {
       

        // 创建 HTTP 客户端，访问 httpbin.org 测试服务
        http_client client(U("https://httpbin.org"));

        // 发起 GET 请求
        uri_builder builder(U("/get"));
        pplx::task<void> requestTask = client.request(methods::GET, builder.to_string())
            .then([=](http_response response) {
            if (response.status_code() == status_codes::OK)
            {
                return response.extract_string();
            }
            return pplx::task_from_result(utility::string_t(U("请求失败")));
                })
            .then([=](pplx::task<utility::string_t> previousTask) {
            try
            {
                auto body = previousTask.get();
            }
            catch (...)
            {
                std::cerr << "异常: " << std::endl;
            }
                });

        // 等待任务完成
        requestTask.wait();
    }
    catch (const std::exception& e)
    {
        std::cerr << "运行时异常: " << e.what() << std::endl;
    }

}

