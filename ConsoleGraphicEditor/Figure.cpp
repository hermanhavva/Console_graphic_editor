#include <deque>
#include <windows.h>
#include <unordered_set>
#include <unordered_map>
#include <iostream>
#include "COORD_logic.h" 

using namespace std;

class Figure abstract
{
public:
	virtual void Draw(HANDLE) = 0;
	Figure(const COORD& startPos, const WORD& colour)
	{
		id++;
		this->startPos = startPos;
		this->colour = colour;
		idToFigurePtrMap[id] = this;
	}
	virtual ~Figure() = default;
	
	int SelectFigById(const size_t& id)
	{
		if (!idToFigurePtrMap.contains(id))
		{
			return -1;
		}
		Figure* selectedFigPtr = idToFigurePtrMap[id];
		unordered_set<COORD, COORDHash, COORDEqual> selectedFigCoordSet = selectedFigPtr->GetThisFigCoordsSet();

		for (size_t index = 0; index < figDrawOrderDeque.size(); index++)
		{
			auto& curCoordSet = figDrawOrderDeque[index];

			if (AreSetsEqual(selectedFigCoordSet, curCoordSet))
			{
				figDrawOrderDeque.erase(figDrawOrderDeque.begin() + index);
				figDrawOrderDeque.push_back(curCoordSet);
				return 0;
			}
		}
		
		return -1;
	}
	
	static deque <unordered_set<COORD, COORDHash, COORDEqual>> GetAllFigsCoordsInDrawOrder() 
	{
		return figDrawOrderDeque;
	}
	
	unordered_set<COORD, COORDHash, COORDEqual> GetThisFigCoordsSet() const 
	{
		return this->figureCoordSet;
	}
protected:

	COORD startPos;
	char symbolToDraw = '*';
	static inline size_t id = 0;
	inline static deque <unordered_set<COORD, COORDHash, COORDEqual>> figDrawOrderDeque;
	WORD colour; 
	unordered_set<COORD, COORDHash, COORDEqual> figureCoordSet;
	inline static unordered_map<size_t, Figure*> idToFigurePtrMap;

	bool AreSetsEqual(const unordered_set <COORD, COORDHash, COORDEqual>& inSet1, const unordered_set<COORD, COORDHash, COORDEqual>& inSet2)
	{
		if (inSet1.size() != inSet2.size())
		{
			return false;
		}
		for (auto& coord : inSet1)
		{
			if (!(inSet2.contains(coord)))
			{
				return false;
			}
		}
		return true;
	}

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

		figDrawOrderDeque.push_back(figureCoordSet);  // push new coordinates to the back
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
		
		figDrawOrderDeque.push_back(figureCoordSet);
	}

private:
	size_t base;
	
};
