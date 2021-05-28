#ifndef IO_HANDLER
#define IO_HANDLER

#include "GameManager.h"

#include <utility>

using std::ifstream;
using std::pair;

/**
 * The possible locations help can be seeked. (i.e., a prompt is provided)
 */
enum HelpLocation {
    MAIN_MENU,
    NEW_GAME,
    LOAD_GAME,
    GAME_ROUND,
    SETTINGS_MENU,
    SETTINGS_CONFIRMATION,
    PLAYER_MODE
};

/// The different line indices that define the save file format.
struct SaveFileFormat {
    unsigned int boardSizeLineIndex;
    unsigned int boardTilesLineIndex;
    unsigned int tileBagLineIndex;
    unsigned int currentPlayerLineIndex;
    unsigned int settingsLineIndexBegin;
    unsigned int settingsLineIndexEnd;
    unsigned int playerNameLineIndex;
    unsigned int playerScoreLineIndex;
    unsigned int playerHandLineIndex;
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
     * Saves the current state of the game to a specified file.
     *
     * @param saveFileName - the name of the file
     */
    static void saveGame(const string& saveFileName);

    /**
     * Loads a game from a file with .save extension.
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
     * @param secondKeyword - secondKeyword in format of D1
     * @param thirdKeyword - needs keyword "at" to place secondKeyword
     * @param fourthKeyword - secondKeyword fourthKeyword in format A0 - Z25
     */
    static bool operationHandler(const string& operation, const string& secondKeyword,
        const string& thirdKeyword, const string& fourthKeyword);

    /**
     * Place tile operation, forwards information to GameManager.
     *
     * @param tile - the tile received from input
     * @param position - the tile position
     */
    static void placeTileOperation(const string& tile, const string& position);

    /**
     * Replace tile operation, forwards information to GameManager.
     *
     * @param tile - the tile received from input
     */
    static void replaceTileOperation(const string& tile);

    /**
     * Prints out round details
     */
    static void printRound();

    /**
     * Prints out messages to notify the players about their operation.
     *
     * @param message - explanatory message from the GameManager about the state
     * @param state - state passed from GameManager to check current state
     */
    static void notify(const string& message, State state);

    /**
     * Will quit the program when called.
     */
    static void quit();

    /// Helper attributes
    static bool gameRunning;
    static bool takingInput;
    static array<pair<string, size_t>, 5> highScores;
    static SaveFileFormat saveFileFormat;

    /// Attributes related to AI (if enabled)
    static bool aiEnabled;
    static string aiName;

    /// Settings
    static bool helpEnabled;
    static bool invalidInputEnabled;
    static bool colourEnabled;
    static bool hintEnabled;
    static unsigned int numberOfPlayers;

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
     *
     * @param range - the range of selection values, starting from 1
     * @param location - the current location the selection is asked at so that
     * if user requests help, it can retrieve help from the help function
     * @return the selected integer
     */
    static unsigned int getSelection(unsigned int range, HelpLocation location);

    /**
     * Asks for user input on confirming an option as either yes or no.
     *
     * @param location - the current location the selection is asked at so that
     * if user requests help, it can retrieve help from the help function
     * @return true if the user responds with Y (yes), false otherwise
     */
    static bool getConfirmation();

    /**
     * Check whether the user is seeking help.
     *
     * @param command - the command user input
     */
    static bool isSeekingHelp(string command);

    /**
     * Prints out a help message based on location.
     */
    static void printHelpMessage(HelpLocation location);

    /**
     * Validates username according to game rules.
     *
     * @param name - the name for the player
     * @return true if valid
     */
    static bool validateName(const string& name, const vector<string>& existingNames);

    /**
     * Checks if tile is valid according to game rules.
     *
     * @param tile - tile to be checked
     * @return true if valid
     */
    static bool checkTile(const string& tile);

    /**
     * Checks if tile position is valid according to game rules.
     *
     * @param position - position to be checked
     * @return true if valid
     */
    static bool checkTilePosition(const string& position);

    /**
     * Checks if file is empty.
     *
     * @param file - the file to be checked
     * @return true if the file is empty
     */
    static bool isEmpty(ifstream& file);

    /**
     * Check what save file format to use.
     * @param formatTag - the line of the save file that defines the format
     */
    static bool deduceSaveFileFormat(const string& formatTag);

    /**
     * Prints out the high scores.
     */
    static void printHighScores();

    /**
     * Saves the high scores.
     */
    static void saveHighScores();

    /**
     * Loads the high scores.
     */
    static void loadHighScores();

    /**
     * Updates the high scores.
     * @param winners - the winners of this game
     */
     static void updateHighScores(const vector<shared_ptr<Player>>& winners);

    /**
     * Prints out credits
     */
    static void credits();
};

#endif // !IO_HANDLER
