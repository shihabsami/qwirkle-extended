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
using std::istringstream;

typedef std::numeric_limits<std::streamsize> CharLimit;

bool IOHandler::gameRunning = false;
bool IOHandler::takingInput = false;
bool IOHandler::aiEnabled = false;
bool IOHandler::helpEnabled = false;
bool IOHandler::invalidInputEnabled = false;
bool IOHandler::colourEnabled = false;
bool IOHandler::hintEnabled = false;
int IOHandler::numberOfPlayers = 2;

void IOHandler::beginGame() {
    cout << "Welcome to Qwirkle!" << endl;
    cout << "-------------------" << endl << endl;

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
    string player1Name;
    string player2Name = "AI";

    cout << "Starting a New Game" << endl << endl;
    bool nameCheck1 = true;
    bool nameCheck2 = !aiEnabled;
    string message = "Must enter a name in CAPS for Player and name must "
                     "not contain numbers or symbols or duplicate names";

    while (!cin.eof() && nameCheck1) {
        cout << "Enter a name for player 1 (uppercase characters only)" << endl;
        prompt();
        cin >> player1Name;

        if (!validateName(player1Name))
            cout << message << endl;
        else
            nameCheck1 = false;
    }

    while (!cin.eof() && nameCheck2) {
        cout << "Enter a name for player 2 (uppercase characters only)" << endl;
        prompt();
        cin >> player2Name;

        // if name is duplicate, retry
        if (!validateName(player2Name) || player1Name == player2Name) {
            cout << message << endl;
            cin.clear();
            cin.ignore(CharLimit::max(), '\n');
        } else {
            nameCheck2 = false;
        }
    }

    if (cin.eof())
        quit();

    cin.clear();
    cin.ignore(CharLimit::max(), '\n');
    cout << "Let's Play!" << endl;
    GameManager::beginGame(player1Name, player2Name);
    gameRunning = true;
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

        transform(
            operation.begin(), operation.end(), operation.begin(), ::tolower);
        transform(tile.begin(), tile.end(), tile.begin(), ::toupper);
        transform(
            keywordAt.begin(), keywordAt.end(), keywordAt.begin(), ::tolower);
        transform(
            position.begin(), position.end(), position.begin(), ::toupper);

        if (operation == "save") {
            takingInput =
                logicHandler(operation, saveName, keywordAt, position);
        } else {
            takingInput = logicHandler(operation, tile, keywordAt, position);
        }
    }
}

bool IOHandler::logicHandler(const string& operation, const string& tile,
    const string& keywordAt, const string& position) {
    string gameFileName = tile;

    if (operation == "place" && keywordAt == "at") {
        if (checkTile(tile) && checkTilePosition(position)) {
            placeTileOperation(tile, position);
        } else {
            takingInput = true;
        }

    } else if (operation == "replace" && keywordAt.empty() &&
        position.empty()) {
        if (checkTile(tile)) {
            replaceTileOperation(tile);
        } else {
            takingInput = true;
        }

    } else if (operation == "hint") {
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
            cout << "Your only possible move is to replace a tile" << endl;
        }
    } else if (operation == "save") {
        gameFileName = gameFileName + ".save";
        std::ofstream file(gameFileName);
        file << GameManager::player1->getName() << endl;
        file << GameManager::player1->getScore() << endl;
        file << *GameManager::player1->getHand()->getTiles() << endl;
        file << GameManager::player2->getName() << endl;
        file << GameManager::player2->getScore() << endl;
        file << *GameManager::player2->getHand()->getTiles() << endl;
        file << BOARD_LENGTH << "," << BOARD_LENGTH << endl;
        file << *GameManager::board << endl;
        file << *GameManager::bag->getTiles() << endl;
        file << GameManager::currentPlayerIndex->getName() << endl;
        file.close();
        cout << endl;
        cout << "Game successfully ," << endl;
        cout << endl;
        takingInput = true;
    } else if (operation == "help" && helpEnabled) {
        help(GAME_ROUND);
    } else if (operation == "quit") {
        quit();
        takingInput = false;
    } else {
        cout << ERROR_MESSAGE << "Not a valid command." << endl;
    }
    return takingInput;
}

void IOHandler::loadGame() {
    bool fileCheck = true;
    while (fileCheck) {
        try {
            string filename;
            cout << "Enter the filename from which to load a game." << endl;
            prompt();
            cin >> filename;

            string response = filename;
            transform(response.begin(), response.end(), response.begin(),
                [](unsigned char c) { return std::tolower(c); });

            if (response == "help") {
                help(LOAD_GAME);
                throw std::invalid_argument("");
            }

            std::ifstream file(filename);

            if (cin.eof()) {
                quit();
            }

            if (!file) {
                throw ::std::invalid_argument("File does not exist.");
            }

            if (isEmpty(file)) {
                throw ::std::invalid_argument("File is empty.");
            }

            cin.clear();
            cin.ignore();

            shared_ptr<PlayerHand> p1Hand = make_shared<PlayerHand>();
            shared_ptr<PlayerHand> p2Hand = make_shared<PlayerHand>();
            shared_ptr<Player> p1 = make_shared<Player>("Player1", p1Hand);
            shared_ptr<Player> p2 = make_shared<Player>("Player2", p2Hand);
            shared_ptr<TileBag> tileBag = make_shared<TileBag>();
            shared_ptr<GameBoard> board = make_shared<GameBoard>();
            shared_ptr<Player> currentPlayer = nullptr;

            int count = 0;
            string text;
            while (getline(file, text)) {
                text.erase(
                    std::remove(text.begin(), text.end(), '\r'), text.end());
                text.erase(
                    std::remove(text.begin(), text.end(), '\n'), text.end());
                if (count == 0 || count == 3 || count == 9) {
                    for (unsigned i = 0; i < text.length() - 1; i++) {
                        int ascii = static_cast<unsigned char>(text[i]);
                        if (ascii < ASCII_ALPHABET_BEGIN - 1 ||
                            ascii > ASCII_ALPHABET_END + 1 || ascii == 0) {
                            throw std::invalid_argument(
                                "Name format is not part of ASCII text.");
                        }
                    }

                    if (count == 0) {
                        p1->setName(text);
                    } else if (count == 3) {
                        p2->setName(text);
                    } else {
                        if (p1->getName() == text) {
                            currentPlayer = p1;
                        } else {
                            currentPlayer = p2;
                        }
                    }

                    count++;
                } else if (count == 1 || count == 4) {
                    int number = stoi(text);
                    if (number < 0) {
                        throw std::invalid_argument(
                            "Player score should be positive.");
                    }

                    if (count == 1) {
                        p1->setScore(number);
                    } else {
                        p2->setScore(number);
                    }

                    count++;
                } else if (count == 2 || count == 5 || count == 8) {
                    std::stringstream ss(text);
                    string c = "\0";
                    while (ss.good()) {
                        string substr;
                        getline(ss, substr, ',');
                        if (text != c) {
                            if (substr.length() != 2) {
                                throw std::invalid_argument(
                                    "Wrong tile list format.");
                            }
                        }
                        if (count == 2) {
                            p1Hand->addTile(
                                make_shared<Tile>(substr[0], substr[1] - '0'));
                        } else if (count == 5) {
                            p2Hand->addTile(
                                make_shared<Tile>(substr[0], substr[1] - '0'));
                        } else {
                            if (!substr.empty()) {
                                tileBag->getTiles()->addBack(make_shared<Tile>(
                                    substr[0], substr[1] - '0'));
                            }
                        }
                    }
                    count++;
                } else if (count == 6) {
                    std::stringstream ss(text);
                    while (ss.good()) {
                        string substr;
                        getline(ss, substr, ',');
                        int number = std::stoi(substr);
                        if (number < 0 || number > 26) {
                            throw std::invalid_argument(
                                "The grid should be more than 0 and less than "
                                "26.");
                        }
                    }
                    count++;
                } else if (count == 7) {
                    std::stringstream ss(text);
                    string c = "\0";
                    while (ss.good()) {
                        string substr;
                        getline(ss, substr, ' ');
                        const char at = '@';
                        if (substr != c) {
                            if (substr[2] != at) {
                                throw std::invalid_argument(
                                    "The board should appear as a list of "
                                    "tile@postion.");
                            }
                        }
                        char last = substr.length() < 6 ? ',' : substr[5];
                        char tile[2] = {substr[0], substr[1]};
                        char pos[3] = {substr[3], substr[4], last};

                        int row = pos[0] - ASCII_ALPHABET_BEGIN;
                        int column =
                            (pos[2] == 44 || pos[2] == '\r' || pos[2] == '\n')
                            ? pos[1] - '0'
                            : (int)(pos[1] - '0') * 10 + (int)(pos[2] - '0');
                        if (!substr.empty()) {
                            board->placeTile(
                                make_shared<Tile>(tile[0], tile[1] - '0'), row,
                                column);
                        }
                    }
                    count++;
                }
            }
            cout << "Qwirkle game successfully loaded." << endl;
            file.close();
            GameManager::loadGame(p1, p2, tileBag, board, currentPlayer);
            gameRunning = true;
            fileCheck = false;

        } catch (const std::invalid_argument& e) {
            cerr << e.what() << endl;
        }
    }
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

    if (aiEnabled && *GameManager::currentPlayerIndex == *GameManager::player2) {
        cout << "It's AI's turn." << endl;
    } else {
        cout << GameManager::currentPlayerIndex->getName() << ", it's your turn "
             << endl;
    }

    cout << "Score for " << GameManager::player1->getName() << ": "
         << GameManager::player1->getScore() << endl;
    cout << "Score for " << GameManager::player2->getName() << ": "
         << GameManager::player2->getScore() << endl;
    cout << endl;
    GameManager::board->print(cout, colourEnabled);
    cout << endl;
    cout << (aiEnabled && *GameManager::currentPlayerIndex == *GameManager::player2
                    ? "AI's"
                    : "Your")
         << "hand is " << endl;
    GameManager::currentPlayerIndex->getHand()->print(cout, colourEnabled);
    cout << endl;
}

void IOHandler::notify(const string& message, State state) {
    if (state == PLACE_SUCCESS || state == REPLACE_SUCCESS ||
        state == QWIRKLE) {
        cout << message << endl;
        takingInput = false;
    } else if (state == PLACE_FAILURE || state == REPLACE_FAILURE) {
        cout << ERROR_MESSAGE << message << endl;
    } else if (state == GAME_OVER) {
        cout << endl;
        GameManager::board->print(cout, colourEnabled);
        cout << endl;
        cout << "Game Over" << endl;
        cout << "Score for " << GameManager::player1->getName() << " : "
             << GameManager::player1->getScore() << endl;
        cout << "Score for " << GameManager::player2->getName() << " : "
             << GameManager::player2->getScore() << endl;

        if (GameManager::player1->getScore() >
            GameManager::player2->getScore()) {
            cout << "Player " << GameManager::player1->getName() << " won!"
                 << endl;
        } else if (GameManager::player1->getScore() ==
            GameManager::player2->getScore()) {
            cout << "Game is drawn between " << GameManager::player1->getName()
                 << " and " << GameManager::player2->getName() << endl;
        } else {
            cout << (!aiEnabled ? "Player" : "")
                 << GameManager::player2->getName() << " won!" << endl;
        }

        cout << "Goodbye" << endl;
        takingInput = false;
        gameRunning = false;
    }
}

void IOHandler::quit() {
    GameManager::resetGame();
    cout << "Goodbye" << endl;
    exit(EXIT_SUCCESS);
}

void IOHandler::help(HelpLocation location) {
    if (location == MAIN_MENU) {
        cout << "MAIN_MENU" << endl;
    } else if (location == NEW_GAME) {
        cout << "NEW_GAME" << endl;
    } else if (location == LOAD_GAME) {
        cout << "LOAD_GAME" << endl;
    } else if (location == GAME_ROUND) {
        cout << "GAME_ROUND" << endl;
    } else if (location == SETTINGS_MENU) {
        cout << "SETTINGS_MENU" << endl;
    }
}

void IOHandler::printMenu() {
    cout << "Menu" << endl;
    cout << "----" << endl;
    cout << "1. New Game" << endl;
    cout << "2. Load Game" << endl;
    cout << "3. Settings" << endl;
    cout << "4. Credits (Show student information)" << endl;
    cout << "5. Quit" << endl;
}

void IOHandler::menuSelection() {
    int option = getSelection(5, MAIN_MENU);

    if (option == 1)
        newGame();
    else if (option == 2)
        loadGame();
    else if (option == 3)
        settingsMenu();
    else if (option == 4)
        credits();
    else if (option == 5 && !cin.eof())
        quit();
}

void IOHandler::printSettings() {
    cout << "Settings" << endl;
    cout << "--------" << endl;
    cout << "1. Help!" << endl;
    cout << "2. Better invalid input" << endl;
    cout << "3. Colour" << endl;
    cout << "4. Hint" << endl;
    cout << "5. 3-4 player modes" << endl;
    cout << "6. Return to menu" << endl;
}

void IOHandler::settingsSelection() {
    int option = getSelection(6, SETTINGS_MENU);

    if (option <= 5) {
        bool enabled = false;
        if (option == 1) {
            helpEnabled = getConfirmation(SETTINGS_CONFIRMATION);
            enabled = helpEnabled;
        } else if (option == 2) {
            invalidInputEnabled = getConfirmation(SETTINGS_CONFIRMATION);
            enabled = invalidInputEnabled;
        } else if (option == 3) {
            colourEnabled = getConfirmation(SETTINGS_CONFIRMATION);
            enabled = colourEnabled;
        } else if (option == 4) {
            hintEnabled = getConfirmation(SETTINGS_CONFIRMATION);
            enabled = hintEnabled;
        } else if (option == 5) {
            cout << "Select the number of players" << endl;
            cout << "----------------------------" << endl;
            cout << "1. 2 player" << endl;
            cout << "2. 3 player" << endl;
            cout << "3. 4 player" << endl;

            numberOfPlayers= getSelection(3, PLAYER_MODE);
            cout << numberOfPlayers << " player mode enabled successfully." << endl;
        }

        cout << (enabled ? "Enabled" : "Disabled") << " successfully" << endl;
        settingsMenu();
    } else if (option == 6) {
        mainMenu();
    }
}

void IOHandler::prompt() { cout << "> "; }

int IOHandler::getSelection(int range, HelpLocation location) {
    bool selecting = true;
    string option;
    int optionNumeric;
    while (!cin.eof() && selecting) {
        cout << endl;
        prompt();
        cin >> option;
        transform(option.begin(), option.end(), option.begin(),
            [](unsigned char c) { return std::tolower(c); });

        if (helpEnabled && option == "help") {
            help(location);
        } else {
            try {
                optionNumeric = std::stoi(option);
                if (optionNumeric >= 1 && optionNumeric <= range) {
                    selecting = false;
                } else if (cin.eof()) {
                    quit();
                } else {
                    cin.clear();
                    cin.ignore(CharLimit::max(), '\n');
                    cout << endl;
                    throw std::runtime_error("Invalid input.");
                }
            } catch (const std::runtime_error& e) {
                cerr << e.what() << endl << endl;
            } catch (const std::invalid_argument& e) {
                cerr << "Invalid input." << endl;
            }
        }
    }

    return optionNumeric;
}

bool IOHandler::getConfirmation(HelpLocation location) {
    cout << "Enable? (Y/N)" << endl;

    bool response;
    bool selecting = true;
    string responseString;
    while (!cin.eof() && selecting) {
        cout << endl;
        prompt();
        cin >> responseString;
        string responseStringTransformed = responseString;
        transform(responseStringTransformed.begin(),
            responseStringTransformed.end(), responseStringTransformed.begin(),
            [](unsigned char c) { return std::tolower(c); });

        if (helpEnabled && responseStringTransformed == "help") {
            help(location);
        } else {
            try {
                if (responseString.length() == 1) {
                    if (std::toupper(responseString[0]) == 'Y') {
                        response = true;
                        selecting = false;
                    } else if (std::toupper(responseString[0]) == 'N') {
                        response = false;
                        selecting = false;
                    }
                } else if (cin.eof()) {
                    quit();
                } else {
                    cin.clear();
                    cin.ignore(CharLimit::max(), '\n');
                    cout << endl;
                    throw std::runtime_error("Invalid input.");
                }
            } catch (const std::runtime_error& e) {
                cerr << e.what() << endl;
                cout << endl;
            }
        }
    }

    return response;
}

bool IOHandler::validateName(const string& name) {
    bool valid = true;
    for (char c : name)
        valid &= isupper(c) != 0;

    return valid;
}

bool IOHandler::checkTile(const string& tile) {
    bool condition = false;
    bool boolLetter = false;
    bool boolNumber = false;
    try {
        string appended;
        if (tile.size() == STRING_SIZE_2) {
            char letter = tile.at(0);
            char num1 = tile.at(1);

            appended.append(1, num1);
            int combinedNumber = stoi(appended);
            for (char i : COLOURS) {
                if (letter == i) {
                    boolLetter = true;
                }
            }
            for (int i : SHAPES) {
                if (combinedNumber == i) {
                    boolNumber = true;
                }
            }
        }
        if (boolLetter && boolNumber) {
            condition = true;
        } else {
            cout << ERROR_MESSAGE "Not a valid tile." << endl;
        }
    } catch (const std::invalid_argument& e) {
        cout << ERROR_MESSAGE "Not a valid tile." << endl;
        condition = false;
    }
    return condition;
}

bool IOHandler::checkTilePosition(const string& position) {
    string appended;
    bool condition = false, boolLetter = false, boolNumber = false;

    try {
        if (position.size() == STRING_SIZE_2) {
            char letter = position.at(FIRST_POSITION);
            char num1 = position.at(SECOND_POSITION);
            int asciiLetter = static_cast<unsigned char>(int(letter));
            if (asciiLetter >= ASCII_ALPHABET_BEGIN &&
                asciiLetter <= ASCII_ALPHABET_END) {
                boolLetter = true;
            } else {
                boolLetter = false;
            }
            appended.append(1, num1);
            if (stoi(appended) <= MAX_TILE_RANGE) {
                boolNumber = true;
            } else {
                boolNumber = false;
            }
        } else if (position.size() == STRING_SIZE_3) {
            char letter = position.at(FIRST_POSITION);
            char num1 = position.at(SECOND_POSITION);
            char num2 = position.at(THIRD_POSITION);

            int asciiLetter = static_cast<unsigned char>(int(letter));
            if (asciiLetter >= ASCII_ALPHABET_BEGIN &&
                asciiLetter <= ASCII_ALPHABET_END) {
                boolLetter = true;
            } else {
                boolLetter = false;
            }
            appended.append(1, num1);
            appended.append(1, num2);
            int combinedNumber = stoi(appended);
            if (combinedNumber > MAX_BOARD_INDEX ||
                combinedNumber < MIN_TILE_RANGE) {
                boolNumber = false;
            } else {
                boolNumber = true;
            }
        }
        if (boolNumber && boolLetter) {
            condition = true;
        } else {
            cout << ERROR_MESSAGE "Not a valid position." << endl;
        }
        return condition;
    } catch (const std::invalid_argument& e) {
        cout << ERROR_MESSAGE "Not a valid position." << endl;
        return false;
    }
}

bool IOHandler::isEmpty(ifstream& file) {
    return file.peek() == ifstream::traits_type::eof();
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
