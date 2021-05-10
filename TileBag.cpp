
#include "TileBag.h"
#include "TileCodes.h"
#include "PlayerHand.h"
#include "Constants.h"

#include <random>

using std::length_error;
using std::random_device;
using std::uniform_int_distribution;

TileBag::TileBag() : tiles(make_shared<LinkedList>()) {
    for (int i = 0; i < 6; ++i) {
        for (int j = 0; j < 6; ++j) {
            tiles->addBack(make_shared<Tile>(COLOURS[i], SHAPES[j]));
            tiles->addBack(make_shared<Tile>(COLOURS[i], SHAPES[j]));
        }
    }
}

TileBag::~TileBag() { tiles.reset(); }

void TileBag::shuffle() {
    for (unsigned int i = 0; i < tiles->size(); ++i) {
        int randomIndex = getRandomIndex();

        shared_ptr<Tile> toBeReplaced = tiles->at(i);
        shared_ptr<Tile> randomTile = tiles->at(randomIndex);
        shared_ptr<Tile> replaced = toBeReplaced;

        tiles->insert(randomTile, i, true);
        tiles->insert(replaced, randomIndex, true);
    }
}

shared_ptr<PlayerHand> TileBag::getHand() {
    if (tiles->size() < HAND_SIZE)
        throw length_error("insufficient number of tiles for TileBag::getHand");

    shared_ptr<PlayerHand> hand = make_shared<PlayerHand>();
    for (int i = 0; i < HAND_SIZE; ++i) {
        hand->addTile(tiles->at(tiles->size() - 1));
        tiles->removeBack();
    }

    return hand;
}

// TODO think of an efficient way to check if tile exists more than twice
shared_ptr<Tile> TileBag::replace(shared_ptr<Tile>& tile) {
    int randomIndex = getRandomIndex();
    shared_ptr<Tile> randomTile = tiles->at(randomIndex);
    tiles->remove(randomIndex);
    tiles->addBack(tile);

    return randomTile;
}

int TileBag::getRandomIndex() {
    random_device engine;
    uniform_int_distribution<int> distribution(0, tiles->size() - 1);
    return distribution(engine);
}

ostream& operator<<(ostream& os, const TileBag& bag) {
    os << *bag.tiles;
    return os;
}
