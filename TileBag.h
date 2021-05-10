
#ifndef TILE_BAG_H
#define TILE_BAG_H

#include "LinkedList.h"

class Tile;
class PlayerHand;

class TileBag {
public:
    TileBag();
    ~TileBag();
    void shuffle();
    shared_ptr<PlayerHand> getHand();
    shared_ptr<Tile> replace(shared_ptr<Tile>& tile);
    int getRandomIndex();
    friend ostream& operator<<(ostream& os, const TileBag& bag);

private:
    shared_ptr<LinkedList> tiles;
};

#endif // !TILE_BAG_H
