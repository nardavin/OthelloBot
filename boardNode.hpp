#ifndef __BOARDNODE_H__
#define __BOARDNODE_H__

#include <vector>
#include <iostream>
#include <limits>
#include <algorithm>
#include <time.h>
#include "common.hpp"
#include "board.hpp"
#include "heuristic.hpp"
using namespace std;

#define PVS_WINDOW 0.0001

class BoardNode{

private:
    Board* board;
    Move lastMove;
    bool sideToMove;
    vector<BoardNode*> children;


public:
    BoardNode(Board* board, bool ourSide);
    BoardNode(Board* b, Move m);
    ~BoardNode();
    Move getBestChoice(int depth, Heuristic* heuristic, TransTableEntry* tTable);
    float searchTreeAB(int depth, float alpha, float beta,
                Heuristic* heuristic);
    float searchTreePVS(int depth, float alpha, float beta,
                Heuristic* heuristic, TransTableEntry* tTable);
    float searchTreeEndGame(Heuristic* heuristic, bool ourSide);
    vector<Move> sortMoves(vector<Move> moves, Heuristic* heuristic,
                                                    int depth);
    Move getMove();
    vector<BoardNode*> getChildren();

};

#endif
