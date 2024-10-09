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
	count++;
	this->startPos = startPos;
	this->colour = colour;
	id = count;
	idToFigurePtrMap[id] = this;
	figTypeEnum = DEFAULT_TYPE;
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
	
int Figure::DeleteThisFig()
{
	for (size_t index = 0; index < figDrawOrderDeque.size(); index++)
	{
		auto curFigurePtr = figDrawOrderDeque[index];
		if (curFigurePtr->IsEqual(this))
		{
			idToFigurePtrMap.erase(this->id);
			figDrawOrderDeque.erase(figDrawOrderDeque.begin() + index);
			return 0;
		}
	}
	return -1;
}

unsigned int Figure::GetID() const
{
	return this->id;
}

bool Figure::IfDuplicate() const
{
	for (auto& curFigure : figDrawOrderDeque)
	{
		if (this->IsEqual(curFigure))
		{
			return true;
		}
	}
	return false;
}

FIGURE_TYPE Figure::GetType() const
{
	return this->figTypeEnum;
}

string Figure::GetFigNameStr() const
{
	return this->FIGURE_NAME;
}

WORD Figure::GetThisFigColour() const
{
	return this->colour;
}

deque <Figure*> Figure::GetAllFigsPtrInDrawOrder() 
	{
		return figDrawOrderDeque;
	}
	
unordered_set<COORD, COORDHash, COORDEqual> Figure::GetThisFigCoordsSet() const 
{
	return this->figureCOORDSet;
}

bool Figure::AreSetsEqual(const unordered_set <COORD, COORDHash, COORDEqual> inSet1, const unordered_set<COORD, COORDHash, COORDEqual> inSet2)
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

COORD Figure::GetThisFigStartPos() const
{
	return this->startPos;
}

Rectangle2::Rectangle2(const COORD& startPos, const short& width, const short& height, const WORD& colour)
:	width(width*2),
	height(height),
	Figure(startPos, colour) 
{
	if (this->width < 0 || this->height < 0)
	{
		throw invalid_argument("Base and Width must be at least 0.");
	}
	FIGURE_NAME = "RECTANGLE";

	for (COORD curCoord = startPos; curCoord.X <= startPos.X + this->width; curCoord.X++)  // horizontal
	{
		figureCOORDSet.insert(curCoord);
		curCoord.Y += this->height;
		figureCOORDSet.insert(curCoord);
		curCoord.Y -= this->height;
	}
	for (COORD curCoord = startPos; curCoord.Y <= startPos.Y + this->height; curCoord.Y++)  // vertical
	{
		figureCOORDSet.insert(curCoord);
		curCoord.X += this->width;
		figureCOORDSet.insert(curCoord);
		curCoord.X -= this->width;
	}
	
	if (this->IfDuplicate())
	{
		throw runtime_error("The figure is duplicate");
	}

	figDrawOrderDeque.push_back(this);  // push new instance to the back
	figTypeEnum = RECTANGLE;
}
	
size_t Rectangle2::GetWidth() const
{
	return this->width;
}
size_t Rectangle2::GetHeight() const
{
	return this->height;
}


/*
bool Rectangle2::IsEqual(Figure* other) const
{
	if (!dynamic_cast<Rectangle2*>(other))
	{
		return false;
	}
	COORD otherStartPos(other->GetThisFigStartPos());
	auto set1 = other->GetThisFigCoordsSet();
	auto set2 = this->GetThisFigCoordsSet();

	return AreSetsEqual(set1, set2) &&  // when trying to grt the other-> i get exception in the file type_traits, why is that
		   this->startPos.X == otherStartPos.X &&
		   this->startPos.Y == otherStartPos.Y; 
}
*/

bool Rectangle2::IsEqual(Figure* other) const
{
	Rectangle2* otherRect = dynamic_cast<Rectangle2*>(other);
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
	return format(" {} {} {} {} {} ", startPos.X, startPos.Y, width, height, colour);
}


Square::Square(const COORD& startPos, 
			   const short& side,
			   const WORD& colour)
:	Rectangle2(startPos, side, side, colour)
{
	figTypeEnum = SQUARE;
	FIGURE_NAME = "SQUARE";
}

string Square::GetFigProperties()
{
	return format(" {} {} {} {} ", startPos.X, startPos.Y, height, colour);
}


Triangle::Triangle(const COORD& startPos,
				   const short& base,  
				   const WORD& colour)
		: base(base + TRIANGLE_MIN_BASE),
		Figure(startPos, colour)  
	{
		if (this->base < TRIANGLE_MIN_BASE)
		{
			throw invalid_argument("Base must be at least 0.");
		}
		else if (this->base % 2 != 0)  // Base must not be odd
		{
			this->base++; 
		}
		
		FIGURE_NAME = "TRIANGLE";

		this->figureCOORDSet.insert(startPos);
	
		COORD baseCOORD = startPos;
		COORD tempCOORD = startPos;

		for (size_t index = 1; index <= this->base; index++)
		{
			baseCOORD.X++;
			this->figureCOORDSet.insert(baseCOORD);
			
			tempCOORD.X = baseCOORD.X;
			
			if (index < (double)this->base / 2 + 1)
			{
				tempCOORD.Y--;
			}
			else
			{
				tempCOORD.Y++;
			}
			
			this->figureCOORDSet.insert(tempCOORD);
		}
		
		if (this->IfDuplicate())
		{
			throw runtime_error("The figure is duplicate");
		}

		figDrawOrderDeque.push_back(this);
		figTypeEnum = TRIANGLE;
	}

string Triangle::GetFigProperties() 
{
	return format(" {} {} {} {} ", startPos.X, startPos.Y, base, colour);
}
	
bool Triangle::IsEqual(Figure* other) const
{
	if (!dynamic_cast<Triangle*>(other))
	{
		return false;
	}
	COORD otherStartPos(this->startPos);
	return AreSetsEqual(this->GetThisFigCoordsSet(), other->GetThisFigCoordsSet()) &&
		this->startPos.X == otherStartPos.X &&
		this->startPos.Y == otherStartPos.Y;
}

Circle::Circle(const COORD & startPos, 
			   const short& radius, 
			   const WORD & colour)
	:	radius(radius), 
		Figure(startPos, colour)
{
	if (radius < 0)
	{
		throw invalid_argument("Radius must be at least 0.");
	}

	FIGURE_NAME = "CIRCLE";

	const size_t verticalRadius = radius;
	const size_t horzontalRadius = radius * 2;

	COORD curCOORD{startPos.X - horzontalRadius + 1, startPos.Y - verticalRadius - 1};
	for (;curCOORD.X <= startPos.X + horzontalRadius - 1; curCOORD.X ++)
	{
		this->figureCOORDSet.insert(curCOORD);
		COORD tempCOORD{ curCOORD.X, curCOORD.Y + 2 * verticalRadius + 2 };
		this->figureCOORDSet.insert(tempCOORD);
	}
	
	curCOORD.X = startPos.X - horzontalRadius - 1;
	curCOORD.Y = startPos.Y - verticalRadius + 1;

	for (; curCOORD.Y <= startPos.Y + verticalRadius - 1; curCOORD.Y ++)
	{
		if (curCOORD.Y == startPos.Y)
		{
			curCOORD.X--;  // -= 1
			this->figureCOORDSet.insert(curCOORD);					
			curCOORD.X++;											// += 1
			COORD tempCOORD{ curCOORD.X + horzontalRadius * 2 + 2 + 1, curCOORD.Y };
			this->figureCOORDSet.insert(tempCOORD);
		}
		else
		{
			this->figureCOORDSet.insert(curCOORD);				
			COORD tempCOORD{ curCOORD.X + horzontalRadius * 2 + 2, curCOORD.Y };
			this->figureCOORDSet.insert(tempCOORD);
		}
		
	}
		
	for (int i = 1; i <= 2; i ++)
	{
		COORD temp{ startPos.X - horzontalRadius, startPos.Y + verticalRadius * pow(-1, i) };
		this->figureCOORDSet.insert(temp);
		temp.X += 2 * horzontalRadius;
		this->figureCOORDSet.insert(temp);
	}

	if (this->IfDuplicate())
	{
		throw runtime_error("The figure is duplicate");
	}

	figDrawOrderDeque.push_back(this);
}

string Circle::GetFigProperties() 
{
	return format(" {} {} {} {} ", startPos.X, startPos.Y, radius, colour);
}

bool Circle::IsEqual(Figure* other) const
{
	if (!dynamic_cast<Circle*>(other))
	{
		return false;
	}
	COORD otherStartPos(this->startPos);
	return AreSetsEqual(this->GetThisFigCoordsSet(), other->GetThisFigCoordsSet()) &&
		this->startPos.X == otherStartPos.X &&
		this->startPos.Y == otherStartPos.Y;
}

string& Figure::GetConfigurationStr()
{
	string result;
	string type;
	for (Figure* curFig : figDrawOrderDeque)
	{
		string curID = to_string(curFig->id);
		if (dynamic_cast<Square*>(curFig))
		{
			result += curID + "SQUARE";
		}
		else if (dynamic_cast<Triangle*>(curFig))
		{
			result += curID + "TRIANGLE";
		}
		else if (dynamic_cast<Circle*>(curFig))
		{
			result += curID + "CIRCLE";
		}
		else
		{
			result += curID + "RECTANGLE";
		}
		result += curFig->GetFigProperties() + '\n';
	}
	return result;
}
