#include "IOHandler.h"
#include "TileCodes.h"
#include "Constants.h"

#include <algorithm>
#include <cctype>
#include <fstream>
#include <iostream>
#include <limits>
#include <sstream>
#include <string>

using std::cin;
using std::cout;
using std::cerr;
using std::endl;

bool IOHandler::gameRunning = false;
bool IOHandler::takingInput = false;

void IOHandler::beginGame() {
    cout << "Welcome to Qwirkle!" << endl;
    cout << "-------------------" << endl;
    cout << " " << endl;
    selection();
}

void IOHandler::menu() {
    cout << "Menu" << endl;
    cout << "----" << endl;
    cout << "1. New Game" << endl;
    cout << "2. Load Game" << endl;
    cout << "3. Credits (Show student information)" << endl;
    cout << "4. Quit" << endl;
}

void IOHandler::prompt() { cout << "> "; }

void IOHandler::selection() {
    menu();
    bool flag = true;
    while (!cin.eof() && flag) {
        int option = 0;
        cout << " " << endl;
        prompt();
        cin >> option;
        try {
            if (option == 1) {
                newGame();
                flag = false;
            } else if (option == 2) {
                loadGame();
                flag = false;
            } else if (option == 3) {
                credits();
                flag = false;
            } else if (option == 4 && !cin.eof()) {
                quit();
                flag = false;
            } else if (cin.eof()) {
                quit();
            } else {
                cin.clear();
                cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                cout << " " << endl;
                throw std::runtime_error("Invalid Input");
            }
        } catch (const std::runtime_error& e) {
            cerr << e.what() << endl;
            cout << endl;
        }
    }
}

void IOHandler::newGame() {
    string player1Name;
    string player2Name;

    cout << "Starting a New Game" << endl;
    cout << endl;
    bool condition = true;

    while (condition) {
        try {
            cout << "Enter a name for player 1 (uppercase characters only)"
                 << endl;
            prompt();
            cin >> player1Name;
            if (cin.eof()) {
                quit();
            }
            if (validateName(player1Name)) {
                cout << endl;
                throw std::invalid_argument(
                    "Must enter a name in CAPS for Player 1 and name must not "
                    "contain numbers or symbols");
            }
            cout << "Enter a name for player 2 (uppercase characters only)"
                 << endl;
            prompt();
            cin >> player2Name;
            if (cin.eof()) {
                quit();
            }
            if (validateName(player2Name)) {
                cout << endl;
                throw std::invalid_argument(
                    "Must enter a name in CAPS for Player 2 and name must not "
                    "contain numbers or symbols");
            }
            int compare = player1Name.compare(player2Name);
            if (compare == 0) {
                throw std::invalid_argument(
                    "Player Names should not be the same");
            }
            condition = false;
        } catch (const std::invalid_argument& e) {
            cerr << e.what() << endl;
            cout << endl;
        }
        cin.clear();
        cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
    cout << "Let's Play!" << endl;
    GameManager::beginGame(player1Name, player2Name);
    gameRunning = true;
}

// Player Names have to be CAPITALIZED
bool IOHandler::validateName(const string& name) {

    bool state = false;
    int counter = 0;
    for (std::size_t i = 0; i < name.length(); i++) {
        char c = name[i];
        if (isupper(c) < 1) {
            counter++;
        }
        if (counter >= 1) {
            state = true;
        }
    }
    return state;
}

void IOHandler::playRound() {
    cout << endl;
    cout << GameManager::currentPlayer->getName() << ", it's your turn "
         << endl;
    cout << "Score for " << GameManager::player1->getName() << ": "
         << GameManager::player1->getScore() << endl;
    cout << "Score for " << GameManager::player2->getName() << ": "
         << GameManager::player2->getScore() << endl;
    cout << endl;
    cout << *GameManager::board << endl;
    cout << "Your hand is " << endl;
    cout << *GameManager::currentPlayer->getHand() << endl;

    takingInput = true;
    while (takingInput) {
        prompt();
        string temp, operation, tile, keywordAT, pos; // D5
        getline(cin, temp);
        std::istringstream command(temp);
        if (cin.eof()) {
            quit();
            cin.clear();
        }
        command >> operation >> tile >> keywordAT >> pos;

        transform(
            operation.begin(), operation.end(), operation.begin(), ::tolower);
        transform(tile.begin(), tile.end(), tile.begin(), ::toupper);
        transform(
            keywordAT.begin(), keywordAT.end(), keywordAT.begin(), ::tolower);
        transform(pos.begin(), pos.end(), pos.begin(), ::toupper);

        if (operation == "place") {
            if (tile.empty() || keywordAT.empty() || pos.empty()) {
                cout << "Invalid command" << endl;
            } else {
                takingInput = testingPurpose(operation, tile, keywordAT, pos);
            }
        } else if (operation == "replace") {
            if (tile.empty()) {
                cout << "Invalid command" << endl;
            } else {
                takingInput = testingPurpose(operation, tile, keywordAT, pos);
            }
        } else if (operation == "save") {
            if (tile.empty()) {
                cout << "Invalid command" << endl;
            } else {
                takingInput = testingPurpose(operation, tile, keywordAT, pos);
            }
        } else {
            cout << "Invalid command" << endl;
        }
    }
}

bool IOHandler::testingPurpose(
    string operation, string tile, string keywordAT, string pos) {
    string gameFileName = tile;

    if (operation == "place" && keywordAT == "at") {
        if (checkTile(tile) && checkTilePosition(pos)) {
            placeTile(tile, pos);
        } else {
            takingInput = true;
        }

    } else if (operation.compare("replace") == 0) {
        if (checkTile(tile)) {
            replaceTile(tile);
        } else {
            takingInput = true;
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
        file << BOARD_LENGTH << "," <<  BOARD_LENGTH << endl;
            file << "/*tiles played* from sam/" << endl;
            file << *GameManager::bag->getTiles() << endl;
            file << GameManager::currentPlayer->getName() << endl;
            file.close();
            takingInput = true;
        } else {
            cout << "Not A Valid Command" << endl;
        }
        return takingInput;
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
    selection();
}

// to load game copy path of t1.save
void IOHandler::loadGame() {
    string filename;
    cout << "Enter the filename from which to load a game." << endl;
    prompt();
    cin >> filename;
    std::ifstream file(filename);

    // Tiles add to back
    shared_ptr<PlayerHand> p1Hand = make_shared<PlayerHand>();
    shared_ptr<PlayerHand> p2Hand = make_shared<PlayerHand>();
    shared_ptr<Player> p1 = make_shared<Player>("Player1", p1Hand);
    shared_ptr<Player> p2 = make_shared<Player>("Player2", p2Hand);
    // getTiles().Add()
    shared_ptr<TileBag> tileBag = make_shared<TileBag>();
    shared_ptr<GameBoard> board = make_shared<GameBoard>();
    shared_ptr<Player> currentPlayer = nullptr;

    int count = 0;
    string text;
    try {
        while (getline(file, text)) {
            // checks if name is correct
            if (count == 0 || count == 3 || count == 9) {
                for (size_t i = 0; i < text.length() - 1; i++) {
                    // check name ASCII
                    int ascii = text[i];

                    if (ascii < ASCII_ALPHABET_BEGIN - 1 ||
                        ascii > ASCII_ALPHABET_END + 1) {
                        throw std::invalid_argument(
                            "Name format is not part of ASCII text");
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
                // checks if score is a number
            } else if (count == 1 || count == 4) {
                // Check integer
                int number = stoi(text);
                if (number < 0) {
                    throw std::invalid_argument(
                        "The number should be positive");
                }

                if (count == 1) {
                    p1->setScore(number);
                } else {
                    p2->setScore(number);
                }

                count++;
                // checks if tiles are separated by ','
            } else if (count == 2 || count == 5 || count == 8) {
                // separated with comma
                std::stringstream ss(text);
                while (ss.good()) {
                    string substr;
                    getline(ss, substr, ',');
                    // removing new lines
                    if (substr[0] != '\n' && substr[0] != '\r') {
                        if (substr.length() > 2) {
                            substr.erase(
                                std::remove(substr.begin(), substr.end(), '\r'),
                                substr.end());
                            substr.erase(
                                std::remove(substr.begin(), substr.end(), '\n'),
                                substr.end());
                        }

                        if (substr.length() != 2) {
                            throw std::invalid_argument(
                                "Wrong tile list format");
                        }

                        // Player 1 hand
                        if (count == 2) {
                            p1Hand->addTile(
                                make_shared<Tile>(substr[0], substr[1] - '0'));
                        } // Player 2 hand
                        else if (count == 5) {
                            p2Hand->addTile(
                                make_shared<Tile>(substr[0], substr[1] - '0'));
                        } // Tile Bag Tiles
                        else {
                            tileBag->getTiles()->addBack(
                                make_shared<Tile>(substr[0], substr[1] - '0'));
                        }
                    }
                }
                count++;
                // checks if board size is separated by ',' and not more than 26
            } else if (count == 6) {
                // height, width
                std::stringstream ss(text);
                while (ss.good()) {
                    string substr;
                    getline(ss, substr, ',');

                    int number = std::stoi(substr);
                    if (number < 0 || (number > 26)) {
                        throw std::invalid_argument(
                            "The grid should be more than 0 and less that "
                            "26");
                    }
                }
                // Board size does nothing at the moment, no loading
                count++;
            } else if (count == 7) {
                // all tiles placed on board
                std::stringstream ss(text);
                while (ss.good()) {
                    string substr;
                    getline(ss, substr, ' ');
                    const char at = '@';

                    if (substr[2] != at) {
                        throw std::invalid_argument(
                            "The board should appear as a list of "
                            "tile@postion");
                    }
                    char tile[2] = {substr[0], substr[1]};
                    char pos[3] = {substr[3], substr[4], substr[5]};

                    int row = pos[0] - ASCII_ALPHABET_BEGIN;
                    int column =
                        (pos[2] == 44 || pos[2] == '\r' || pos[2] == '\n')
                        ? pos[1] - '0'
                        : (int)(pos[1] - '0') * 10 + (int)(pos[2] - '0');

                    board->placeTile(
                        make_shared<Tile>(tile[0], tile[1] - '0'), row, column);
                }
                count++;
            }
        }
        cout << "Qwirkle game successfully loaded." << endl;
        file.close();
        GameManager::loadGame(p1, p2, tileBag, board, currentPlayer);
        // Testing prints
        /*std::cout << GameManager::player1->getName() << std::endl;
        std::cout << GameManager::player1->getScore() << std::endl;
        std::cout << *GameManager::player1->getHand() << std::endl;
        std::cout << GameManager::player2->getName() << std::endl;
        std::cout << GameManager::player2->getScore() << std::endl;
        std::cout << *GameManager::player2->getHand() << std::endl;

        std::cout << *GameManager::bag << std::endl;
        std::cout << GameManager::currentPlayer->getName() << std::endl;
        std::cout << *GameManager::board << std::endl;*/

        gameRunning = true;
        playRound();

    } catch (const std::invalid_argument& e) {
        cerr << "The error is " << e.what() << endl;
    }
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
            for (unsigned int i = 0; i < COLOURS.size(); i++) {
                if (letter == COLOURS[i]) {
                    boolLetter = true;
                }
            }
            for (unsigned int i = 0; i < SHAPES.size(); i++) {
                if (combinedNumber == SHAPES[i]) {
                    boolNumber = true;
                }
            }
        }
        if (boolLetter && boolNumber) {
            condition = true;
        } else {
            cout << "Not a valid tile." << endl;
        }
    } catch (const std::invalid_argument& e) {
        cout << "Not a valid tile." << endl;
        condition = false;
    }
    return condition;
}

// Tile position checker
bool IOHandler::checkTilePosition(const string& position) {
    string appended = "";
    bool condition = false;
    bool boolLetter = false;
    bool boolNumber = false;

    try {
        if (position.size() == STRING_SIZE_2) {
            char letter = position.at(FIRST_POSITION);
            char num1 = position.at(SECOND_POSITION);
            int asciiLetter = int(letter);
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
            // get the position
            char letter = position.at(FIRST_POSITION);
            char num1 = position.at(SECOND_POSITION);
            char num2 = position.at(THIRD_POSITION);

            int asciiLetter = int(letter);
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
            cout << "Not a valid position." << endl;
        }
        return condition;
    } catch (const std::invalid_argument& e) {
        cout << "Not a valid position." << endl;
        return false;
    }
}

// what is the return type
void IOHandler::placeTile(const string& tile, const string& position) {
    string appended;
    Colour colour = tile.at(0);
    Shape shape = static_cast<int>(tile.at(1)) - ASCII_NUMERICAL_BEGIN;
    int row = 0;
    int col = 0;

    if (position.size() == STRING_SIZE_2) {
        char asciiLetter = position.at(FIRST_POSITION);
        int rowi = int(asciiLetter);
        row = rowi - ASCII_ALPHABET_BEGIN;
        char coli = position.at(SECOND_POSITION);
        appended.append(1, coli);
        col = stoi(appended);

    } else if (position.size() == STRING_SIZE_3) {
        char asciiLetter = position.at(FIRST_POSITION);
        char num1 = position.at(SECOND_POSITION);
        char num2 = position.at(THIRD_POSITION);
        int rowi = int(asciiLetter);
        row = rowi - ASCII_ALPHABET_BEGIN;
        appended.append(1, num1);
        appended.append(1, num2);
        col = stoi(appended);
    }
    GameManager::placeTile(colour, shape, row, col);
}

void IOHandler::replaceTile(const string& tile) {
    Colour colour = tile.at(0);
    Shape shape = static_cast<int>(tile.at(1)) - ASCII_NUMERICAL_BEGIN;
    GameManager::replaceTile(colour, shape);
}

void IOHandler::notify(const string& message, State state) {
    if (state == PLACE_SUCCESS) {
        cout << message << endl;
        takingInput = false;
    } else if (state == PLACE_FAILURE) {
        cout << message << endl;
    } else if (state == REPLACE_SUCCESS) {
        cout << message << endl;
        takingInput = false;
    } else if (state == REPLACE_FAILURE) {
        cout << message << endl;
    } else if (state == QWIRKLE) {
        cout << message << endl;
        takingInput = false;
    } else if (state == GAME_OVER) {
        cout << message << endl;
        cout << "Game Over" << endl;
        cout << "Score for " << GameManager::player1 << " : "
             << GameManager::player1->getScore() << endl;
        cout << "Score for " << GameManager::player2 << " : "
             << GameManager::player2->getScore() << endl;
        if (GameManager::player1->getScore() >
            GameManager::player2->getScore()) {
            cout << "Player " << GameManager::player1 << " won!" << endl;
        } else {
            cout << "Player " << GameManager::player2 << " won!" << endl;
        }
        cout << "Goodbye" << endl;
        takingInput = false;
        gameRunning = false;
        GameManager::resetGame();
    }
}

void IOHandler::quit() {
    cout << "Goodbye" << endl;
    exit(EXIT_SUCCESS);
}
