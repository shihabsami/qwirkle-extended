#include "TileBag.h"

#include "Tile.h"
#include "TileCodes.h"
#include "Constants.h"

#include <iostream>
#include <random>

TileBag::TileBag() : list(new LinkedList()) {
    Colour colours[]{RED, ORANGE, YELLOW, GREEN, BLUE, PURPLE};
    Shape shapes[]{CIRCLE, STAR_4, DIAMOND, SQUARE, STAR_6, CLOVER};
    for (int i = 0; i < 6; ++i) {
        for (int j = 0; j < 6; ++j) {
            list->addBack(new Tile(colours[i], shapes[j]));
            list->addBack(new Tile(colours[i], shapes[j]));
        }
    }
}

TileBag::~TileBag() { delete list; }

void TileBag::shuffle() {
    std::random_device engine;
    std::uniform_int_distribution<int> distribution(0, (int)list->size() - 1);

    for (unsigned int i = 0; i < list->size(); ++i) {
        int randomIndex = distribution(engine);
        Tile* toBeReplaced = list->at(i);
        Tile* randomTile = new Tile(*list->at(randomIndex));
        Tile* temp = new Tile(*toBeReplaced);

        list->insert(randomTile, i, true);
        list->insert(temp, randomIndex, true);
    }
}

LinkedList* TileBag::getHand() {
    LinkedList* hand = new LinkedList();

    // TODO assumes bag has 6 tiles
    // TODO ordered list?
    for (int i = 0; i < HAND_SIZE; ++i) {
        hand->addBack(list->at(list->size() - 1));
        list->removeBack();
    }
}

Tile* TileBag::replace(Tile* tile) {
    // TODO although game logic does not allow same tile more than twice
    // should check if tile exists more than twice?

    std::random_device engine;
    std::uniform_int_distribution<int> distribution(0, (int)list->size() - 1);

    int randomIndex = distribution(engine);
    Tile* temp = list->at(randomIndex);
    // Pretty sure the replaced tile needs to be added at the back
    *list->at(randomIndex) = *tile;
    return temp;
}

std::ostream& operator<<(std::ostream& os, const TileBag& bag) {
    os << *bag.list;
    return os;
}
