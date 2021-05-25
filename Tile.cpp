
#include "Tile.h"

#include <fstream>
#include <string>

using std::string;
using std::invalid_argument;

Tile::Tile(Colour colour, Shape shape) : colour(colour), shape(shape) {}

Colour Tile::getColour() const { return colour; }

Shape Tile::getShape() const { return shape; }

bool Tile::hasSameColour(const Tile& other) const {
    return colour == other.colour;
}

bool Tile::hasSameShape(const Tile& other) const {
    return shape == other.shape;
}

bool Tile::operator==(const Tile& other) const {
    return hasSameColour(other) && hasSameShape(other);
}

ostream& operator<<(ostream& os, const Tile& tile) {
    if (tile.colour == RED) {
        os << RED_ANSI;
    } else if (tile.colour == ORANGE) {
        os << ORANGE_ANSI;
    } else if (tile.colour == YELLOW) {
        os << YELLOW_ANSI;
    } else if (tile.colour == GREEN) {
        os << GREEN_ANSI;
    } else if (tile.colour == BLUE) {
        os << BLUE_ANSI;
    } else if (tile.colour == PURPLE) {
        os << PURPLE_ANSI;
    }

    os << tile.colour << tile.shape << ANSI_END;
    return os;
}

ofstream& operator<<(ofstream& ofs, const Tile& tile) {
    ofs << tile.colour << tile.shape;
    return ofs;
}
