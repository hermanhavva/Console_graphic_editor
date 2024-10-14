#pragma once

#include <windows.h>
#include <iostream>
#include <unordered_set>
#include "COORD_logic.h"

using namespace std;

class Polygon1
{
private:
	const size_t width;
	const size_t height;
	const COORD startLeftPoint;
	const string symbol = "\xB1\xB1";
	const WORD defaultColour = BACKGROUND_GREEN | BACKGROUND_RED | BACKGROUND_INTENSITY;
	bool  IsCoordInBounds(const COORD& inCoord) const;

public:
	Polygon1(const size_t& width, const size_t& height, const COORD startLeftPoint);
	void ClearPolygon(HANDLE hout) const;
	void PrintPolygon(HANDLE hout) const;
	void PrintFigure(const HANDLE& hout, const unordered_set<COORD, COORDHash, COORDEqual>& figCoordSet, const WORD& color) const;
	bool IsFigurePrintable(const COORD& startPos, const unordered_set<COORD, COORDHash, COORDEqual>& figereSetCoords) const;
};
