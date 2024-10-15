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
    cout << ">>";
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
        if (HandleAddFigure() == -1) {
            cout << "\nInvalid entry";
        }
        break;

    case UNDO:
        if (!figDrawOrderDeque.empty())
        {
            figDrawOrderDeque.pop_back();
        }
        break;

    case CLEAR:
        polygon->ClearPolygon(hout);
        break;

    case SAVE:
        HandleSaveToFile();
        break;

    case LOAD:
        HandleLoadFromFile();
        break;

    case DEFAULT:
        throw runtime_error("Unknown command or command not specified\n");
        return -1;
    }
    SetConsoleCursorPosition(hout, defaultPos);
    
    return 0;
}

int Program::HandleLoadFromFile()
{
    string token;
    unsigned short paramCounter = 0, paramAmount = 1;
    
    figDrawOrderDeque.clear();
    idToFigurePtrMap.clear();

    while (ssInput >> token)
    {
        paramCounter++;
    }
    if (paramCounter > paramAmount)
    {
        throw runtime_error("Too many arguments passed");
    }
    wstring fileName = wstring(token.begin(), token.end());
    
    auto fileLoader = make_unique<FileLoader>(fileName);
    vector<string> lines = fileLoader->LoadFiguresConfig();
    try
    {
        for (auto& line : lines)
        {
            ssInput = stringstream(line);
            HandleAddFigure();
        }
    }
    catch (const exception& e)
    {
        throw runtime_error(format("Problem parsing the file: {}", e.what()));
    }

    return 0;
}

int Program::HandleSaveToFile() 
{
    string token;
    unsigned short paramCounter = 0, paramAmount = 1;
    while (ssInput >> token)
    {
        paramCounter++;
    }
    if (paramCounter > paramAmount)
    {
        throw runtime_error("Too many arguments passed");
    }
    wstring fileName = wstring(token.begin(), token.end());
    auto fileSaver = make_unique<FileSaver>(fileName);  // here the cinstructor is called and just after that destructor is called making the object invalid

    if (figDrawOrderDeque.empty())
    {
        fileSaver->SaveFiguresConfig("empty");  // that is why here an invalid handle
    }
    else 
    {
        fileSaver->SaveFiguresConfig(GetConfigurationStr());
    }
}


deque <shared_ptr<Figure>> Program::GetAllFigsPtrInDrawOrder() 
{
    return figDrawOrderDeque;
}

string Program::GetConfigurationStr() 
{
    string result;
    string type;
    for (auto& curFigPtr : figDrawOrderDeque)
    {
        //string curID = to_string(curFigPtr->GetID());
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

int Program::SelectFigById(const unsigned int& id)
{
    if (!idToFigurePtrMap.contains(id))
    {
        return -1;
    }
    auto& selectedFigPtr = idToFigurePtrMap[id];
    unordered_set<COORD, COORDHash, COORDEqual> selectedFigCoordSet = selectedFigPtr->GetThisFigCoordsSet();

    for (size_t index = 0; index < figDrawOrderDeque.size(); index++)
    {
        auto curCoordSet = figDrawOrderDeque[index]->GetThisFigCoordsSet();

        if (AreSetsEqual(selectedFigCoordSet, curCoordSet))
        {
            figDrawOrderDeque.erase(figDrawOrderDeque.begin() + index);
            figDrawOrderDeque.push_back(selectedFigPtr);
            return 0;
        }
    }

    return -1;
}

int Program::DeleteThisFig(shared_ptr<Figure> inFigure)
{
    for (size_t index = 0; index < figDrawOrderDeque.size(); index++)
    {
        auto curFigurePtr = figDrawOrderDeque[index];
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

int Program::HandleAddFigure() 
{
    string figType, param;
    unsigned short figMeasure1 = 0, figMeasure2 = 0, paramAmount = 0, paramCounter = 0;
    FIGURE_TYPE figureType = DEFAULT_TYPE;
    WORD figColour;
    COORD startPos{ 0, 0 };
    ssInput >> figType;

    if (figType == "rectangle") {
        paramAmount = 5;
        figureType = RECTANGLE;
    }
    else if (figType == "square") {
        paramAmount = 4;
        figureType = SQUARE;
    }
    else if (figType == "triangle") {
        paramAmount = 4;
        figureType = TRIANGLE;
    }
    else if (figType == "circle") {
        paramAmount = 4;
        figureType = CIRCLE;
    }

    while (ssInput >> param && paramCounter <= paramAmount) {
        paramCounter++;
        if (paramAmount == paramCounter && IsUnsignedDigit(param)) 
        {
            if (stoi(param) < 1 || stoi(param) > 6)
            {
                throw runtime_error("The colour must be >= 1 && <= 6");
            }
            figColour = colourMap[stoi(param)];
            break;
        }
        if (!IsUnsignedDigit(param)) {
            throw runtime_error("Invalid input(must be unsigned digit)");
            return -1;
        }
        if (paramCounter == 1) {
            startPos.X = stoi(param);
        }
        else if (paramCounter == 2) {
            startPos.Y = stoi(param);
        }
        else if (paramCounter == 3) {
            figMeasure1 = stoi(param);
        }
        else if (paramCounter == 4){
            figMeasure2 = stoi(param);
        }
        else
        {
            throw runtime_error("The arguments do not match");
        }
        
    }
    if (paramCounter != paramAmount)
    {
        throw runtime_error("Wrong number of arguments");
    }

    shared_ptr<Figure> figure;
    switch (figureType) 
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
        DeleteThisFig(figure);
        throw runtime_error("Figure is not printable");
        return -1;
    }
    idToFigurePtrMap[figure->GetID()] = figure;
    figDrawOrderDeque.push_back(figure);
}

bool Program::IsUnsignedDigit(string strToCheck) {
    return all_of(strToCheck.begin(), strToCheck.end(), ::isdigit);
}

/*
Program::FileParser::FileParser(const wstring& fileName) 
    :   fileName(fileName)
{
    fileHandle = CreateFile(this->fileName.c_str(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

    if (fileHandle == INVALID_HANDLE_VALUE)
    {
        throw runtime_error("Could not open the file\n");
    }
}
*/




Program::FileSaver::FileSaver(const wstring& fileName)
{
    fileHandle = CreateFile(fileName.c_str(),
        GENERIC_WRITE,
        FILE_SHARE_READ,
        NULL,
        CREATE_ALWAYS,
        FILE_ATTRIBUTE_NORMAL,
        NULL);
    cout << GetLastError() << endl;
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
    cout << GetLastError();
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
    cout << GetLastError() << endl;
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


/*
Program::FileParser::FileParser()
{
    isFileLoadable = false;
}

Program::FileParser::~FileParser()
{
    CloseHandle(fileHandle);
}
vector<string> Program::FileParser::GetFiguresConfig()
{
    unique_ptr<char[]> buffer(new char[CHUNK_SIZE]);
    DWORD bytesRead;
    string line;
    vector<string> result;
    if (!isFileLoadable)
    {
        return result;  // empty
    }

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
                    line.clear();  // Clear the line buffer
                }
            }
            else if (line == "empty" && rowCounter == 0)
            {
                return result;
            }
            else if (ch != '\r') {
                line += ch;  // Append character to line
            }
        }
    }
    return result;
}

int Program::FileParser::SaveFiguresConfig(const wstring& fileName, const string& config)
{
    if (isFileLoadable)
    {
        return -1;
    }

    fileHandle = CreateFile(fileName.c_str(), GENERIC_READ, FILE_SHARE_READ, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

    if (fileHandle == INVALID_HANDLE_VALUE)
    {
        throw runtime_error("Could not open the file\n");
    }

    const char* buffer = config.c_str();
    DWORD bytesWritten = 0;
    DWORD bytesToWrite = static_cast<DWORD>(config.size());

    // Write the data to the file
    BOOL result = WriteFile
    (
        fileHandle,        
        buffer,            
        bytesToWrite,     
        &bytesWritten,    
        NULL              
    );

    if (!result || bytesWritten != bytesToWrite)
    {
        throw runtime_error("Error writing to the file");
    }

    return 0;  
}
*/

