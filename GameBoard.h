
#ifndef GAME_BOARD_H
#define GAME_BOARD_H

#define BOARD_LENGTH 26

#include "Tile.h"
#include <vector>

class GameBoard {
public:
    GameBoard();
    ~GameBoard();

private:
    std::vector<std::vector<Tile*>> board;
};

#endif // !GAME_BOARD_H
