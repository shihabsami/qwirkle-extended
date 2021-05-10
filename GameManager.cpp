#include "GameManager.h"
#include "Constants.h"
#include <iostream>
#include <string>

shared_ptr<GameBoard> GameManager::board = nullptr;
shared_ptr<TileBag> GameManager::bag = nullptr;
shared_ptr<Player> GameManager::player1 = nullptr;
shared_ptr<Player> GameManager::player2 = nullptr;
shared_ptr<Player> GameManager::currentPlayer = nullptr;

using std::invalid_argument;

void GameManager::beginGame(
    const string& player1Name, const string& player2Name) {
    bag = make_shared<TileBag>();
    board = make_shared<GameBoard>();
    player1 = make_shared<Player>(player1Name, bag->getHand());
    player2 = make_shared<Player>(player2Name, bag->getHand());
    currentPlayer = player1;
}

void GameManager::placeTile(Colour colour, Shape shape, int row, int column) {
    string message = "Tile placed successfully.";
    State state = PLACE_SUCCESS;

    try {
        shared_ptr<Tile> tile = make_shared<Tile>(colour, shape);
        if (board.at(row, column) != nullptr) {
            message = "A tile is already present in the provided grid location.";
            throw invalid_argument("grid location is not empty");
        } else if (!player1->getHand().contains(tile)) {
            message = "Tile is not present in hand.";
            throw invalid_argument("tile absent from player hand");
        } else if ( /* TODO no more than 6 tiles per line */ ) {
        } else if ( /* TODO no same tile twice in one line */ ) {
        } else if ( /* TODO has game finished */ ) {}

        board->placeTile(tile, row, column);
        GameManager::switchPlayer();
    } catch (...) {
        state = PLACE_FAILURE;
    }

    IOHandler::notify(message, state);
}

void GameManager::replaceTile(Colour colour, Shape shape) {
    try {
        player1->getHand()->replaceTile({tileCode.at(0), tileCode.at(1)}, *bag);

        GameManager::switchPlayer();
    } catch (...) {
        // TODO notify input/printer class of invalid location(/tileCode?)
    }

    IOHandler::notify(message, State);
}

void GameManager::switchPlayer() {
    currentPlayer = (*currentPlayer == *player1) ? player2 : player1;
}

unsigned int GameManager::calculateScore(const Tile& playedTile, int row, int column) {
    int score = 0;
    bool qwirkle = true;

    int rowStart = row;
    int columnStart = column;
    // Finding start places for score counting
    bool endReached = false;    
    for (int i = row - 1; i <= 0; i--) {
        if (!endReached)
            if (board->at(i, column) != nullptr) {
                rowStart = i;
            } else {
                endReached = true;
            }
                
    }

    endReached = false;
    for (int j = column - 1; j <= 0; j--) {
        if (!endReached)
            if (board->at(row, j) != nullptr) {
                columnStart = j;
            } else {
                endReached = true;
            }
                
    }

    // Counting up score
    for (int i = rowStart; i < rowStart + 6; i++) {
        if (qwirkle) {
            shared_ptr<Tile> current = board->at(i, column);
            if (current != nullptr) {
                if (current->hasSameColour(playedTile) || current->hasSameShape(playedTile)) {
                    score++;
                } 
            } else {
                qwirkle = false;
            }
        }
    }
    if (qwirkle) score += 6;

    qwirkle = true;
    for (int j = columnStart; j < columnStart + 6; j++) {
        if (qwirkle) {
            shared_ptr<Tile> current = board->at(row, j);
            if (current != nullptr) {
                if (current->hasSameColour(playedTile) || current->hasSameShape(playedTile)) {
                    score++;
                } 
            } else {
                qwirkle = false;
            }
        }
    }
    if (qwirkle) score += 6;

    return score;
}

void GameManager::reset() {
    board.reset();
    bag.reset();
    player1.reset();
    player2.reset();
    currentPlayer.reset();
}

bool GameManager::isGridLocationEmpty(int row, int column) { return false; }
bool GameManager::isTileInHand(const Tile& tile) { return false; }
bool GameManager::isTileSimilar(const Tile& tile) { return false; }
bool GameManager::tileUniqueOnLine(const Tile& tile) { return false; }
bool GameManager::doesLineExceedSix(int row, int column) { return false; }
bool GameManager::hasGameEnded() { return false; }
