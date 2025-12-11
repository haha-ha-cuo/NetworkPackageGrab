#pragma once
#include "Pages.hpp"
#include <windows.h>
#include <string>
#include <vector>

int Pages::Select(const std::vector<std::string>& items)
{
    if (items.empty()) 
        return -1;
    HANDLE hIn = GetStdHandle(STD_INPUT_HANDLE);
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD old; GetConsoleMode(hIn, &old);
    SetConsoleMode(hIn, old & ~(ENABLE_LINE_INPUT | ENABLE_ECHO_INPUT));

    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(hOut, &csbi);
    COORD base = csbi.dwCursorPosition;

    int idx = 0, count = static_cast<int>(items.size());
    auto draw = [&]() {
        SetConsoleCursorPosition(hOut, base);
        for (int i = 0; i < count; ++i) 
        {
            std::string ln = (i == idx ? "-> " : "  ") + items[i] + (i == idx ? " <-" : "");
            ln.append(csbi.dwSize.X - ln.size(), ' ');
            std::cout << ln << endl;
        }
        };
    draw();

    INPUT_RECORD rec; DWORD read;
    while (ReadConsoleInputW(hIn, &rec, 1, &read) && read) 
    {
        if (rec.EventType != KEY_EVENT || !rec.Event.KeyEvent.bKeyDown) 
            continue;
        switch (rec.Event.KeyEvent.wVirtualKeyCode) 
        {
            case VK_UP:  
                if (idx > 0) 
            { 
                --idx; draw(); 
            } break;
            case VK_DOWN: 
                if (idx + 1 < count) 
                { 
                    ++idx; draw();
                } break;
            case VK_RETURN: 
                SetConsoleMode(hIn, old); 
                return idx;
            case VK_ESCAPE:
                SetConsoleMode(hIn, old); 
                return -1;
        }
    }
    SetConsoleMode(hIn, old);
    return -1;
}