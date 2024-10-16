#include "Program.h"

Program::Program(const HANDLE& hout,
    const COORD& polygonPos,
    const size_t& width,
    const size_t& height,
    const COORD& promtPos,
    const WORD& textColour)
    : hout(hout),
      PROMT_POS(promtPos),
      TEXT_COLOUR(textColour) 
{
    
    polygon = make_unique<Polygon1>(width, height, polygonPos);
}

void Program::ClearPromtMenu() const
{
    SetConsoleTextAttribute(hout, yellowFontBlackText);

    CONSOLE_SCREEN_BUFFER_INFO consoleInfo;
    GetConsoleScreenBufferInfo(hout, &consoleInfo);

    int consoleWidth = consoleInfo.srWindow.Right - consoleInfo.srWindow.Left - 2;
    
    COORD curCOORD = PROMT_POS;
    for (; curCOORD.Y <= 29; curCOORD.Y++)
    {
        SetConsoleCursorPosition(hout, curCOORD);
        for (; curCOORD.X < consoleWidth; curCOORD.X++)
        {
            cout << " ";
        }
        curCOORD.X = 0;
    }
    //SetConsoleCursorPosition(hout, MENU_POS);
}

void Program::PrintMainMenu() const noexcept
{
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
    else if (commandStr == "select") {
        curCommand = SELECT;
    }
    else if (commandStr == "remove") {
        curCommand = REMOVE;
    } 
    else if (commandStr == "paint") {
        curCommand = PAINT;
    }
    else if (commandStr == "move"){
        curCommand = MOVE;
    }
    else if (commandStr == "edit") {
        curCommand = EDIT;
    }
    else if (commandStr == "save") {
        curCommand = SAVE;
    }
    else if (commandStr == "load") {
        curCommand = LOAD;
    }
    else if (commandStr == "help")
    {
        curCommand = PRINT_MENU;
    }
    else {
        curCommand = COMMAND_TYPE::DEFAULT_COMMAND;
        throw runtime_error("Unknown or not specified command");
    }

    if ((curCommand == DRAW      || 
        curCommand == LIST       || 
        curCommand == SHAPES     || 
        curCommand == UNDO       || 
        curCommand == CLEAR      || 
        curCommand == PRINT_MENU ||
        curCommand == REMOVE)    && 
        (userCommandVector.size() != 1))
    {
        curCommand = COMMAND_TYPE::DEFAULT_COMMAND;
        throw runtime_error("Wrong arguments");
    }
    else if ((curCommand == SAVE  || 
              curCommand == LOAD) && 
              (userCommandVector.size() != 2))
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
    else if ((curCommand == SELECT || curCommand == PAINT) && userCommandVector.size() != 2)
    {
        throw runtime_error("Wrong arguments, must be <id> for select");
    }
    else if (curCommand == SELECT)
    {
        if (!IsUnsignedDigit(userCommandVector[1]))
        {
            throw runtime_error("Wrong argumnets, select <id>");
        }
    }
    else if (curCommand == PAINT)
    {
        auto& colourStr = userCommandVector[1];

        if (!IsUnsignedDigit(colourStr) || stoi(colourStr) < 0 || stoi(colourStr) > 6)
        {
            throw runtime_error("Wrong argumnets, paint <colour>");
        }
    }
    else if (curCommand == MOVE && userCommandVector.size() != 3)
    {
        throw runtime_error("Wrong argumnets, paint <colour>");
    }
    else if (curCommand == MOVE)
    {
        for (size_t index = 1; index <= 2; index++)
        {
            if (!IsUnsignedDigit(userCommandVector[index]))
            {
                throw runtime_error("Wrong arguments, move <X> <Y>");
            }
        }
    }
    else if (curCommand == EDIT && userCommandVector.size() < 2 || userCommandVector.size() > 3) { // validate the selected figure 
        throw runtime_error("Wrong argumnets");
    }
    else if (curCommand == EDIT)
    {
        size_t paramAmount;
        if (selectedFigurePtr == nullptr)
        {
            throw runtime_error("Figure is not selected");
        }
        if (!dynamic_cast<Square*>(selectedFigurePtr.get()) && dynamic_cast<Rectangle2*>(selectedFigurePtr.get()))
        {
            paramAmount = 2;
        }
        else 
        {
            paramAmount = 1;
        }
        if (userCommandVector.size() != paramAmount + 1)
        {
            throw runtime_error("Wrong number of argumnets for selected figure");
        }
        for (size_t index = 1; index <= paramAmount; index++)
        {
            if (!IsUnsignedDigit(userCommandVector[index]))
            {
                throw runtime_error("Wrong argumnets, must be unsigned");
            }
        }
    } 
            
    return userCommandVector;
}

vector<string> Program::GetUserCommand() {
    cout << "\nEnter your command, pleare\n>>";
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

vector<string> Program::GetUserCommand(string line) 
{
    ssInput = stringstream(line);
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
        HandleDraw();
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
        HandleDraw();
        break;

    case SHAPES:
        break;

    case ADD:
        HandleAddFigure(commandVector);
        break;

    case UNDO:
        figDrawOrderDeque.pop_back();
        break;

    case CLEAR:
        polygon->ClearPolygon(hout);
        break;
    case SELECT:
        HandleSelectFigById(stoi(commandVector[1]));
        break;
    case REMOVE:
        HandleRemove();
        break;
    case EDIT:
        HandleEdit(commandVector);
        break;
    case MOVE:
        HandleMove(stoi(commandVector[1]), stoi(commandVector[2]));
        break;
    case PAINT:
        HandlePaint(stoi(commandVector[1]));
        break;
    case PRINT_MENU:
        HandlePrintMenu();
        break;
    case SAVE:
        HandleSaveToFile(wstring(commandVector[1].begin(), commandVector[1].end()));
        break;
    case LOAD:
        HandleLoadFromFile(wstring(commandVector[1].begin(), commandVector[1].end()));
        break;

    case DEFAULT_COMMAND:
        throw runtime_error("Unknown command or command not specified\n");
    }
    SetConsoleCursorPosition(hout, defaultPos);
    curCommand = DEFAULT_COMMAND;
    
    return 0;
}


deque <shared_ptr<Figure>> Program::GetAllFigsPtrInDrawOrder() 
{
    return figDrawOrderDeque;
}

string Program::GetConfigurationStr() 
{
    string result;
    const string COMMAND = "add ";
    for (auto& curFigPtr : figDrawOrderDeque)
    {
        result += COMMAND;
        if (dynamic_cast<Square*>(curFigPtr.get()))
        {
            result += "square";
        }
        else if (dynamic_cast<Triangle*>(curFigPtr.get()))
        {
            result += "triangle";
        }
        else if (dynamic_cast<Circle*>(curFigPtr.get()))
        {
            result += "circle";
        }
        else
        {
            result += "rectangle";
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

void Program::HandleAddFigure(vector<string> commandVector) 
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
        figure = make_shared<Rectangle2>(startPos, figMeasure1, figMeasure2, figColour);
        break;
    case TRIANGLE:
        figure = make_shared<Triangle>(startPos, figMeasure1, figColour);
        break;
    case SQUARE:
        figure = make_shared<Square>(startPos, figMeasure1, figColour);
        break;
    case CIRCLE:
        figure = make_shared<Circle>(startPos, figMeasure1, figColour);
        break;
    }
    if (IfDuplicate(figure))
    {
        throw runtime_error("The figure is duplicate");
    }
    else if (!polygon->IsFigurePrintable(figure->GetThisFigStartPos(), figure->GetThisFigCOORDSet())) {
        throw runtime_error("Figure is not printable");
    }
    idToFigurePtrMap[figure->GetID()] = figure;
    figDrawOrderDeque.push_back(figure);
    selectedFigurePtr = figure;
}

void Program::HandleDraw() const
{
    polygon->PrintPolygon(hout);
    polygon->ClearPolygon(hout);
    for (auto& curFigure : GetAllFigsPtrInDrawOrder())
    {
        polygon->PrintFigure(hout, curFigure->GetThisFigCOORDSet(), curFigure->GetThisFigColour());
    }
    SetConsoleTextAttribute(hout, yellowFontBlackText);
}

void Program::HandleSelectFigById(const unsigned int& id)
{
    if (!idToFigurePtrMap.contains(id))
    {
        throw runtime_error("Wrong ID specified");
    }
    selectedFigurePtr = idToFigurePtrMap[id];

    for (size_t index = 0; index < figDrawOrderDeque.size(); index++)
    {
        auto& curFigure = figDrawOrderDeque[index];

        if (selectedFigurePtr->IsEqual(curFigure))
        {
            figDrawOrderDeque.erase(figDrawOrderDeque.begin() + index);
            figDrawOrderDeque.push_back(selectedFigurePtr);
            selectedFigurePtr = selectedFigurePtr;
        }
    }
    HandleDraw();   
}

void Program::HandleRemove()
{
    if (selectedFigurePtr == nullptr)
    {
        throw runtime_error("Figure is not seleted");
    }

    DeleteThisFig(selectedFigurePtr);
    selectedFigurePtr = nullptr;
}

void Program::HandlePaint(const short int& colour)
{
    if (selectedFigurePtr == nullptr)
    {
        throw runtime_error("Figure is not selected");
    }
    selectedFigurePtr->SetColour(colour);
}

void Program::HandleEdit(const vector<string> commandVector)
{
    if (selectedFigurePtr == nullptr)
    {
        throw runtime_error("Figure not selected");
    }

    if (commandVector.size() == 3)
    {
        auto castedFigPtr = dynamic_cast<Rectangle2*>(selectedFigurePtr.get());
        if (castedFigPtr == nullptr)
        {
            throw runtime_error("Wrong arguments");
        }
        castedFigPtr->SetWidthAndHeight(stoi(commandVector[1]), stoi(commandVector[2]));
    }
    else if (commandVector.size() == 2)
    {
        auto castedSquarePtr = dynamic_cast<Square*>(selectedFigurePtr.get());
        if (castedSquarePtr)
        {
            castedSquarePtr->SetWidthAndHeight(stoi(commandVector[1]), stoi(commandVector[2]));
        }
        auto castedTrianglePtr = dynamic_cast<Triangle*>(selectedFigurePtr.get());
        if (castedTrianglePtr)
        {
            castedTrianglePtr->SetBase(stoi(commandVector[1]));
        }
        auto castedCirclePtr = dynamic_cast<Circle*>(selectedFigurePtr.get());
        if (castedCirclePtr)
        {
            castedCirclePtr->SetRadius(stoi(commandVector[1]));
        }
    }
    else 
    {
        throw runtime_error("Wrong argumnets");
    }
}

void Program::HandleMove(const short int& Xpos, const short int& Ypos)
{
    if (selectedFigurePtr == nullptr)
    {
        throw runtime_error("Figure is not selected");
    }
    COORD oldStartPos = selectedFigurePtr->GetThisFigStartPos();
    COORD newStartPos{Xpos, Ypos};

    selectedFigurePtr->SetStartPos(newStartPos);
    if(!polygon->IsFigurePrintable(newStartPos, selectedFigurePtr->GetThisFigCOORDSet()))
    {
        selectedFigurePtr->SetStartPos(oldStartPos);
        throw runtime_error("Figure is not printable");
    }
    
}

void Program::HandlePrintMenu() const noexcept
{
    SetConsoleTextAttribute(hout, yellowFontBlackText);
    system("cls");

    PrintMainMenu();
    cout << "\n\t*Press any key to return to main menu* ";
    _getch();
    system("cls");
    polygon->PrintPolygon(hout);

    HandleDraw();
}

void Program::HandleLoadFromFile(const wstring& fileName)
{
    figDrawOrderDeque.clear();
    idToFigurePtrMap.clear();

    auto fileLoader = make_unique<FileLoader>(fileName);
    vector<string> lines = fileLoader->LoadFiguresConfig();
    try
    {
        for (auto& line : lines)
        {
            ExecuteCommand(GetUserCommand(line));
        }
    }
    catch (const exception& e)
    {
        throw runtime_error(format("Problem parsing the file: {}", e.what()));
    }
}

void Program::HandleSaveToFile(const wstring& fileName) const
{
    auto fileSaver = make_unique<FileSaver>(fileName);  

    if (figDrawOrderDeque.empty())
    {
        fileSaver->SaveFiguresConfig("empty");  
    }
    else
    {
        fileSaver->SaveFiguresConfig(GetConfigurationStr());
    }
}


Program::FileSaver::FileSaver(const wstring& fileName)
{
    fileHandle = CreateFile(fileName.c_str(),
        GENERIC_WRITE,
        FILE_SHARE_READ,
        NULL,
        CREATE_ALWAYS,
        FILE_ATTRIBUTE_NORMAL,
        NULL);
    
    if (fileHandle == INVALID_HANDLE_VALUE)
    {
        throw runtime_error("Could not create the file\n");
    }

}
int Program::FileSaver::SaveFiguresConfig(string config) const
{
    const char* buffer = config.c_str();
    DWORD bytesWritten = 0;
    DWORD bytesToWrite = static_cast<DWORD>(config.size());

    BOOL result = WriteFile
    (
        this->fileHandle,
        buffer,
        bytesToWrite,
        &bytesWritten,
        NULL
    );
    
    if (!result)
    {
        throw runtime_error("Error writing to the file");
    }

    return 0;
}
Program::FileSaver::~FileSaver()
{
    CloseHandle(fileHandle);
    fileHandle = NULL;
}

Program::FileLoader::FileLoader(const wstring& fileName)
{
    fileHandle = CreateFile(fileName.c_str(),
        GENERIC_READ,
        FILE_SHARE_READ,
        NULL,
        OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL,
        NULL);

    if (fileHandle == INVALID_HANDLE_VALUE)
    {
        throw runtime_error("Could not open the file");
    }
}
Program::FileLoader::~FileLoader()
{
    CloseHandle(fileHandle);
    fileHandle = NULL;
}

vector<string> Program::FileLoader::LoadFiguresConfig()
{
    unique_ptr<char[]> buffer(new char[CHUNK_SIZE]);
    DWORD bytesRead;
    string line;
    vector<string> result;


    while (ReadFile(fileHandle, buffer.get(), CHUNK_SIZE, &bytesRead, NULL) && bytesRead > 0) {  // error reading symbols

        for (DWORD index = 0; index < bytesRead; ++index)
        {
            char ch = buffer[index];

            if (ch == '\n')
            {
                rowCounter++;
                result.push_back(line);

                if (!line.empty())
                {
                    line.clear();
                }
            }
            else if (line == "empty" && rowCounter == 1)
            {
                return result;
            }
            else if (ch != '\r') {
                line += ch;
            }
        }
    }
    return result;
}


bool Program::IsUnsignedDigit(string strToCheck) {
    return all_of(strToCheck.begin(), strToCheck.end(), ::isdigit);
}
