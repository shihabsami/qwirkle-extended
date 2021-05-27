
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

// TODO comment these structs
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

/**
 * The various states in the game that assists in notifying the IOHandler.
 */
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
    /**
     * Initialise the board, tilebag and the players for the game.
     *
     * @param player1Name - name of the first player
     * @param player2name - name of the second player
     */
    static void beginGame(const vector<string>& playerNames);

    /**
     * Loads the GameManager data structures, from loaded data structures
     * received from the IOHandler.
     *
     * @param loadedPlayers - the players in the game
     * @param currentPlayerName - the current player's name
     * @param loadedBag - the loaded TileBag
     * @param loadedBoard - the loaded GameBoard
     */
    static void loadGame(vector<shared_ptr<Player>>& loadedPlayers,
        const string& currentPlayerName, shared_ptr<GameBoard>& loadedBoard,
        shared_ptr<TileBag>& loadedBag
        );

    /**
     * Place a tile on to the board from the current player's hand.
     *
     * @param colour,shape - the tile attributes
     * @param row,column - the specified grid location
     */
    static void placeTile(
        Colour colour, Shape shape, size_t row, size_t column);

    /**
     * Replace the tile from the current player's hand.
     *
     * @param colour,shape - the tile attributes
     */
    static void replaceTile(Colour colour, Shape shape);

    /**
     * Switch player at the end of a round.
     */
    static void switchPlayer();

    /**
     * Get the current player in the game.
     *
     * @return a pointer to the Player
     */
    static shared_ptr<Player> getCurrentPlayer();

    /**
     * Check if location in board is empty for tile being placed.
     *
     * @param row,column - the specified grid location
     */
    static bool isGridLocationEmpty(size_t row, size_t column);

    /**
     * Check if the player hand contains a tile.
     *
     * @param tile - the tile to be checked
     */
    static bool isTileInHand(const Tile& tile);

    /**
     * Gets the adjacent lines formed by the tile if placed at the specified
     * location.
     *
     * @param tile - the placed tile
     * @param row,column - the specified grid location
     * @return Lines (pair<LinkedList, LinkedList) for the horizontal and
     * vertical lines
     */
    static Lines getAdjacentLines(const Tile& tile, size_t row, size_t column);

    /**
     * Check the surrounding grid locations if an adjacent tile is present to be
     * able to form a line.
     *
     * @param tile - the tile to be checked
     * @param lines - the horizontal and vertical lines
     * @return boolean indicating whether the tile has adjacent neighbors
     */
    static bool hasAdjacentTile(const Tile& tile, const Lines& lines);

    /**
     * Check if the tile being placed violate the following rules.
     * - No more than 6 tiles per line.
     * - No same tile more than once per line.
     * - Tile has either same colour or shape with both the horizontal and
     * vertical lines.
     *
     * @param tile - the tile to be placed
     * @param lines - the horizontal and vertical lines
     * @return boolean indicating whether the tile can be placed on line
     */
    static bool isTileValidOnLine(const Tile& tile, const Lines& lines);

    /**
     * Calculate the score for a move, based on the adjacent lines formed.
     *
     * @param lines - the adjacent lines
     * @return the score as size_t
     */
    static size_t calculateScore(const Lines& lines);

    /**
     * Update the points for a round based on the formed lines.
     *
     * @param lines - the horizontal and vertical lines
     */
    static void updateScore(const Lines& lines);

    /**
     * Gets all the possible moves the player can player in a single round.
     *
     * @return a vector of Move structs
     */
    static vector<Move> getPossibleMoves();

    /**
     * Check if conditions hold for game ending.
     *
     * @return true if game can be ended
     */
    static bool hasGameEnded();

    /**
     * Resets the data structures for a new game.
     */
    static void resetGame();

    static vector<shared_ptr<Player>> players;
    static int currentPlayerIndex;
    static shared_ptr<TileBag> bag;
    static shared_ptr<GameBoard> board;
    static unordered_map<shared_ptr<Tile>, Location> tileRegister;
};

#endif // !GAME_MANAGER_H
