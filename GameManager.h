
#ifndef GAME_MANAGER_H
#define GAME_MANAGER_H

#define UP 0
#define DOWN 1
#define LEFT 2
#define RIGHT 3

#include "GameBoard.h"
#include "Player.h"

enum State {
    PLACE_SUCCESS, PLACE_FAILURE, REPLACE_SUCCESS, REPLACE_FAILURE, GAME_OVER, QWIRKLE
};

class GameManager {
public:
    static void beginGame(const string& player1Name, const string& player2Name);

    static void placeTile(Colour colour, Shape shape, int row, int column);

    static void replaceTile(Colour colour, Shape shape);

    static void switchPlayer();

    static unsigned int calculateScore(const Tile& playedTile, int row, int column);

    static bool isGridLocationEmpty(int row, int column);

    static bool isTileInHand(const Tile& tile);

    static bool hasAdjacentTile(const Tile& tile, int row, int column);

    static bool isTileValidOnLine(const Tile& tile, int row, int column);

    static bool hasGameEnded();

    static void reset();

    static bool gameBegan;
    static shared_ptr<TileBag> bag;
    static shared_ptr<Player> player1;
    static shared_ptr<Player> player2;
    static shared_ptr<Player> currentPlayer;
    static shared_ptr<GameBoard> board;
};

#endif // !GAME_MANAGER_H
