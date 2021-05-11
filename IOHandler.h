#ifndef IO_HANDLER
#define IO_HANDLER

#include <iostream>

#include "GameManager.h"

class IOHandler {
public:
    static void beginGame();
    static void menu();
    static void prompt();
    static void selection();
    static void newGame();
    static bool validateName(const string& name);
    static void playRound();
    static bool testingPurpose(string operation, string tile, string keywordAT, string pos);
    static void credits();
    static void loadGame();
    static bool checkTile(const string& tile);
    static bool checkTilePosition(const string& position);
    static bool placeTile(const string& tile, const string& position);
    static bool replaceTile(const string& tile);
    static void notify(const string& message, State state);
    static int quit();
    static void Test();


    static bool gameRunning;
};

#endif // !IO_HANDLER
