#include "PlayerHand.h"

PlayerHand::PlayerHand() { tiles = std::make_unique<LinkedList>(new LinkedList()); }

PlayerHand::PlayerHand(LinkedList* tileList) { tiles = std::make_unique<LinkedList>(tileList); }
PlayerHand::PlayerHand(std::shared_ptr<LinkedList> tileList) { tiles = std::make_unique<LinkedList>(tileList); }
PlayerHand::PlayerHand(std::unique_ptr<LinkedList> tileList) : tiles(std::move(tileList)) 
{}

PlayerHand::~PlayerHand() { tiles.reset(); }

void PlayerHand::addTile(std::shared_ptr<Tile> tile) { tiles->addBack(tile); }

std::shared_ptr<Tile> PlayerHand::playTile(Colour& colour, Shape& shape) {
    Tile temp(colour, shape);
    return playTile(temp);
}

// returns nullptr if fails to play tile
std::shared_ptr<Tile> PlayerHand::playTile(Tile& tile) {
    std::shared_ptr<Tile> toPlay = getTile(tile);

    if (toPlay != nullptr) {
        tiles->remove(toPlay);
    }

    return toPlay;
}

std::shared_ptr<Tile> PlayerHand::replaceTile(Colour& colour, Shape& shape, TileBag& bag) {
    Tile temp(colour, shape);
    return replaceTile(temp, bag);
}

std::shared_ptr<Tile> PlayerHand::replaceTile(Tile& toReplace, TileBag& bag) {
    std::shared_ptr<Tile> newTile = nullptr;
    std::shared_ptr<Tile> replace = getTile(toReplace);

    if (replace != nullptr) {
        newTile = bag.replace(replace);
        tiles->remove(replace);
        tiles->addBack(newTile);
    }

    return newTile;
}

std::shared_ptr<Tile> PlayerHand::getTile(Colour& colour, Shape& shape) {
    Tile temp(colour, shape);
    return getTile(temp);
}

std::shared_ptr<Tile> PlayerHand::getTile(Tile& tile) {
    bool found = false;
    std::shared_ptr<Tile> currentTile = nullptr;
    std::shared_ptr<Tile> toReturn = nullptr;

    for (int i = 0; i < tiles->size(); i++) {
        currentTile = tiles->at(i);

        if (tile == *currentTile && !found) {
            toReturn = nullptr; 
            found = true;
        }
            
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
    os << *(hand.tiles.get());
    return os;
}