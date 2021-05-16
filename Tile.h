
#ifndef ASSIGN2_TILE_H
#define ASSIGN2_TILE_H

#include <ostream>

using std::ostream;

// Define a Colour type.
typedef char Colour;

// Define a Shape type.
typedef int Shape;

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
     */
    bool hasSameColour(const Tile& other) const;

    /**
     * Check if other has same shape as this.
     */
    bool hasSameShape(const Tile& other) const;

    /**
     * Check for equality of two tiles based on colour and shape.
     */
    bool operator==(const Tile& other) const;

    /**
     * Overloaded output stream operator for easy printing.
     */
    friend ostream& operator<<(ostream& os, const Tile& tile);

private:
    const Colour colour;
    const Shape shape;
};

#endif // ASSIGN2_TILE_H
