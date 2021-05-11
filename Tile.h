
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
     * Check for equality of two tiles based on colour and shape.
     */
    bool operator==(const Tile& other) const;

    /**
     * Overloaded output stream operator for easy printing.
     */
    friend ostream& operator<<(ostream& os, const Tile& tile);

    Colour colour;
    Shape shape;
};

#endif // ASSIGN2_TILE_H
