#include <deque>
#include <unordered_set>
#include <unordered_map>
#include <string>
#include <format>
#include "Figure.h"
#include "COORD_logic.h" 
using namespace std;




Figure::Figure(const COORD& startPos, const WORD& colour)
{
	id++;
	this->startPos = startPos;
	this->colour = colour;
	idToFigurePtrMap[id] = this;
	figTypeEnum = DEFAULT;
}
	
int Figure::SelectFigById(const size_t& id)
	{
		if (!idToFigurePtrMap.contains(id))
		{
			return -1;
		}
		Figure* selectedFigPtr = idToFigurePtrMap[id];
		unordered_set<COORD, COORDHash, COORDEqual> selectedFigCoordSet = selectedFigPtr->GetThisFigCoordsSet();

		for (size_t index = 0; index < figDrawOrderDeque.size(); index++)
		{
			auto curCoordSet = figDrawOrderDeque[index]->GetThisFigCoordsSet();

			if (AreSetsEqual(selectedFigCoordSet, curCoordSet))
			{
				figDrawOrderDeque.erase(figDrawOrderDeque.begin() + index);
				figDrawOrderDeque.push_back(this);
				return 0;
			}
		}
		
		return -1;
	}
	
FIGURE_TYPE Figure::GetType() const
	{
		return this->figTypeEnum;
	}

deque <Figure*> Figure::GetAllFigsCoordsInDrawOrder() 
	{
		return figDrawOrderDeque;
	}
	
unordered_set<COORD, COORDHash, COORDEqual> Figure::GetThisFigCoordsSet() const 
{
	return this->figureCoordSet;
}

bool Figure::AreSetsEqual(const unordered_set <COORD, COORDHash, COORDEqual>& inSet1, const unordered_set<COORD, COORDHash, COORDEqual>& inSet2)
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

Rectangle2::Rectangle2(const COORD& startPos, const size_t& width, const size_t& height, const WORD& colour)
:	width(width*2),
	height(height),
	Figure(startPos, colour) 
{
	for (COORD curCoord = startPos; curCoord.X <= startPos.X + this->width; curCoord.X++)  // horizontal
	{
		figureCoordSet.insert(curCoord);
		curCoord.Y += this->height;
		figureCoordSet.insert(curCoord);
		curCoord.Y -= this->height;
	}
	for (COORD curCoord = startPos; curCoord.Y <= startPos.Y + this->height; curCoord.Y++)  // vertical
	{
		figureCoordSet.insert(curCoord);
		curCoord.X += this->width;
		figureCoordSet.insert(curCoord);
		curCoord.X -= this->width;
	}
	figDrawOrderDeque.push_back(this);  // push new instance to the back
	figTypeEnum = RECTANGLE;
}
	
	//virtual ~Rectangle() = default;
string Rectangle2::GetFigProperties() 
{
	return format(" {} {} {} {} {} ", startPos.X, startPos.Y, width, height, colour);
}


Square::Square(const COORD& startPos, 
			   const size_t& side,
			   const WORD& colour)
:	Rectangle2(startPos, side, side, colour)
{
	figTypeEnum = SQUARE;
}

string Square::GetFigProperties()
{
	return format(" {} {} {} {} ", startPos.X, startPos.Y, height, colour);
}


Triangle::Triangle(const COORD& startPos,
				   const size_t& base,  
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
		
		figDrawOrderDeque.push_back(this);
		figTypeEnum = TRIANGLE;
	}

string Triangle::GetFigProperties() 
{
	return format(" {} {} {} {} ", startPos.X, startPos.Y, base, colour);
}
	


Circle::Circle(const COORD & startPos, const size_t & radius, const WORD & colour)
	:	radius(radius), 
		Figure(startPos, colour)
{
	const size_t verticalRadius = radius;
	const size_t horizontalRadius = radius * 2;
	
	this->figureCoordSet.insert(startPos);

	COORD curCOORD{startPos.X - radius + 1, startPos.Y - radius - 1};
	for (;curCOORD.X <= startPos.X + horizontalRadius - 1; curCOORD.X ++)
	{
		this->figureCoordSet.insert(curCOORD);
		COORD tempCOORD{ curCOORD.X, curCOORD.Y + 2 * verticalRadius + 2 };
		this->figureCoordSet.insert(tempCOORD);
	}
	
	curCOORD.X = startPos.X - horizontalRadius - 1;
	curCOORD.Y = startPos.Y - verticalRadius + 1;

	for (; curCOORD.Y <= startPos.Y + verticalRadius - 1; curCOORD.Y ++)
	{
		this->figureCoordSet.insert(curCOORD);
		COORD tempCOORD{ curCOORD.X + horizontalRadius * 2 + 2, curCOORD.Y};
		this->figureCoordSet.insert(tempCOORD);
	}
		
	for (int i = 1; i <= 2; i ++)
	{
		COORD temp{ startPos.X - radius, startPos.Y + verticalRadius * pow(-1, i) };
		this->figureCoordSet.insert(temp);
		temp.X += 2 * horizontalRadius;
		this->figureCoordSet.insert(temp);
	}

	figDrawOrderDeque.push_back(this);
}
string Circle::GetFigProperties() 
{
	return format(" {} {} {} {} ", startPos.X, startPos.Y, radius, colour);
}



string& Figure::GetConfigurationStr()
{
	string result;
	string type;
	for (Figure* curFig : figDrawOrderDeque)
	{
		if (dynamic_cast<Square*>(curFig))
		{
			result += "SQUARE";
		}
		else if (dynamic_cast<Triangle*>(curFig))
		{
			result += "TRIANGLE";
		}
		else if (dynamic_cast<Circle*>(curFig))
		{
			result += "CIRCLE";
		}
		else
		{
			result += "RECTANGLE";
		}
		result += curFig->GetFigProperties() + '\n';
	}
	return result;
}
