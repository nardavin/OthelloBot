#ifndef __NEWBOARD_H__
#define __NEWBOARD_H__

#include <bitset>
#include <iostream>
#include "common.hpp"
using namespace std;

#define POS(x, y) ((8 - x) + (9 * (7 - y)))

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
    void printBits(bitset<72> bits);

    int parity;

public:
    NewBoard();
    ~NewBoard();
    NewBoard *copy();
    void printBoard();

    int countStable(bool side);


    vector<Move*> possibleMoves(bool side);
    //int getParity();

    bool isDone();
    bool hasMoves(bool side);
    int countMoves(bool side);
    bool checkMove(Move *m, bool side);
    void doMove(Move *m, bool side);
    int count(bool side);
    int getFrontierSize(bool side);

    void setBoard(char data[]);
};

#endif
