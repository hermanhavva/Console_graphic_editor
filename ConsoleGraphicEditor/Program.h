
#include <iostream>
#include <windows.h>
#include <string>
#include <sstream>
#include <memory>
#include <conio.h>
#include <format>
#include <algorithm>
#include <unordered_map>
#include <deque> 
#include "Figure.h"
#include "colours.h"
#include "maze.h"
#include "COORD_logic.h"


using namespace std;

enum COMMAND_TYPE
{
    DRAW = 1,
    LIST = 2,
    SHAPES = 3,
    ADD = 4,
    UNDO = 5,
    CLEAR = 6,
    SAVE = 7,
    LOAD = 8,
    DEFAULT_COMMAND = 9
};

class Program
{
public:
    Program(const HANDLE& hout,
        const COORD& polygonPos,
        const size_t& width,
        const size_t& height,
        const COORD& menuPos,
        const WORD& textColour);

    void ClearMainMenu() const;
    void PrintMainMenu() const;
    void PrintPolygon() const;
    vector<string> GetUserCommand();
    int ExecuteCommand(const vector<string>&);

    static deque<shared_ptr<Figure>> GetAllFigsPtrInDrawOrder() ;
    static string GetConfigurationStr();
    static bool AreSetsEqual(const unordered_set<COORD, COORDHash, COORDEqual> inSet1, const unordered_set<COORD, COORDHash, COORDEqual> inSet2);
    static bool IfDuplicate(shared_ptr<Figure>);
    static int DeleteThisFig(shared_ptr<Figure>);
    static int SelectFigById(const unsigned int& id);

    inline static deque<shared_ptr<Figure>> figDrawOrderDeque;
    inline static unordered_map<size_t, shared_ptr<Figure>> idToFigurePtrMap;
    

private:
    const string MAIN_MENU_STR = "List of commands:\tdraw\t\tlist\t\tshapes\t\t  add  | <shape> <COORD.X> <COORD.Y> <property>\n"
        "\t\t\tundo\t\tclear\t\tsave | <filename> load | <filename>\nEnter your command, please:\n";
    const COORD MENU_POS;
    const WORD TEXT_COLOUR;
    const HANDLE hout;
    
    vector<string> GetUserCommand(string);
    
    int  HandleAddFigure(vector<string>);
    void HandleSaveToFile(wstring) const;
    void HandleLoadFromFile(wstring);
    int  HandleChangeColour();
    
    inline static shared_ptr<Figure> selectedFigurePtr = nullptr;

    vector<string> GetValidUserInputAndSetCurCommand();
    bool IsUnsignedDigit(string strToCheck);
    string userInput = "";
    stringstream ssInput;
    COMMAND_TYPE curCommand = COMMAND_TYPE::DEFAULT_COMMAND;
    COORD figPosition{ 0,0 };
    WORD figColour = BLACK;
    unique_ptr<Polygon1> polygon;

    class FileSaver
    {
    public:
        FileSaver(const wstring& fileName);
        ~FileSaver();
        int SaveFiguresConfig(string) const;
    private:
        HANDLE fileHandle;
    };

    class FileLoader
    {
    public:
        FileLoader(const wstring& fileName);
        ~FileLoader();
        vector<string> LoadFiguresConfig();
    private:
        HANDLE fileHandle;
        unsigned int rowCounter = 1;
        int const CHUNK_SIZE = 1024;
    };
};

