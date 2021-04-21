
#ifndef TILE_BAG_H
#define TILE_BAG_H

#define MAX_TILES 72

class LinkedList;
class Tile;
#include <random>

class TileBag {
public:
    TileBag();
    ~TileBag();
    void shuffle();
    Tile* replace(Tile* tile);
    friend std::ostream& operator<<(std::ostream& os, const TileBag& bag);

private:
    LinkedList* list;
};

#endif // !TILE_BAG_H
