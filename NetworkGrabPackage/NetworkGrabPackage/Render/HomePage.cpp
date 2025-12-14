#include "HomePage.hpp"
#include "HttpPage.hpp"
#include <vector>
#include "Render.hpp"
#include <iostream>

HomePage::HomePage()
{

    subPages.push_back(new DevicePage());
    subPages.push_back(new StatisticsPage());
    subPages.push_back(new HttpPage()); // HTTP解析页面
    subPages.push_back(nullptr);
    
    
}

HomePage::~HomePage()
{
    for (auto *p : subPages)
    {
        delete p;
    }
}

Pages *HomePage::display()
{
    system("cls");
	vector<string> v1;
	v1.push_back("抓包测试");
    v1.push_back("流量分析");
    v1.push_back("http解析");
    Render render;

    int n= render.Select(v1);
    /*int n;
    cin >> n;*/

    Pages::pageStack.push(this);

    return chiocePage(n);
}

Pages *HomePage::chiocePage(int index)
{
    if (index < 1 || index > static_cast<int>(subPages.size()))
    {
        return nullptr;
    }
    return subPages[index-1];
}