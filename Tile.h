
#ifndef ASSIGN2_TILE_H
#define ASSIGN2_TILE_H

#include <ostream>

#include "TileCodes.h"

using std::ostream;
using std::ofstream;

class Tile {
public:
    /**
     * Construct a tile with provided colour (char) and shape (int).
     */
    Tile(Colour color, Shape shape);

    /**
     * Get the tile's colour.
     *
     * @return the Colour (char)
     */
    Colour getColour() const;

    /**
     * Get the tile's shape.
     *
     * @return the Shape (int)
     */
    Shape getShape() const;

    /**
     * Check if other has same colour as this.
     *
     * @param other - the other tile to compare to
     */
    bool hasSameColour(const Tile& other) const;

    /**
     * Check if other has same shape as this.
     *
     * @param other - the other tile to compare to
     */
    bool hasSameShape(const Tile& other) const;

    /**
     * Prints the tile to an output stream.
     * @param os - a reference to the stream
     * @param coloured - whether the tile should be printed in colour
     */
    void print(ostream& os, bool coloured) const;

    /**
     * Check for equality of two tiles based on colour and shape.
     *
     * @param other - the other tile to compare to
     * @return true if this tile has the same colour and shape as the other
     */
    bool operator==(const Tile& other) const;

private:
    const Colour colour;
    const Shape shape;
};

#endif // ASSIGN2_TILE_H
