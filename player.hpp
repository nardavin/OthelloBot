#ifndef __PLAYER_H__
#define __PLAYER_H__

#include <iostream>
#include "common.hpp"
#include "board.hpp"
#include "boardNode.hpp"
#include "heuristics.hpp"
using namespace std;

class Player {
private:
    Move minimax(float (*heuristic)(Board*, bool), int depth, int msLeft);
    Move endGameSolve(Move opponentsMove, int msLeft);
    Board* othelloBoard;
    int movesPlayed;
    bool ourSide;
    bool otherSide;
    BoardNode* endGameHead;
    BoardNode* endGameTracker;
public:
    Player(bool side);
    ~Player();
    void setBoard(Board* b);
    Move doMove(Move opponentsMove, int msLeft);
    // Flag to tell if the player is running within the test_minimax context
    bool testingMinimax;
};

#endif
