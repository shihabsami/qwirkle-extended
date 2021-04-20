
#include "LinkedList.h"

#include <iostream>

#define EXIT_SUCCESS 0

int main(void) {
    // TODO: Implement Qwirkle!

    LinkedList* list = new LinkedList();
    Tile* tile1 = new Tile('R', 1);
    Tile* tile2 = new Tile('O', 2);
    Tile* tile3 = new Tile('Y', 3);
    Tile* tile4 = new Tile('G', 4);
    Tile* tile5 = new Tile('B', 5);
    Tile* tile6 = new Tile('P', 6);

    list->addFront(tile1);
    list->addBack(tile2);
    list->addFront(tile3);
    list->addBack(tile4);
    list->addFront(tile5);
    list->addBack(tile6);

    std::cout << *list << std::endl;

    list->removeFront();
    list->removeBack();

    std::cout << *list << std::endl;

    list->removeFront();
    list->removeBack();
    list->removeFront();
    list->removeBack();

    std::cout << *list << std::endl;
    delete list;

    return EXIT_SUCCESS;
}
