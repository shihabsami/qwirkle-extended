
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

bool Tile::hasSameColour(const Tile& tile) {
    return this->colour == tile.colour;
}

bool Tile::hasSameShape(const Tile& tile) {
    return this->shape == tile.shape;
}
