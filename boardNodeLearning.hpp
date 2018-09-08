#ifndef __BOARDNODELEARN_H__
#define __BOARDNODELEARN_H__

#include <vector>
#include <iostream>
#include <limits>
#include <algorithm>
#include "common.hpp"
#include "board.hpp"
#include "heuristic.hpp"
using namespace std;

#define PVS_WINDOW 0.0001

class BoardNodeLearning{

private:
    Board* board;
    Board* principalBoard;
    Move lastMove;
    bool sideToMove;
    vector<BoardNodeLearning*> children;


public:
    BoardNodeLearning(Board* board, bool ourSide);
    BoardNodeLearning(Board* b, Move m);
    ~BoardNodeLearning();
    Move getBestChoice(int depth, Heuristic* heuristic);
    float searchTreeAB(int depth, float alpha, float beta,
                Heuristic* heuristic);
    float searchTreePVS(int depth, float alpha, float beta,
                Heuristic* heuristic);
    vector<Move> sortMoves(vector<Move> moves, Heuristic* heuristic,
                                                    int depth);
    Move getMove();
    vector<BoardNodeLearning*> getChildren();
    Board* getPrincipalBoard();

};

#endif
