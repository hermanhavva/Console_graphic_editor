#include <deque>
#include <windows.h>
#include <memory>
#include <unordered_set>
#include <iostream>

using namespace std;

class Figure abstract
{

public:
	virtual void Draw(HANDLE) = 0;
	Figure(const COORD& start, const WORD& colour)
	{
		id++;
		this->start = start;
		this->colour = colour;
	}
	virtual ~Figure();
protected:
	COORD start;
	char symbolToDraw = '*';
	static inline size_t id = 0;
	deque <Figure*> figureDeque;
	WORD colour; 
	unordered_set<COORD> figureCoordSet;
private:
	 
};
class Rectangle : public Figure
{
public:
	Rectangle(const size_t& width, const size_t& height, const COORD& start, const WORD& colour)
		:	width(width),
			height(height),
			Figure(start, colour) {}
	
	virtual ~Rectangle();
	void Draw(HANDLE hout) override  
	{
		figureDeque.push_front(this);

		for (COORD curCoord = start; curCoord.X <= start.X + width; curCoord.X++)  // horizontal
		{
			SetConsoleCursorPosition(hout, curCoord);
			cout << symbolToDraw;
			curCoord.Y += height;
			SetConsoleCursorPosition(hout, curCoord);
			cout << symbolToDraw;
			curCoord.Y -= height;
		}
		for (COORD curCoord = start; curCoord.Y <= start.Y + height; curCoord.Y++)  // vertical
		{
			SetConsoleCursorPosition(hout, curCoord);
			cout << symbolToDraw;
			curCoord.X += width;
			SetConsoleCursorPosition(hout, curCoord);
			cout << symbolToDraw;
			curCoord.X -= width;
		}
	}
protected:
	const size_t width;
	const size_t height;
};
class Square : public Rectangle
{
public:	
	Square(size_t side)
	:	Rectangle(side, side, start) {}
private: 

};
class Triangle : Figure
{
public:
	Triangle(size_t side, COORD start)
		:	side(side),
			Figure(start)
	{}

private:
	size_t side;
	
};