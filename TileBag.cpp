
#include "TileBag.h"
#include "TileCodes.h"
#include "Constants.h"

#include <random>

TileBag::TileBag()
    : list(make_shared<LinkedList>()) {
    Colour colours[]{RED, ORANGE, YELLOW, GREEN, BLUE, PURPLE};
    Shape shapes[]{CIRCLE, STAR_4, DIAMOND, SQUARE, STAR_6, CLOVER};
    for (int i = 0; i < 6; ++i) {
        for (int j = 0; j < 6; ++j) {
            list->addBack(make_shared<Tile>(colours[i], shapes[j]));
            list->addBack(make_shared<Tile>(colours[i], shapes[j]));
        }
    }
}

TileBag::~TileBag() { list.reset(); }

void TileBag::shuffle() {
    for (unsigned int i = 0; i < list->size(); ++i) {
        int randomIndex = getRandomIndex();

        shared_ptr<Tile> toBeReplaced = list->at(i);
        shared_ptr<Tile> randomTile = list->at(randomIndex);
        shared_ptr<Tile> temp = toBeReplaced;

        list->insert(randomTile, i, true);
        list->insert(temp, randomIndex, true);
    }
}

// TODO assumes bag has 6 tiles
// TODO ordered list?
shared_ptr<LinkedList> TileBag::getHand() {
    shared_ptr<LinkedList> hand = make_shared<LinkedList>();
    for (int i = 0; i < HAND_SIZE; ++i) {
        hand->addBack(list->at(list->size() - 1));
        list->removeBack();
    }

    return hand;
}

// TODO although game logic does not allow same tile more than twice
// TODO should check if tile exists more than twice?
shared_ptr<Tile> TileBag::replace(shared_ptr<Tile>& tile) {
    int randomIndex = getRandomIndex();

    shared_ptr<Tile> newTile = list->at(randomIndex);; 
    list->remove(randomIndex);
    list->addBack(tile);
    return newTile;
}

int TileBag::getRandomIndex() {
    std::random_device engine;
    std::uniform_int_distribution<int> distribution(0, list->size() - 1);
    return distribution(engine);
}

ostream& operator<<(ostream& os, const TileBag& bag) {
    os << *bag.list;
    return os;
}
