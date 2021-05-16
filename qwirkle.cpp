
#include "TileBag.h"
#include "LinkedList.h"
#include "TileCodes.h"
#include "PlayerHand.h"

#include <iostream>

using std::cout;
using std::endl;

// tests for LinkedList implementation
void testLinkedList();

// tests for TileBag implementation
void testTileBag();

// tests for PlayerHand implementation
void testPlayerHand();

int main(void) {
    // testLinkedList();

    testTileBag();

    testPlayerHand();

    return EXIT_SUCCESS;
}

void testLinkedList() {
    cout << "testing linkedlist..." << endl;
    shared_ptr<LinkedList> list = make_shared<LinkedList>();

    shared_ptr<Tile> tile1 = make_shared<Tile>(RED, CIRCLE);
    shared_ptr<Tile> tile2 = make_shared<Tile>(ORANGE, STAR_4);
    shared_ptr<Tile> tile3 = make_shared<Tile>(YELLOW, DIAMOND);
    shared_ptr<Tile> tile4 = make_shared<Tile>(GREEN, SQUARE);
    shared_ptr<Tile> tile5 = make_shared<Tile>(BLUE, STAR_6);
    shared_ptr<Tile> tile6 = make_shared<Tile>(PURPLE, CLOVER);

    cout << "adding tiles to linkedlist..." << endl;
    list->addFront(tile1);
    list->addBack(tile2);
    list->addFront(tile3);

    list->addBack(tile4);
    list->addFront(tile5);
    list->addBack(tile6);
    cout << *list << endl;

    cout << "removing tiles to linkedlist..." << endl;
    list->removeFront();
    list->removeBack();
    cout << *list << endl;

    cout << "testing size() and at()" << endl;
    cout << "size() - " << list->size() << endl;
    cout << "at(1) - " << *list->at(1) << endl;
    cout << "at(size() - 1) - " << *list->at(list->size() - 1) << endl;
    cout << *list << endl;

    shared_ptr<Tile> tile7 = make_shared<Tile>(RED, STAR_4);
    shared_ptr<Tile> tile8 = make_shared<Tile>(ORANGE, DIAMOND);

    cout << "inserting item at index..." << endl;
    list->insert(tile7, 0, false);
    cout << *list << endl;

    cout << "inserting and replacing item at index..." << endl;
    list->insert(tile8, 4, true);
    cout << *list << endl;

    cout << "removing tiles by value..." << endl;
    list->remove(tile1);
    cout << *list << endl;

    cout << "removing tiles at index..." << endl;
    list->remove(2);
    cout << *list << endl;

    cout << "testing if linkedlist contains tile..." << endl;
    cout << "contains " << *tile7 << " - " << (list->contains(*tile7) ? "true" : "false") << endl;
    cout << "contains " << *tile4 << " - " << (list->contains(*tile4) ? "true" : "false") << endl;

    cout << "testing if linkedlist is empty..." << endl;
    cout << "empty - " << (list->isEmpty() ? "true" : "false") << endl;
}

void testTileBag() {
    cout << "testing tilebag..." << endl;
    shared_ptr<TileBag> bag = make_shared<TileBag>();
    bag->fill();
    cout << *bag << endl;

    cout << "shuffling tilebag..." << endl;
    bag->shuffle();
    cout << *bag << endl;

    cout << "getting hand of tiles from tilebag..." << endl;
    shared_ptr<PlayerHand> hand = bag->getHand();
    cout << "hand -" << *hand << endl;
    cout << *bag << endl;

    cout << "replacing a tile from tilebag..." << endl;
    shared_ptr<Tile> tile = make_shared<Tile>(PURPLE, CLOVER);
    cout << "tile " << *tile;
    tile = bag->replace(tile);
    cout << " replaced with " << *tile << endl;
    cout << *bag << endl;
}

void testPlayerHand() {
    shared_ptr<TileBag> bag = make_shared<TileBag>();
    bag->fill();
    bag->shuffle();

    cout << "testing playerhand..." << endl;
    shared_ptr<PlayerHand> hand = bag->getHand();
    cout << "getting a hand of tiles..." << endl;
    cout << *hand->getTiles() << endl;

    cout << "adding two new tiles to the hand..." << endl;
    shared_ptr<Tile> tile1 = make_shared<Tile>(RED, STAR_4);
    shared_ptr<Tile> tile2 = make_shared<Tile>(ORANGE, DIAMOND);
    hand->addTile(tile1);
    hand->addTile(tile2);
    cout << *hand << endl;

    cout << "playing a tile from the hand..." << endl;
    hand->playTile(*tile1);
    cout << *hand << endl;

    cout << "replacing a tile from the hand..." << endl;
    hand->replaceTile(*tile2, *bag);
    cout << *hand << endl;
}
