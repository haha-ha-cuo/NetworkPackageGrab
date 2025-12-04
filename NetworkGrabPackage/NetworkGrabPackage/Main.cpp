#include "Application.hpp"
#include <iostream>
#include <Windows.h>
#include <pcap.h>
#include <cpr/cpr.h>

using namespace std;

int main() {
	SetConsoleCP(65001);
    Application application;
    application.StartApplication();
    return 0;
}

//#include "FileStorage.hpp"
//#include <iostream>
//
//
//int main() {
//    //创建对象打印Output文件夹中文件名称及大小
//    FileStorage printOutput;
//    printOutput.printOutputFiles();
//    return 0;
//}

