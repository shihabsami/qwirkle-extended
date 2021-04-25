#ifndef PLAYER_HAND_H
#define PLAYER_HAND_H

#include "LinkedList.h"
#include "TileBag.h"

class PlayerHand {
public:
    PlayerHand();
    PlayerHand(LinkedList* hand);
    ~PlayerHand();

    void addTile(Tile* tile);

    Tile* playTile(Colour& colour, Shape& shape);
    Tile* playTile(Tile& tile);

    // Unsure about how to implement this, need to check on how
    // the player interacts with the tilebag.
    Tile* replaceTile(Colour& colour, Shape& shape, TileBag& bag);
    Tile* replaceTile(Tile& toReplace, TileBag& bag);

    Tile* getTile(Colour& colour, Shape& shape);
    Tile* getTile(Tile& tile);

    bool isHoldingTile(Colour& colour, Shape& shape);
    bool isHoldingTile(Tile& tile);

    friend std::ostream& operator<<(std::ostream& os, const PlayerHand& hand);

private:
    LinkedList* tiles;
};

#endif