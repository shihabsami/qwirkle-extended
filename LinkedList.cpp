
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
    Node* toBeAdded = new Node(tile, nullptr, nullptr);
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
    Node* toBeAdded = new Node(tile, nullptr, nullptr);
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
    }
    --length;
}

void LinkedList::insert(Node* node, unsigned int index) {
    if (index >= length)
        throw std::out_of_range("invalid index for LinkedList::insert");

    Node* current = head;
    for (unsigned int i = 0; i < index; ++i)
        current = current->next;

    current->previous->next = node;
    current->previous = node;
}

void LinkedList::remove(Node* node) {
    // TODO discuss whether this removes original Node* or Node with same value
}

void LinkedList::remove(unsigned int index) {
    if (index >= length)
        throw std::out_of_range("invalid index for LinkedList::remove");

    Node* toBeDeleted = head;
    for (unsigned int i = 0; i < index; ++i)
        toBeDeleted = toBeDeleted->next;

    delete toBeDeleted;
}

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
