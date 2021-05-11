#include <algorithm>
#include <cctype>
#include <fstream>
#include <iostream>
#include <limits>
#include <sstream>
#include <string>

#include "IOHandler.h"
#include "TileCodes.h"
#include "GameManager.h"


void printStart();
void menu();
void selection();
int quit();



using std::cin;
using std::cout;
using std::endl;

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
        //prompt();
        cin >> option;
        try {
            if (option == 1) {
                //newGame();
                flag = false;
            } else if (option == 2) {
                //loadGame();
                flag = false;
            } else if (option == 3) {
               // credits();
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

int quit() {
    cout << "Goodbye" << endl;
    exit(EXIT_SUCCESS);
}


int main(){
    selection();
    cout << "BLAH";
}
