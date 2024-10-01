#include <windows.h> 
#include <iostream>
#include <unordered_set>

using namespace std;

class Maze
{
private:
	const size_t width;
	const size_t height; 
	const COORD startLeftPoint;
	const string  symbol = "\xB1\xB1";
	const WORD defaultColour = BACKGROUND_GREEN | BACKGROUND_RED | BACKGROUND_INTENSITY;
	bool IsCoordInBounds(const COORD& inCoord) const
	{
		return (inCoord.X > startLeftPoint.X &&
				inCoord.X < startLeftPoint.X + width &&
				inCoord.Y > startLeftPoint.Y &&
				inCoord.Y < startLeftPoint.Y + height);
	}

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
	void PrintMaze(HANDLE hout) const
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
	bool IsFigurePrintable(COORD startPos, unordered_set<COORD> figereSetCoords) const
	{
		// if more than two dots(and startPos) are printable -> the figure is printable 
		if (!(IsCoordInBounds(startPos)))
		{
			return false;
		}

		size_t counter = 0;
		for (auto& coord : figereSetCoords)
		{
			if (IsCoordInBounds(coord))
			{
				counter++;
			}
		}
		return counter > 2;  			
	}

};