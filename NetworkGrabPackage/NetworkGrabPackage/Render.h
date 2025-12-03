#pragma once
#include<Windows.h>

class Render
{
private:

	HANDLE hOutput = GetStdHandle(STD_OUTPUT_HANDLE);

	HANDLE hOutBuf = CreateConsoleScreenBuffer(
		GENERIC_READ | GENERIC_WRITE,
		FILE_SHARE_READ | FILE_SHARE_WRITE,
		NULL,
		CONSOLE_TEXTMODE_BUFFER,
		NULL
	);

public:

	void SetDoubleBuff();

};

