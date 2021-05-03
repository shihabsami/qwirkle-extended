#include "PlayerHand.h"

PlayerHand::PlayerHand() { tiles = new LinkedList(); }

PlayerHand::PlayerHand(LinkedList* hand) { tiles = hand; }

PlayerHand::~PlayerHand() { delete tiles; }

void PlayerHand::addTile(Tile* tile) { tiles->addBack(tile); }

Tile* PlayerHand::playTile(Colour& colour, Shape& shape) {
    Tile temp(colour, shape);
    return playTile(temp);
}

// returns nullptr if fails to play tile
Tile* PlayerHand::playTile(Tile& tile) {
    Tile* toPlay = getTile(tile);

    if (toPlay != nullptr) {
        tiles->remove(toPlay);
    }

    return toPlay;
}

Tile* PlayerHand::replaceTile(Colour& colour, Shape& shape, TileBag& bag) {
    Tile temp(colour, shape);
    return replaceTile(temp, bag);
}

Tile* PlayerHand::replaceTile(Tile& toReplace, TileBag& bag) {
    Tile* newTile = nullptr;
    Tile* replace = getTile(toReplace);

    if (replace != nullptr) {
        newTile = bag.replace(replace);
    }

    return newTile;
}

Tile* PlayerHand::getTile(Colour& colour, Shape& shape) {
    Tile temp(colour, shape);
    return getTile(temp);
}

Tile* PlayerHand::getTile(Tile& tile) {
    Tile* currentTile = nullptr;
    Tile* toReturn = nullptr;

    for (int i = 0; i < tiles->size(); i++) {
        currentTile = tiles->at(i);

        if (tile == *currentTile)
            toReturn = nullptr;
    }

    return toReturn;
}

bool PlayerHand::isHoldingTile(Colour& colour, Shape& shape) {
    Tile temp(colour, shape);
    return isHoldingTile(temp);
}

bool PlayerHand::isHoldingTile(Tile& tile) {
    return (getTile(tile) != nullptr) ? true : false;
}

std::ostream& operator<<(std::ostream& os, const PlayerHand& hand) {
    os << hand.tiles;
    return os;
}