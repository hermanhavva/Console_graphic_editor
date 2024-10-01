#include <iostream>
#include <windows.h>
#include <vector>
#include <format>
#include "maze.cpp"
#include "figure.cpp"
#include "colours.h"
using namespace std;
 
/*
HANDLE hout = GetStdHandle(STD_OUTPUT_HANDLE);
CONSOLE_SCREEN_BUFFER_INFO consoleInfo;

const WORD originalAttributes = GetConsoleScreenBufferInfo(hout, &consoleInfo);

const WORD yellowFontBlackText = BACKGROUND_GREEN | BACKGROUND_RED | BACKGROUND_INTENSITY;
const WORD redText = FOREGROUND_RED | FOREGROUND_INTENSITY;
const WORD cyanText = FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY;
const WORD purpleText = FOREGROUND_RED | FOREGROUND_BLUE;
const WORD blueText = FOREGROUND_BLUE | FOREGROUND_INTENSITY;
const WORD greenText = FOREGROUND_GREEN;
*/ 

int main()
{
	
	SetConsoleTextAttribute(hout, yellowFontBlackText);
	system("cls");
	COORD startPos;
	startPos.X = 10;
	startPos.Y = 1;
	auto maze = new Maze(80, 20, startPos);
	//maze->Print(hout);
	delete maze;
	return 0;

}
/*
class Book abstract
{
public:
	virtual string GetName() const = 0;

	virtual int GetPriceWithoutDiscount() const = 0;

	virtual double Purchase() = 0;

protected:
	string name;
	int price;

};

class HistoryBook : public Book
{
public:

	const static int LIMIT = 5;
	const double DISCOUNT = 0.95;

	HistoryBook(const string& name, const int& price)
	{
		this->name = name;
		this->price = price;
	}
	double Purchase() override  // retuns price with discount or without
	{
		counter++;
		if (counter < LIMIT)
		{
			return price * DISCOUNT;
		}

		return price;
	}

	int GetPriceWithoutDiscount() const override
	{
		return this->price;
	}

	string GetName() const override
	{
		return this->name;
	}

private:
	static inline size_t counter = 0;
};
const int HistoryBook::LIMIT;

class ProgrammingBook : public Book
{
public:

	const static int LIMIT = 2;
	const double DISCOUNT = 0.9;

	ProgrammingBook(const string& name, const int& price)
	{
		this->name = name;
		this->price = price;  // will a copy constructor be called here for int type
	}
	double Purchase() override // retuns price with discount or without
	{
		counter++;
		if (counter < LIMIT)
		{
			return price * DISCOUNT;
		}

		return price;
	}

	int GetPriceWithoutDiscount() const override
	{
		return this->price;
	}

	string GetName() const override
	{
		return this->name;
	}

private:
	static inline size_t counter = 0;
};
const int ProgrammingBook::LIMIT;

class DetectiveBook : public Book
{
public:

	const static int LIMIT = 5;
	const double DISCOUNT = 0.8;

	DetectiveBook(const string& name, const int& price)
	{
		this->name = name;
		this->price = price;  // will a copy constructor be called here for int type
	}
	double Purchase() override  // retuns price with discount or without
	{
		counter++;
		if (counter < LIMIT)
		{
			return price * DISCOUNT;
		}

		return price;
	}
	int GetPriceWithoutDiscount() const override
	{
		return this->price;
	}

	string GetName() const override
	{
		return this->name;
	}

private:
	static inline size_t counter = 0;

};
const int DetectiveBook::LIMIT;

class SciFiBook : public Book
{
public:

	const static int LIMIT = 7;
	const double DISCOUNT = 0.8;

	SciFiBook(const string& name, const int& price)
	{
		this->name = name;
		this->price = price;
	}
	double Purchase() override  // retuns price with discount or without
	{
		counter++;
		if (counter < LIMIT)
		{
			return price * DISCOUNT;
		}

		return price;
	}
	int GetPriceWithoutDiscount() const override
	{
		return this->price;
	}

	string GetName() const override
	{
		return this->name;
	}

private:
	static inline size_t counter = 0;

};
const int SciFiBook::LIMIT;



int main()
{
	unique_ptr<vector<shared_ptr<Book>>> vectorOfBooks = make_unique<vector<shared_ptr<Book>>>();

	for (size_t counter = 0; counter < 10; counter++)  // filling the vector of base type with children types
	{
		shared_ptr <HistoryBook> historyBook = make_shared<HistoryBook>("The history of cryptography", 100);
		vectorOfBooks->push_back(historyBook);

		shared_ptr<ProgrammingBook> programmingBook = make_shared<ProgrammingBook>("How to write memory save C++", 205);
		vectorOfBooks->push_back(programmingBook);

		shared_ptr<DetectiveBook> detectiveBook = make_shared<DetectiveBook>("The story of lost semicolon on the line 57", 120);
		vectorOfBooks->push_back(detectiveBook);

		shared_ptr<SciFiBook> scifiBook = make_shared<SciFiBook>("The time C++ disappered", 200);
		vectorOfBooks->push_back(scifiBook);
	}
	for (auto& book : *(vectorOfBooks))
	{
		cout << format("the name: {} | price {} \n", book->GetName(), book->Purchase());
	}
}
*/