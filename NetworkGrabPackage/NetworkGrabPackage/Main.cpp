//#include "Application.hpp"
//#include <iostream>
//#include <pcap.h>
//#include <cpr/cpr.h>
//
//using namespace std;
//
//int main() {
//    /*Application application;
//    application.StartApplication();*/
//
//    
//   
//    //// 发起一个简单的 GET 请求
//    //cpr::Response response = cpr::Get(cpr::Url{ "https://httpbin.org/get" });
//
//    //// 输出响应状态码和内容
//    //std::cout << "Status Code: " << response.status_code << std::endl;
//    //std::cout << "Response Body:\n" << response.text << std::endl;
//
//    //return 0;
//}

#include <iostream>

// 声明 printOutputFiles 函数（定义在 FileStorage.cpp 中）,该函数定义的为全局函数
void printOutputFiles();

int main() {
    printOutputFiles();
    return 0;
}

