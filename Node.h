
#ifndef ASSIGN2_NODE_H
#define ASSIGN2_NODE_H

#include "Tile.h"

#include <memory>

using std::shared_ptr;
using std::make_shared;

class Node {
public:
    explicit Node(const shared_ptr<Tile>& tile);
    ~Node();
    Node(
        const shared_ptr<Tile>& tile,
        const shared_ptr<Node>& next,
        const shared_ptr<Node>& previous);
    friend bool operator==(const Node& node1, const Node& node2);

    // TODO move constructor if needed

    shared_ptr<Tile> tile;
    shared_ptr<Node> next;
    shared_ptr<Node> previous;
};

#endif // ASSIGN2_NODE_H
