#ifndef PLAYER_HAND_H
#define PLAYER_HAND_H

#include "TileBag.h"

class PlayerHand {
public:
    PlayerHand();
    /**
     * @brief Construct a new Player Hand 
     * 
     * @param Converts inputted list into unique_ptr for class
     */
    PlayerHand(LinkedList* tileList);
    PlayerHand(std::shared_ptr<LinkedList> tileList);
    /**
     * @brief Constructs a new player hand
     * 
     * @param Takes a linked list of tiles. ~WARNING~ This 
     * constructor moves the inserted pointer to the unique_ptr inside.
     * Old unique_ptr will be pointing to dead memory
     */
    PlayerHand(std::unique_ptr<LinkedList> tileList);
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
    std::unique_ptr<LinkedList> tiles;
};

#endif