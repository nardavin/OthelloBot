#ifndef __COMMON_H__
#define __COMMON_H__

#include <vector>
#include <iostream>

using namespace std;

#define BLACK false
#define WHITE true

#define NULL_MOVE Move(0xff)

class Move {

private:
    unsigned char move;
public:
    Move(int x, int y) : Move(((char)(x) << 4) | (char)y) {}

    Move(unsigned char m) { move = m; }

    Move() : Move(0xff) {};

    ~Move() {}

    int getX() { return (int)(move >> 4); }
    int getY() { return (int)(move & 0x0f); }

    void setX(int x) { this->move = ((char)(x) << 4) | (move & 0x0f); }
    void setY(int y) { this->move = (move & 0xf0) | (char)y; }

    bool isNull() { return move == 0xff; }

};

#endif
