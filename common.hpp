#ifndef __COMMON_H__
#define __COMMON_H__

#include <vector>

#define BLACK false
#define WHITE true

class Move {

private:
    char move;
public:
    Move(int x, int y) {
        move = ((char)(x) << 4) | (char)y;
    }
    ~Move() {}

    int getX() { return (int)(move >> 4); }
    int getY() { return (int)(move & 0x0f); }

    void setX(int x) { this->move = ((char)(x) << 4) | (move & 0x0f); }
    void setY(int y) { this->move = (move & 0xf0) | (char)y; }
};

#endif
