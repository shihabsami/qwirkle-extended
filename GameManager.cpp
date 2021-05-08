#include "GameManager.h"
#include "Constants.h"

GameManager::GameManager() {}

void GameManager::beginGame(
    const shared_ptr<Player>& player1, const shared_ptr<Player>& player2) {
    this->player1 = player1;
    this->player2 = player2;
    currentPlayer = player1;
    board = make_shared<GameBoard>();
    bag = make_shared<TileBag>();
}
void GameManager::placeTile(string tileCode, string gridLocation) {
    shared_ptr<Tile> tile = make_shared<Tile>(tileCode.at(0), tileCode.at(1));
    board->placeTile(
        tile, ((int)gridLocation.at(0)) - ASCII_BEGIN, gridLocation.at(1));
}

void GameManager::replaceTile(string tileCode) {
    player1->getHand()->getTile(tileCode.at(0), tileCode.at(1))
}

void GameManager::switchPlayer() {
    currentPlayer = (*currentPlayer == *player1) ? player2 : player1;
    // TODO notify input/printer class
}
