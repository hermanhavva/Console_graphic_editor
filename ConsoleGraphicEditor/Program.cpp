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

vector<string> Program::GetValidUserInputAndSetCurCommand()
{
    string token, commandStr, figureTypeStr;
    size_t paramAmount = 0;
    vector<string> userCommandVector;
    
    while(ssInput >> token)
    {
        userCommandVector.push_back(token);
    }


    if (userCommandVector.empty())
    {
        throw runtime_error("Empty input");
    }

    commandStr = userCommandVector[0];
 
    //  validation rules
    if (commandStr == "draw") {
        curCommand = DRAW;
    }
    else if (commandStr == "list") {
        curCommand = LIST;
    }
    else if (commandStr == "shapes") {
        curCommand = SHAPES;
    }
    else if (commandStr == "add") {
        curCommand = ADD;
    }
    else if (commandStr == "undo") {
        curCommand = UNDO;
    }
    else if (commandStr == "clear") {
        curCommand = CLEAR;
    }
    else if (commandStr == "save") {
        curCommand = SAVE;
    }
    else if (commandStr == "load") {
        curCommand = LOAD;
    }
    else {
        curCommand = COMMAND_TYPE::DEFAULT_COMMAND;
        throw runtime_error("Unknown or not specified command");
    }

    if ((curCommand == DRAW || curCommand == LIST || curCommand == SHAPES || curCommand == UNDO || curCommand == CLEAR) && (userCommandVector.size() != 1) )
    {
        curCommand = COMMAND_TYPE::DEFAULT_COMMAND;
        throw runtime_error("Wrong arguments");
    }
    else if ((curCommand == SAVE || curCommand == LOAD) && (userCommandVector.size() != 2))
    {
        curCommand = COMMAND_TYPE::DEFAULT_COMMAND;
        throw runtime_error("Wrong arguments");
    }
    
    if (curCommand == ADD && (userCommandVector.size() < 6 || userCommandVector.size() > 7))
    {
        throw runtime_error("Wrong arguments");
    }
    else if (curCommand == ADD)
    {
        figureTypeStr = userCommandVector[1];

        if (figureTypeStr == "rectangle")
        {
            paramAmount = 5;
        }
        else if (figureTypeStr == "circle" || figureTypeStr == "triangle" || figureTypeStr == "square")
        {
            paramAmount = 4;
        }
        if (userCommandVector.size() != paramAmount + 2)
        {
            throw runtime_error("Wrong number of arguments");
        }

        // validating the numerical part
        size_t index = 2;
        while (index <= paramAmount + 1)
        {
            if (!IsUnsignedDigit(userCommandVector[index]) ||
                (index == paramAmount + 1 && (stoi(userCommandVector[index]) < MIN_COLOUR_INDEX || stoi(userCommandVector[index]) > MAX_COLOUR_INDEX)))
            {
                throw runtime_error("Arguments must be numeric and unsigned and 1 <= colour <= 6");
            }

            index++;
        }
    }
    
    

    return userCommandVector;
}

vector<string> Program::GetUserCommand() {
    cout << ">>";
    getline(cin, userInput);
    ssInput = stringstream(userInput); 
    vector<string> validCommandVector;
    try
    {
        validCommandVector = GetValidUserInputAndSetCurCommand();
    }
    catch (const std::exception& e)
    {
        throw e;
    }
    return validCommandVector;
}

int Program::ExecuteCommand(const vector<string>& commandVector) {
    string token;
    COORD defaultPos{ MENU_POS.X, MENU_POS.Y + 4 };

    switch (curCommand) {
    case DRAW:
        polygon->PrintPolygon(hout);
        polygon->ClearPolygon(hout);
        for (auto& curFigure : GetAllFigsPtrInDrawOrder()) 
        {
            polygon->PrintFigure(hout, curFigure->GetThisFigCoordsSet(), curFigure->GetThisFigColour());
        }
        SetConsoleTextAttribute(hout, yellowFontBlackText);
        break;

    case LIST:
        SetConsoleTextAttribute(hout, yellowFontBlackText);
        system("cls");
        for (auto& curFig : GetAllFigsPtrInDrawOrder()) 
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
        this->PrintMainMenu();
        break;

    case SHAPES:
        break;

    case ADD:
        if (HandleAddFigure(commandVector) == -1) {
            cout << "\nInvalid entry";
        }
        break;

    case UNDO:
        figDrawOrderDeque.pop_back();
        break;

    case CLEAR:
        polygon->ClearPolygon(hout);
        
        break;

    case SAVE:
        break;

    case LOAD:
        break;

    case DEFAULT_COMMAND:
        throw runtime_error("Unknown command or command not specified\n");
        return -1;
    }
    SetConsoleCursorPosition(hout, defaultPos);
    
    return 0;
}


deque <shared_ptr<Figure>> Program::GetAllFigsPtrInDrawOrder() 
{
    return figDrawOrderDeque;
}

string& Program::GetConfigurationStr() 
{
    string result;
    string type;
    for (auto& curFigPtr : figDrawOrderDeque)
    {
        string curID = to_string(curFigPtr->GetID());
        if (dynamic_cast<Square*>(curFigPtr.get()))
        {
            result += curID + "SQUARE";
        }
        else if (dynamic_cast<Triangle*>(curFigPtr.get()))
        {
            result += curID + "TRIANGLE";
        }
        else if (dynamic_cast<Circle*>(curFigPtr.get()))
        {
            result += curID + "CIRCLE";
        }
        else
        {
            result += curID + "RECTANGLE";
        }
        result += curFigPtr->GetFigProperties() + '\n';
    }
    return result;
}

bool Program::AreSetsEqual(const unordered_set <COORD, COORDHash, COORDEqual> inSet1, const unordered_set<COORD, COORDHash, COORDEqual> inSet2)
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

int Program::SelectFigById(const unsigned int& id)
{
    if (!idToFigurePtrMap.contains(id))
    {
        return -1;
    }
    auto& selectedFigPtr = idToFigurePtrMap[id];

    for (size_t index = 0; index < figDrawOrderDeque.size(); index++)
    {
        auto& curFigure = figDrawOrderDeque[index];

        if (selectedFigPtr->IsEqual(curFigure))
        {
            figDrawOrderDeque.erase(figDrawOrderDeque.begin() + index);
            figDrawOrderDeque.push_back(selectedFigPtr);
            selectedFigurePtr = selectedFigPtr;
            return 0;
        }
    }

    return -1;
}


int Program::DeleteThisFig(shared_ptr<Figure> inFigure)
{
    for (size_t index = 0; index < figDrawOrderDeque.size(); index++)
    {
        auto& curFigurePtr = figDrawOrderDeque[index];
        if (curFigurePtr->IsEqual(inFigure))
        {
            idToFigurePtrMap.erase(inFigure->GetID());
            figDrawOrderDeque.erase(figDrawOrderDeque.begin() + index);
            return 0;
        }
    }
    return -1;
}

bool Program::IfDuplicate(shared_ptr<Figure> inFigure) 
{
    for (auto& curFigure : figDrawOrderDeque)
    {
        if (inFigure->IsEqual(curFigure))
        {
            return true;
        }
    }
    return false;
}

int Program::HandleAddFigure(vector<string> commandVector) 
{
    string figTypeStr, param;
    unsigned short figMeasure1 = 0, figMeasure2 = 0, paramAmount = 0;
    int figColour = 0;
    figTypeStr = commandVector[1];
    FIGURE_TYPE curFigure = FIGURE_TYPE::DEFAULT_TYPE;

    COORD startPos{ 0, 0 };

    if (figTypeStr == "rectangle") {
        paramAmount = 5;
        curFigure = RECTANGLE;
    }
    else if (figTypeStr == "square") {
        paramAmount = 4;
        curFigure = SQUARE;
    }
    else if (figTypeStr == "triangle") {
        paramAmount = 4;
        curFigure = TRIANGLE;
    }
    else if (figTypeStr == "circle") {
        paramAmount = 4;
        curFigure = CIRCLE;
    }

    for (size_t index = 2; index < commandVector.size(); index++) 
    {
       
        if (index == 2) {
            startPos.X = stoi(commandVector[index]);
        }
        else if (index == 3) {
            startPos.Y = stoi(commandVector[index]);
        }
        else if (index == 4) {
            figMeasure1 = stoi(commandVector[index]);
        }
        else if (index == 5 && curFigure == RECTANGLE)
        {
            figMeasure2 = stoi(commandVector[index]);
        }
        else
        {
            figColour = stoi(commandVector[index]);
        }
        
    }

    shared_ptr<Figure> figure;
    switch (curFigure)
    {
    case RECTANGLE:
        figure = make_shared<Rectangle2>(Rectangle2(startPos, figMeasure1, figMeasure2, figColour));
        break;
    case TRIANGLE:
        figure = make_shared<Triangle>(Triangle(startPos, figMeasure1, figColour));
        break;
    case SQUARE:
        figure = make_shared<Square>(Square(startPos, figMeasure1, figColour));
        break;
    case CIRCLE:
        figure = make_shared<Circle>(Circle(startPos, figMeasure1, figColour));
        break;
    }
    if (IfDuplicate(figure))
    {
        throw runtime_error("The figure is duplicate");
    }
    else if (!polygon->IsFigurePrintable(figure->GetThisFigStartPos(), figure->GetThisFigCoordsSet())) {
        throw runtime_error("Figure is not printable");
    }
    idToFigurePtrMap[figure->GetID()] = figure;
    figDrawOrderDeque.push_back(move(figure));
}
/*
int Program::HandleChangeColour()
{
    string token;
    unsigned short paramCounter = 0;
    const unsigned short paramAmount = 1;
    while(ssInput >> token)
    {
        ssInput >> token;
        paramCounter++;
    }
    
    if (!IsUnsignedDigit(token) || stoi(token) >  || paramCounter > paramAmount || paramCounter == 0)
    {
        throw runtime_error("Wrong arguments");
    }
    if (selectedFigurePtr == nullptr)
    {
        throw runtime_error("Select the figure first");
    }
    
 //   selectedFigurePtr->SetColour(colourEnumToWordMap[token]);

}
*/

bool Program::IsUnsignedDigit(string strToCheck) {
    return all_of(strToCheck.begin(), strToCheck.end(), ::isdigit);
}
