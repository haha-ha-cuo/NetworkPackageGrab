<<<<<<< Updated upstream
#include ""

class HomePage : public
{
private:

public:


=======
#include "Pages.hpp"
#include "DevicePage.hpp"
#include <vector>

class HomePage : public Pages

{
private:
	std::vector<Pages*> subPages;
public:
	// 将构造函数内联定义在头文件，避免缺少链接符号（如果使用此方案，请在 HomePage.cpp 中移除或注释掉 HomePage::HomePage 的实现）
	HomePage() {
		// 可以在这里初始化子页
		subPages.push_back(new DevicePage());
		// 其他子页面可以类似添加
	}

	void display() override;

	Pages* chiocePage(int index) override;
>>>>>>> Stashed changes
};