
#include "TileBag.h"
#include "PlayerHand.h"
#include "Constants.h"

#include <random>

using std::length_error;
using std::random_device;
using std::uniform_int_distribution;

TileBag::TileBag() : tiles(make_shared<LinkedList>()) {}

TileBag::~TileBag() { tiles.reset(); }

void TileBag::fill() {
    if (tiles->isEmpty()) {
        for (Colour colour : COLOURS) {
            for (Shape shape : SHAPES) {
                // two of each colour and shape combination
                tiles->addBack(make_shared<Tile>(colour, shape));
                tiles->addBack(make_shared<Tile>(colour, shape));
            }
        }
    } else {
        throw length_error("bag is not empty for TileBag::fill");
    }
}

void TileBag::shuffle() {
    for (size_t i = 0; i < tiles->size(); ++i) {
        size_t randomIndex = getRandomIndex();

        shared_ptr<Tile> toBeReplaced = tiles->at(i);
        shared_ptr<Tile> randomTile = tiles->at(randomIndex);
        const shared_ptr<Tile>& replaced = toBeReplaced;

        tiles->insert(randomTile, i, true);
        tiles->insert(replaced, randomIndex, true);
    }
}

shared_ptr<PlayerHand> TileBag::getHand() {
    if (tiles->size() < HAND_SIZE)
        throw length_error("insufficient number of tiles for TileBag::getHand");

    shared_ptr<PlayerHand> hand = make_shared<PlayerHand>();
    for (size_t i = 0; i < HAND_SIZE; ++i) {
        hand->addTile(tiles->at(tiles->size() - 1));
        tiles->removeBack();
    }

    return hand;
}

shared_ptr<LinkedList> TileBag::getTiles() { return tiles; }

shared_ptr<Tile> TileBag::replace(shared_ptr<Tile>& tile) {
    shared_ptr<Tile> toBeReplaced = tiles->at(FIRST_POSITION);
    tiles->removeFront();
    tiles->addBack(tile);

    return toBeReplaced;
}

size_t TileBag::getRandomIndex() {
    random_device engine;
    uniform_int_distribution<size_t> distribution(0, tiles->size() - 1);

    return distribution(engine);
}

void TileBag::print(ostream& os, bool coloured) const {
    tiles->print(os, coloured);
}
