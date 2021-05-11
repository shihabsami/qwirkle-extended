#include "GameManager.h"
#include "Constants.h"
#include "IOHandler.h"

#include <iostream>
#include <string>

using std::invalid_argument;
using std::out_of_range;

shared_ptr<GameBoard> GameManager::board = nullptr;
shared_ptr<TileBag> GameManager::bag = nullptr;
shared_ptr<Player> GameManager::player1 = nullptr;
shared_ptr<Player> GameManager::player2 = nullptr;
shared_ptr<Player> GameManager::currentPlayer = nullptr;
unsigned int GameManager::roundsPlayed = 0;

void GameManager::beginGame(
    const string& player1Name, const string& player2Name) {
    bag = make_shared<TileBag>();
    bag->shuffle();
    player1 = make_shared<Player>(player1Name, bag->getHand());
    player2 = make_shared<Player>(player2Name, bag->getHand());
    currentPlayer = player1;
    board = make_shared<GameBoard>();
    roundsPlayed = 0;
}

void GameManager::placeTile(Colour colour, Shape shape, int row, int column) {
    string message = "Tile placed successfully.";
    State state = PLACE_SUCCESS;

    try {
        shared_ptr<Tile> tile = make_shared<Tile>(colour, shape);
        if (!isTileInHand(*tile)) {
            message = "Tile is not present in hand.";
            throw invalid_argument("");
        } else if (!isGridLocationEmpty(row, column)) {
            message ="A tile is already present in the provided grid location.";
            throw invalid_argument("");
        } else if (roundsPlayed > 0 && !hasAdjacentTile(*tile, row, column)) {
            message = "No adjacent tile to form line.";
            throw invalid_argument("");
        }

//  else if (!tileValidOnLine(*tile, row, column)) {
//            message = "Tile violates line rules.";
//            throw invalid_argument("");
//        }

        board->placeTile(tile, row, column);
        GameManager::switchPlayer();
        ++roundsPlayed;
        // std::cout << "roundsPlayed - " << roundsPlayed << ", currentPlayer - " << currentPlayer->getName() << std::endl;
    } catch (...) {
        state = PLACE_FAILURE;
    }

    IOHandler::notify(message, state);

    if (hasGameEnded()) IOHandler::notify("", GAME_OVER);
}

void GameManager::replaceTile(Colour colour, Shape shape) {
    string message = "Tile placed successfully.";
    State state = REPLACE_SUCCESS;

    try {
        shared_ptr<Tile> tile = make_shared<Tile>(colour, shape);
        if (!isTileInHand(*tile)) {
            message = "Tile is not present in hand.";
            throw invalid_argument("");
        }

        player1->getHand()->replaceTile({colour, shape}, *bag);
        GameManager::switchPlayer();
    } catch (...) {
        state = REPLACE_FAILURE;
    }

    IOHandler::notify(message, state);
}

void GameManager::switchPlayer() {
    currentPlayer = (*currentPlayer == *player1) ? player2 : player1;
}

unsigned int GameManager::calculateScore(
    const Tile& playedTile, int row, int column) {
    int score = 0;
    bool qwirkle = true;

    int rowStart = row;
    int columnStart = column;

    // Finding start places for score counting
    bool endReached = false;
    for (int i = row - 1; i <= 0; i--) {
        if (!endReached) {
            if (board->at(i, column) != nullptr) {
                rowStart = i;
            } else {
                endReached = true;
            }
        }
    }

    endReached = false;
    for (int j = column - 1; j <= 0; j--) {
        if (!endReached) {
            if (board->at(row, j) != nullptr) {
                columnStart = j;
            } else {
                endReached = true;
            }
        }
    }

    // Counting up score
    for (int i = rowStart; i < rowStart + 6; i++) {
        if (qwirkle) {
            shared_ptr<Tile> current = board->at(i, column);
            if (current != nullptr) {
                if (current->hasSameColour(playedTile) ||
                    current->hasSameShape(playedTile)) {
                    score++;
                }
            } else {
                qwirkle = false;
            }
        }
    }
    if (qwirkle)
        score += SCORE_BONUS;

    qwirkle = true;
    for (int j = columnStart; j < columnStart + 6; j++) {
        if (qwirkle) {
            shared_ptr<Tile> current = board->at(row, j);
            if (current != nullptr) {
                if (current->hasSameColour(playedTile) ||
                    current->hasSameShape(playedTile)) {
                    score++;
                }
            } else {
                qwirkle = false;
            }
        }
    }
    if (qwirkle)
        score += SCORE_BONUS;

    return score;
}

bool GameManager::isGridLocationEmpty(int row, int column) {
    return board->at(row, column) == nullptr;
}

bool GameManager::isTileInHand(const Tile& tile) {
    return currentPlayer->getHand()->getTiles()->contains(tile);
}

bool GameManager::isTileSimilar(const Tile& tile, int row, int column) {
    // TODO
    return false;
}

bool GameManager::tileValidOnLine(const Tile& tile, int row, int column) {
    bool tileUnique = true;

    int currentRow = row;
    int currentColumn = column;
    int currentDirection = UP;
    bool allDirectionTraversed = false;

    int horizontalLineCount = 0;
    int verticalLineCount = 0;

    while (tileUnique && !allDirectionTraversed) {
        if (currentDirection == UP) {
            --currentRow;
        } else if (currentDirection == DOWN) {
            ++currentRow;
        } else if (currentDirection == LEFT) {
            --currentColumn;
        } else if (currentDirection == RIGHT) {
            ++currentColumn;
        } else {
            allDirectionTraversed = true;
        }

        try {
            if (board->at(currentRow, currentColumn) == nullptr) {
                currentRow = row;
                currentColumn = column;
                ++currentDirection;
            } else if (*board->at(currentRow, currentColumn) == tile) {
                tileUnique = false;
            } else {
                if (currentDirection == UP || currentDirection == DOWN)
                    ++verticalLineCount;
                else
                    ++horizontalLineCount;
            }
        } catch (out_of_range& exception) {
            currentRow = row;
            currentColumn = column;
            ++currentDirection;
        }
    }

    std::cout << "hc " << horizontalLineCount << " vc " << verticalLineCount
              << std::endl;

    return tileUnique && horizontalLineCount < MAX_LINE_SIZE &&
        verticalLineCount < MAX_LINE_SIZE;
}

bool GameManager::hasGameEnded() {
    return (player1->getHand()->getTiles()->isEmpty() ||
               player2->getHand()->getTiles()->isEmpty()) &&
        bag->getTiles()->isEmpty();
}

void GameManager::reset() {
    board.reset();
    bag.reset();
    player1.reset();
    player2.reset();
    currentPlayer.reset();
}
bool GameManager::hasAdjacentTile(const Tile& tile, int row, int column) {
    bool hasAdjacent = false;

    int currentRow = row;
    int currentColumn = column;
    int currentDirection = 0;
    while (currentDirection < 4 && !hasAdjacent) {
        if (currentDirection == UP)
            --currentRow;
        else if (currentDirection == DOWN)
            ++currentRow;
        else if (currentDirection == LEFT)
            --currentColumn;
        else if (currentDirection == RIGHT)
            ++currentColumn;

        try {
            hasAdjacent = board->at(currentRow, currentColumn) != nullptr;
        } catch (out_of_range& exception) {}

        currentRow = row;
        currentColumn = column;
        ++currentDirection;
    }

    return hasAdjacent;
}
