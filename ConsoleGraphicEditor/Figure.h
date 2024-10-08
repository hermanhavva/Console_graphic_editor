#pragma once
#include <deque>
#include <unordered_set>
#include <unordered_map>
#include <string>
#include "COORD_logic.h" 

using namespace std;

enum FIGURE_TYPE
{
    RECTANGLE = 1,
    SQUARE = 2,
    TRIANGLE = 3,
    CIRCLE = 4,
    DEFAULT = 5
};

class Figure abstract
{
public:
    Figure(const COORD& startPos, const WORD& colour);
    virtual ~Figure() = default;

    virtual string GetFigProperties() = 0;

    int SelectFigById(const size_t& id);
    FIGURE_TYPE GetType() const;

    static deque<Figure*> GetAllFigsCoordsInDrawOrder();
    static string& GetConfigurationStr();

    unordered_set<COORD, COORDHash, COORDEqual> GetThisFigCoordsSet() const;

protected:
    FIGURE_TYPE figTypeEnum;
    COORD startPos;
    char symbolToDraw = '*';
    WORD colour;
    unordered_set<COORD, COORDHash, COORDEqual> figureCoordSet;

    static inline size_t id = 0;
    inline static deque<Figure*> figDrawOrderDeque;
    inline static unordered_map<size_t, Figure*> idToFigurePtrMap;

    bool AreSetsEqual(const unordered_set<COORD, COORDHash, COORDEqual>& inSet1, const unordered_set<COORD, COORDHash, COORDEqual>& inSet2);
};

// Rectangle class derived from Figure
class Rectangle2 : public Figure
{
public:
    Rectangle2(const size_t& width, const size_t& height, const COORD& startPos, const WORD& colour);
    string GetFigProperties() override;

protected:
    const size_t width = 0;
    const size_t height = 0;
};

// Square class derived from Rectangle
class Square : public Rectangle2
{
public:
    Square(const size_t& side, const COORD& startPos, const WORD& colour);
    string GetFigProperties() override;
};

// Triangle class derived from Figure
class Triangle : public Figure
{
public:
    Triangle(const size_t& base, const COORD& startPos, const WORD& colour);
    string GetFigProperties() override;

private:
    size_t base;
};

// Circle class derived from Figure
class Circle : public Figure
{
public:
    Circle(const COORD& startPos, const size_t& radius, const WORD& colour);
    string GetFigProperties() override;

private:
    size_t radius;
};

