#ifndef __BOARDNODE_H__
#define __BOARDNODE_H__

#include <vector>
#include <iostream>
#include <limits>
#include "common.hpp"
#include "board.hpp"
using namespace std;

class BoardNode{

private:
    Move* move;
    vector<BoardNode*> children;
    bool side;
    //float score;
    //float getNodeBestScore(float (*heuristic)(Board*, bool), bool side, bool isMaxing);

public:
    Board* board;
    BoardNode(Board* board, bool ourSide);
    BoardNode(Board* b, Move* m, bool s);
    ~BoardNode();
    //void buildTree(bool side, int depth);
    Move* getBestChoice(Board* board, int depth, float (*heuristic)(Board*, bool), bool ourSide);
    float searchTree(int depth, float alpha, float beta,
                float (*heuristic)(Board*, bool), bool ourSide);
    Move getMove();
};

#endif
