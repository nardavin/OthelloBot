#ifndef __BOARD_H__
#define __BOARD_H__

#include <bitset>
#include <iostream>
#include "common.hpp"
using namespace std;

enum Direction {NW, N, NE, E, SE, S, SW, W};

class Board {

private:
    unsigned long long pieces[2];

    unsigned long long moves[2][8];
    unsigned long long allMoves[2];
    bool isMovesCalc[2];

    unsigned long long shiftBits(unsigned long long bits, Direction dir);
    void calcMoves(bool side);
    void printBits(unsigned long long bits);

    bool parity;

public:
    Board();
    ~Board();
    Board *copy();
    void printBoard();

    void test();

    int countStable(bool side);

    vector<Move> possibleMoves(bool side);
    bool getParity();

    bool isDone();
    bool hasMoves(bool side);
    int countMoves(bool side);
    bool checkMove(Move m);
    void doMove(Move m);
    int count(bool side);
    int getFrontierSize(bool side);

    void setBoard(char data[]);
};

#endif
