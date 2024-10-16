#include <iostream>
#include <windows.h>
#include "colours.h"
#include "Program.h"
#include "Figure.h"
 

HANDLE hout = GetStdHandle(STD_OUTPUT_HANDLE);
int main() 
  {
	
	SetConsoleTextAttribute(hout, yellowFontBlackText);
	system("cls");
	COORD startPos{ 0 };
	startPos.X = 10;
	startPos.Y = 1; 
	
	startPos.X = 15;
	startPos.Y = 10;
	COORD promtPos{ 0, 23 };
	COORD polygonPos{10,1};
	unique_ptr<Program> program = make_unique<Program>(hout, polygonPos, 80, 20, promtPos, yellowFontBlackText);
	while (true) 
	{
		try  
		{
			program->PrintPolygon();
			auto commandVector = program->GetUserCommand();
			
			if (program->ExecuteCommand(commandVector) == 0)
			{
				program->ClearPromtMenu();
				SetConsoleCursorPosition(hout, promtPos);
				cout << ">>success\t\t*Press any key to proceed*";
				_getch();
				program->ClearPromtMenu();
			}
		}
		catch (const std::exception& e)
		{
			cout << e.what() << "\tPress any key to proceed";
			_getch();
			program->ClearPromtMenu();
		}	

	}
	
	
	Sleep(10000);
	
	Sleep(4100);
 	
	return 0;
	
}
