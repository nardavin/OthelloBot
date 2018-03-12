#ifndef __BOARDNODE_H__
#define __BOARDNODE_H__

#include <vector>
#include <iostream>
#include <limits>
#include <algorithm>
#include "common.hpp"
#include "board.hpp"
using namespace std;

class BoardNode{

private:
    Move* move;
    Board* board;
    vector<BoardNode*> children;
    bool side;
public:
    BoardNode(Board* board, bool ourSide);
    BoardNode(Board* b, Move* m, bool s);
    ~BoardNode();
    Move* getBestChoice(int depth, float (*heuristic)(Board*, bool), bool ourSide);
    float searchTreeAB(int depth, float alpha, float beta,
                float (*heuristic)(Board*, bool));
    float searchTreePVS(int depth, float alpha, float beta,
                float (*heuristic)(Board*, bool));
    float searchTreeEndGame(float (*heuristic)(Board*, bool), bool ourSide);
    vector<Move*> sortMoves(vector<Move*> moves, float (*heuristic)(Board*, bool),
                                                    int depth);
    Move *getMove();
    vector<BoardNode*> getChildren();

};

#endif
