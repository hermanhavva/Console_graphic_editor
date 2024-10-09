#include "Program.h"

Program::Program(const HANDLE& hout,
    const COORD& polygonPos,
    const size_t& width,
    const size_t& height,
    const COORD& menuPos,
    const WORD& textColour)
    : hout(hout),
    MENU_POS(menuPos),
    TEXT_COLOUR(textColour) 
{

    polygon = make_unique<Polygon1>(width, height, polygonPos);
}

void Program::ClearMainMenu() const
{
    SetConsoleTextAttribute(hout, yellowFontBlackText);

    CONSOLE_SCREEN_BUFFER_INFO consoleInfo;
    GetConsoleScreenBufferInfo(hout, &consoleInfo);

    int consoleWidth = consoleInfo.srWindow.Right - consoleInfo.srWindow.Left - 2;
    
    COORD curCOORD = MENU_POS;
    for (; curCOORD.Y <= 31; curCOORD.Y++)
    {
        SetConsoleCursorPosition(hout, curCOORD);
        for (; curCOORD.X < consoleWidth; curCOORD.X++)
        {
            cout << " ";
        }
        curCOORD.X = 0;
    }
    SetConsoleCursorPosition(hout, MENU_POS);
}

void Program::PrintMainMenu() const {
    SetConsoleTextAttribute(hout, TEXT_COLOUR);
    SetConsoleCursorPosition(hout, MENU_POS);
    cout << MAIN_MENU_STR;
}

void Program::PrintPolygon() const
{
    polygon->PrintPolygon(hout);
}

int Program::GetUserCommand() {
    getline(cin, userInput);
    ssInput = stringstream(userInput);
    string token;
    ssInput >> token;

    if (token == "draw") {
        curCommand = DRAW;
    }
    else if (token == "list") {
        curCommand = LIST;
    }
    else if (token == "shapes") {
        curCommand = SHAPES;
    }
    else if (token == "add") {
        curCommand = ADD;
    }
    else if (token == "undo") {
        curCommand = UNDO;
    }
    else if (token == "clear") {
        curCommand = CLEAR;
    }
    else if (token == "save") {
        curCommand = SAVE;
    }
    else if (token == "load") {
        curCommand = LOAD;
    }
    else {
        curCommand = COMMAND_TYPE::DEFAULT;
        return -1;
    }

    return 0;
}

int Program::ExecuteCommand() {
    string token;

    switch (curCommand) {
    case DRAW:
        polygon->PrintPolygon(hout);
        polygon->ClearPolygon(hout);
        for (auto& curFigure : Figure::GetAllFigsPtrInDrawOrder()) {
            polygon->PrintFigure(hout, curFigure->GetThisFigCoordsSet(), curFigure->GetThisFigColour());
        }
        break;

    case LIST:
        SetConsoleTextAttribute(hout, yellowFontBlackText);
        system("cls");
        for (auto& curFig : Figure::GetAllFigsPtrInDrawOrder()) 
        {
            auto a = curFig->GetID();
            auto b = curFig->GetFigNameStr();
            auto c = curFig->GetFigProperties();
            cout << format("{} {} {}\n", a, b, c);
        }
        cout << "\n\t*Press any key to return to main menu* ";
        _getch();
        system("cls");
        polygon->PrintPolygon(hout);
        break;

    case SHAPES:
        break;

    case ADD:
        if (HandleAddFigure() == -1) {
            cout << "\nInvalid entry";
        }
        break;

    case UNDO:
        break;

    case CLEAR:
        break;

    case SAVE:
        break;

    case LOAD:
        break;

    case DEFAULT:
        throw runtime_error("Unknown command or command not specified\n");
        return -1;
    }

    return 0;
}

int Program::HandleAddFigure() {
    string figType, param;
    unsigned short figMeasure1 = 0, figMeasure2 = 0, paramAmount = 0, paramCounter = 0;
    FIGURE_TYPE figureType = DEFAULT_TYPE;
    WORD figColour;
    COORD startPos{ 0, 0 };
    ssInput >> figType;

    if (figType == "rectangle") {
        paramAmount = 4;
        figureType = RECTANGLE;
    }
    else if (figType == "square") {
        paramAmount = 3;
        figureType = SQUARE;
    }
    else if (figType == "triangle") {
        paramAmount = 3;
        figureType = TRIANGLE;
    }
    else if (figType == "circle") {
        paramAmount = 3;
        figureType = CIRCLE;
    }

    while (ssInput >> param && paramCounter <= paramAmount) {
        if (paramAmount == paramCounter && IsUnsignedDigit(param)) {
            figColour = colourMap[stoi(param)];
            continue;
        }
        if (!IsUnsignedDigit(param)) {
            cout << "Invalid input(must be unsigned digit)\n";
            return -1;
        }
        if (paramCounter == 0) {
            startPos.X = stoi(param);
        }
        else if (paramCounter == 1) {
            startPos.Y = stoi(param);
        }
        else if (paramCounter == 2) {
            figMeasure1 = stoi(param);
        }
        else {
            figMeasure2 = stoi(param);
        }
        paramCounter++;
    }

    unique_ptr<Figure> figure;
    switch (figureType) {
    case RECTANGLE:
        figure = make_unique<Rectangle2>(Rectangle2(startPos, figMeasure1, figMeasure2, figColour));
        break;
    case TRIANGLE:
        figure = make_unique<Triangle>(Triangle(startPos, figMeasure1, figColour));
        break;
    case SQUARE:
        figure = make_unique<Square>(Square(startPos, figMeasure1, figColour));
        break;
    case CIRCLE:
        figure = make_unique<Circle>(Circle(startPos, figMeasure1, figColour));
        break;
    }

    if (!polygon->IsFigurePrintable(figure->GetThisFigStartPos(), figure->GetThisFigCoordsSet())) {
        figure->DeleteThisFig();
        throw runtime_error("Figure is not printable");
        return -1;
    }
}

bool Program::IsUnsignedDigit(string strToCheck) {
    return all_of(strToCheck.begin(), strToCheck.end(), ::isdigit);
}
