
#ifndef ASSIGN2_LINKEDLIST_H
#define ASSIGN2_LINKEDLIST_H

#include "Node.h"

class LinkedList {
public:
    /**
     * Construct a LinkedList with default head and tail references.
     */
    LinkedList();

    /**
     * Destructor to free up all Node elements.
     */
    ~LinkedList();

    /**
     * Add an element to the front.
     *
     * @param tile - a pointer to the tile to be added
     */
    void addFront(const shared_ptr<Tile>& tile);

    /**
     * Add an element to the back.
     *
     * @param tile - a pointer to the tile to be added
     */
    void addBack(const shared_ptr<Tile>& tile);

    /**
     * Remove an element from the front.
     */
    void removeFront();

    /**
     * Remove an element from the back.
     */
    void removeBack();

    /**
     * Insert an element at a given index.
     *
     * @param tile - a pointer to the tile to be inserted
     * @param index - the index to insert the tile at
     * @param replace - whether the tile should replace the existing tile
     */
    void insert(const shared_ptr<Tile>& tile, unsigned int index, bool replace);

    /**
     * Remove an element with based on tile value.
     *
     * @param tile - the tile to be removed
     */
    void remove(const shared_ptr<Tile>& tile);

    /**
     * Remove an element at a given index.
     *
     * @param index - the index to remove the tile at
     * */
    void remove(unsigned int index);

    /**
     * Get the size of the linkedlist.
     *
     * @return an integer representing the count of elements
     */
    unsigned int size() const;

    /**
     * Get the element at a given index.
     *
     * @param the index of the tile to get
     * @return a pointer to the tile
     */
    shared_ptr<Tile> at(unsigned int index) const;

    /**
     * Query whether the given tile exists.
     *
     * @param tile - the tile being checked
     */
    bool contains(const Tile& tile) const;

    /**
     * Query whether the LinkedList is empty.
     *
     * @return true if list is empty
     */
    bool isEmpty() const;

    /**
     * Overloaded output stream operator for easy printing.
     *
     * @param os - a reference to the output stream
     * @param list - the LinkedList to be printed
     */
    friend ostream& operator<<(ostream& os, const LinkedList& list);

private:
    unsigned int length;
    shared_ptr<Node> head;
    shared_ptr<Node> tail;
};

#endif // ASSIGN2_LINKEDLIST_H
