
#include "TileCodes.h"
#include "Constants.h"
#include "IOHandler.h"

#include <iostream>
#include <algorithm>

#include <thread>         // std::this_thread::sleep_for
#include <chrono>         // std::chrono::seconds

using std::cout;
using std::cin;
using std::endl;
using std::sort;
using std::get;

// tests for LinkedList implementation
void testLinkedList();

// tests for TileBag implementation
void testTileBag();

// tests for PlayerHand implementation
void testPlayerHand();

// tests for GameBoard implementation
void testGameBoard();

int main(int argc, char** argv) {
//         testLinkedList();
//         testTileBag();
//         testPlayerHand();
//         testGameBoard();

    if (argc > 1) {
        for (int i = 0; i < argc; ++i) {
            string arg = argv[i];
            if (arg == "--ai")
                IOHandler::aiEnabled = true;
        }
    }

    // begin the game
    cout << SPLASH_SCREEN << endl;
    IOHandler::beginGame();

    // if AI is enabled the player2 is considered to be the AI
    shared_ptr<Player> ai =
        IOHandler::aiEnabled ? GameManager::player2 : nullptr;

    // run the main game loop
    while (!cin.eof() && IOHandler::gameRunning) {
        IOHandler::printRound();

        if (IOHandler::aiEnabled && *GameManager::currentPlayer == *ai) {
            vector<Move> moves = GameManager::getPossibleMoves();
            shared_ptr<PlayerHand> hand = ai->getHand();

            if (moves.empty()) {
                GameManager::replaceTile(hand->getTiles()->at(0)->getColour(),
                    hand->getTiles()->at(0)->getShape());
            } else {
                sort(moves.begin(), moves.end(),
                    [](const Move& m1, const Move& m2) {
                      return m1.points > m2.points;
                    });

                auto move = moves.begin();
                GameManager::placeTile(move->tile->getColour(),
                    move->tile->getShape(), move->location.row,
                    move->location.column);
            }
        } else {
            IOHandler::playRound();
        }

        if (cin.eof())
            IOHandler::quit();
    }

    //    bool hasPlayedFirstRound = false;
//
//    // run the main game loop
//    while (!cin.eof() && IOHandler::gameRunning) {
//        IOHandler::printRound();
//        vector<Move> moves = GameManager::getPossibleMoves();
//        shared_ptr<PlayerHand> hand = GameManager::currentPlayer->getHand();
//
//       if (!hasPlayedFirstRound) {
//           auto tile = hand->getTiles()->at(0);
//           GameManager::placeTileOperation(tile->getColour(),
//               tile->getShape(), 13, 13);
//            hasPlayedFirstRound = true;
//       } else if (moves.empty()) {
//            GameManager::replaceTileOperation(hand->getTiles()->at(0)->getColour(),
//                hand->getTiles()->at(0)->getShape());
//        } else {
//            sort(moves.begin(), moves.end(),
//                [](const Move& m1, const Move& m2) {
//                  return m1.points > m2.points;
//                });
//
//            auto move = moves.begin();
//            GameManager::placeTileOperation(move->tile->getColour(),
//                move->tile->getShape(), move->location.row,
//                move->location.column);
//        }
//
//        std::this_thread::sleep_for (std::chrono::seconds(1));
//
//        if (cin.eof())
//            IOHandler::quit();
//    }

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
    list->print(cout, true);
    cout << endl;

    cout << "removing tiles to linkedlist..." << endl;
    list->removeFront();
    list->removeBack();
    list->print(cout, true);
    cout << endl;

    cout << "testing size() and at()" << endl;
    cout << "size() - " << list->size() << endl;

    cout << "at(1) - ";
    list->at(1)->print(cout, true);
    cout << endl;

    cout << "at(size() - 1) - ";
    list->at(list->size() - 1)->print(cout, true);
    cout << endl;
    list->print(cout, true);
    cout << endl;

    shared_ptr<Tile> tile7 = make_shared<Tile>(RED, STAR_4);
    shared_ptr<Tile> tile8 = make_shared<Tile>(ORANGE, DIAMOND);

    cout << "inserting item at index..." << endl;
    list->insert(tile7, 0, false);
    list->print(cout, true);
    cout << endl;

    cout << "inserting and replacing item at index..." << endl;
    list->insert(tile8, 4, true);
    list->print(cout, true);
    cout << endl;

    cout << "removing tiles by value..." << endl;
    list->remove(tile1);
    list->print(cout, true);
    cout << endl;

    cout << "removing tiles at index..." << endl;
    list->remove(2);
    list->print(cout, true);
    cout << endl;

    cout << "testing if linkedlist contains tile..." << endl;
    cout << "contains ";
    tile7->print(cout, true);
    cout << " - " << (list->contains(*tile7) ? "true" : "false") << endl;
    cout << "contains ";
    tile4->print(cout, true);
    cout << " - " << (list->contains(*tile4) ? "true" : "false") << endl;

    cout << "testing if linkedlist is empty..." << endl;
    cout << "empty - " << (list->isEmpty() ? "true" : "false") << endl;
}

void testTileBag() {
    cout << "testing tilebag..." << endl;
    shared_ptr<TileBag> bag = make_shared<TileBag>();
    bag->fill();
    bag->print(cout, true);
    cout << endl;

    cout << "shuffling tilebag..." << endl;
    bag->shuffle();
    bag->print(cout, true);
    cout << endl;
    cout << "getting hand of tiles from tilebag..." << endl;
    shared_ptr<PlayerHand> hand = bag->getHand();
    cout << "hand -";
    hand->print(cout, true);
    cout << endl;
    bag->print(cout, true);
    cout << endl;

    cout << "replacing a tile from tilebag..." << endl;
    shared_ptr<Tile> tile = make_shared<Tile>(PURPLE, CLOVER);
    cout << "tile ";
    tile->print(cout, true);
    tile = bag->replace(tile);
    cout << " replaced with ";
    tile->print(cout, true);
    cout << endl;
    bag->print(cout, true);
    cout << endl;
}

void testPlayerHand() {
    shared_ptr<TileBag> bag = make_shared<TileBag>();
    bag->fill();
    bag->shuffle();

    cout << "testing playerhand..." << endl;
    shared_ptr<PlayerHand> hand = bag->getHand();
    cout << "getting a hand of tiles..." << endl;
    hand->getTiles()->print(cout, true);
    cout << endl;

    cout << "adding two new tiles to the hand..." << endl;
    shared_ptr<Tile> tile1 = make_shared<Tile>(RED, STAR_4);
    shared_ptr<Tile> tile2 = make_shared<Tile>(ORANGE, DIAMOND);
    hand->addTile(tile1);
    hand->addTile(tile2);
    hand->print(cout, true);
    cout << endl;

    cout << "playing a tile from the hand..." << endl;
    hand->playTile(*tile1);
    hand->print(cout, true);
    cout << endl;

    cout << "replacing a tile from the hand..." << endl;
    hand->replaceTile(*tile2, *bag);
    hand->print(cout, true);
    cout << endl;
}

void testGameBoard() {
    cout << "testing gameboard..." << endl;
    shared_ptr<GameBoard> board = make_shared<GameBoard>();

    shared_ptr<Tile> tile1 = make_shared<Tile>(RED, CIRCLE);
    shared_ptr<Tile> tile2 = make_shared<Tile>(ORANGE, STAR_4);
    shared_ptr<Tile> tile3 = make_shared<Tile>(YELLOW, DIAMOND);
    shared_ptr<Tile> tile4 = make_shared<Tile>(GREEN, SQUARE);
    shared_ptr<Tile> tile5 = make_shared<Tile>(BLUE, STAR_6);
    shared_ptr<Tile> tile6 = make_shared<Tile>(PURPLE, CLOVER);

    cout << "placing tiles on gameboard..." << endl;
    board->placeTile(tile1, 5, 10);
    board->placeTile(tile2, 10, 5);
    board->placeTile(tile3, 15, 20);
    board->placeTile(tile4, 20, 10);
    board->placeTile(tile5, 20, 5);
    board->placeTile(tile6, 10, 20);
    board->print(cout, true);
    cout << endl;
}
