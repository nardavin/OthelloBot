#ifndef __BOARDNODE_H__
#define __BOARDNODE_H__

#include <vector>
#include <iostream>
#include "common.hpp"
#include "board.hpp"
#include <chrono>
#include <thread>
using namespace std;

class BoardNode{

private:

    Move* move;
    vector<BoardNode*> children;
    bool isBottom;
    float score;
    float getNodeBestScore(float (*heuristic)(Board*, Side), Side side, bool isMaxing);
public:
    Board* board;
    BoardNode(Board* b, Move* m);
    ~BoardNode();
    void buildTree(Side side, int depth);
    Move* getBestChoice(float (*heuristic)(Board*, Side), Side side);
    Move getMove();
};

#endif
