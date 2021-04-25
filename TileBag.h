
#ifndef TILE_BAG_H
#define TILE_BAG_H

#include "LinkedList.h"

class Tile;

class TileBag {
public:
    TileBag();
    ~TileBag();
    void shuffle();
    LinkedList* getHand();
    Tile* replace(Tile* tile);
    friend std::ostream& operator<<(std::ostream& os, const TileBag& bag);

private:
    LinkedList* list;
};

#endif // !TILE_BAG_H
