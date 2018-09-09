#ifndef __PLAYER_H__
#define __PLAYER_H__

#include <iostream>
#include "common.hpp"
#include "board.hpp"
#include "boardNode.hpp"
#include "linearHeuristic.hpp"
using namespace std;

class Player {
private:
    Move minimax(Heuristic* heuristic, int depth, int msLeft);
    Move endGameSolve(Move opponentsMove, int msLeft);
    Board* othelloBoard;
    int movesPlayed;
    bool ourSide;
    bool otherSide;
    BoardNode* endGameHead;
    BoardNode* endGameTracker;
    LinearHeuristic* mainHeuristic;
    LinearHeuristic* endgameHeuristic;
public:
    Player(bool side, char* weightName);
    ~Player();
    void setBoard(Board* b);
    Move doMove(Move opponentsMove, int msLeft);
};

#endif
