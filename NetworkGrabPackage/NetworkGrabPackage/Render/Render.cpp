#include "Render.hpp"

void Render::SetDoubleBuff() const
{
    SetConsoleActiveScreenBuffer(hOutBuf);
}
