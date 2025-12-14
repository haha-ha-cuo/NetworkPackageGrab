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

int Render::Select(const std::vector<std::string>& items) const
{
    if (items.empty())
    {
        return -1;
    }
    CONSOLE_SCREEN_BUFFER_INFO csbi{};
    GetConsoleScreenBufferInfo(hOutBuf, &csbi);
    SHORT bufW = csbi.dwSize.X;
    SHORT bufH = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;

    const WORD normalAttr = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE;                   
    // 灰字黑底
    const WORD highAttr = BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE;                  
    // 黑字白底

    int  idx = 0;
    std::ios::sync_with_stdio(false);

    while (true)
    {
        DWORD written;
        COORD home = { 0, 0 };
        FillConsoleOutputCharacterW(hOutBuf, L' ', bufW * bufH, home, &written);
        FillConsoleOutputAttribute(hOutBuf, normalAttr, bufW * bufH, home, &written);
        SetConsoleCursorPosition(hOutBuf, home);

        for (size_t i = 0; i < items.size(); ++i)
        {
            std::string line = "  " + items[i];
            line.resize(bufW, ' ');

            std::vector<WORD> attrLine(line.size(),
                static_cast<int>(i) == idx ? highAttr : normalAttr);

            COORD pos = { 0, static_cast<SHORT>(i) };
            DWORD written;
            WriteConsoleOutputCharacterA(hOutBuf, line.c_str(),
                static_cast<DWORD>(line.size()),
                pos, &written);

            WriteConsoleOutputAttribute(hOutBuf, attrLine.data(),
                static_cast<DWORD>(attrLine.size()),
                pos, &written);
        }

        SetConsoleActiveScreenBuffer(hOutBuf);

        int key = _getch();
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