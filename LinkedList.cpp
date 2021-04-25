
#include "LinkedList.h"

#include <exception>
#include <iostream>

LinkedList::LinkedList() : head(nullptr), tail(nullptr), length(0) {}

LinkedList::~LinkedList() {
    Node* current = head;
    Node* toBeDeleted = current;

    for (unsigned int i = 0; i < length; ++i) {
        toBeDeleted = toBeDeleted->next;
        delete current;
        current = toBeDeleted;
    }
}

void LinkedList::addFront(Tile* tile) {
    Node* toBeAdded = new Node(new Tile(*tile), nullptr, nullptr);
    if (length == 0) {
        head = toBeAdded;
        tail = toBeAdded;
    } else {
        head->previous = toBeAdded;
        toBeAdded->next = head;
        head = toBeAdded;
    }
    ++length;
}

void LinkedList::addBack(Tile* tile) {
    Node* toBeAdded = new Node(new Tile(*tile), nullptr, nullptr);
    if (length == 0) {
        head = toBeAdded;
        tail = toBeAdded;
    } else {
        tail->next = toBeAdded;
        toBeAdded->previous = tail;
        tail = toBeAdded;
    }
    ++length;
}

void LinkedList::removeFront() {
    if (length == 0) {
        delete head;
        tail = nullptr;
    } else {
        Node* toBeDeleted = head;
        head = head->next;
        delete toBeDeleted;
        toBeDeleted = nullptr;
    }
    --length;
}

void LinkedList::removeBack() {
    if (length == 0) {
        delete head;
        tail = nullptr;
    } else {
        Node* toBeDeleted = tail;
        tail = tail->previous;
        delete toBeDeleted;
        toBeDeleted = nullptr;
    }
    --length;
}

void LinkedList::insert(Tile* tile, unsigned int index, bool replace) {
    if (index >= length)
        throw std::out_of_range("invalid index for LinkedList::insert");

    Node* toBeInserted = new Node(new Tile(*tile), nullptr, nullptr);

    Node* current = head;
    for (unsigned int i = 0; i < index; ++i)
        current = current->next;

    if (replace) {
        /*
        when replacing, update new node's references to point to current node's
        next and previous nodes, also update the next and previous nodes'
        references to point to the new node
        */
        toBeInserted->next = current->next;
        toBeInserted->previous = current->previous;

        // if replacing head or tail, update references of head and tail
        if (index == 0) {
            current->next->previous = toBeInserted;
            head = toBeInserted;
        } else if (index == length - 1) {
            current->previous->next = toBeInserted;
            tail = toBeInserted;
        } else {
            current->next->previous = toBeInserted;
            current->previous->next = toBeInserted;
        }

        delete current;
    } else {
        if (index == 0) {
            toBeInserted->next = current;
            current->previous = toBeInserted;
            head = toBeInserted;
        } else if (index == length - 1) {
            toBeInserted->previous = current;
            current->next = toBeInserted;
            tail = toBeInserted;
        } else {
            toBeInserted->next = current;
            toBeInserted->previous = current->previous;
            current->previous->next = toBeInserted;
            current->previous = toBeInserted;
        }

        ++length;
    }
}

void LinkedList::remove(Tile* tile) {
    Node* current = head;
    for (unsigned int i = 0; i < length; ++i) {
        if (*current->tile == *tile)
            remove(i);

        current = current->next;
    }
}

void LinkedList::remove(unsigned int index) {
    if (index >= length)
        throw std::out_of_range("invalid index for LinkedList::remove");

    if (index == 0)
        removeFront();
    else if (index == length - 1)
        removeBack();
    else {
        Node* toBeDeleted = head;
        for (unsigned int i = 0; i < index; ++i)
            toBeDeleted = toBeDeleted->next;

        delete toBeDeleted;
        toBeDeleted = nullptr;
    }
}

unsigned int LinkedList::size() { return length; }

Tile* LinkedList::at(unsigned int index) {
    if (index >= length)
        throw std::out_of_range("invalid index for LinkedList::at");

    Node* toReturn = head;
    for (unsigned int i = 0; i < index; ++i)
        toReturn = toReturn->next;

    return toReturn->tile;
}

std::ostream& operator<<(std::ostream& os, const LinkedList& list) {
    Node* current = list.head;
    for (unsigned int i = 0; i < list.length; ++i) {
        os << *current->tile << (i < list.length - 1 ? ", " : "");
        current = current->next;
    }
    return os;
}
