
#ifndef TILE_BAG_H
#define TILE_BAG_H

#include "LinkedList.h"

class Tile;

class TileBag {
public:
    TileBag();
    ~TileBag();
    void shuffle();
    shared_ptr<LinkedList> getHand();
    void replace(shared_ptr<Tile>& tile);
    int getRandomIndex();
    friend ostream& operator<<(ostream& os, const TileBag& bag);

private:
    shared_ptr<LinkedList> list;
};

#endif // !TILE_BAG_H
