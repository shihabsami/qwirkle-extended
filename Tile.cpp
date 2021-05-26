
#include "Tile.h"

#include <iostream>
#include <string>

using std::cout;
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

void Tile::print(ostream& os, bool coloured) const {
    if (coloured) {
        if (colour == RED) {
            os << RED_ANSI;
        } else if (colour == ORANGE) {
            os << ORANGE_ANSI;
        } else if (colour == YELLOW) {
            os << YELLOW_ANSI;
        } else if (colour == GREEN) {
            os << GREEN_ANSI;
        } else if (colour == BLUE) {
            os << BLUE_ANSI;
        } else if (colour == PURPLE) {
            os << PURPLE_ANSI;
        }
    }

    os << colour << shape << (coloured ? ANSI_END : "");
}
