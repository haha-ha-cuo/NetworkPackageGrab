#include "Render.h"


void Render::SetDoubleBuff() const {
	SetConsoleActiveScreenBuffer(hOutBuf);
}