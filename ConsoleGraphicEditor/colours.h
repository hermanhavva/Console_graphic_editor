#pragma once
#include <windows.h>
#include <unordered_map>
using namespace std;

const WORD yellowFontBlackText = BACKGROUND_GREEN | BACKGROUND_RED | BACKGROUND_INTENSITY;
const WORD redText = FOREGROUND_RED | FOREGROUND_INTENSITY;
const WORD cyanText = FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY;
const WORD purpleText = FOREGROUND_RED | FOREGROUND_BLUE;
const WORD blueText = FOREGROUND_BLUE | FOREGROUND_INTENSITY;
const WORD greenText = FOREGROUND_GREEN;

const short int MAX_COLOUR_INDEX = 6;
const short int MIN_COLOUR_INDEX = 1;

enum COLOR 
{
	BLACK = 1, 
	RED = 2,
	CYAN = 3, 
	PURPLE = 4, 
	BLUE = 5,
	GREEN = 6
};

extern const std::unordered_map<int, WORD> colourEnumToWordMap;

extern const std::unordered_map<WORD, int> colourWordToEnumMap;
