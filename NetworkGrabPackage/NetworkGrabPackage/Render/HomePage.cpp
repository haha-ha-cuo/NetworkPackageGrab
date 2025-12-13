#include "HomePage.hpp"
#include <iostream>

HomePage::HomePage()
{
	
    subPages.push_back(new DevicePage());
    subPages.push_back(new StatisticsPage());
}

HomePage::~HomePage()
{
    for (auto *p : subPages)
    {
        delete p;
    }
}

Pages* HomePage::display()
{
    system("cls");
    std::cout << "1: 抓包测试" << std::endl;
    std::cout << "2: 流量分析" << std::endl;
    std::cout << "3: http解析" << std::endl;
    std::cout << "4: 文件转存" << std::endl;
    std::cout << ">>";
    int n;
    cin >> n;

    Pages::pageStack.push(this);

	return chiocePage(n);
}

Pages *HomePage::chiocePage(int index)
{
    if (index < 1 || index > static_cast<int>(subPages.size()))
    {
        return nullptr;
    }
    return subPages[index - 1];
}
