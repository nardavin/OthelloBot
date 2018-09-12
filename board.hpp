#ifndef __BOARD_H__
#define __BOARD_H__

#include <bitset>
#include <iostream>
#include <random>
#include "common.hpp"
using namespace std;

enum Direction {NW, N, NE, E, SE, S, SW, W};

class Board {

private:
    static unsigned long long pieceHash[128];
    static unsigned long long moveHash[3072];
    // 64 positions * 8 directions * 6 distances (min 1, max 6)
    static unsigned long long defaultHash;
    static bool isHashInit;

    unsigned long long pieces[2];

    unsigned long long moves[2][8];
    unsigned long long allMoves[2];

    unsigned long long hash;
    bool isMovesCalc[2];

    unsigned long long shiftBits(unsigned long long bits, Direction dir);
    void calcMoves(bool side);
    void printBits(unsigned long long bits);

    bool parity;

public:
    static void initHash();
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
    unsigned long long getHash();

    void setBoard(char data[]);
};

#endif
