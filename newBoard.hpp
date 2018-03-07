#ifndef __NEWBOARD_H__
#define __NEWBOARD_H__

#include <bitset>
#include <iostream>
#include "common.hpp"
using namespace std;

#define POS(x, y) ((x) + (9 * (y)))

enum Direction {NW, N, NE, E, SE, S, SW, W};

class NewBoard {

private:
    bitset<72> pieces[2];


    bitset<72> moves[8];
    bitset<72> allMoves;
    bool isMovesCalc;
    bool calcSide;

    static bitset<72> shiftBits(bitset<72> bits, Direction dir);
    void calcMoves(bool side);

    int parity;

public:
    NewBoard();
    ~NewBoard();
    NewBoard *copy();
    void printBoard();

    //int countStableHeuristic(bool side);
    //vector<Move*> possibleMoves(bool side);
    //int getParity();
    //bool isCorner(int x, int y);
    //bool isStable(int x, int y, bool side);

    //bool isDone();
    //bool get(bool side, int x, int y);
    bool hasMoves(bool side);
    int countMoves(bool side);
    bool checkMove(Move *m, bool side);
    void doMove(Move *m, bool side);
    int count(bool side);
    int getFrontierSize(bool side);
    //int countBlack();
    //int countWhite();

    void setBoard(char data[]);
};

#endif
