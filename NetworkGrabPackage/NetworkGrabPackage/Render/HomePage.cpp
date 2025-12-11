#include "HomePage.hpp"
#include <iostream>

HomePage::HomePage()
{
	
    subPages.push_back(new DevicePage());
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
    std::vector<std::string> m{ "  1.抓包测试  ","  2.流量分析  ","  3.http解析  ","  4.文件转存  " };
    int s = Select(m);
    if (s >= 0) std::cout << "你选择了：" << s + 1 <<endl;

    Pages::pageStack.push(this);

    return choicePage(s);
}

Pages* HomePage::choicePage(int index)
{
    if (index < 1 || index > static_cast<int>(subPages.size()))
    {
        return nullptr;
    }
    return subPages[index - 1];
}
