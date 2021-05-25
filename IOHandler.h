#ifndef IO_HANDLER
#define IO_HANDLER

#include "GameManager.h"

using std::ifstream;

enum HelpLocation {
    MAIN_MENU,
    NEW_GAME,
    LOAD_GAME,
    PLACE_TILE,
    REPLACE_TILE,
    SETTINGS_MENU
};

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
     * Game Loader
     * Loads .save file based on user input
     */
    static void loadGame();

    /**
     * Initiates gameplay for a single round for a single player.
     */
    static void playRound();

    /**
     * Based on user input will either
     * place tiles, replace tiles or save game
     * @param operation - place ,replace ,save
     * @param tile - tile in format of D1
     * @param keywordAt - needs keyword "at" to place tile
     * @param position - tile position in format A0 - Z25
     */
    static bool logicHandler(const string& operation, const string& tile,
        const string& keywordAt, const string& position);

    /**
     * Place tile operation, forwards information to GameManager.
     * @param tile - the tile received from input
     * @param position - the tile position
     */
    static void placeTileOperation(const string& tile, const string& position);

    /**
     * Replace tile operation, forwards information to GameManager.
     * @param tile - the tile received from input
     */
    static void replaceTileOperation(const string& tile);

    /**
     * Prints out round details
     */
    static void printRound();

    /**
     * Prints out messages to notify the players about their operation.
     * @param message - explanatory message from the GameManager about the state
     * @param state - state passed from GameManager to check current state
     */
    static void notify(const string& message, State state);

    /**
     * Will quit the program when called.
     */
    static void quit();

    static bool gameRunning;
    static bool takingInput;
    static bool aiEnabled;
    static bool helpEnabled;
    static bool invalidInputEnabled;
    static bool colourEnabled;
    static bool hintEnabled;
    static bool multipleTilesEnabled;

private:
    /**
     * Prints out a help message based on location.
     */
    static void help(HelpLocation location);

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
     * @param range - the range of selection values, starting from 1
     * @return the selected integer
     */
    static int getSelection(int range);

    /**
     * Asks for user input on confirming an option as either yes or no.
     * @return true if the user responds with Y (yes), false otherwise
     */
    static bool getConfirmation();

    /**
     * Validates username.
     * @param name - the name for the player
     */
    static bool validateName(const string& name);

    /**
     * Checks if tile is valid.
     * @param tile - tile to be checked
     */
    static bool checkTile(const string& tile);

    /**
     * Checks if tile position is valid.
     * @param position - position to be checked
     */
    static bool checkTilePosition(const string& position);

    /**
     * Checks if file is empty.
     * @param file - the file to be checked
     */
    static bool isEmpty(ifstream& file);

    /**
     * Prints out credits
     */
    static void credits();
};

#endif // !IO_HANDLER
