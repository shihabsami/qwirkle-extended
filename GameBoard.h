
#ifndef GAME_BOARD_H
#define GAME_BOARD_H

#include "Tile.h"

#include <memory>
#include <vector>
#include <fstream>

using std::ofstream;
using std::vector;
using std::shared_ptr;
using std::make_shared;

class GameBoard {
public:
    GameBoard();
    ~GameBoard();
    void placeTile(
        const shared_ptr<Tile>& tile, unsigned int row, unsigned int column);
    shared_ptr<Tile> at(unsigned int row, unsigned int column);
    bool isEmpty() const;
    friend ostream& operator<<(ostream& os, const GameBoard& board);
    friend ostream& operator<<(ofstream& ofs, const GameBoard& board);

private:
    unsigned int tileCount;
    vector<vector<shared_ptr<Tile>>> board;
};

#endif // !GAME_BOARD_H
