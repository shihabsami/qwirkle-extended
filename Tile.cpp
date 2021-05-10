
#include "Tile.h"
#include "TileCodes.h"

using std::invalid_argument;

Tile::Tile(Colour colour, Shape shape) {
    bool validColour = false;
    bool validShape = false;
    for (Colour c : COLOURS)
        if (c == colour)
            validColour = true;
    for (Shape s : SHAPES)
        if (s == shape)
            validShape = true;

    if (!validColour || !validShape)
        throw invalid_argument("invalid colour or shape for Tile::Tile");
    else {
        this->colour = colour;
        this->shape = shape;
    }
}

bool Tile::operator==(const Tile& other) const {
    return colour == other.colour && shape == other.shape;
}

ostream& operator<<(ostream& os, const Tile& tile) {
    os << tile.colour << tile.shape;
    return os;
}

    bool Tile::hasSameColour(const Tile& tile) {
        return this->colour == tile.colour;
    }

    bool Tile::hasSameShape(const Tile& tile) {
        return this->shape == tile.shape;
    }