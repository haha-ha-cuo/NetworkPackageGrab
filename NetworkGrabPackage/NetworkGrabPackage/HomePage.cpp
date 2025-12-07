#include "HomePage.hpp"
#include <iostream>


void HomePage::display()
{
	system("cls");
	std::cout << "1: 抓包测试" << std::endl;
	std::cout << "2: 流量分析" << std::endl;
	std::cout << "3: http解析" << std::endl;
	std::cout << "4: 文件转存" << std::endl;
	std::cout << ">>";
}

Pages* HomePage::chiocePage(int index)
{
	if (index < 1 || index > subPages.size()) {
		return nullptr; // 或者抛出异常
	}
	return subPages[index - 1];
}