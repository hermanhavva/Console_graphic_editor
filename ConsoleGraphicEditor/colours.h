#pragma once
#include <windows.h>
#include <unordered_map>
//using namespace std;
//#include "helper.h"

//const WORD originalAttributes = GetConsoleScreenBufferInfo(hout, &consoleInfo);

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
/*
extern std::unordered_map<int, WORD> colourMap = 
{
	{BLACK, yellowFontBlackText},
	{RED, redText | yellowFontBlackText},
	{CYAN, cyanText | yellowFontBlackText},
	{PURPLE, purpleText | yellowFontBlackText},
	{BLUE, blueText | yellowFontBlackText},
	{GREEN, greenText | yellowFontBlackText}
};
*/
