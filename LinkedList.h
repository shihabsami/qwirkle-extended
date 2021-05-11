
#ifndef ASSIGN2_LINKEDLIST_H
#define ASSIGN2_LINKEDLIST_H

#include "Node.h"

class LinkedList {
public:
    LinkedList();
    ~LinkedList();

    void addFront(const shared_ptr<Tile>& tile);
    void addBack(const shared_ptr<Tile>& tile);
    void removeFront();
    void removeBack();
    void insert(const shared_ptr<Tile>& tile, unsigned int index, bool replace);
    void remove(const shared_ptr<Tile>& tile);
    void remove(unsigned int index);
    unsigned int size() const;
    shared_ptr<Tile> at(unsigned int index) const;
    bool contains(const Tile& tile) const;
    bool isEmpty();
    friend ostream& operator<<(ostream& os, const LinkedList& list);

private:
    unsigned int length;
    shared_ptr<Node> head;
    shared_ptr<Node> tail;
};

#endif // ASSIGN2_LINKEDLIST_H
