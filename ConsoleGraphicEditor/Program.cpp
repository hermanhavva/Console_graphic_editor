#include <iostream>
#include <windows.h>
#include <string>
using namespace std;

enum CommandType
{
	DRAW = 1,
	LIST = 1,
	SHAPES = 3,
	ADD = 4,
	UNDO = 5,
	CLEAR = 6,
	SAVE = 7,
	LOAD = 8
};

class Program
{
public:
	Program(const HANDLE& hout,
			const COORD& menuPos, 
			const WORD& textColour)
		:	hout(hout),
			MENU_POS(menuPos),
			TEXT_COLOUR(textColour)
	{}
	void PrintMainMenu() const
	{
		SetConsoleTextAttribute(hout, TEXT_COLOUR);
		SetConsoleCursorPosition(hout, MENU_POS);
		
		cout << MAIN_MENU_STR;		
	}
	
private:
	const string MAIN_MENU_STR = "List of commands:\tdraw\t\tlist\t\tshapes\t\tadd <shape> <COORD.X> <COORD.Y> <property>\n"
								 "\t\t\tundo\t\tclear\t\tsave <filename>\nEnter your command, please:\n";
	const COORD MENU_POS;
	const WORD TEXT_COLOUR;
	const HANDLE hout;
};
