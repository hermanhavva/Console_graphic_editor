#include <windows.h> 
#include <iostream>

using namespace std;

class Maze
{
private:
	const size_t width;
	const size_t height; 
	const COORD startLeftPoint;
	const string  symbol = "\xB1\xB1";
	const WORD defaultColour = BACKGROUND_GREEN | BACKGROUND_RED | BACKGROUND_INTENSITY;

public:

	Maze(const size_t& width, 
		 const size_t& height, 
		 const COORD   startLeftPoint)
		:	width(width),
			height(height),
			startLeftPoint(startLeftPoint) { }

	void Erase(HANDLE hout) 
	{

	}
	void Print(HANDLE hout) const
	{
		SetConsoleTextAttribute(hout, defaultColour);
		SetConsoleCursorPosition(hout, startLeftPoint);
		
		for (COORD curCoord = startLeftPoint; curCoord.X <= startLeftPoint.X + width; curCoord.X ++)  // horizontal
		{
			SetConsoleCursorPosition(hout, curCoord);
			cout << symbol;
			curCoord.Y += height;
			SetConsoleCursorPosition(hout, curCoord);
			cout << symbol;
			curCoord.Y -= height;
		}
		for (COORD curCoord = startLeftPoint; curCoord.Y <= startLeftPoint.Y + height; curCoord.Y++)  // vertical
		{
			SetConsoleCursorPosition(hout, curCoord);
			cout << symbol;
			curCoord.X += width;
			SetConsoleCursorPosition(hout, curCoord);
			cout << symbol;
			curCoord.X -= width; 
		}

	}

};