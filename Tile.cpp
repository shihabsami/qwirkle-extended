
#include "Tile.h"

using std::invalid_argument;

Tile::Tile(Colour colour, Shape shape) : colour(colour), shape(shape) {}

bool Tile::operator==(const Tile& other) const {
    return colour == other.colour && shape == other.shape;
}

ostream& operator<<(ostream& os, const Tile& tile) {
    os << tile.colour << tile.shape;
    return os;
}
