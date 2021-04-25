#include <exception>
#include <iostream>

using std::cin;
using std::cout;
using std::endl;

void menu();
void selection();

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

    try {
        cout << "Enter a name for player 1 (uppercase characters only)" << endl;
        cin >> player1Name;

        cout << "Enter a name for player 2 (uppercase characters only)" << endl;
        cin >> player2Name;
    } catch (const) {
    }

    cout << "Let's Play!" << endl;
}
// void loadGame() {}

// void credits() {}
