#include <algorithm>
#include <cctype>
#include <fstream>
#include <iostream>
#include <limits>
#include <sstream>
#include <string>

using std::cin;
using std::cout;
using std::endl;

void menu();
void printStart();
void selection();
void newGame();
bool StringCheck(std::string name);
void credits();
void loadGame();
bool checkTilePosition(std::string tilePosition);
bool checkTile(std::string tile);
void prompt();
bool placeTile(std::string tile, std::string pos);
bool replaceTile(std::string tile);
void gameStart();
void Test();
int quit();

int main(int argc, char** argv) { Test(); }

void printStart() {
    cout << "Welcome to Qwirkle!" << endl;
    cout << "-------------------" << endl;
    cout << " " << endl;
    selection();
}

void menu() {
    cout << "Menu" << endl;
    cout << "----" << endl;
    cout << "1. New Game" << endl;
    cout << "2. Load Game" << endl;
    cout << "3. Credits (Show student information)" << endl;
    cout << "4. Quit" << endl;
}

void selection() {
    menu();
    bool flag = true;
    while (!std::cin.eof() && flag) {
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
            std::cerr << e.what() << endl;
            cout << endl;
        }
    }
}

void newGame() {
    std::string player1Name;
    std::string player2Name;

    cout << "Starting a New Game" << endl;
    cout << endl;
    bool condition = true;

    while (condition) {
        try {
            cout << "Enter a name for player 1 (uppercase characters only)"
                 << endl;
            prompt();
            cin >> player1Name;
            if (StringCheck(player1Name)) {
                cout << endl;
                throw std::invalid_argument(
                    "Must enter a name in CAPS for Player 1 and name must not "
                    "contain numbers or symbols");
            }
            cout << "Enter a name for player 2 (uppercase characters only)"
                 << endl;
            prompt();
            cin >> player2Name;
            if (StringCheck(player2Name)) {
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
            std::cerr << e.what() << endl;
            cout << endl;
        }
    }
    cout << "Let's Play!" << endl;
    gameStart();
}

void gameStart() {

    // print player info
    // print grid
    // print player A' hand

    bool gameStarted = true;
    // sam method for game ended or not?
    //
    while (gameStarted) {
        // cout << //sams ( current player ) "its your turn"
        cout << " Score for /*sams ( current player )*/ :" /*sam method for
                                                              score*/
             << endl;
        cout << " Score for /*sams ( current player )*/ :" /*sam method for
                                                              score*/
             << endl;

        // sam method for the current grid

        cout << "Your hand is " << endl;
        // sam hand method

        bool errorChecking = true;
        while (errorChecking) {
            // place D2 at C4

            // outter while loop only ends when endgame is called
            // inner while loop only ends when input and logic is gucci
            prompt();
            std::string operation, tile, keywordAT, pos; // D5
            cin >> operation >> tile >> keywordAT >> pos;
            std::string gameFileName = tile;

            transform(operation.begin(), operation.end(), operation.begin(),
                      ::tolower);
            transform(tile.begin(), tile.end(), tile.begin(), ::toupper);
            transform(keywordAT.begin(), keywordAT.end(), keywordAT.begin(),
                      ::tolower);
            transform(pos.begin(), pos.end(), pos.begin(), ::toupper);

            if (operation.compare("place") == 0) {
                if (placeTile(tile, pos) == true) {
                    errorChecking = false;
                } else {
                    cout << "Invalid Tile or Tile Position" << endl;
                }

            } else if (operation.compare("replace") == 0) {
                if (replaceTile(tile) == true) {
                    errorChecking = false;
                } else {
                    cout << "Invalid Tile" << endl;
                }

            } else if (operation.compare("save") == 0) {
                gameFileName = gameFileName + ".save";
                std::ofstream file(gameFileName);
                file << "/*player1 name from sam*/";
                file << "/*player1 score from sam*/";
                file << "/*player1 tile from sam*/";
                file << "/*player2 name from sam*/";
                file << "/*player2 score from sam*/";
                file << "/*player2 tile from sam*/";
                file << "/*board state from sam*/";
                file << "/*tiles played* from sam/";
                file << "/*tiles left in bag* from sam/";
                file << "/*current player* from sam/";
                file.close();
            } else {
                cout << "not a valid command" << endl;
            }

            // help command  can be added here

            // place -> placeTile()
            // repalce -> replaceTile()
            cin.clear();
            cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }

        // sam method checking for if the game has ended or note
    }
}

// what is the return type
bool placeTile(std::string tile, std::string pos) {
    if (checkTile(tile) == true && checkTilePosition(pos) == true) {
        // can do or cannot do
        //->tile && ->pos
        // give to sam
    }
}

bool replaceTile(std::string tile) {
    if (checkTile(tile) == true) {
        //->tile
        // give to sam
    }
}

// to load game copy path of t1.save
void loadGame() {
    std::string filename;
    cout << "Enter the filename from which load a game" << endl;
    prompt();
    cin >> filename;
    std::ifstream file(filename);

    // if (file.is_open())
    // cout << file.rdbuf();

    int count = 0;
    std::string text;
    try {
        while (getline(file, text)) {
            // cout << text << endl;
            if (count == 0 || count == 3 || count == 9) {
                // check name ASCII
                int ascii = 0;

                for (size_t i = 0; i < text.length(); i++) {
                    ascii = text[i];
                    if (!(ascii >= 65) || !(ascii <= 90)) {
                        throw std::invalid_argument(
                            "Name format is not part of ASCII text");
                    }
                }

                count++;
            } else if (count == 1 || count == 4) {
                // Check Interger
                int number = std::stoi(text);
                if (!(number >= 0)) {
                    throw std::invalid_argument(
                        "The number should be positive");
                }
                count++;
            } else if (count == 2 || count == 5 || count == 8) {
                // seperated with comma
                std::stringstream ss(text);
                while (ss.good()) {
                    std::string substr;
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
                    std::string substr;
                    getline(ss, substr, ',');

                    int number = std::stoi(substr);
                    if (!(number >= 0) || (number > 26)) {
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
                    std::string substr;
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
        std::cerr << "The error is " << e.what() << endl;
    }
}

void credits() {
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

// Player Names have to be CAPITALIZED
bool StringCheck(std::string name) {

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

// Tile position checker
bool checkTilePosition(std::string tilePosition) {
    try {
        std::string appended;
        bool condition = false;
        bool boolLetter = false;
        bool boolNum = false;
        if (tilePosition.size() == 2) {
            char letter = tilePosition.at(0);
            char num1 = tilePosition.at(1);
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
        } else if (tilePosition.size() == 3) {
            // get the position
            char letter = tilePosition.at(0);
            char num1 = tilePosition.at(1);
            char num2 = tilePosition.at(2);

            int asciiLetter = int(letter);
            if (asciiLetter >= 65 && asciiLetter <= 90) {
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
        std::cout << condition;
        return condition;
    } catch (const std::invalid_argument& e) {
        std::cerr << "The error is " << e.what() << std::endl;
        return false;
    }
}

bool checkTile(std::string tile) {
    try {
        bool condition = false;
        bool boolchar = false;
        bool boolnum = false;
        std::string appended;
        if (tile.size() == 2) {
            char letter = tile.at(0);
            char num1 = tile.at(1);

            appended.append(1, num1);
            // convert string into int
            int combinedNumber = stoi(appended);

            if (letter == 'R' || letter == 'O' || letter == 'Y' ||
                letter == 'G' || letter == 'B' || letter == 'P') {
                boolchar = true;
            } else {
                boolchar = false;
            }

            if (combinedNumber > 7 && combinedNumber < 0) {
                boolnum = false;
            } else {
                boolnum = true;
            }
        } else {
            boolchar = false;
            boolnum = false;
        }
        if (boolchar == true && boolnum == true) {
            condition = true;
        } else {
            condition = false;
        }
        return condition;
    } catch (const std::invalid_argument& e) {
        return false;
    }
}

void prompt() { cout << "> "; }

int quit() {
    cout << "Goodbye" << endl;
    return EXIT_SUCCESS;
}

void Test() { checkTilePosition("32"); }
