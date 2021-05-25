
#ifndef PLAYER_HAND_H
#define PLAYER_HAND_H

#include "TileBag.h"

class PlayerHand {
public:
    /**
     * Construct a hand with an empty LinkedList.
     */
    PlayerHand();

    /**
     * Construct a hand with a LinkedList of tiles.
     */
    explicit PlayerHand(const shared_ptr<LinkedList>& tiles);

    /**
     * Destructor to free the tiles in the hand.
     */
    ~PlayerHand();

    /**
     * Add a tile to the end of the hand.
     *
     * @param tile - the tile to be added
     */
    void addTile(const shared_ptr<Tile>& tile);

    /**
     * Play a tile from the player's hand.
     *
     * @param tile - the tile to be played
     * @return a pointer to the tile
     */
    shared_ptr<Tile> playTile(const Tile& tile);

    /**
     * Replace a tile from the player's hand.
     *
     * @param tile - the tile to be replaced
     * @param bag - the bag to replaced the tile from
     */
    void replaceTile(const Tile& tile, TileBag& bag);

    /**
     * Get a tile of the given value.
     *
     * @param tile - the tile
     * @return a pointer to the tile
     */
    shared_ptr<Tile> getTile(const Tile& tile);

    /**
     * Get all the tiles from the hand.
     *
     * @return a pointer to the tile
     */
    shared_ptr<LinkedList> getTiles();

    /**
     * Prints out the PlayerHand to an output stream.
     *
     * @param os - a reference to the output stream
     * @param coloured - whether the tiles should be printed in colour
     */
    void print(ostream& os, bool coloured);

private:
    shared_ptr<LinkedList> tiles;
};

#endif // !PLAYER_HAND_H
