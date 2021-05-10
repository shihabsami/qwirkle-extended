#include "GameBoard.h"
#include "Constants.h"

#include <iomanip>
#include <iostream>

using std::left;
using std::right;
using std::setw;
using std::endl;
using std::invalid_argument;
using std::out_of_range;

GameBoard::GameBoard() {
    for (int i = 0; i < BOARD_LENGTH; ++i) {
        board.push_back(vector<shared_ptr<Tile>>());
        board.at(i).resize(BOARD_LENGTH, nullptr);
    }
}

GameBoard::~GameBoard() {
    for (int i = 0; i < BOARD_LENGTH; ++i)
        board.at(i).clear();
}

ostream& operator<<(ostream& os, const GameBoard& gameBoard) {
    // TODO top tier formatting
    os << left << setw(3) << " ";
    for (int i = 0; i < BOARD_LENGTH; ++i)
        os << setw(3) << i;
    os << endl;

    os << setw(3) << " ";
    for (int i = 0; i < BOARD_LENGTH * 3 - 1; ++i)
        os << "-";
    os << endl;

    for (int i = 0; i < BOARD_LENGTH; ++i) {
        os << (char)(i + ASCII_BEGIN) << " |";
        for (int j = 0; j < BOARD_LENGTH; ++j) {
            if (gameBoard.board.at(i).at(j))
                os << *gameBoard.board.at(i).at(j);
            else
                os << "  ";
            os << "|";
        }

        os << endl;
    }

    return os;
}

void GameBoard::placeTile(
    const shared_ptr<Tile>& tile, unsigned int row, unsigned int column) {
    board.at(row).at(column) = tile;
}
