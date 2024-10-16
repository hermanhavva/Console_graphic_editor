#pragma once

#include "maze.h"


bool Polygon1::IsCoordInBounds(const COORD& inCoord) const noexcept
{
	return (inCoord.X >= startLeftPoint.X + 2 &&
			inCoord.X <= startLeftPoint.X + width - 2 &&
			inCoord.Y > startLeftPoint.Y &&
			inCoord.Y < startLeftPoint.Y + height);
}



Polygon1::Polygon1(const size_t& width, 
				   const size_t& height, 
  				   const COORD   startLeftPoint)
			   :   width(width),
				   height(height),
				   startLeftPoint(startLeftPoint) { }

void Polygon1::ClearPolygon(HANDLE hout) const 
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

void Polygon1::PrintPolygon(HANDLE hout) const
{
	SetConsoleTextAttribute(hout, defaultColour);
	SetConsoleCursorPosition(hout, startLeftPoint);
		
	for (COORD curCOORD = startLeftPoint; curCOORD.X <= startLeftPoint.X + width; curCOORD.X ++)  // horizontal
	{
		SetConsoleCursorPosition(hout, curCOORD);
		cout << symbol;
		curCOORD.Y += height;
		SetConsoleCursorPosition(hout, curCOORD);
		cout << symbol;
		curCOORD.Y -= height;  
			
		if (curCOORD.X % 3 == 0)
		{
			COORD temp{ curCOORD.X, curCOORD.Y - 1};
			SetConsoleCursorPosition(hout, temp);
			cout << temp.X;
		}
		
	}
	for (COORD curCOORD = startLeftPoint; curCOORD.Y <= startLeftPoint.Y + height; curCOORD.Y++)  // vertical
	{
		SetConsoleCursorPosition(hout, curCOORD);
		cout << symbol;
		curCOORD.X += width;
		SetConsoleCursorPosition(hout, curCOORD);
		cout << symbol;
		curCOORD.X -= width; 
		
		if (curCOORD.Y % 2 == 0)
		{
			COORD temp{ curCOORD.X - 3, curCOORD.Y };
			SetConsoleCursorPosition(hout, temp);
			cout << temp.Y;
		}
			
	}
}
bool Polygon1::IsFigurePrintable(const COORD& startPos, const unordered_set<COORD, COORDHash, COORDEqual>& figereSetCoords) const noexcept
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

void Polygon1::PrintFigure(const HANDLE& hout, const unordered_set<COORD, COORDHash, COORDEqual>& figCoordSet, const WORD& color) const
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