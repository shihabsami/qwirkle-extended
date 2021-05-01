
#include "LinkedList.h"
#include "TileCodes.h"

#include <iostream>
#include <memory>

using std::cout;
using std::endl;
using std::unique_ptr;
using std::make_unique;
using std::shared_ptr;
using std::make_shared;

// tests for LinkedList implementation
void testLinkedList();

int main() {
    testLinkedList();

    return EXIT_SUCCESS;
}

void testLinkedList() {
    unique_ptr<LinkedList> list = make_unique<LinkedList>();

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

    std::cout << "removing tiles to linkedlist..." << endl;
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

    std::cout << "removing tiles by value..." << endl;
    list->remove(tile1);
    cout << *list << endl;

    std::cout << "removing tiles at index..." << endl;
    list->remove(2);
    cout << *list << endl;
}
