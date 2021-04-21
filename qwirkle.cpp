
#include "LinkedList.h"
#include "TileCodes.h"
#include <iostream>

using std::cout;
using std::endl;

// tests for LinkedList implementation
void testLinkedList();

int main(void) {
    testLinkedList();

    return EXIT_SUCCESS;
}

void testLinkedList() {
    LinkedList* list = new LinkedList();
    Tile* tile1 = new Tile(RED, CIRCLE);
    Tile* tile2 = new Tile(ORANGE, STAR_4);
    Tile* tile3 = new Tile(YELLOW, DIAMOND);
    Tile* tile4 = new Tile(GREEN, SQUARE);
    Tile* tile5 = new Tile(BLUE, STAR_6);
    Tile* tile6 = new Tile(PURPLE, CLOVER);

    cout << "adding tiles to linkedlist..." << endl;
    list->addFront(tile1);
    list->addBack(tile2);
    list->addFront(tile3);
    list->addBack(tile4);
    list->addFront(tile5);
    list->addBack(tile6);
    cout << *list << endl;

    std::cout << "removing tiles to linkedlist..." << endl;
    list->removeFront();
    list->removeBack();
    cout << *list << endl;

    cout << "testing size() and at()" << endl;
    cout << "size() - " << list->size() << endl;
    cout << "at(1) - " << *list->at(1) << endl;
    cout << "at(size() - 1) - " << *list->at(list->size() - 1) << endl;
    cout << *list << endl;

    Tile* tile7 = new Tile(RED, STAR_4);
    Tile* tile8 = new Tile(ORANGE, DIAMOND);

    cout << "inserting item at index..." << endl;
    list->insert(tile7, 0, false);
    cout << *list << endl;

    cout << "inserting and replacing item at index..." << endl;
    list->insert(tile8, 4, true);
    cout << *list << endl;

    delete list;
}
