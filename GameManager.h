
#ifndef GAME_MANAGER_H
#define GAME_MANAGER_H

#define UP 0
#define DOWN 1
#define LEFT 2
#define RIGHT 3

#include "GameBoard.h"
#include "Player.h"

#include <utility>

using std::pair;
using std::make_pair;

typedef pair<LinkedList, LinkedList> Lines;

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

    static void loadGame(shared_ptr<Player> p1, shared_ptr<Player> p2, shared_ptr<TileBag> loadedBag, shared_ptr<GameBoard> loadedBoard, shared_ptr<Player> currentP);

    static void placeTile(Colour colour, Shape shape, int row, int column);

    static void replaceTile(Colour colour, Shape shape);

    static void switchPlayer();

    static bool isGridLocationEmpty(int row, int column);

    static bool isTileInHand(const Tile& tile);

    static Lines getAdjacentLines(const Tile& tile, int row, int column);

    static bool hasAdjacentTile(const Tile& tile, const Lines& lines);

    static bool isTileValidOnLine(const Tile& tile, const Lines& lines);

    static void updateScore(const Lines& lines);

    static bool hasGameEnded();

    static void resetGame();

    static bool gameBegan;
    static shared_ptr<TileBag> bag;
    static shared_ptr<Player> player1;
    static shared_ptr<Player> player2;
    static shared_ptr<Player> currentPlayer;
    static shared_ptr<GameBoard> board;
};

#endif // !GAME_MANAGER_H
