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
    bool errorChecking = true;
    while (errorChecking) {
        // place D2 at C4

        // outer while loop only ends when endgame is called
        // inner while loop only ends when input and logic is gucci
        prompt();
        string operation, tile, keywordAT, pos; // D5
        cin >> operation >> tile >> keywordAT >> pos;
        string gameFileName = tile;

        transform(
            operation.begin(), operation.end(), operation.begin(), ::tolower);
        transform(tile.begin(), tile.end(), tile.begin(), ::toupper);
        transform(
            keywordAT.begin(), keywordAT.end(), keywordAT.begin(), ::tolower);
        transform(pos.begin(), pos.end(), pos.begin(), ::toupper);

        if (operation.compare("place") == 0) {
            if (placeTile(tile, pos)) {
                // right here

                errorChecking = false;

            } else {
                cout << "Invalid Tile or Tile Position" << endl;
            }

        } else if (operation.compare("replace") == 0) {
            errorChecking = false;

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
            cout << "not a valid command" << endl;
        }
        cin.clear();
        cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
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
                int number = std::stoi(text);
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
        bool boolchar = false;
        bool boolnum = false;
        string appended;
        if (tile.size() == 2) {
            char letter = tile.at(0);
            char num1 = tile.at(1);

            appended.append(1, num1);
            // convert string into int
            int combinedNumber = stoi(appended);
            for (unsigned int i = 0; i < COLOURS.size(); i++) {
                if (letter == COLOURS[i]) {
                    boolchar = true;
                }
            }
            for (unsigned int i = 0; i < SHAPES.size(); i++) {
                if (combinedNumber == SHAPES[i]) {
                    boolnum = true;
                }
            }
        } else {
            boolchar = false;
            boolnum = false;
        }
        if (boolchar && boolnum) {
            condition = true;
        } else {
            condition = false;
        }
        return condition;
    } catch (const std::invalid_argument& e) {
        return false;
    }
}

// Tile position checker
bool IOHandler::checkTilePosition(const string& position) {
    try {
        string appended = "";
        bool condition = false;
        bool boolLetter = false;
        bool boolNum = false;

        if (position.size() == 2) {
            char letter = position.at(0);
            char num1 = position.at(1);
            int asciiLetter = int(letter);
            if (asciiLetter >= 65 && asciiLetter <= 90) {
                boolLetter = true;
            } else {
                boolLetter = false;
            }
            appended.append(1, num1);
            stoi(appended);
            if (stoi(appended) >= 0 && stoi(appended) <= 9) {
                boolNum = true;
            } else {
                boolNum = false;
            }
        } else if (position.size() == 3) {
            // get the position
            char letter = position.at(0);
            char num1 = position.at(1);
            char num2 = position.at(2);

            int asciiLetter = int(letter);
            if (asciiLetter >= ASCII_ALPHABET_BEGIN &&
                asciiLetter <= ASCII_ALPHABET_END) {
                boolLetter = true;
            } else {
                boolLetter = false;
            }
            // append the 2 chars into string
            appended.append(1, num1);
            appended.append(1, num2);
            // convert string into int
            int combinedNumber = stoi(appended);
            if (combinedNumber > 25) {
                boolNum = false;
            } else {
                boolNum = true;
            }
        } else {
            boolLetter = false;
            boolNum = false;
        }
        if (boolNum && boolLetter) {
            condition = true;
        } else {
            condition = false;
        }
        return condition;
    } catch (const std::invalid_argument& e) {
        cerr << "The error is " << e.what() << endl;
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
            if (position.size() == 2) {
                char asciiLetter = position.at(0);
                int rowi = int(asciiLetter);
                row = rowi - 65;
                // blame jon if it doesn't work
                char coli = position.at(1);
                appended.append(1, coli);
                col = stoi(appended);

            } else if (position.size() == 3) {
                char asciiLetter = position.at(0);
                char num1 = position.at(1);
                char num2 = position.at(2);
                int rowi = int(asciiLetter);
                row = rowi - 65;
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
        //->tile
        // give to sam
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
