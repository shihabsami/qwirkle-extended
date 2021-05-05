#ifndef PLAYER_HAND_H
#define PLAYER_HAND_H

#include "TileBag.h"

class PlayerHand {
public:
    PlayerHand();
    
    PlayerHand(LinkedList& tileList);
    PlayerHand(std::shared_ptr<LinkedList> tileList);

    ~PlayerHand();

    void addTile(std::shared_ptr<Tile> tile);

    std::shared_ptr<Tile> playTile(Colour& colour, Shape& shape);
    std::shared_ptr<Tile> playTile(Tile& tile);

    // Unsure about how to implement this, need to check on how
    // the player interacts with the tilebag.
    std::shared_ptr<Tile> replaceTile(Colour& colour, Shape& shape, TileBag& bag);
    std::shared_ptr<Tile> replaceTile(Tile& toReplace, TileBag& bag);

    std::shared_ptr<Tile> getTile(Colour& colour, Shape& shape);
    std::shared_ptr<Tile> getTile(Tile& tile);

    bool isHoldingTile(Colour& colour, Shape& shape);
    bool isHoldingTile(Tile& tile);

    friend std::ostream& operator<<(std::ostream& os, const PlayerHand& hand);

private:
    std::shared_ptr<LinkedList> tiles;
};

#endif