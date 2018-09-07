#ifndef __HEURISTICS_H__
#define __HEURISTICS_H__

#include "common.hpp"
#include "board.hpp"
using namespace std;

class Heuristics {
public:
    static float naiveHeuristic(Board* board, bool side);
    static float heuristic(Board* board, bool ourSide);
    static float endgameHeuristic(Board* board, bool ourSide);
};

#endif
