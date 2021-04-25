#include <cstring>
#include <exception>
#include <iostream>
#include <locale>
#include <string>

using std::cin;
using std::cout;
using std::endl;

void menu();
void selection();
void newGame();
bool StringCheck(std::string name);

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
            // loadGame();
        } else if (option == 3) {
            // credits();
        } else if (option == 4) {
            // std::exit;
        } else {
            cout << " " << endl;
            throw std::runtime_error("You can only enter numbers 1 - 4");
        }
    } catch (const std::runtime_error& e) {
        std::cerr << e.what() << " Please Try Again!" << std::endl;
        cout << endl;
        selection();
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
                    "Must enter a name in CAPS for player 1");
            }
            cout << "Enter a name for player 2 (uppercase characters only)"
                 << endl;
            cin >> player2Name;
            if (StringCheck(player2Name)) {
                cout << endl;
                throw std::invalid_argument(
                    "Must enter a name in CAPS for player 2");
            }
            condition = false;
        } catch (const std::invalid_argument& e) {
            std::cerr << e.what() << endl;
            cout << endl;
        }
    }
    cout << "Let's Play!" << endl;
}
// void loadGame() {}

// void credits() {}
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
