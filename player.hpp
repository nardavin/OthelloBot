#ifndef __PLAYER_H__
#define __PLAYER_H__

#include <iostream>
#include "common.hpp"
#include "board.hpp"
#include "boardNode.hpp"
using namespace std;

class Player {
private:
    Move *minimax(Board* board, int depth, int msLeft);
    Board* othelloBoard;
    Side ourSide;
    Side otherSide;
public:
    Player(Side side);
    ~Player();
    void setBoard(Board* b);
    Move *doMove(Move *opponentsMove, int msLeft);
    // Flag to tell if the player is running within the test_minimax context
    bool testingMinimax;
};

#endif
