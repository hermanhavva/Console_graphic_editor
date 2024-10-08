#pragma once
#include <windows.h>

HANDLE hout = GetStdHandle(STD_OUTPUT_HANDLE);
CONSOLE_SCREEN_BUFFER_INFO consoleInfo;

const WORD originalAttributes = GetConsoleScreenBufferInfo(hout, &consoleInfo);

const WORD yellowFontBlackText = BACKGROUND_GREEN | BACKGROUND_RED | BACKGROUND_INTENSITY;
const WORD redText = FOREGROUND_RED | FOREGROUND_INTENSITY;
const WORD cyanText = FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY;
const WORD purpleText = FOREGROUND_RED | FOREGROUND_BLUE;
const WORD blueText = FOREGROUND_BLUE | FOREGROUND_INTENSITY;
const WORD greenText = FOREGROUND_GREEN;

enum COLOR 
{
	BLACK = 1, 
	RED = 2,
	CYAN = 3, 
	PURPLE = 4, 
	BLUE = 5,
	GREEN = 6
};
