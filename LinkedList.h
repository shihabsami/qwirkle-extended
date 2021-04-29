
#ifndef ASSIGN2_LINKEDLIST_H
#define ASSIGN2_LINKEDLIST_H

#include "Node.h"

#include <ostream>

using std::ostream;
using std::out_of_range;
using std::shared_ptr;
using std::make_shared;

class LinkedList {
public:
    LinkedList();

    void addFront(shared_ptr<Tile>& tile);
    void addBack(const shared_ptr<Tile>& tile);
    void removeFront();
    void removeBack();
    void insert(const shared_ptr<Tile>& tile, unsigned int index, bool replace);
    void remove(const shared_ptr<Tile>& tile);
    void remove(unsigned int index);
    unsigned int size() const;
    shared_ptr<Tile> at(unsigned int index);
    friend ostream& operator<<(ostream& os, const LinkedList& list);

private:
    shared_ptr<Node> head;
    shared_ptr<Node> tail;

    // TODO can use std::size_t from <cstring>
    unsigned int length;
};

#endif // ASSIGN2_LINKEDLIST_H
