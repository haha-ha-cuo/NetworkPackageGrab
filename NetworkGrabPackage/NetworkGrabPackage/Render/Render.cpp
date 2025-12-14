#include "Render.hpp"
#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <conio.h>

Render::Render()
{
    CONSOLE_SCREEN_BUFFER_INFO csbi{};
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    GetConsoleScreenBufferInfo(hOut, &csbi);
    hOutBuf = CreateConsoleScreenBuffer(
        GENERIC_READ | GENERIC_WRITE,
        FILE_SHARE_READ | FILE_SHARE_WRITE,
        NULL,
        CONSOLE_TEXTMODE_BUFFER,
        NULL);

    SetConsoleScreenBufferSize(hOutBuf, csbi.dwSize);
}
//创建一块与当前窗口大小相同的备用屏幕缓冲区（双缓冲）

Render::~Render()
{
    CloseHandle(hOutBuf);
}
//程序退出时把缓冲区释放掉，防止句柄泄漏


void Render::SetDoubleBuff() const
{
    SetConsoleActiveScreenBuffer(hOutBuf);
}
//把刚刚画好的备用缓冲区一下子切到前台，用户瞬间看到最新画面

int Render::Select(const std::vector<std::string>& items)
{
    if (items.empty())
    {
        return -1;
    }
    CONSOLE_SCREEN_BUFFER_INFO csbi{};
    GetConsoleScreenBufferInfo(hOutBuf, &csbi);
    const SHORT bufW = csbi.dwSize.X;

    int  idx = 0;
    int  key;
    const WORD normalAttr = BACKGROUND_BLUE |
        FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE;
    const WORD highAttr = BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE;
    //白底黑字高亮

    std::ios::sync_with_stdio(false);

    while (true)
    {
        SetConsoleTextAttribute(hOutBuf, normalAttr);
        SetConsoleCursorPosition(hOutBuf, { 0, 0 });

        for (size_t i = 0; i < items.size(); i++)
        {
            SetConsoleTextAttribute(hOutBuf,
                                    static_cast<int>(i) == idx ? highAttr
                                                               : normalAttr);
            std::cout << std::left << std::setw(bufW - 1)
                      << ("  " + items[i]) << '\n';
        }

        SetConsoleTextAttribute(hOutBuf, normalAttr);
        DWORD written;
        COORD tail = { 0, static_cast<SHORT>(items.size()) };
        FillConsoleOutputCharacterW(hOutBuf, L' ',
                                    bufW * 10, tail, &written);

        key = _getch();
        if (key == 0 || key == 0xE0) 
        {
            key = _getch();
        }

        switch (key)
        {
        case 72: // ↑
            idx = (idx - 1 + static_cast<int>(items.size())) %
                static_cast<int>(items.size());
            break;
        case 80: // ↓
            idx = (idx + 1) % static_cast<int>(items.size());
            break;
        case 13: // Enter
            return idx;
        }
    }
}
//在控制台里做一个“上下选择菜单”，高亮当前项，按 ↑ ↓ 改变光标，按 Enter 返回选中序号

void Render::PauseEnter()
{
    std::cout <<">>按下Enter继续";
    while (std::cin.get() != '\n') {}
}
//在控制台里显示“按下Enter继续”，等待用户按下回车键