#include <iostream>
#include <windows.h>

#include "maze.cpp"
#include "colours.h"
#include "Program.h"
#include "Figure.h"
//#include "helper.h" 
 

HANDLE hout = GetStdHandle(STD_OUTPUT_HANDLE);
int main() 
  {
	
	SetConsoleTextAttribute(hout, yellowFontBlackText);
	system("cls");
	COORD startPos{ 0 };
	startPos.X = 10;
	startPos.Y = 1; 
	//auto maze = make_unique<Polygon1>(80, 20, startPos);
	//maze->PrintPolygon(hout);  
	startPos.X = 15;
	startPos.Y = 10;
	COORD menuPos{ 0, 23 };
	COORD polygonPos{10,1};
	unique_ptr<Program> program = make_unique<Program>(Program(hout, polygonPos, 80, 20, menuPos, yellowFontBlackText));
	unique_ptr<Figure> startFigure = make_unique<Rectangle2>(Rectangle2(startPos, 0,0, yellowFontBlackText));
	while (true) 
	{
		try  
		{
			program->PrintPolygon();
			program->PrintMainMenu();
			program->GetUserCommand();
			if (program->ExecuteCommand() == 0)
			{
				cout << ">>success\t\t*Press any key to proceed*";
				_getch();
			}
			//_getch();
			program->ClearMainMenu();

		}
		catch (const std::exception& e)
		{
			cout << e.what() << "\tPress any key to proceed";
			_getch();
			program->ClearMainMenu();
		}	

	}
	
	
	//unique_ptr<Figure> figure1 = make_unique<Rectangle2>(startPos, 2, 2, redText);
	
	//maze->PrintFigure(hout, figure1->GetThisFigCoordsSet(), redText | yellowFontBlackText);
	Sleep(10000);
	
	//maze->ClearPolygon(hout);
	Sleep(4100);
 	
	return 0;
	
}
