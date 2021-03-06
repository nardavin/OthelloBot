#ifndef __COMMON_H__
#define __COMMON_H__

#include <vector>
#include <iostream>

using namespace std;

#define BLACK false
#define WHITE true

#define NULL_MOVE(side) Move(side)

class Move {

private:
    unsigned char move;

    Move(unsigned char m) { move = m; }
public:
    Move(int x, int y, bool side) :
                Move((unsigned char)((x << 3) | (y) | (side ? 0x80 : 0x00))) {}

    Move(bool side) : Move((unsigned char)(side ? 0xc0 : 0x40)) {}

    ~Move() {}

    int getX() { return (int)((move >> 3) & 0x07); }
    int getY() { return (int)(move & 0x07); }
    bool getSide() { return (bool)(move & 0x80); }

    bool isNull() { return (bool)(move & 0x40); }

    bool operator == (const Move &Ref) const
        {
            return(move == Ref.move);
        }

};


#define NUM_T_TABLE_ENTRIES 10000000
//10000000

typedef struct {
    unsigned long long hash = 0x0ULL;
    char depth = 0;
    Move move = NULL_MOVE(BLACK);
} TransTableEntry;

#endif
