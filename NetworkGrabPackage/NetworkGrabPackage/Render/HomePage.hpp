#pragma once
#include "Pages.hpp"
#include "DevicePage.hpp"
#include <vector>

class HomePage : public Pages
{
private:
    std::vector<Pages*> subPages;
public:
    HomePage();
    ~HomePage();
    void display() override;
    Pages* chiocePage(int i);
};
