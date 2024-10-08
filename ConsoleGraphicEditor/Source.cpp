#include <iostream>
#include <windows.h>

#include "maze.cpp"
#include "colours.h"
#include "Figure.h" 
using namespace std;
 

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
	Rectangle2 a(startPos, 2, 10, cyanText);
	Figure* figure = new Circle(startPos, 0, cyanText);  // still e0079
	maze->PrintFigure(hout, figure->GetThisFigCoordsSet(), cyanText | yellowFontBlackText);
	Sleep(10000);
	delete figure;
	maze->ClearMaze(hout);
	Sleep(4100);
 	delete maze;
	return 0;

}
