#ifndef ASSIGN2_IOHANDLER
#define ASSIGN2_IOHANDLER

#include <iostream>
#include <string>



class IOHandler{
    public:
    static void menu();
    static void printStart();
    static void selection();
    static void newGame();
    static bool StringCheck(std::string name);
    static void credits();
    static void loadGame();
    static bool checkTilePosition(std::string tilePosition);
    static bool checkTile(std::string tile);
    static void prompt();
    static bool placeTile(std::string tile, std::string pos);
    static bool replaceTile(std::string tile);
    static void gameStart();
    static void Test();
    static int quit();

};



#endif // ASSIGN2_IOHANDLER