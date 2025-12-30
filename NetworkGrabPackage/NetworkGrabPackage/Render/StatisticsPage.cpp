#include "StatisticsPage.hpp"

Pages *StatisticsPage::display()
{
	system("cls");
	if (!ts.parseInputFileToOutput())
	{
		cout << "[Warn]Parse failed. Please generate ../Output/input.txt first." << endl;
	}
	ts.analyzeTraffic();
	ts.generateReport();
	cout << "[Info]Press [Enter] to continue >>";
	getchar();
	getchar();
	return Pages::Back();
}