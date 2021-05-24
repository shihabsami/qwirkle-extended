#ifndef IO_HANDLER
#define IO_HANDLER

#include <iostream>

#include "GameManager.h"

class IOHandler {
public:
    /**
     * Begins game by printing out the initial message.
     */
    static void beginGame();

    /**
     * The main menu of the game.
     */
    static void mainMenu();

    /**
     * The settings menu of the game.
     */
    static void settingsMenu();

    /**
     * Starts a new game.
     */
    static void newGame();

    /**
     * Validates username.
     * @param name - the name for the player
     */
    static bool validateName(const string& name);

    /**
     * Prints out base gameplay
     * Current player name
     * Scores of player 1 and 2
     * Ask for user input
     */
    static void playRound();

    /**
     * Based on user input will either
     * place tiles , replace tiles or save game
     * @parms operation - place ,replace ,save
     * @parms tile - tile in format of D1
     * @parms keywordAt - needs keyword at to place tile
     * @parms position - tile position in format A0 - Z25
     */
    static bool logicHandler(const string& operation, const string& tile,
        const string& keywordAt, const string& position);

    /**
     * Prints out round details
     */
    static void printRound();

    /**
     * Prints out credits
     */
    static void credits();

    /**
     * Game Loader
     * Loads .save file based on user input
     */
    static void loadGame();

    static bool is_empty(std::ifstream& file);

    /**
     * Checks if tile is valid.
     * @parms tile - tile to be checked
     */
    static bool checkTile(const string& tile);

    /**
     * Checks if tile position is valid.
     * @parms position - position to be checked
     */
    static bool checkTilePosition(const string& position);

    /**
     * Place tile operation, forwards information to GameManager.
     * @parms tile - the tile received from input
     * @parms position - the tile position
     */
    static void placeTile(const string& tile, const string& position);

    /**
     * Replace tile operation, forwards information to GameManager.
     * @parms tile - the tile received from input
     */
    static void replaceTile(const string& tile);

    /**
     * Prints out messages to notify the players about their operation.
     * @parms message - message from the gameManager to notify the current
     * situation
     * @parms state - state passed from gameManager to check current situation
     */
    static void notify(const string& message, State state);

    /**
     * Will quit the program when called
     */
    static void quit();

    static bool gameRunning;

    static bool takingInput;

    static bool aiEnabled;

private:
    /**
     * Prints out the main menu.
     */
    static void printMenu();

    /**
     * Make a selection from main menu and act based on that.
     */
    static void menuSelection();

    /**
     * Prints out the settings menu.
     */
    static void printSettings();

    /**
     * Make a selection from settings menu and act based on that.
     */
    static void settingsSelection();

    /**
     * Prints out '>' prompt character after every input needed.
     */
    static void prompt();

    /**
     * Asks for user input on selecting an item from the provided numeric range.
     * @param startOption - the first option
     * @param endOption - the last option
     * @return the selected integer
     */
    static int getSelection(int startOption, int endOption);

    /**
     * Asks for user input on confirming an option as either yes or no.
     * @return true if the user responds with Y (yes), false otherwise
     */
    static bool getConfirmation();
};

#endif // !IO_HANDLER
