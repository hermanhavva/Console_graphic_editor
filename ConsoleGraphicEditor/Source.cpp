#include <iostream>
#include <windows.h>
//#include <vector>
//#include <format>
#include "maze.cpp"
#include "colours.h"
#include "Figure.h" 
using namespace std;
 
/*
HANDLE hout = GetStdHandle(STD_OUTPUT_HANDLE);
CONSOLE_SCREEN_BUFFER_INFO consoleInfo;

const WORD originalAttributes = GetConsoleScreenBufferInfo(hout, &consoleInfo);

const WORD yellowFontBlackText = BACKGROUND_GREEN | BACKGROUND_RED | BACKGROUND_INTENSITY;
const WORD redText = FOREGROUND_RED | FOREGROUND_INTENSITY;
const WORD cyanText = FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY;
const WORD purpleText = FOREGROUND_RED | FOREGROUND_BLUE;
const WORD blueText = FOREGROUND_BLUE | FOREGROUND_INTENSITY;
const WORD greenText = FOREGROUND_GREEN;
*/ 

int main()
  {
	
	SetConsoleTextAttribute(hout, yellowFontBlackText);
	system("cls");
	COORD startPos{ 0 };
	startPos.X = 10;
	startPos.Y = 1;
	auto maze = new Maze(80, 20, startPos);
	maze->PrintMaze(hout);
	startPos.X = 20;
	startPos.Y = 10;
	Rectangle2 a(5, 10, startPos, cyanText);
	Figure* figure = new Rectangle2(5, 10, startPos, cyanText);  // still e0079
	maze->PrintFigure(hout, figure->GetThisFigCoordsSet(), cyanText | yellowFontBlackText);
	Sleep(3000);
	maze->ClearMaze(hout);
	Sleep(4100);
 	delete maze;
	return 0;

}
