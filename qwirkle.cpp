
#include "LinkedList.h"
#include "TileCodes.h"
#include "GameBoard.h"

#include <iostream>

using std::cout;
using std::endl;

// tests for LinkedList implementation
void testLinkedList();

void testGameBoard();

int main() {
    // testLinkedList();

    testGameBoard();

    return EXIT_SUCCESS;
}

void testLinkedList() {
    shared_ptr<LinkedList> list = make_shared<LinkedList>();

    shared_ptr<Tile> tile1 = make_shared<Tile>(RED, CIRCLE);
    shared_ptr<Tile> tile2 = make_shared<Tile>(ORANGE, STAR_4);
    shared_ptr<Tile> tile3 = make_shared<Tile>(YELLOW, DIAMOND);
    shared_ptr<Tile> tile4 = make_shared<Tile>(GREEN, SQUARE);
    shared_ptr<Tile> tile5 = make_shared<Tile>(BLUE, STAR_6);
    shared_ptr<Tile> tile6 = make_shared<Tile>(PURPLE, CLOVER);

    cout << "adding tiles to linkedlist..." << endl;
    list->addFront(tile1);
    list->addBack(tile2);
    list->addFront(tile3);

    list->addBack(tile4);
    list->addFront(tile5);
    list->addBack(tile6);
    cout << *list << endl;

    cout << "removing tiles to linkedlist..." << endl;
    list->removeFront();
    list->removeBack();
    cout << *list << endl;

    cout << "testing size() and at()" << endl;
    cout << "size() - " << list->size() << endl;
    cout << "at(1) - " << *list->at(1) << endl;
    cout << "at(size() - 1) - " << *list->at(list->size() - 1) << endl;
    cout << *list << endl;

    shared_ptr<Tile> tile7 = make_shared<Tile>(RED, STAR_4);
    shared_ptr<Tile> tile8 = make_shared<Tile>(ORANGE, DIAMOND);

    cout << "inserting item at index..." << endl;
    list->insert(tile7, 0, false);
    cout << *list << endl;

    cout << "inserting and replacing item at index..." << endl;
    list->insert(tile8, 4, true);
    cout << *list << endl;

    cout << "removing tiles by value..." << endl;
    list->remove(tile1);
    cout << *list << endl;

    cout << "removing tiles at index..." << endl;
    list->remove(2);
    cout << *list << endl;

    cout << "testing if linkedlist contains tile..." << endl;
    cout << "contains " << *tile7 << " - " << (list->contains(*tile7) ? "true" : "false") << endl;
    cout << "contains " << *tile4 << " - " << (list->contains(*tile4) ? "true" : "false") << endl;
}

void testGameBoard() {
    cout << "testing gameboard..." << endl;
    shared_ptr<GameBoard> board = make_shared<GameBoard>();

    shared_ptr<Tile> tile1 = make_shared<Tile>(RED, CIRCLE);
    shared_ptr<Tile> tile2 = make_shared<Tile>(ORANGE, STAR_4);
    shared_ptr<Tile> tile3 = make_shared<Tile>(YELLOW, DIAMOND);
    shared_ptr<Tile> tile4 = make_shared<Tile>(GREEN, SQUARE);

    cout << "placing tiles on gameboard..." << endl;
    board->placeTile(tile1, 5, 10);
    board->placeTile(tile2, 10, 5);
    board->placeTile(tile3, 15, 20);
    board->placeTile(tile4, 20, 10);
    cout << *board << endl;

}