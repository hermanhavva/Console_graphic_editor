#pragma once

#include <string>
#include "COORD_logic.h" 
#include "colours.h"

using namespace std;

enum FIGURE_TYPE
{ 
    RECTANGLE = 1,
    SQUARE = 2,
    TRIANGLE = 3,
    CIRCLE = 4,
    DEFAULT_TYPE = 5
};

class Figure abstract
{
public:
    Figure(const COORD& startPos, const int& colour);
    
    virtual ~Figure() = default;
    virtual string GetFigProperties() = 0;
    virtual bool IsEqual(shared_ptr<Figure> other) const = 0;

    unsigned int GetID() const;

    string GetFigNameStr() const;
    COORD GetThisFigStartPos() const;

    //FIGURE_TYPE GetType() const;
    WORD GetThisFigColour() const;
  
    unordered_set<COORD, COORDHash, COORDEqual> GetThisFigCOORDSet() const;

    void SetColour(const int&);
    void SetStartPos(const COORD&);

protected:
    virtual void GenerateSetOfCOORD() = 0;
   // FIGURE_TYPE figTypeEnum;
    COORD startPos;
    string FIGURE_NAME;
    WORD colour;

    unordered_set<COORD, COORDHash, COORDEqual> figureCOORDSet;

    unsigned int id = 0;

    inline static unsigned int count = 0;
};

// Rectangle class derived from Figure
class Rectangle2 : public Figure
{
public:
    Rectangle2(const COORD& startPos, const short& width, const short& height, const int& colour);
    void SetWidthAndHeight(const size_t&, const size_t&);
    
    size_t GetWidth() const;
    size_t GetHeight() const;
    string GetFigProperties() override;

    bool IsEqual (shared_ptr<Figure> other) const override;

protected:
    void GenerateSetOfCOORD() override;
    size_t width = 0;
    size_t height = 0;
};

// Square class derived from Rectangle
class Square : public Rectangle2
{
public:
    Square(const COORD& startPos, const short& side, const int& colour);
    string GetFigProperties() override;
};

// Triangle class derived from Figure
class Triangle : public Figure
{
public:
    Triangle(const COORD& startPos, const size_t& base, const int& colour);
    string GetFigProperties() override;
    bool IsEqual(shared_ptr<Figure> other) const override;
    size_t GetBase() const;
    void SetBase(const size_t&);


private:
    void GenerateSetOfCOORD() override;
    const SHORT TRIANGLE_MIN_SIZE_OFFSET = 4;
    const SHORT TRIANGLE_MIN_BASE = 2;
    size_t base;
};

// Circle class derived from Figure
class Circle : public Figure
{
public:
    Circle(const COORD& startPos, const size_t& radius, const int& colour);
    string GetFigProperties() override;
    size_t GetRadius() const;
    void SetRadius(const size_t&);


    bool IsEqual(shared_ptr<Figure> other) const override;

private:
    void GenerateSetOfCOORD() override;
    size_t radius;
};

