
#ifndef GAME_MANAGER_H
#define GAME_MANAGER_H

#include "GameBoard.h"
#include "TileBag.h"
#include "Player.h"

class GameManager {
    static void beginGame(const string& player1Name, const string& player2Name);

    static void placeTile(string tileCode, string gridLocation);

    static void replaceTile(string tileCode);

    static void switchPlayer();

    // TODO method signature takes player? returns score?
    static unsigned int calculateScore(const Tile& playedTile);

    static shared_ptr<GameBoard> board;
    static shared_ptr<TileBag> bag;
    static shared_ptr<Player> player1;
    static shared_ptr<Player> player2;
    static shared_ptr<Player> currentPlayer;
};

#endif // !GAME_MANAGER_H
