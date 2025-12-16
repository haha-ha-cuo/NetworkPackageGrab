#pragma once

#include <windows.h>
#include <string>
#include <vector>


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
	Render();
	~Render();
	void SetDoubleBuff() const;
	int Select(const std::vector<std::string>& items) const;
	int Select(const char* items[]) const;
};
