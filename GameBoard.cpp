#include "GameBoard.h"
#include "Constants.h"

#include <iomanip>
#include <iostream>

GameBoard::GameBoard() {
    for (int i = 0; i < BOARD_LENGTH; ++i) {
        board.push_back(vector<shared_ptr<Tile>>());
        board.at(i).resize(BOARD_LENGTH, nullptr);
    }
}

GameBoard::~GameBoard() {
    // TODO might be redundant
    for (int i = 0; i < BOARD_LENGTH; ++i)
        board.at(i).clear();
}

ostream& operator<<(ostream& os, const GameBoard& gameBoard) {
    // TODO top tier formatting
    os << std::left << std::setw(3) << " ";
    for (int i = 0; i < BOARD_LENGTH; ++i)
        os << std::setw(3) << i;
    os << std::endl;

    os << std::setw(3) << " ";
    for (int i = 0; i < BOARD_LENGTH * 3 - 1; ++i)
        os << "-";
    os << std::endl;

    for (int i = 0; i < BOARD_LENGTH; ++i) {
        os << (char)(i + ASCII_BEGIN) << " |";
        for (int j = 0; j < BOARD_LENGTH; ++j) {
            if (gameBoard.board.at(i).at(j))
                os << *gameBoard.board.at(i).at(j);
            else
                os << "  ";
            os << "|";
        }

        os << std::endl;
    }

    return os;
}

void GameBoard::placeTile(
    const shared_ptr<Tile>& tile, unsigned int row, unsigned int column) {
    board.at(column).at(row) = tile;
}
