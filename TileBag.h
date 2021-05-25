
#ifndef TILE_BAG_H
#define TILE_BAG_H

#include "LinkedList.h"

class Tile;
class PlayerHand;

class TileBag {
public:
    /**
    * Construct a bag with an empty LinkedList.
    */
    TileBag();

    /**
     * Destructor to free the tiles in the bag.
     */
    ~TileBag();

    /**
     * Fill in the bag with maximum number of tiles possible.
     */
    void fill();

    /**
     * Shuffle the bag using an uniform distribution.
     */
    void shuffle();

    /**
     * Get a hand (6) of tiles.
     *
     * @return a pointer to PlayerHand
     */
    shared_ptr<PlayerHand> getHand();

    /**
     * Get all the tiles in the bag.
     *
     * @return a pointer to a LinkedList of tiles
     * */
    shared_ptr<LinkedList> getTiles();

    /**
     * Replace a tile from the bag.
     *
     * @param tile - the tile that is inserted
     * @return a pointer to a tile from the bag
     * */
    shared_ptr<Tile> replace(shared_ptr<Tile>& tile);

    /**
     * Prints out the TileBag to an output stream.
     *
     * @param os - a reference to the output stream
     * @param coloured - whether the tiles should be printed in colour
     */
    void print(ostream& os, bool coloured) const;

private:
    /**
     * A random number generator.
     *
     * @return an index within the bounds of the bag
     */
    size_t getRandomIndex();

    shared_ptr<LinkedList> tiles;
};

#endif // !TILE_BAG_H
