#include "StatisticsPage.hpp"

Pages* StatisticsPage::display()
{
	system("cls");
	ts.analyzeTraffic();
	ts.generateReport();
	cout << "[Info]Press [Enter] to continue >>";
	getchar();
	getchar();
	return Pages::Back();
}