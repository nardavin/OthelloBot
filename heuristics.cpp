#include "heuristics.hpp"

float Heuristics::naiveHeuristic(Board* board, bool side) {
    // Sample total score heuristic
    return board->count(side) - board->count(!side);
}

float Heuristics::heuristic(Board* board, bool ourSide) {
    bool theirSide = !ourSide;
    float numPiecesMult = 0.5;
    float numMovesMult = 2.0;
    float numStableMult = 7.0;
    float frontierSizeMult = 1.0;
    float parityMult = 2.0;
    float gameEndMult = 100000.0;

    float value = 0;

    int ourCount = board->count(ourSide);
    int ourMoves = board->countMoves(ourSide);

    value += ourCount * numPiecesMult;
    value += ourMoves * numMovesMult;
    value += board->countStable(ourSide) * numStableMult;
    value -= board->getFrontierSize(ourSide) * frontierSizeMult;
    value += (board->getParity() == ourSide) ? parityMult : -parityMult;


    int theirCount = board->count(theirSide);
    int theirMoves = board->countMoves(theirSide);
    value -= theirCount * numPiecesMult;
    value -= theirMoves * numMovesMult;
    value -= board->countStable(theirSide) * numStableMult;
    value += board->getFrontierSize(theirSide) * frontierSizeMult;

    if(ourMoves == 0 && theirMoves == 0){
        value += (ourCount > theirCount) ? gameEndMult : -gameEndMult;
    }

    return value;
}

float Heuristics::endgameHeuristic(Board* board, bool ourSide){
    int ourCount = board->count(ourSide);
    int theirCount = board->count(!ourSide);
    if(ourCount > theirCount){
        return 1;
    }
    else if(theirCount > ourCount){
        return -1;
    }
    else{
        return 0;
    }
}
