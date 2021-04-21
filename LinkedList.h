
#ifndef ASSIGN2_LINKEDLIST_H
#define ASSIGN2_LINKEDLIST_H

#include "Node.h"

#include <ostream>

class LinkedList {
public:
    LinkedList();
    ~LinkedList();
    void addFront(Tile* tile);
    void addBack(Tile* tile);
    void removeFront();
    void removeBack();
    void insert(Tile* tile, unsigned int index, bool replace);
    void remove(Tile* tile);
    void remove(unsigned int index);
    unsigned int size();
    Tile* at(unsigned int index);
    friend std::ostream& operator<<(std::ostream& os, const LinkedList& list);

private:
    Node* head;
    Node* tail;

    // TODO can use std::size_t from <cstring>
    unsigned int length;
};

#endif // ASSIGN2_LINKEDLIST_H
