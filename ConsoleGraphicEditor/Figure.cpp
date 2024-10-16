#include <deque>
#include <unordered_set>
#include <unordered_map>
#include <string>
#include <format>
#include "Figure.h"
#include "COORD_logic.h" 
using namespace std;

Figure::Figure(const COORD& startPos, const int& colour)
{
	count++;
	this->startPos = startPos;
	this->colour = colourEnumToWordMap.at(colour);
	id = count;
//	figTypeEnum = DEFAULT_TYPE;
}

unsigned int Figure::GetID() const
{
	return this->id;
}

string Figure::GetFigNameStr() const
{
	return this->FIGURE_NAME;
}

WORD Figure::GetThisFigColour() const
{
	return this->colour;
}

unordered_set<COORD, COORDHash, COORDEqual> Figure::GetThisFigCOORDSet() const 
{
	return this->figureCOORDSet;
}

COORD Figure::GetThisFigStartPos() const
{
	return this->startPos;
}

void Figure::SetColour(const int& colour)
{
	this->colour = colourEnumToWordMap.at(colour);
}

void Figure::SetStartPos(const COORD& startPos)
{
	this->startPos = startPos;
	GenerateSetOfCOORD();
}

Rectangle2::Rectangle2(const COORD& startPos, const short& width, const short& height, const int& colour)
:	width(width),
	height(height),
	Figure(startPos, colour) 
{

	if (this->width < 0 || this->height < 0)
	{
		throw invalid_argument("Base and Width must be at least 0.");
	}
	FIGURE_NAME = "RECTANGLE";
	//figTypeEnum = RECTANGLE;
	GenerateSetOfCOORD();
}


void Rectangle2::SetWidthAndHeight(const size_t& width, const size_t& height)
{
	this->width = width;
	this->height = height;
	GenerateSetOfCOORD();
}

void Rectangle2::GenerateSetOfCOORD() 
{
	figureCOORDSet.clear();

	const short unsigned HORIZONTAL_WIDTH = width * 2;

	for (COORD curCOORD = startPos; curCOORD.Y < startPos.Y + this->height; curCOORD.Y++)
	{
		for (;curCOORD.X < startPos.X + HORIZONTAL_WIDTH; curCOORD.X++)
		{
			figureCOORDSet.insert(curCOORD);
		}
		curCOORD.X = startPos.X;
	}
	/*
	for (COORD curCoord = startPos; curCoord.X <= startPos.X + HORIZONTAL_WIDTH; curCoord.X++)  // horizontal
	{
		figureCOORDSet.insert(curCoord);
		curCoord.Y += this->height;
		figureCOORDSet.insert(curCoord);
		curCoord.Y -= this->height;
	}
	for (COORD curCoord = startPos; curCoord.Y <= startPos.Y + this->height; curCoord.Y++)  // vertical
	{
		figureCOORDSet.insert(curCoord);
		curCoord.X += HORIZONTAL_WIDTH;
		figureCOORDSet.insert(curCoord);
		curCoord.X -= HORIZONTAL_WIDTH;
	}
	*/
}

size_t Rectangle2::GetWidth() const
{
	return this->width;
}

size_t Rectangle2::GetHeight() const
{
	return this->height;
}

bool Rectangle2::IsEqual(shared_ptr<Figure> other) const
{
	Rectangle2* otherRect = dynamic_cast<Rectangle2*>(other.get());
	if (!otherRect)
	{
		return false;
	}
	COORD otherStartPos(other->GetThisFigStartPos());
	COORD thisStratPos(this->GetThisFigStartPos());
	
	return (thisStratPos.X == otherStartPos.X && otherStartPos.Y == thisStratPos.Y && this->GetWidth() == otherRect->GetWidth() && this->GetHeight() == otherRect->GetHeight());
}

string Rectangle2::GetFigProperties() 
{
	return format(" {} {} {} {} {} ", startPos.X, startPos.Y, width, height, colourWordToEnumMap.at(colour));
}

Square::Square(const COORD& startPos, 
			   const short& side,
			   const int& colour)
:	Rectangle2(startPos, side, side, colour)
{
	FIGURE_NAME = "SQUARE";
}

string Square::GetFigProperties()
{
	return format(" {} {} {} {} ", startPos.X, startPos.Y, height, colourWordToEnumMap.at(colour));
}

Triangle::Triangle(const COORD& startPos,
				   const size_t& base,  
				   const int& colour)
		: base(base),
		Figure(startPos, colour)
{
	
	FIGURE_NAME = "TRIANGLE";
	GenerateSetOfCOORD();
}

void Triangle::GenerateSetOfCOORD()
{
	short int intermediateBase = this->base + TRIANGLE_MIN_BASE;

	if (base < 0)
	{
		throw invalid_argument("Base must be at least 0.");
	}
	else if (intermediateBase % 2 != 0)  // Base must not be odd
	{
		intermediateBase++;
	}
	figureCOORDSet.clear();
	this->figureCOORDSet.insert(startPos);  

	COORD tempCOORD = startPos;
	short int halfBase = intermediateBase / 2;

	// Loop to generate both sides and fill the inside
	for (short int row = 0; row <= halfBase; ++row)
	{
		// Calculate left and right boundaries for each row
		short int leftX = startPos.X - row;
		short int rightX = startPos.X + row;

		// Loop to fill the entire row between leftX and rightX
		for (short int x = leftX; x <= rightX; ++x)
		{
			tempCOORD.X = x;
			tempCOORD.Y = startPos.Y + row;  // Move downwards as we go through the rows

			this->figureCOORDSet.insert(tempCOORD);  // Insert every point between left and right boundary
		}
	}
	
	/*
	figureCOORDSet.clear();

	this->figureCOORDSet.insert(startPos);

	COORD baseCOORD = startPos;
	COORD tempCOORD = startPos;

	for (size_t index = 1; index <= intermediateBase; index++)
	{
		baseCOORD.X++;
		this->figureCOORDSet.insert(baseCOORD);

		tempCOORD.X = baseCOORD.X;

		if (index < (double)intermediateBase / 2 + 1)
		{
			tempCOORD.Y--;
		}
		else
		{
			tempCOORD.Y++;
		}

		this->figureCOORDSet.insert(tempCOORD);
	}*/
}

void Triangle::SetBase(const size_t& base)
{
	this->base = base;
	GenerateSetOfCOORD();
}

size_t Triangle::GetBase() const
{
	return this->base;
}

string Triangle::GetFigProperties() 
{
	return format(" {} {} {} {} ", startPos.X, startPos.Y, base, colourWordToEnumMap.at(colour));
}
	
bool Triangle::IsEqual(shared_ptr<Figure> other) const
{
	if (!dynamic_cast<Triangle*>(other.get()))
	{
		return false;
	}
	COORD otherStartPos(this->startPos);
	return AreCOORDSetsEqual(this->GetThisFigCOORDSet(), other->GetThisFigCOORDSet()) &&
		this->startPos.X == otherStartPos.X &&
		this->startPos.Y == otherStartPos.Y;
}

Circle::Circle(const COORD & startPos, 
			   const size_t& radius, 
			   const int& colour)
	:	radius(radius), 
		Figure(startPos, colour)
{

	FIGURE_NAME = "CIRCLE";
	GenerateSetOfCOORD();
}

void Circle::GenerateSetOfCOORD()
{
	if (radius < 0)
	{
		throw runtime_error("The radius must be >= 0");
	}
	figureCOORDSet.clear();
	
	if (radius == 0)
	{
		figureCOORDSet.insert(startPos);
	}
	else
	{
		const short int VERTICAL_RADIUS = radius;
		const short int HORIZONTAL_RADIUS = radius * 2;

		// Starting from the top of the hexagon and moving downward
		for (int yOffset = -VERTICAL_RADIUS; yOffset <= VERTICAL_RADIUS; ++yOffset)
		{
			int xOffsetLimit = HORIZONTAL_RADIUS - abs(yOffset);

			COORD curCOORD{};

			// Loop from the left side to the right side of the hexagon for this row
			for (int xOffset = -xOffsetLimit; xOffset <= xOffsetLimit; ++xOffset)
			{
				curCOORD.X = startPos.X + xOffset;
				curCOORD.Y = startPos.Y + yOffset;

				this->figureCOORDSet.insert(curCOORD);  
			}
		}
	}

	
	/*
	const size_t VERTICAL_RADIUS = radius;
	const size_t HORIZONTAL_RADIUS = radius * 2;

	COORD curCOORD{ startPos.X - HORIZONTAL_RADIUS + 1, startPos.Y - VERTICAL_RADIUS - 1 };
	for (; curCOORD.X <= startPos.X + HORIZONTAL_RADIUS - 1; curCOORD.X++)
	{
		this->figureCOORDSet.insert(curCOORD);
		COORD tempCOORD{ curCOORD.X, curCOORD.Y + 2 * VERTICAL_RADIUS + 2 };
		this->figureCOORDSet.insert(tempCOORD);
	}

	curCOORD.X = startPos.X - HORIZONTAL_RADIUS - 1;
	curCOORD.Y = startPos.Y - VERTICAL_RADIUS + 1;

	for (; curCOORD.Y <= startPos.Y + VERTICAL_RADIUS - 1; curCOORD.Y++)
	{
		if (curCOORD.Y == startPos.Y)
		{
			curCOORD.X--;  // -= 1
			this->figureCOORDSet.insert(curCOORD);
			curCOORD.X++;											// += 1
			COORD tempCOORD{ curCOORD.X + HORIZONTAL_RADIUS * 2 + 2 + 1, curCOORD.Y };
			this->figureCOORDSet.insert(tempCOORD);
		}
		else
		{
			this->figureCOORDSet.insert(curCOORD);
			COORD tempCOORD{ curCOORD.X + HORIZONTAL_RADIUS * 2 + 2, curCOORD.Y };
			this->figureCOORDSet.insert(tempCOORD);
		}

	}

	for (int i = 1; i <= 2; i++)
	{
		COORD temp{ startPos.X - HORIZONTAL_RADIUS, startPos.Y + VERTICAL_RADIUS * pow(-1, i) };
		this->figureCOORDSet.insert(temp);
		temp.X += 2 * HORIZONTAL_RADIUS;
		this->figureCOORDSet.insert(temp);
	}
	*/
}

void Circle::SetRadius(const size_t& radius)
{
	this->radius = radius;
	GenerateSetOfCOORD();
}

size_t Circle::GetRadius() const
{
	return this->radius;
}
string Circle::GetFigProperties() 
{
	return format(" {} {} {} {} ", startPos.X, startPos.Y, radius, colourWordToEnumMap.at(colour));
}

bool Circle::IsEqual(shared_ptr<Figure> other) const
{
	if (!dynamic_cast<Circle*>(other.get()))
	{
		return false;
	}
	COORD otherStartPos(this->startPos);
	return AreCOORDSetsEqual(this->GetThisFigCOORDSet(), other->GetThisFigCOORDSet()) &&
		this->startPos.X == otherStartPos.X &&
		this->startPos.Y == otherStartPos.Y;
}
