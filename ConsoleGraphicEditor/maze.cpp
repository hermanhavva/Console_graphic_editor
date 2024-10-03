#include <windows.h> 
#include <iostream>
#include <unordered_set>
#include "COORD_logic.h" 

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
		return (inCoord.X >= startLeftPoint.X + 2 &&
				inCoord.X <= startLeftPoint.X + width - 2 &&
				inCoord.Y > startLeftPoint.Y + 1 &&
				inCoord.Y < startLeftPoint.Y + height);
	}

public:

	Maze(const size_t& width, 
		 const size_t& height, 
		 const COORD   startLeftPoint)
		:	width(width),
			height(height),
			startLeftPoint(startLeftPoint) { }

	void ClearMaze(HANDLE hout) const 
	{
		COORD curCOORD = COORD{ static_cast<SHORT>(startLeftPoint.X + 2), static_cast<SHORT>(startLeftPoint.Y + 1) };

		for (;curCOORD.Y < startLeftPoint.Y + height; curCOORD.Y ++)
		{
			for (curCOORD.X = startLeftPoint.X + 2; curCOORD.X <= startLeftPoint.X + width - 2; curCOORD.X++)
			{
				SetConsoleCursorPosition(hout, curCOORD);
				cout << " ";
			}
		}
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
	bool IsFigurePrintable(const COORD& startPos, const unordered_set<COORD, COORDHash, COORDEqual>& figereSetCoords) const
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

	void PrintFigure(const HANDLE& hout, const unordered_set<COORD, COORDHash, COORDEqual>& figCoordSet, const WORD& color) const
	{
		SetConsoleTextAttribute(hout, color);
	
		for (auto& coord : figCoordSet)
		{
			if (IsCoordInBounds(coord))
			{
				SetConsoleCursorPosition(hout, coord);
				cout << "*";
			}
		}
	}
};