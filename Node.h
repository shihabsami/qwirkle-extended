
#ifndef ASSIGN2_NODE_H
#define ASSIGN2_NODE_H

#include "Tile.h"

class Node {
public:
    Node(Tile* tile, Node* next, Node* previous);
    Node(const Node& other);
    ~Node();
    friend bool operator==(const Node& node1, const Node& node2);

    Tile* tile;
    Node* next;
    Node* previous;
};

#endif // ASSIGN2_NODE_H
