
#include "Node.h"

Node::Node(Tile* tile, Node* next, Node* previous)
    : tile(tile), next(next), previous(previous) {}

Node::Node(const Node& other)
    : tile(other.tile), next(other.next), previous(other.previous) {
    // TODO discuss if this should do deep copy
    /*
    tile = new Tile(*other.tile);
    next = new Node(*other.next);
    previous = new Node(*other.previous);
    */
}

Node::~Node() { delete tile; }

bool operator==(const Node& node1, const Node& node2) {
    return node1.tile->colour == node2.tile->colour &&
           node1.tile->shape == node2.tile->shape;
}
