#pragma once
#include "Pages.hpp"
#include "DevicePage.hpp"
#include "StatisticsPage.hpp"
#include <vector>

class HomePage : public Pages
{
private:
    std::vector<Pages*> subPages;
public:
    HomePage();
    ~HomePage();
    Pages* display() override;
    Pages* chiocePage(int i);
};
