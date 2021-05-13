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
    static bool logicHandler(const string& operation, const string& tile, const string& keywordAT, const string& pos);
    static void credits();
    static void loadGame();
    static bool checkTile(const string& tile);
    static bool checkTilePosition(const string& position);
    static void placeTile(const string& tile, const string& position);
    static void replaceTile(const string& tile);
    static void notify(const string& message, State state);
    static void quit();

    static bool gameRunning;
    static bool takingInput;
};

#endif // !IO_HANDLER
