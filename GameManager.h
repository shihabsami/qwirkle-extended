
#ifndef GAME_MANAGER_H
#define GAME_MANAGER_H

#include "GameBoard.h"
#include "Player.h"

#include <tuple>
#include <unordered_map>
#include <utility>

// Line calculation constants
#define DIRECTION_UP 0
#define DIRECTION_DOWN 1
#define DIRECTION_LEFT 2
#define DIRECTION_RIGHT 3

using std::pair;
using std::tuple;
using std::unordered_map;

struct Location {
    size_t row;
    size_t column;
};

struct Lines {
    LinkedList horizontal;
    LinkedList vertical;
};

struct Move {
    shared_ptr<Tile> tile;
    Location location;
    size_t points;
};

enum State {
    PLACE_SUCCESS,
    PLACE_FAILURE,
    REPLACE_SUCCESS,
    REPLACE_FAILURE,
    GAME_OVER,
    QWIRKLE
};

class GameManager {
public:
    static void beginGame(const string& player1Name, const string& player2Name);

    static void loadGame(const shared_ptr<Player>& p1,
        const shared_ptr<Player>& p2, const shared_ptr<TileBag>& loadedBag,
        const shared_ptr<GameBoard>& loadedBoard, const shared_ptr<Player>& cp);

    static void placeTile(
        Colour colour, Shape shape, size_t row, size_t column);

    static void replaceTile(Colour colour, Shape shape);

    static void switchPlayer();

    static bool isGridLocationEmpty(size_t row, size_t column);

    static bool isTileInHand(const Tile& tile);

    static Lines getAdjacentLines(const Tile& tile, size_t row, size_t column);

    static bool hasAdjacentTile(const Tile& tile, const Lines& lines);

    static bool isTileValidOnLine(const Tile& tile, const Lines& lines);

    static vector<Move> getPossibleMoves();

    static size_t calculateScore(const Lines& lines);

    static void updateScore(const Lines& lines);

    static bool hasGameEnded();

    static void resetGame();

    static shared_ptr<TileBag> bag;
    static shared_ptr<Player> player1;
    static shared_ptr<Player> player2;
    static shared_ptr<Player> currentPlayer;
    static shared_ptr<GameBoard> board;
    static shared_ptr<unordered_map<shared_ptr<Tile>, Location>> tileRegister;
};

#endif // !GAME_MANAGER_H
