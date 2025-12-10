#pragma once

#include <Windows.h>

class Render {
public:
    void SetDoubleBuff() const;
private:
    HANDLE hOutBuf{ GetStdHandle(STD_OUTPUT_HANDLE) };
};
