
#include "Player.h"

Player::Player(const string& name, shared_ptr<PlayerHand> hand)
    : name(name), hand(hand) {}

string Player::getName() { return name; }

unsigned int Player::getScore() { return score; }

shared_ptr<PlayerHand> Player::getHand() { return hand; }

bool operator==(const Player& player1, const Player& player2) {
    return player1.name == player2.name;
}
