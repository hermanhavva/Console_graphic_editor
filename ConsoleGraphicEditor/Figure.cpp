#include <deque>
#include <windows.h>
#include <memory>
#include <unordered_set>
#include <iostream>

using namespace std;

struct COORDHash
{
	size_t operator()(const COORD& coord) const
	{
		return std::hash<SHORT>()(coord.X) ^ (std::hash<SHORT>()(coord.Y) << 1);
	}
};
struct COORDEqual
{
	bool operator()(const COORD& coord1, const COORD& coord2) const
	{
		return coord1.X == coord2.X && coord1.Y == coord2.Y;
	}
};

class Figure abstract
{
public:
	virtual void Draw(HANDLE) = 0;
	Figure(const COORD& startPos, const WORD& colour)
	{
		id++;
		this->startPos = startPos;
		this->colour = colour;
	}
	virtual ~Figure() = default;
	
	
	static deque <unordered_set<COORD, COORDHash, COORDEqual>> GetAllFiguresCoordsInDrawOrder() 
	{
		return figuresDrawDeque;
	}
	unordered_set<COORD, COORDHash, COORDEqual> GetThisCoords(const HANDLE& hout) const 
	{
		return this->figureCoordSet;
	}
protected:

	COORD startPos;
	char symbolToDraw = '*';
	static inline size_t id = 0;
	inline static deque <unordered_set<COORD, COORDHash, COORDEqual>> figuresDrawDeque;
	WORD colour; 
	unordered_set<COORD, COORDHash, COORDEqual> figureCoordSet;

};
class Rectangle : public Figure
{
public:
	Rectangle(const size_t& width, const size_t& height, const COORD& startPos, const WORD& colour)
		:	width(width),
			height(height),
			Figure(startPos, colour) 
	{
		for (COORD curCoord = startPos; curCoord.X <= startPos.X + width; curCoord.X++)  // horizontal
		{
			figureCoordSet.insert(curCoord);
			curCoord.Y += height;
			figureCoordSet.insert(curCoord);
			curCoord.Y -= height;
		}
		for (COORD curCoord = startPos; curCoord.Y <= startPos.Y + height; curCoord.Y++)  // vertical
		{
			figureCoordSet.insert(curCoord);
			curCoord.X += width;
			figureCoordSet.insert(curCoord);
			curCoord.X -= width;
		}
		//  here check if it is possible to draw 

		figuresDrawDeque.push_back(figureCoordSet);  // push new coordinates to the back
	}
	
	virtual ~Rectangle() = default;
	

protected:
	const size_t width;
	const size_t height;
};
class Square : public Rectangle
{
public:	
	Square(const size_t& side, 
		   const COORD& startPos, 
		   const WORD& colour)
		: Rectangle(side, side, startPos, colour) 
	{}
};
class Triangle : Figure
{
public:
	Triangle(const size_t& base, 
			 const COORD& startPos, 
			 const WORD& colour)
		: base(base),
		Figure(startPos, colour)  // here calling base class constructor
	{
		if (base < 5)
		{
			throw std::invalid_argument("Base must be at least 5.");
		}
		else if (base % 2 == 0)
		{
			this->base++; 
		}
		this->figureCoordSet.insert(startPos);
	
		COORD curCoord = startPos;
		
		for (size_t index = 1; index <= base; index++)
		{
			curCoord.X ++;
			this->figureCoordSet.insert(curCoord);

			if (index < (double)base / 2 + 1)
			{
				curCoord.Y--;
			}
			else
			{
				curCoord.Y++;
			}
			
			this->figureCoordSet.insert(curCoord);
		}
		//  here check if it is possible to draw 
		figuresDrawDeque.push_back(figureCoordSet);
	}

private:
	size_t base;
	
};