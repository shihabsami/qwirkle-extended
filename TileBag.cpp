#include "TileBag.h"
#include "LinkedList.h"
#include "Tile.h"
#include "TileCodes.h"
#include <iostream>

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
        Tile* randomTile = list->at(randomIndex);
        Tile* temp = new Tile(*toBeReplaced);

        std::cout << "toBeReplaced " << *toBeReplaced << ", randomTile "
                  << *randomTile << ", temp " << *temp << std::endl;

        list->insert(randomTile, i, true);
        std::cout << "toBeReplaced " << *toBeReplaced << ", randomTile "
                  << *randomTile << ", temp " << *temp << std::endl;

        list->insert(temp, randomIndex, true);
    }
}

Tile* TileBag::replace(Tile* tile) {
    std::random_device engine;
    std::uniform_int_distribution<int> distribution(0, (int)list->size() - 1);
    int randomIndex = distribution(engine);
    Tile* temp = list->at(randomIndex);
    *list->at(randomIndex) = *tile;
    return temp;
}

std::ostream& operator<<(std::ostream& os, const TileBag& bag) {
    os << *(bag.list);
    return os;
}
