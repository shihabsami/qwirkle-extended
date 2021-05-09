
#include "GameManager.h"
#include "Constants.h"

void GameManager::beginGame(
    const string& player1Name, const string& player2Name) {
    bag = make_shared<TileBag>();
    board = make_shared<GameBoard>();
    player1 = make_shared<Player>(player1Name, bag->getHand());
    player2 = make_shared<Player>(player2Name, bag->getHand());
    currentPlayer = player1;
}

void GameManager::placeTile(string tileCode, string gridLocation) {
    try {
        shared_ptr<Tile> tile =
            make_shared<Tile>(tileCode.at(0), tileCode.at(1));
        board->placeTile(
            tile, ((int)gridLocation.at(0)) - ASCII_BEGIN, gridLocation.at(1));

        GameManager::switchPlayer();
    } catch (...) {
        // TODO notify input/printer class of invalid location(/tileCode?)
    }
}

void GameManager::replaceTile(string tileCode) {
    try {
        player1->getHand()->replaceTile({tileCode.at(0), tileCode.at(1)}, *bag);

        GameManager::switchPlayer();
    } catch (...) {
        // TODO notify input/printer class of invalid location(/tileCode?)
    }
}

void GameManager::switchPlayer() {
    currentPlayer = (*currentPlayer == *player1) ? player2 : player1;
}

unsigned int GameManager::calculateScore(const Tile& playedTile) {
    // TODO calculate scores based on playedTile
}
