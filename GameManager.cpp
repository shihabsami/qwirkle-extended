#include "GameManager.h"
#include "IOHandler.h"
#include "Constants.h"

using std::invalid_argument;
using std::out_of_range;

unsigned int GameManager::roundsPlayed = 0;
shared_ptr<TileBag> GameManager::bag = nullptr;
shared_ptr<Player> GameManager::player1 = nullptr;
shared_ptr<Player> GameManager::player2 = nullptr;
shared_ptr<Player> GameManager::currentPlayer = nullptr;
shared_ptr<GameBoard> GameManager::board = nullptr;

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

/**
 * Place a tile on to the board from the current player's hand.
 *
 * @param colour,shape - the tile attributes
 * @param row, column - the specified grid location
 */
void GameManager::placeTile(Colour colour, Shape shape, int row, int column) {
    string message = "Tile placed successfully.";
    State state = PLACE_SUCCESS;

    try {
        Tile tile(colour, shape);
        if (!isTileInHand(tile)) {
            message = "The specified tile is not present in hand.";
            throw invalid_argument("");
        } else if (!isGridLocationEmpty(row, column)) {
            message =
                "A tile is already present in the provided grid location.";
            throw invalid_argument("");
        } else if (roundsPlayed > 0 && !hasAdjacentTile(tile, row, column)) {
            message = "No adjacent tile to form line.";
            throw invalid_argument("");
        } else if (!isTileValidOnLine(tile, row, column)) {
            message = "Tile violates line rules.";
            throw invalid_argument("");
        }

        board->placeTile(currentPlayer->getHand()->playTile(tile), row, column);
        currentPlayer->getHand()->addTile(bag->getRandomTile());
        currentPlayer->setScore(calculateScore(tile, row, column));
        GameManager::switchPlayer();
        ++roundsPlayed;
    } catch (...) {
        state = PLACE_FAILURE;
    }

    IOHandler::notify(message, state);
    if (hasGameEnded())
        IOHandler::notify("", GAME_OVER);
}

/**
 * Replace the tile from the current player's hand.
 *
 * @param colour,shape - the tile attributes
 */
void GameManager::replaceTile(Colour colour, Shape shape) {
    string message = "Tile replaced successfully.";
    State state = REPLACE_SUCCESS;

    try {
        Tile tile(colour, shape);
        if (!isTileInHand(tile)) {
            message = "The specified tile is not present in hand.";
            throw invalid_argument("");
        }

        currentPlayer->getHand()->replaceTile(tile, *bag);
        GameManager::switchPlayer();
        ++roundsPlayed;
    } catch (...) {
        state = REPLACE_FAILURE;
    }

    IOHandler::notify(message, state);
}

/**
 * Switch player at the end of a round.
 */
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

/**
 * Check if location in board is empty for tile being placed.
 * @param row,column - the specified grid location
 */
bool GameManager::isGridLocationEmpty(int row, int column) {
    return board->at(row, column) == nullptr;
}

/**
 * Check if the player hand contains a tile.
 * @param tile - the tile to be checked
 */
bool GameManager::isTileInHand(const Tile& tile) {
    return currentPlayer->getHand()->getTiles()->contains(tile);
}

/**
 * Check the surrounding grid locations if an adjacent tile is present to be
 * able to form a line.
 *
 * @param tile - the tile to be checked
 * @param row,column - the specified grid location
 *
 * @note Off the top this method may seem very similar to the following
 * \b GameManager::isValidOnTile method, however this only traverses the next
 * cell in each directions, whereas the latter traverses as long as there are
 * non-empty locations in each direction
 */
bool GameManager::hasAdjacentTile(const Tile& tile, int row, int column) {
    bool hasAdjacent = false;

    int currentRow = row;
    int currentColumn = column;
    int currentDirection = 0;

    // traverse the 4 surrounding cells, stop if an adjacent tile is found
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
        }
        // exception may be thrown by board if the coordinates are out of bounds
        catch (out_of_range& exception) {
        }

        currentRow = row;
        currentColumn = column;
        ++currentDirection;
    }

    return hasAdjacent;
}

/**
 * Check if the tile being placed violate the following rules.
 * - No more than 6 tiles per line.
 * - No same tile more than once per line.
 * - Tile has either same colour or shape with both the horizontal and vertical
 *   lines.
 *
 * @param tile - the tile to be placed
 * @param row,column - the specified grid location
 * @return boolean indicating whether the tile can be placed on line
 */
bool GameManager::isTileValidOnLine(const Tile& tile, int row, int column) {
    bool tileUnique = true;
    LinkedList horizontalTiles{};
    LinkedList verticalTiles{};

    int currentRow = row;
    int currentColumn = column;
    int currentDirection = UP;
    bool allDirectionTraversed = false;

    // only traverse as many times as there are tiles in all directions
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
            shared_ptr<Tile> other = board->at(currentRow, currentColumn);

            /*
             * if location is empty (nullptr), proceed to traverse a different
             * direction, else if location is not empty and the tile present at
             * the location is the same then tile is no longer unique
             */
            if (!other) {
                currentRow = row;
                currentColumn = column;
                ++currentDirection;
            } else if (*other == tile) {
                tileUnique = false;
            } else {
                // keep track of traversed tiles for further operations
                if (currentDirection == UP || currentDirection == DOWN)
                    verticalTiles.addBack(other);
                else
                    horizontalTiles.addBack(other);
            }
        }
        // exception may be thrown by board if the coordinates are out of bounds
        catch (out_of_range& exception) {
            currentRow = row;
            currentColumn = column;
            ++currentDirection;
        }
    }

    std::cout << "h(" << horizontalTiles.size() << ") - " << horizontalTiles
              << " v(" << verticalTiles.size() << ") - " << verticalTiles
              << std::endl;

    /*
     * check whether tile shares similarity with the other tiles on both the
     * horizontal and vertical lines, is only traversed as long as one of the
     * similarities remain, if more than 2 comparisons are made and conditions
     * still hold then no further iteration is made as the tile cannot have
     * dissimilarities any longer
     */
    bool hasSameColourAsLine = true;
    bool hasSameShapeAsLine = true;

    for (unsigned int i = 0; i < horizontalTiles.size() &&
         (hasSameColourAsLine || hasSameShapeAsLine) && i < 2;
         ++i) {
        hasSameColourAsLine &= tile.hasSameColour(*horizontalTiles.at(i));
        hasSameShapeAsLine &= tile.hasSameShape(*horizontalTiles.at(i));
    }

    bool matchHorizontalLine = hasSameColourAsLine || hasSameShapeAsLine;

    for (unsigned int i = 0;
         i < verticalTiles.size() && i < 2 && matchHorizontalLine; ++i) {
        hasSameColourAsLine &= tile.hasSameColour(*verticalTiles.at(i));
        hasSameShapeAsLine &= tile.hasSameShape(*verticalTiles.at(i));
    }

    bool matchVerticalLine = hasSameColourAsLine || hasSameShapeAsLine;

    return tileUnique && matchHorizontalLine && matchVerticalLine &&
        horizontalTiles.size() < MAX_LINE_SIZE &&
        verticalTiles.size() < MAX_LINE_SIZE;
}

/**
 * Check if conditions hold for game ending.
 */
bool GameManager::hasGameEnded() {
    return (player1->getHand()->getTiles()->isEmpty() ||
               player2->getHand()->getTiles()->isEmpty()) &&
        bag->getTiles()->isEmpty();
}

/**
 * Resets the data structures for a new game.
 */
void GameManager::reset() {
    board.reset();
    bag.reset();
    player1.reset();
    player2.reset();
    currentPlayer.reset();
}
