
#ifndef GAME_MANAGER_H
#define GAME_MANAGER_H

#define LINE_UP 0
#define LINE_DOWN 1
#define LINE_LEFT 2
#define LINE_RIGHT 3

#include "GameBoard.h"
#include "TileBag.h"
#include "Player.h"

enum State {
    PLACE_SUCCESS, PLACE_FAILURE, REPLACE_SUCCESS, REPLACE_FAILURE, GAME_OVER
};

class GameManager {
public: 
    static void beginGame(const string& player1Name, const string& player2Name);

    static void placeTile(Colour colour, Shape shape, int row, int column);

    static void replaceTile(Colour colour, Shape shape);

    static void switchPlayer();

    static unsigned int calculateScore(const Tile& playedTile, int row, int column);

    static void reset();

    static bool isGridLocationEmpty(int row, int column);

    static bool isTileInHand(const Tile& tile);

    static bool isTileSimilar(const Tile& tile, int row, int column);

    static bool tileValidOnLine(const Tile& tile, int row, int column);

    static bool hasGameEnded();

    static shared_ptr<GameBoard> board;
    static shared_ptr<TileBag> bag;
    static shared_ptr<Player> player1;
    static shared_ptr<Player> player2;
    static shared_ptr<Player> currentPlayer;
};

#endif // !GAME_MANAGER_H
