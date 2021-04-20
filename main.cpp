#include <iostream>


using std::cout;
using std::endl;
using std::cin;

void menu();
void selection();

int main(int argc, char** argv) {
    cout << "Welcome to Qwirkle!" << endl;
    cout << "-------------------" << endl;

    selection();   

}


void menu(){
    cout << "Menu" << endl;
    cout << "----" << endl;
    cout << "1. New Game" << endl;
    cout << "2. Load Game" << endl;
    cout << "3. Credits (Show student information)" << endl;
    cout << "4. Quit" << endl;
}

void selection(){
    menu();
    int option = 0;
    if(option == 1) {
        newGame();
    } else if(option == 2) {
        loadGame();
    } else if(option == 3) {
        credits();
    } else if(option == 4) {
        std::exit;
    }


}

void newGame(){

}

void loadGame(){

}

void credits(){
    
}
