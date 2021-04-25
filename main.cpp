#include <cctype>
#include <exception>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

using std::cin;
using std::cout;
using std::endl;

void menu();
void selection();
void newGame();
bool StringCheck(std::string name);
void credits();
void loadGame();
int quit();

int main(int argc, char** argv) {

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
    int option = 0;
    cout << " " << endl;
    cin >> option;
    try {
        if (option == 1) {
            newGame();
        } else if (option == 2) {
            loadGame();
        } else if (option == 3) {
            credits();
        } else if (option == 4) {
            quit();
        } else {
            cout << " " << endl;
            throw std::runtime_error("You can only enter numbers 1 - 4");
        }
    } catch (const std::runtime_error& e) {
        std::cerr << e.what() << " Please Try Again!" << std::endl;
        cout << endl;
        quit();
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
            cin >> player1Name;
            if (StringCheck(player1Name)) {
                cout << endl;
                throw std::invalid_argument(
                    "Must enter a name in CAPS for Player 1 and name must not "
                    "contain numbers or symbols");
            }
            cout << "Enter a name for player 2 (uppercase characters only)"
                 << endl;
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
    selection();
}
void loadGame() {
    std::string filename;
    cout << "Enter the filename from which load a game" << endl;
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
                            "The grid should be more than 0 and less that 26");
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

int quit() {
    cout << "Goodbye" << endl;
    return EXIT_SUCCESS;
}
