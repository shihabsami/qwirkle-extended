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

    bool errorChecking = true;
    while (!cin.eof() && errorChecking) {
        prompt();
        string temp, operation, tile, keywordAT, pos; // D5
        getline(cin, temp);
        std::istringstream command(temp);

        command >> operation >> tile >> keywordAT >> pos;
        if (operation == "place") {
            if (tile.empty() || keywordAT.empty() || pos.empty()) {
                cout << "Invalid Command" << endl;
            } else {
                testingPurpose(operation, tile, keywordAT, pos);
                errorChecking = false;
            }
        } else if (operation == "replace") {
            if (tile.empty()) {
                cout << "Invalid Command" << endl;
            } else {
                testingPurpose(operation, tile, keywordAT, pos);
                errorChecking = false;
            }
        }
        else{
            cout << "Invalid Command" << endl;
        }
        cin.clear();
        //cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
}


bool IOHandler::testingPurpose(
    string operation, string tile, string keywordAT, string pos) {
    string gameFileName = tile;
    bool errorChecking = true;
    transform(operation.begin(), operation.end(), operation.begin(), ::tolower);
    transform(tile.begin(), tile.end(), tile.begin(), ::toupper);
    transform(keywordAT.begin(), keywordAT.end(), keywordAT.begin(), ::tolower);
    transform(pos.begin(), pos.end(), pos.begin(), ::toupper);

    if (operation == "place" && keywordAT == "at") {
        if (placeTile(tile, pos)) {
            errorChecking = false;
        } else {
            cout << "Invalid Tile or Tile Position" << endl;
        }
    } else if (operation.compare("replace") == 0) {
        if (replaceTile(tile)) {
            errorChecking = false;
        } else {
        }

    } else if (operation.compare("save") == 0) {
        gameFileName = gameFileName + ".save";
        std::ofstream file(gameFileName);
        file << GameManager::player1;
        file << GameManager::player1->getScore();
        file << "/*player1 tile from sam*/";
        file << GameManager::player2;
        file << GameManager::player2->getScore();
        file << "/*player2 tile from sam*/";
        file << GameManager::board;
        file << "/*tiles played* from sam/";
        file << GameManager::bag;
        file << GameManager::currentPlayer;
        file.close();
    } else {
        cout << "Not A Valid Command" << endl;
    }
    return errorChecking;
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
    cout << "Enter the filename from which load a game" << endl;
    prompt();
    cin >> filename;
    std::ifstream file(filename);

    // if (file.is_open())
    // cout << file.rdbuf();

    int count = 0;
    string text;
    try {
        while (getline(file, text)) {
            // cout << text << endl;
            if (count == 0 || count == 3 || count == 9) {
                // check name ASCII
                int ascii = 0;

                for (size_t i = 0; i < text.length(); i++) {
                    ascii = text[i];
                    if (ascii < ASCII_ALPHABET_BEGIN ||
                        ascii > ASCII_ALPHABET_END) {
                        throw std::invalid_argument(
                            "Name format is not part of ASCII text");
                    }
                }

                count++;
            } else if (count == 1 || count == 4) {
                // Check Interger
                int number = stoi(text);
                if (number < 0) {
                    throw std::invalid_argument(
                        "The number should be positive");
                }
                count++;
            } else if (count == 2 || count == 5 || count == 8) {
                // seperated with comma
                std::stringstream ss(text);
                while (ss.good()) {
                    string substr;
                    getline(ss, substr, ',');
                    if (substr.length() != 2) {
                        throw std::invalid_argument("Wrong Format");
                    }
                }
                count++;
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
                }
                count++;
            }
        }
        cout << "Qwirkle game successfully loaded" << endl;
        file.close();
        selection();

    } catch (const std::invalid_argument& e) {
        cerr << "The error is " << e.what() << endl;
    }
}

bool IOHandler::checkTile(const string& tile) {
    try {
        bool condition = false;
        bool boolLetter = false;
        bool boolNumber = false;
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
            cout << "Tile Not Valid" << endl;
        }

        return condition;
    } catch (const std::invalid_argument& e) {
        cout << "Tile Not Valid" << endl;
        return false;
    }
}

// Tile position checker
bool IOHandler::checkTilePosition(const string& position) {
    try {
        string appended = "";
        bool condition = false;
        bool boolLetter = false;
        bool boolNumber = false;

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
            cout << "Not A Valid Position" << endl;
        }
        return condition;
    } catch (const std::invalid_argument& e) {
        cout << "Not A Valid Position" << endl;
        return false;
    }
}

// what is the return type
bool IOHandler::placeTile(const string& tile, const string& position) {
    if (checkTile(tile) && checkTilePosition(position)) {
        string appended;
        Colour colour = tile.at(0);
        Shape shape = static_cast<int>(tile.at(1)) - ASCII_NUMERICAL_BEGIN;
        int row = 0;
        int col = 0;

        try {
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
        } catch (const std::invalid_argument& e) {
            cerr << "The error is " << e.what() << endl;
            return false;
        }
    }
    return true;
}

bool IOHandler::replaceTile(const string& tile) {
    if (checkTile(tile)) {
        Colour colour = tile.at(0);
        Shape shape = static_cast<int>(tile.at(1)) - ASCII_NUMERICAL_BEGIN;
        GameManager::replaceTile(colour, shape);
    }
    return true;
}

void IOHandler::notify(const string& message, State state) {
    if (state == PLACE_SUCCESS) {
        cout << message << endl;
    } else if (state == PLACE_FAILURE) {
        cout << message << endl;
    } else if (state == REPLACE_SUCCESS) {
        cout << message << endl;
    } else if (state == REPLACE_FAILURE) {
        cout << message << endl;
        playRound();
    } else if (state == QWIRKLE) {
        cout << message << endl;
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
        cout << "GoodBye" << endl;
        gameRunning = false;
        GameManager::reset();
    }
}

int IOHandler::quit() {
    cout << "Goodbye" << endl;
    return EXIT_SUCCESS;
}

void IOHandler::Test() {
    //    cout << checkTile("H4") << endl;
    //    cout << checkTile("4H") << endl;
    //    cout << checkTile("44") << endl;
    //    cout << checkTile("H44") << endl;
    //    cout << checkTile("~H44") << endl;
    //    cout << checkTile("$H44") << endl;
    //    cout << checkTile("asdasdH44") << endl;
    //    cout << checkTile("R32") << endl;
    //    cout << checkTile("G56") << endl;
    //    cout << checkTile("^4") << endl;
    //    cout << checkTile("-4") << endl;
    //    cout << checkTile("Y644") << endl;
    //    cout << checkTile("P344") << endl;

    //    cout << checkTile("G7") << endl;
    //
    //    cout << checkTile("R1") << endl;
    //    cout << checkTile("O2") << endl;
    //    cout << checkTile("Y3") << endl;
    //    cout << checkTile("G4") << endl;
    //    cout << checkTile("B5") << endl;
    //    cout << checkTile("P6") << endl;

    //    cout << checkTilePosition("A0") << endl;
    //    cout << checkTilePosition("d3") << endl;
    //    cout << checkTilePosition("--3") << endl;
    //    cout << checkTilePosition("+A3") << endl;
    //    cout << checkTilePosition("DD3") << endl;
    //    cout << checkTilePosition("0A3") << endl;
    //    cout << checkTilePosition("d333") << endl;
    //    cout << checkTilePosition("B-p8") << endl;
    //    cout << checkTilePosition("G*(") << endl;
    //    cout << checkTilePosition("D`") << endl;
    //    cout << checkTilePosition("111") << endl;
    //    cout << checkTilePosition("jkh") << endl;
    //    cout << checkTilePosition("gh") << endl;
    //    cout << checkTilePosition("sddda") << endl;

    //   cout << checkTilePosition("111") << endl;
    //    cout << checkTilePosition("D`") << endl;
    //    cout << checkTilePosition("D4");
    //    cout << checkTilePosition("E5");
    //    cout << checkTilePosition("F6");
    //    cout << checkTilePosition("G7");
    //    cout << checkTilePosition("H8");
    //    cout << checkTilePosition("I9");
    //    cout << checkTilePosition("J10");
    //    cout << checkTilePosition("K11");
    //    cout << checkTilePosition("L12");
    //    cout << checkTilePosition("M13");
    //    cout << checkTilePosition("N14");
    //    cout << checkTilePosition("O15");
    //    cout << checkTilePosition("P16");
    //    cout << checkTilePosition("Q17");
    //    cout << checkTilePosition("R18");
    //    cout << checkTilePosition("S19");
    //    cout << checkTilePosition("T20");
    //    cout << checkTilePosition("U21");
    //    cout << checkTilePosition("V22");
    //    cout << checkTilePosition("W23");
    //    cout << checkTilePosition("X24");
    //    cout << checkTilePosition("Y25");
    //    cout << checkTilePosition("Z25");
    // playRound();
}