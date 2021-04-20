
#include "Tile.h"

Tile::Tile(Colour colour, Shape shape) : colour(colour), shape(shape) {}

std::ostream& operator<<(std::ostream& os, const Tile& tile) {
    os << tile.colour << tile.shape;
    return os;
}
