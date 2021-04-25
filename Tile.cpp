
#include "Tile.h"

Tile::Tile(Colour colour, Shape shape) : colour(colour), shape(shape) {}

bool Tile::operator==(const Tile& other) {
    return colour == other.colour && shape == other.shape;
}

std::ostream& operator<<(std::ostream& os, const Tile& tile) {
    os << tile.colour << tile.shape;
    return os;
}
