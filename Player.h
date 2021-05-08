
#ifndef PLAYER_H
#define PLAYER_H

#include "PlayerHand.h"

#include <string>

using std::string;

class Player {
    Player(const string& name);
    string getName();
    int getScore();
    shared_ptr<PlayerHand> getHand();
    friend bool operator==(const Player& player1, const Player& player2);

private:
    string name;
    int score;
    shared_ptr<PlayerHand> hand;
};

#endif // !PLAYER_H
