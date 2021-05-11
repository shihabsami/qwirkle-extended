
#ifndef PLAYER_HAND_H
#define PLAYER_HAND_H

#include "TileBag.h"

class PlayerHand {
public:
    PlayerHand();
    PlayerHand(const shared_ptr<LinkedList>& tiles);
    PlayerHand(const PlayerHand& hand);
    ~PlayerHand();

    void addTile(shared_ptr<Tile> tile);
    shared_ptr<Tile> playTile(const Tile& tile);
    void replaceTile(const Tile& tile, TileBag& bag);
    shared_ptr<Tile> getTile(const Tile& tile);
    shared_ptr<LinkedList> getTiles();

    friend ostream& operator<<(ostream& os, const PlayerHand& hand);

private:
    shared_ptr<LinkedList> tiles;
};

#endif // PLAYER_HAND_H
