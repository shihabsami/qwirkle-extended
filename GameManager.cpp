#include "GameManager.h"
#include "IOHandler.h"
#include "Constants.h"

#include <sstream>
#include <algorithm>

using std::get;
using std::move;
using std::make_pair;
using std::make_tuple;
using std::ostringstream;
using std::invalid_argument;
using std::out_of_range;

/// Initialise the static variables
vector<shared_ptr<Player>> GameManager::players;
int GameManager::currentPlayerIndex = FIRST_POSITION;
shared_ptr<TileBag> GameManager::bag = nullptr;
shared_ptr<GameBoard> GameManager::board = nullptr;
unordered_map<shared_ptr<Tile>, Location> GameManager::tileRegister;

void GameManager::beginGame(const vector<string>& playerNames) {
    board = make_shared<GameBoard>();
    bag = make_shared<TileBag>();
    bag->fill();
    bag->shuffle();
    for (const string& name : playerNames)
        players.emplace_back(make_shared<Player>(name, bag->getHand()));
}

void GameManager::loadGame(vector<shared_ptr<Player>>& loadedPlayers,
    const string& currentPlayerName, shared_ptr<GameBoard>& loadedBoard,
    shared_ptr<TileBag>& loadedBag) {
    board = move(loadedBoard);
    bag = move(loadedBag);
    for (size_t i = 0; i < BOARD_LENGTH; ++i)
        for (size_t j = 0; j < BOARD_LENGTH; ++j)
            if (board->at(i, j) != nullptr)
                tileRegister[board->at(i, j)] = {i, j};

    players = move(loadedPlayers);
    for (size_t i = 0; i < players.size(); ++i)
        if (players.at(i)->getName() == currentPlayerName)
            GameManager::currentPlayerIndex = (int)i;
}

void GameManager::placeTile(
    Colour colour, Shape shape, size_t row, size_t column) {
    string message = "Tile could not be placed";
    State state;

    try {
        Tile tile(colour, shape);
        if (!isTileInHand(tile)) {
            message = IOHandler::invalidInputEnabled
                ? "The specified tile is not present in hand"
                : message;
            throw invalid_argument("");
        } else if (!isGridLocationEmpty(row, column)) {
            message = IOHandler::invalidInputEnabled
                ? "A tile is already present in the provided grid location"
                : message;
            throw invalid_argument("");
        }

        Lines lines = getAdjacentLines(tile, row, column);
        if (!board->isEmpty() && !hasAdjacentTile(tile, lines)) {
            message = IOHandler::invalidInputEnabled
                ? "No adjacent tile to form line"
                : message;
            throw invalid_argument("");
        } else if (!isTileValidOnLine(tile, lines)) {
            message = IOHandler::invalidInputEnabled
                ? "Tile violates line rules"
                : message;
            throw invalid_argument("");
        }

        // points for the first round
        if (board->isEmpty())
            getCurrentPlayer()->setScore(1);

        updateScore(lines);
        shared_ptr<Tile> playedTile =
            getCurrentPlayer()->getHand()->playTile(tile);

        board->placeTile(playedTile, row, column);
        tileRegister[playedTile] = {row, column};
        message = "Tile placed successfully";
        state = PLACE_SUCCESS;

        if (!bag->getTiles()->isEmpty()) {
            getCurrentPlayer()->getHand()->addTile(
                bag->getTiles()->at(FIRST_POSITION));
            bag->getTiles()->removeFront();
        }

        if (IOHandler::aiEnabled &&
            getCurrentPlayer()->getName() == IOHandler::aiName) {
            ostringstream stream;
            stream << "AI played ";
            tile.print(stream, IOHandler::colourEnabled);
            message = stream.str();
        }

        GameManager::switchPlayer();
    } catch (...) {
        state = PLACE_FAILURE;
    }

    IOHandler::notify(message, state);
    if (hasGameEnded())
        IOHandler::notify("", GAME_OVER);
}

void GameManager::replaceTile(Colour colour, Shape shape) {
    string message = "Tile could not be replaced";
    State state;

    try {
        Tile tile(colour, shape);
        if (!isTileInHand(tile)) {
            message = IOHandler::invalidInputEnabled
                ? "The specified tile is not present in hand"
                : message;
            throw invalid_argument("");
        } else if (board->isEmpty()) {
            message = IOHandler::invalidInputEnabled
                ? "Must place a tile on the first move"
                : message;
            throw invalid_argument("");
        }

        // dead end prevention
        if (getPossibleMoves().empty()) {
            message = IOHandler::invalidInputEnabled
                ? "No more moves possible"
                : message;

            IOHandler::notify(message, REPLACE_SUCCESS);
            IOHandler::notify(message, GAME_OVER);
        }

        if (!bag->getTiles()->isEmpty()) {
            getCurrentPlayer()->getHand()->replaceTile(tile, *bag);
            message = "Tile replaced successfully";
            state = REPLACE_SUCCESS;
            GameManager::switchPlayer();
        } else {
            message = IOHandler::invalidInputEnabled
                ? "No more tiles remain to be replaced"
                : message;

            throw out_of_range("");
        }
    } catch (...) {
        state = REPLACE_FAILURE;
    }

    IOHandler::notify(message, state);
}

void GameManager::switchPlayer() {
    currentPlayerIndex = (currentPlayerIndex + 1) % IOHandler::numberOfPlayers;
}

shared_ptr<Player> GameManager::getCurrentPlayer() {
    return players.at(currentPlayerIndex);
}

bool GameManager::isGridLocationEmpty(size_t row, size_t column) {
    return board->at(row, column) == nullptr;
}

bool GameManager::isTileInHand(const Tile& tile) {
    return getCurrentPlayer()->getHand()->getTiles()->contains(tile);
}

Lines GameManager::getAdjacentLines(
    const Tile& tile, size_t row, size_t column) {
    Lines lines{LinkedList{}, LinkedList{}};

    size_t currentRow = row;
    size_t currentColumn = column;
    int currentDirection = DIRECTION_UP;
    bool allDirectionTraversed = false;

    // only traverse as many times as there are tiles in all directions
    while (!allDirectionTraversed) {
        if (currentDirection == DIRECTION_UP)
            --currentRow;
        else if (currentDirection == DIRECTION_DOWN)
            ++currentRow;
        else if (currentDirection == DIRECTION_LEFT)
            --currentColumn;
        else if (currentDirection == DIRECTION_RIGHT)
            ++currentColumn;

        try {
            shared_ptr<Tile> other = board->at(currentRow, currentColumn);

            /*
             * if location is empty (nullptr), proceed to traverse a different
             * direction else keep track of traversed tiles
             */
            if (other != nullptr) {
                if (currentDirection == DIRECTION_UP ||
                    currentDirection == DIRECTION_DOWN)
                    lines.vertical.addBack(other);
                else
                    lines.horizontal.addBack(other);
            } else {
                throw out_of_range("");
            }
        }
        // exception may be thrown by board if the coordinates are out of bounds
        catch (out_of_range& exception) {
            currentRow = row;
            currentColumn = column;
            ++currentDirection;
            allDirectionTraversed = currentDirection > DIRECTION_RIGHT;
        }
    }

    return lines;
}

bool GameManager::hasAdjacentTile(const Tile& tile, const Lines& lines) {
    return !lines.horizontal.isEmpty() || !lines.vertical.isEmpty();
}

bool GameManager::isTileValidOnLine(const Tile& tile, const Lines& lines) {
    const LinkedList& horizontalTiles = lines.horizontal;
    const LinkedList& verticalTiles = lines.vertical;

    bool tileUnique =
        !horizontalTiles.contains(tile) && !verticalTiles.contains(tile);

    /*
     * check whether tile shares similarity with the other tiles on both the
     * horizontal and vertical lines, is only traversed as long as one of the
     * similarities remain, if more than 2 comparisons are made and conditions
     * still hold then no further iteration is made as the tile cannot have
     * dissimilarities any longer, the large number of conditions are there to
     * ensure that the algorithm is as efficient as could be
     */
    bool hasSameColourAsLine = true;
    bool hasSameShapeAsLine = true;

    for (size_t i = 0; i < horizontalTiles.size() &&
         (hasSameColourAsLine || hasSameShapeAsLine) && i < 2;
         ++i) {
        hasSameColourAsLine &= tile.hasSameColour(*horizontalTiles.at(i));
        hasSameShapeAsLine &= tile.hasSameShape(*horizontalTiles.at(i));
    }

    bool matchHorizontalLine = hasSameColourAsLine || hasSameShapeAsLine;
    hasSameColourAsLine = true;
    hasSameShapeAsLine = true;

    // only iterate if the tile matched the horizontal line above
    for (size_t i = 0; i < verticalTiles.size() && matchHorizontalLine &&
         (hasSameColourAsLine || hasSameShapeAsLine) && i < 2;
         ++i) {
        hasSameColourAsLine &= tile.hasSameColour(*verticalTiles.at(i));
        hasSameShapeAsLine &= tile.hasSameShape(*verticalTiles.at(i));
    }

    bool matchVerticalLine = hasSameColourAsLine || hasSameShapeAsLine;

    return tileUnique && matchHorizontalLine && matchVerticalLine &&
        horizontalTiles.size() < MAX_LINE_SIZE &&
        verticalTiles.size() < MAX_LINE_SIZE;
}

void GameManager::updateScore(const Lines& lines) {
    // qwirkle is printed twice if it happens twice on the same move
    if (lines.horizontal.size() + 1 == MAX_LINE_SIZE) {
        IOHandler::notify("QWIRKLE!!!", QWIRKLE);
    }
    if (lines.vertical.size() + 1 == MAX_LINE_SIZE) {
        IOHandler::notify("QWIRKLE!!!", QWIRKLE);
    }

    getCurrentPlayer()->setScore(
        getCurrentPlayer()->getScore() + calculateScore(lines));
}

size_t GameManager::calculateScore(const Lines& lines) {
    size_t horizontalScore =
        lines.horizontal.isEmpty() ? 0 : lines.horizontal.size() + 1;
    size_t verticalScore =
        lines.vertical.isEmpty() ? 0 : lines.vertical.size() + 1;

    // bonus points for emptying hand or scoring qwirkle
    int bonusCount = 0;
    if (getCurrentPlayer()->getHand()->getTiles()->size() - 1 == 0)
        ++bonusCount;
    if (horizontalScore == MAX_LINE_SIZE)
        ++bonusCount;
    if (verticalScore == MAX_LINE_SIZE)
        ++bonusCount;

    return horizontalScore + verticalScore + SCORE_BONUS * bonusCount;
}

vector<Move> GameManager::getPossibleMoves() {
    vector<Move> moves;
    shared_ptr<LinkedList> handTiles =
        players.at(currentPlayerIndex)->getHand()->getTiles();

    // for each tile in hand
    for (size_t i = 0; i < handTiles->size(); ++i) {
        shared_ptr<Tile> tileToBePlaced = handTiles->at(i);

        // for each tileToBePlaced already present on the board
        for (const auto& pair : tileRegister) {
            shared_ptr<Tile> tileComparingAgainst = pair.first;
            size_t row = pair.second.row;
            size_t column = pair.second.column;

            // proceed only if tileToBePlaced shares similarity with the
            // tileToBePlaced to be placed
            if (pair.first->hasSameColour(*tileToBePlaced) ||
                pair.first->hasSameShape(*tileToBePlaced)) {
                size_t currentRow = row;
                size_t currentColumn = column;

                int currentDirection = DIRECTION_UP;
                bool allDirectionTraversed = false;

                while (!allDirectionTraversed) {
                    if (currentDirection == DIRECTION_UP)
                        --currentRow;
                    else if (currentDirection == DIRECTION_DOWN)
                        ++currentRow;
                    else if (currentDirection == DIRECTION_LEFT)
                        --currentColumn;
                    else if (currentDirection == DIRECTION_RIGHT)
                        ++currentColumn;

                    try {
                        if (board->at(currentRow, currentColumn) == nullptr) {
                            Lines lines = getAdjacentLines(
                                *tileToBePlaced, currentRow, currentColumn);

                            if (isTileValidOnLine(*tileToBePlaced, lines)) {
                                bool unique = true;
                                for (const auto& move : moves)
                                    if (move.location.row == currentRow &&
                                        move.location.column == currentColumn)
                                        unique = false;

                                if (unique) {
                                    moves.emplace_back(Move{tileToBePlaced,
                                        {currentRow, currentColumn},
                                        calculateScore(lines)});
                                }
                            }
                        }
                    } catch (out_of_range& exception) {
                    }

                    ++currentDirection;
                    allDirectionTraversed = currentDirection > DIRECTION_RIGHT;
                    currentRow = row;
                    currentColumn = column;
                }
            }
        }
    }

    return moves;
}

bool GameManager::hasGameEnded() {
    bool anyPlayerHandEmpty = false;
    for (const auto& player : players)
        anyPlayerHandEmpty |= player->getHand()->getTiles()->isEmpty();

    return anyPlayerHandEmpty && bag->getTiles()->isEmpty();
}

void GameManager::resetGame() {
    players.clear();
    bag.reset();
    board.reset();
    tileRegister.clear();
}
