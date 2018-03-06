#ifndef __BOARD_H__
#define __BOARD_H__

#include <bitset>
#include <iostream>
#include "common.hpp"
using namespace std;

class Board {

private:
    bitset<64> black;
    bitset<64> taken;

    bool occupied(int x, int y);
    void set(bool side, int x, int y);
    bool onBoard(int x, int y);
    int parity;

public:
    Board();
    ~Board();
    Board *copy();

    int countStableHeuristic(bool side);
    vector<Move*> possibleMoves(bool side);
    int getParity();

    bool isDone();
    bool get(bool side, int x, int y);
    bool hasMoves(bool side);
    bool checkMove(Move *m, bool side);
    void doMove(Move *m, bool side);
    int count(bool side);
    int countBlack();
    int countWhite();

    void setBoard(char data[]);
};

#endif
