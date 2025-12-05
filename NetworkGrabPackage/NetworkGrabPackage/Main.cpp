#include "Application.hpp"
#include <iostream>
#include <Windows.h>
#include <pcap.h>
<<<<<<< Updated upstream
#include <cpr/cpr.h>
=======
>>>>>>> Stashed changes

using namespace std;

int main() {
	SetConsoleCP(65001);
    Application application;
    application.StartApplication();
    return 0;
}
<<<<<<< Updated upstream

=======
//
>>>>>>> Stashed changes
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

<<<<<<< Updated upstream
=======
//#include "TrafficStatistics.hpp"
//#include<iostream>
//
//int main() {
//	TrafficStatistics ts;
//	ts.analyzeTraffic();
//	ts.generateReport();
//}
>>>>>>> Stashed changes
