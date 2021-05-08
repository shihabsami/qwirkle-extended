
#ifndef GAME_MANAGER_H
#define GAME_MANAGER_H

#include "GameBoard.h"
#include "TileBag.h"
#include "Player.h"

#include <string>

using std::string;

class GameManager {
    GameManager();
    void beginGame(const shared_ptr<Player>& player1, const shared_ptr<Player>& player2);

    // TODO figure out communication between this and input handler
    void placeTile(string tileCode, string gridLocation);

    void replaceTile(string tileCode);

    void switchPlayer();

private:
    shared_ptr<GameBoard> board;
    shared_ptr<TileBag> bag;
    shared_ptr<Player> player1;
    shared_ptr<Player> player2;
    shared_ptr<Player> currentPlayer;
};

#endif // !GAME_MANAGER_H
