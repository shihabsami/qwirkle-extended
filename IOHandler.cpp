#include "IOHandler.h"
#include "Constants.h"

#include <algorithm>
#include <cctype>
#include <iostream>
#include <limits>
#include <sstream>
#include <string>

using std::cin;
using std::cout;
using std::cerr;

using std::endl;
using std::get;
using std::toupper;
using std::tolower;
using std::to_string;
using std::max_element;

using std::stringstream;
using std::istringstream;
using std::invalid_argument;
using std::runtime_error;

typedef std::numeric_limits<std::streamsize> CharLimit;
typedef std::numeric_limits<int> IntLimit;

bool IOHandler::gameRunning = false;
bool IOHandler::takingInput = false;
array<pair<string, size_t>, 5> IOHandler::highScores{};
SaveFileFormat IOHandler::saveFileFormat;

bool IOHandler::aiEnabled = false;
string IOHandler::aiName{};
unsigned int IOHandler::numberOfPlayers = 2;

// these settings are enabled by default
bool IOHandler::helpEnabled = true;
bool IOHandler::invalidInputEnabled = true;
bool IOHandler::colourEnabled = true;
bool IOHandler::hintEnabled = true;

void IOHandler::beginGame() {
    cout << "Welcome to Qwirkle!" << endl;
    cout << "-------------------" << endl;

    loadHighScores();
    mainMenu();
}

void IOHandler::mainMenu() {
    printMenu();
    menuSelection();
}

void IOHandler::settingsMenu() {
    printSettings();
    settingsSelection();
}

void IOHandler::newGame() {
    cout << endl << "Starting a new game" << endl;

    vector<string> playerNames;
    numberOfPlayers = aiEnabled ? 2 : numberOfPlayers;
    for (unsigned int i = 0; i < numberOfPlayers; ++i) {
        bool nameCheck = true;
        string name;
        while (nameCheck) {
            bool aiNameCheck = aiEnabled && i == SECOND_POSITION;
            cout << endl
                 << "Enter a name for " << (aiNameCheck ? "AI" : "player ")
                 << (aiNameCheck ? "" : to_string(i + 1))
                 << " (uppercase characters only)" << endl;
            prompt();
            cin >> name;
            if (cin.eof())
                quit();

            if (isSeekingHelp(name)) {
                printHelpMessage(NEW_GAME);
            } else if (!validateName(name, playerNames)) {
                cout << "Must enter a name in CAPS for player and name must "
                        "not contain numbers or symbols or duplicate names"
                     << endl;
                cin.clear();
                cin.ignore(CharLimit::max(), '\n');
            } else {
                nameCheck = false;
                playerNames.push_back(name);
                aiName = aiNameCheck ? name : aiName;
            }
        }
    }

    cin.clear();
    cin.ignore(CharLimit::max(), '\n');
    cout << endl << "Let's Play!" << endl;
    GameManager::beginGame(playerNames);
    gameRunning = true;
}

void IOHandler::saveGame(const string& saveFileName) {
    ofstream file(saveFileName);
    file << NEW_SAVE_FILE_FORMAT << endl;
    file << BOARD_LENGTH << "," << BOARD_LENGTH << endl;
    file << *GameManager::board << endl;
    file << *GameManager::bag->getTiles() << endl;
    file << GameManager::getCurrentPlayer()->getName() << endl;

    file << GET_SETTING_NAME(aiEnabled) << "-" << aiEnabled << endl;
    file << GET_SETTING_NAME(aiName) << "-" << aiName << endl;
    file << GET_SETTING_NAME(helpEnabled) << "-" << helpEnabled << endl;
    file << GET_SETTING_NAME(invalidInputEnabled) << "-" << invalidInputEnabled
         << endl;
    file << GET_SETTING_NAME(colourEnabled) << "-" << colourEnabled << endl;
    file << GET_SETTING_NAME(hintEnabled) << "-" << hintEnabled << endl;
    file << GET_SETTING_NAME(numberOfPlayers) << "-" << numberOfPlayers << endl;

    for (const auto& player : GameManager::players) {
        file << player->getName() << endl;
        file << player->getScore() << endl;
        file << *player->getHand()->getTiles() << endl;
    }

    file.close();
}

void IOHandler::loadGame() {
    bool fileCheck = true;
    while (fileCheck) {
        try {
            string filename;
            cout << "Enter the filename from which to load a game" << endl;
            prompt();
            cin >> filename;

            if (cin.eof())
                quit();

            if (isSeekingHelp(filename)) {
                printHelpMessage(LOAD_GAME);
                throw invalid_argument("");
            }

            ifstream file(filename);
            if (!file)
                throw invalid_argument("File does not exist");
            if (isEmpty(file))
                throw invalid_argument("File is empty");

            cin.clear();
            cin.ignore();

            vector<shared_ptr<Player>> players;
            shared_ptr<TileBag> tileBag = make_shared<TileBag>();
            shared_ptr<GameBoard> board = make_shared<GameBoard>();
            string currentPlayerName;

            unsigned int playerNamesRead = 0;
            unsigned int playerScoresRead = 0;
            unsigned int playerHandsRead = 0;
            bool oldFormat = false;

            unsigned int lineIndex = 0;
            string text;
            while (getline(file, text)) {
                text.erase(remove(text.begin(), text.end(), '\r'), text.end());
                text.erase(remove(text.begin(), text.end(), '\n'), text.end());
                if (lineIndex == 0)
                    oldFormat = deduceSaveFileFormat(text);

                if (lineIndex == saveFileFormat.boardSizeLineIndex) {
                    stringstream ss(text);
                    while (ss.good()) {
                        string substr;
                        getline(ss, substr, ',');
                        int number = stoi(substr);
                        if (number < 0 || number > BOARD_LENGTH) {
                            throw invalid_argument(
                                "The board size should be more than 0 and less "
                                "than 26");
                        }
                    }
                } else if (lineIndex == saveFileFormat.boardTilesLineIndex) {
                    stringstream ss(text);
                    while (ss.good()) {
                        string substr;
                        getline(ss, substr, ' ');
                        const char at = '@';
                        if (substr[FIRST_POSITION] != '\0')
                            if (substr[THIRD_POSITION] != at)
                                throw invalid_argument(
                                    "The board should appear as a list of "
                                    "tile@postion");

                        char last = substr.length() < 6 ? ',' : substr[5];
                        char tile[2] = {substr[0], substr[1]};
                        char position[3] = {substr[3], substr[4], last};

                        int row =
                            position[FIRST_POSITION] - ASCII_ALPHABET_BEGIN;
                        int column = (position[THIRD_POSITION] == ASCII_COMMA ||
                                         position[THIRD_POSITION] == '\r' ||
                                         position[THIRD_POSITION] == '\n')
                            ? position[SECOND_POSITION] - '0'
                            : (int)(position[SECOND_POSITION] - '0') * 10 +
                                (int)(position[THIRD_POSITION] - '0');

                        if (!substr.empty())
                            board->placeTile(
                                make_shared<Tile>(tile[0], tile[1] - '0'), row,
                                column);
                    }

                } else if (lineIndex >= saveFileFormat.settingsLineIndexBegin &&
                    lineIndex <= saveFileFormat.settingsLineIndexEnd) {
                    size_t delimiterPosition = text.find('-');
                    string settingName = text.substr(0, delimiterPosition);
                    string settingValue = text.substr(delimiterPosition + 1);

                    if (settingName == GET_SETTING_NAME(aiEnabled))
                        aiEnabled = settingValue != "0";
                    if (settingName == GET_SETTING_NAME(aiName))
                        aiName = settingValue;
                    else if (settingName == GET_SETTING_NAME(helpEnabled))
                        helpEnabled = settingValue != "0";
                    else if (settingName ==
                        GET_SETTING_NAME(invalidInputEnabled))
                        invalidInputEnabled = settingValue != "0";
                    else if (settingName == GET_SETTING_NAME(colourEnabled))
                        colourEnabled = settingValue != "0";
                    else if (settingName == GET_SETTING_NAME(hintEnabled))
                        hintEnabled = settingValue != "0";
                    else if (settingName == GET_SETTING_NAME(numberOfPlayers))
                        numberOfPlayers = stoi(settingValue);
                } else if (lineIndex == saveFileFormat.currentPlayerLineIndex ||
                    (lineIndex >= saveFileFormat.playerNameLineIndex &&
                        (lineIndex - saveFileFormat.playerNameLineIndex) % 3 ==
                            0)) {
                    for (unsigned i = 0; i < text.length() - 1; i++) {
                        int ascii = static_cast<unsigned char>(text[i]);
                        if (ascii < ASCII_ALPHABET_BEGIN - 1 ||
                            ascii > ASCII_ALPHABET_END + 1 || ascii == 0) {
                            throw invalid_argument(
                                "Name format is not part of ASCII text");
                        }
                    }

                    if (lineIndex == saveFileFormat.currentPlayerLineIndex) {
                        currentPlayerName = text;
                    } else {
                        if (oldFormat) {
                            if (playerNamesRead < 2) {
                                players.emplace_back(make_shared<Player>(
                                    text, make_shared<PlayerHand>()));
                            }
                        } else {
                            players.emplace_back(make_shared<Player>(
                                text, make_shared<PlayerHand>()));
                        }

                        ++playerNamesRead;
                    }
                } else if (lineIndex >= saveFileFormat.playerScoreLineIndex &&
                    (lineIndex - saveFileFormat.playerScoreLineIndex) % 3 ==
                        0) {
                    int score = stoi(text);
                    if (score < 0)
                        throw invalid_argument(
                            "Player score should be positive");

                    if (oldFormat) {
                        if (playerScoresRead < 2) {
                            players.at(playerScoresRead)->setScore(score);
                        }
                    } else {
                        players.at(playerScoresRead)->setScore(score);
                    }

                    ++playerScoresRead;
                } else if (lineIndex == saveFileFormat.tileBagLineIndex ||
                    (lineIndex >= saveFileFormat.playerHandLineIndex &&
                        (lineIndex - saveFileFormat.playerHandLineIndex) % 3 ==
                            0)) {
                    stringstream ss(text);
                    bool readPlayerHand = false;
                    while (ss.good()) {
                        string substr;
                        getline(ss, substr, ',');
                        if (text[FIRST_POSITION] != '\0')
                            if (substr.length() != 2)
                                throw invalid_argument(
                                    "Wrong tile list format");

                        if (lineIndex == saveFileFormat.tileBagLineIndex) {
                            tileBag->getTiles()->addBack(
                                make_shared<Tile>(substr[FIRST_POSITION],
                                    substr[SECOND_POSITION] - '0'));
                        } else {
                            if (oldFormat) {
                                if (playerHandsRead < 2) {
                                    players.at(playerHandsRead)
                                        ->getHand()
                                        ->addTile(make_shared<Tile>(
                                            substr[FIRST_POSITION],
                                            substr[SECOND_POSITION] - '0'));
                                }
                            } else {
                                players.at(playerHandsRead)
                                    ->getHand()
                                    ->addTile(make_shared<Tile>(
                                        substr[FIRST_POSITION],
                                        substr[SECOND_POSITION] - '0'));
                            }

                            readPlayerHand = true;
                        }
                    }
                    playerHandsRead =
                        readPlayerHand ? playerHandsRead + 1 : playerHandsRead;
                }

                ++lineIndex;
            }

            cout << "Qwirkle game successfully loaded" << endl;
            file.close();
            numberOfPlayers = (unsigned int)players.size();
            GameManager::loadGame(players, currentPlayerName, board, tileBag);
            gameRunning = true;
            fileCheck = false;
        } catch (const invalid_argument& e) {
            cerr << e.what() << endl;
        }
    }
}

void IOHandler::playRound() {
    takingInput = true;
    while (takingInput) {
        prompt();
        string commandString, operation, tile, keywordAt, position, saveName;
        getline(cin, commandString);
        istringstream command(commandString);

        if (cin.eof()) {
            quit();
            cin.clear();
        }

        command >> operation >> tile >> keywordAt >> position;
        saveName = tile;

        auto toLower = [](unsigned char c) { return tolower(c); };
        auto toUpper = [](unsigned char c) { return toupper(c); };
        transform(
            operation.begin(), operation.end(), operation.begin(), toLower);
        transform(tile.begin(), tile.end(), tile.begin(), toUpper);
        transform(
            keywordAt.begin(), keywordAt.end(), keywordAt.begin(), toLower);
        transform(position.begin(), position.end(), position.begin(), toUpper);

        if (operation == "save") {
            takingInput =
                operationHandler(operation, saveName, keywordAt, position);
        } else {
            takingInput =
                operationHandler(operation, tile, keywordAt, position);
        }
    }
}

bool IOHandler::operationHandler(const string& operation,
    const string& secondKeyword, const string& thirdKeyword,
    const string& fourthKeyword) {

    if (operation == "place" && thirdKeyword == "at") {
        if (checkTile(secondKeyword) && checkTilePosition(fourthKeyword))
            placeTileOperation(secondKeyword, fourthKeyword);
        else
            takingInput = true;
    } else if (operation == "replace" && thirdKeyword.empty() &&
        fourthKeyword.empty()) {
        if (checkTile(secondKeyword))
            replaceTileOperation(secondKeyword);
        else
            takingInput = true;
    } else if (hintEnabled && operation == "hint") {
        vector<Move> moves = GameManager::getPossibleMoves();
        if (!moves.empty()) {
            cout << "Your possible moves are as follows" << endl;
            for (const auto& move : moves) {
                move.tile->print(cout, colourEnabled);
                cout << " at "
                     << (char)(move.location.row + ASCII_ALPHABET_BEGIN)
                     << move.location.column << " for " << move.points
                     << " points" << endl;
            }
        } else {
            if (GameManager::board->isEmpty())
                cout << "You can place any tile at any location" << endl;
            else
                cout << "Your only possible move is to replace a tile" << endl;
        }
    } else if (operation == "save" && !secondKeyword.empty()) {
        saveGame(secondKeyword + ".save");
        cout << endl;
        cout << "Game successfully saved" << endl;
        cout << endl;
        takingInput = true;
    } else if (isSeekingHelp(operation)) {
        printHelpMessage(GAME_ROUND);
    } else if (operation == "quit") {
        quit();
        takingInput = false;
    } else {
        cout << ERROR_MESSAGE
             << (invalidInputEnabled ? " - Not a valid command" : "") << endl;
    }

    return takingInput;
}

void IOHandler::placeTileOperation(const string& tile, const string& position) {
    string appended;
    Colour colour = tile.at(FIRST_POSITION);
    Shape shape =
        static_cast<int>(tile.at(SECOND_POSITION)) - ASCII_NUMERICAL_BEGIN;
    int rowNumeric = 0;
    int columnNumeric = 0;

    if (position.length() >= 2) {
        int rowChar =
            static_cast<unsigned char>(int(position.at(FIRST_POSITION)));
        rowNumeric = rowChar - ASCII_ALPHABET_BEGIN;

        char columnFirstDigit = position.at(SECOND_POSITION);
        appended.append(1, columnFirstDigit);

        if (position.size() == STRING_SIZE_2) {
            columnNumeric = stoi(appended);
        } else if (position.size() == STRING_SIZE_3) {
            char columnSecondDigit = position.at(THIRD_POSITION);
            appended.append(1, columnSecondDigit);
            columnNumeric = stoi(appended);
        }
    }

    GameManager::placeTile(colour, shape, rowNumeric, columnNumeric);
}

void IOHandler::replaceTileOperation(const string& tile) {
    Colour colour = tile.at(FIRST_POSITION);
    Shape shape =
        static_cast<int>(tile.at(SECOND_POSITION)) - ASCII_NUMERICAL_BEGIN;
    GameManager::replaceTile(colour, shape);
}

void IOHandler::printRound() {
    cout << endl;
    cout << GameManager::getCurrentPlayer()->getName() << ", it's your turn"
         << endl;

    for (const shared_ptr<Player>& player : GameManager::players)
        cout << "Score for " << player->getName() << ": " << player->getScore()
             << endl;

    cout << endl;
    GameManager::board->print(cout, colourEnabled);
    cout << endl;
    cout << (aiEnabled && GameManager::getCurrentPlayer()->getName() == aiName
                    ? "AI's"
                    : "Your")
         << " hand is" << endl;
    GameManager::getCurrentPlayer()->getHand()->print(cout, colourEnabled);
    cout << endl;
}

void IOHandler::notify(const string& message, State state) {
    if (state == PLACE_SUCCESS || state == REPLACE_SUCCESS ||
        state == QWIRKLE) {
        cout << message << endl;
        takingInput = false;
    } else if (state == PLACE_FAILURE || state == REPLACE_FAILURE) {
        if (invalidInputEnabled)
            cout << ERROR_MESSAGE << " - " << message << endl;
        else
            cout << ERROR_MESSAGE << endl;
    } else if (state == GAME_OVER) {
        cout << endl;
        GameManager::board->print(cout, colourEnabled);
        cout << endl;
        cout << "Game Over!" << endl;

        for (const shared_ptr<Player>& player : GameManager::players)
            cout << "Score for " << player->getName() << ": "
                 << player->getScore() << endl;

        size_t maxScore = max_element(GameManager::players.begin(),
            GameManager::players.end(),
            [](const shared_ptr<Player>& p1, const shared_ptr<Player>& p2) {
                return p1->getScore() > p2->getScore();
            })
                              ->get()
                              ->getScore();

        vector<shared_ptr<Player>> winners;
        for (const shared_ptr<Player>& player : GameManager::players) {
            if (player->getScore() == maxScore)
                winners.push_back(player);
        }

        updateHighScores(winners);
        if (winners.size() == 1) {
            cout << "Player " << winners.begin()->get()->getName() << " won!"
                 << endl;
        } else {
            cout << "Game is drawn between ";
            for (size_t i = 0; i < winners.size(); ++i) {
                cout << winners.at(i)->getName();
                cout << (i == winners.size() - 1 ? "" : " and ");
            }
            cout << endl;
        }

        saveHighScores();
        quit();
    }
}

void IOHandler::quit() {
    GameManager::resetGame();
    cout << "Goodbye" << endl;
    exit(EXIT_SUCCESS);
}

bool IOHandler::isSeekingHelp(string command) {
    transform(command.begin(), command.end(), command.begin(),
        [](unsigned char c) { return tolower(c); });

    return helpEnabled && command == "help";
}

void IOHandler::printHelpMessage(HelpLocation location) {
    if (location == MAIN_MENU) {
        cout << "Choose an option from 1 to 5 inclusive" << endl;
    } else if (location == NEW_GAME) {
        cout << "Type the name of the player according to the rules" << endl;
    } else if (location == LOAD_GAME) {
        cout
            << "Type the name of the file with the .save extension to load from"
            << endl;
    } else if (location == GAME_ROUND) {
        cout << "You may type one of the following commands" << endl
             << "PLACE __ (tile) AT __ (location)" << endl
             << "Or" << endl
             << "REPLACE __ (tile)" << endl
             << "Or" << endl
             << "HINT (if enabled from settings)" << endl;
    } else if (location == SETTINGS_MENU) {
        cout << "Choose an option from 1 to 6 inclusive" << endl;
    } else if (location == SETTINGS_CONFIRMATION) {
        cout << "Type Y to enable, or N to disable the setting" << endl;
    } else {
        cout << "Choose and option from 1 to 3 inclusive" << endl;
    }
}

void IOHandler::printMenu() {
    cout << endl << "Menu" << endl;
    cout << "----" << endl;
    cout << "1. New Game" << endl;
    cout << "2. Load Game" << endl;
    cout << "3. Settings" << endl;
    cout << "4. High Scores" << endl;
    cout << "5. Credits (Show student information)" << endl;
    cout << "6. Quit" << endl;
}

void IOHandler::menuSelection() {
    unsigned int option = getSelection(6, MAIN_MENU);

    if (option == 1)
        newGame();
    else if (option == 2)
        loadGame();
    else if (option == 3)
        settingsMenu();
    else if (option == 4)
        printHighScores();
    else if (option == 5)
        credits();
    else if (option == 6 && !cin.eof())
        quit();
}

void IOHandler::printSettings() {
    cout << endl << "Settings" << endl;
    cout << "--------" << endl;
    cout << "1. Help!" << endl;
    cout << "2. Better invalid input" << endl;
    cout << "3. Colour" << endl;
    cout << "4. Hint" << endl;
    cout << "5. 3-4 player modes" << endl;
    cout << "6. Return to menu" << endl;
}

void IOHandler::settingsSelection() {
    unsigned int option = getSelection(6, SETTINGS_MENU);

    if (option <= 5) {
        bool enabled = false;
        if (option == 1) {
            helpEnabled = getConfirmation();
            enabled = helpEnabled;
        } else if (option == 2) {
            invalidInputEnabled = getConfirmation();
            enabled = invalidInputEnabled;
        } else if (option == 3) {
            colourEnabled = getConfirmation();
            enabled = colourEnabled;
        } else if (option == 4) {
            hintEnabled = getConfirmation();
            enabled = hintEnabled;
        } else if (option == 5) {
            cout << "Select the number of players" << endl;
            cout << "----------------------------" << endl;
            cout << "1. 2 player" << endl;
            cout << "2. 3 player" << endl;
            cout << "3. 4 player" << endl;

            numberOfPlayers = getSelection(3, PLAYER_MODE) + 1;
            enabled = true;
        }

        cout << (enabled ? "Enabled" : "Disabled") << " successfully" << endl;
        settingsMenu();
    } else if (option == 6) {
        mainMenu();
    }
}

void IOHandler::prompt() { cout << "> "; }

unsigned int IOHandler::getSelection(unsigned int range, HelpLocation location) {
    bool selecting = true;
    string option;
    unsigned int optionNumeric;
    while (selecting) {
        cout << endl;
        prompt();
        cin >> option;
        if (cin.eof())
            quit();
        transform(option.begin(), option.end(), option.begin(),
            [](unsigned char c) { return tolower(c); });

        if (isSeekingHelp(option)) {
            printHelpMessage(location);
        } else {
            try {
                optionNumeric = stoi(option);
                if (optionNumeric >= 1 && optionNumeric <= range) {
                    selecting = false;
                } else {
                    cin.clear();
                    cin.ignore(CharLimit::max(), '\n');
                    throw runtime_error("");
                }
            } catch (const runtime_error& e) {
                cerr << ERROR_MESSAGE << endl;
            } catch (const invalid_argument& e) {
                cerr << ERROR_MESSAGE << endl;
            }
        }
    }

    return optionNumeric;
}

bool IOHandler::getConfirmation() {
    cout << endl << "Enable? (Y/N)" << endl;

    bool response;
    bool selecting = true;
    string responseString;
    while (!cin.eof() && selecting) {
        prompt();
        cin >> responseString;

        if (isSeekingHelp(responseString)) {
            printHelpMessage(SETTINGS_CONFIRMATION);
        } else {
            try {
                if (responseString.length() == 1) {
                    if (toupper(responseString[0]) == 'Y') {
                        response = true;
                        selecting = false;
                    } else if (toupper(responseString[0]) == 'N') {
                        response = false;
                        selecting = false;
                    }
                } else if (cin.eof()) {
                    quit();
                } else {
                    cin.clear();
                    cin.ignore(CharLimit::max(), '\n');
                    throw runtime_error("");
                }
            } catch (const runtime_error& e) {
                cerr << ERROR_MESSAGE << endl;
            }
        }
    }

    return response;
}

bool IOHandler::validateName(
    const string& name, const vector<string>& existingNames) {
    bool valid = true;
    for (char c : name)
        valid &= isupper(c) != 0;
    for (const auto& n : existingNames)
        valid &= name != n;

    return valid;
}

bool IOHandler::checkTile(const string& tile) {
    bool condition = false;
    bool validLetter = false;
    bool validNumber = false;
    try {
        string appended;
        if (tile.size() == STRING_SIZE_2) {
            char letter = tile.at(FIRST_POSITION);
            char number = tile.at(SECOND_POSITION);

            appended.append(1, number);
            int combinedNumber = stoi(appended);
            for (Colour colour : COLOURS)
                if (letter == colour)
                    validLetter = true;

            for (Shape shape : SHAPES)
                if (combinedNumber == shape)
                    validNumber = true;
        }

        if (validLetter && validNumber)
            condition = true;
        else
            throw invalid_argument("");
    } catch (const invalid_argument& e) {
        cerr << ERROR_MESSAGE
             << (invalidInputEnabled ? " - Not a valid tile" : "") << endl;
    }

    return condition;
}

bool IOHandler::checkTilePosition(const string& position) {
    string appended;
    bool condition = false, validLetter = false, validNumber = false;

    try {
        if (position.size() == STRING_SIZE_2 ||
            position.size() == STRING_SIZE_3) {
            char letter = position.at(FIRST_POSITION);
            int asciiLetter = static_cast<unsigned char>(int(letter));
            validLetter = asciiLetter >= ASCII_ALPHABET_BEGIN &&
                asciiLetter <= ASCII_ALPHABET_END;

            char firstDigit = position.at(SECOND_POSITION);
            appended.append(1, firstDigit);

            if (position.size() == STRING_SIZE_3) {
                char lastDigit = position.at(THIRD_POSITION);
                appended.append(1, lastDigit);
                int combinedNumber = stoi(appended);
                validNumber =
                    combinedNumber < BOARD_LENGTH - 1 && combinedNumber > 0;
            } else {
                validNumber = stoi(appended) <= MAX_ASCII_DIGIT;
            }
        }

        if (validNumber && validLetter)
            condition = true;
        else
            throw invalid_argument("");
    } catch (const invalid_argument& e) {
        cerr << ERROR_MESSAGE
             << (invalidInputEnabled ? " - Not a valid position" : "") << endl;
    }

    return condition;
}

bool IOHandler::isEmpty(ifstream& file) {
    return file.peek() == ifstream::traits_type::eof();
}


bool IOHandler::deduceSaveFileFormat(const string& formatTag) {
    bool oldFormat = formatTag != NEW_SAVE_FILE_FORMAT;
    if (!oldFormat) {
        saveFileFormat.boardSizeLineIndex = 1;
        saveFileFormat.boardTilesLineIndex = 2;
        saveFileFormat.tileBagLineIndex = 3;
        saveFileFormat.currentPlayerLineIndex = 4;
        saveFileFormat.settingsLineIndexBegin = 5;
        saveFileFormat.settingsLineIndexEnd = 11;
        saveFileFormat.playerNameLineIndex = 12;
        saveFileFormat.playerScoreLineIndex = 13;
        saveFileFormat.playerHandLineIndex = 14;
    } else {
        saveFileFormat.boardSizeLineIndex = 6;
        saveFileFormat.boardTilesLineIndex = 7;
        saveFileFormat.tileBagLineIndex = 8;
        saveFileFormat.currentPlayerLineIndex = 9;
        saveFileFormat.settingsLineIndexBegin = IntLimit::max();
        saveFileFormat.settingsLineIndexEnd = IntLimit::max();
        saveFileFormat.playerNameLineIndex = 0;
        saveFileFormat.playerScoreLineIndex = 1;
        saveFileFormat.playerHandLineIndex = 2;
    }

    return oldFormat;
}


void IOHandler::printHighScores() {
    for (size_t i = 0; i < highScores.size(); ++i) {
        cout << i + 1 << ". ";
        if (highScores.at(i).first.empty())
            cout << "Empty slot" << endl;
        else
            cout << highScores.at(i).second << " by " << highScores.at(i).first
                 << endl;
    }

    mainMenu();
}

void IOHandler::saveHighScores() {
    ofstream file(HIGH_SCORES_FILE_NAME);
    for (const pair<string, size_t>& score : highScores)
        file << score.first << "-" << score.second << endl;
}

void IOHandler::loadHighScores() {
    ifstream file(HIGH_SCORES_FILE_NAME);
    string text;
    int highScoresRead = 0;
    while (getline(file, text)) {
        text.erase(remove(text.begin(), text.end(), '\r'), text.end());
        text.erase(remove(text.begin(), text.end(), '\n'), text.end());

        size_t delimiterPosition = text.find('-');
        if (delimiterPosition != 0) {
            string playerName = text.substr(0, delimiterPosition);
            size_t playerScore = stoi(text.substr(delimiterPosition + 1));
            highScores.at(highScoresRead) = make_pair(playerName, playerScore);
            ++highScoresRead;
        }
    }
}

void IOHandler::updateHighScores(const vector<shared_ptr<Player>>& winners) {
    shared_ptr<Player> winner = winners.at(FIRST_POSITION);
    size_t maxScore = winner->getScore();
    bool scoreUpdated = false;
    for (size_t i = 0; i < highScores.size() && !scoreUpdated; ++i) {
        if (maxScore > highScores.at(i).second) {
            if (winners.size() > 1) {
                for (size_t j = 0, k = i;
                     j < winners.size() && k < highScores.size(); ++j, ++k)
                    highScores.at(i) = make_pair(
                        winners.at(j)->getName(), winners.at(j)->getScore());
                scoreUpdated = true;
            } else {
                highScores.at(i) =
                    make_pair(winner->getName(), winner->getScore());
                scoreUpdated = true;
            }
        }
    }
}

void IOHandler::credits() {
    cout << "----------------------------------" << endl;
    cout << "Name : Shihab Sami" << endl;
    cout << "Student ID : S3823710" << endl;
    cout << "Email : S3823710@student.rmit.edu.au" << endl;

    cout << endl;

    cout << "Name : Harrison Lorraway" << endl;
    cout << "Student ID : S3856166" << endl;
    cout << "Email : S3856166@student.rmit.edu.au" << endl;

    cout << endl;

    cout << "Name : Yeu Haw Teh" << endl;
    cout << "Student ID : S3813866" << endl;
    cout << "Email : S3813866@student.rmit.edu.au" << endl;

    cout << endl;

    cout << "Name : Andrew Nhan Trong Tran" << endl;
    cout << "Student ID : S3785952" << endl;
    cout << "Email : S3785952@student.rmit.edu.au" << endl;
    cout << "----------------------------------" << endl;

    mainMenu();
}
