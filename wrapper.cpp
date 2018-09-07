#include <iostream>
#include <cstdlib>
#include <cstring>
#include "player.hpp"
using namespace std;

int main(int argc, char *argv[]) {
    // Read in side the player is on.
    if (argc != 2)  {
        cerr << "usage: " << argv[0] << " side" << endl;
        exit(-1);
    }
    bool side = (!strcmp(argv[1], "Black")) ? BLACK : WHITE;

    // Initialize player.
    Player *player = new Player(side);

    // Tell java wrapper that we are done initializing.
    cout << "Init done" << endl;
    cout.flush();

    int moveX, moveY, msLeft;

    // Get opponent's move and time left for player each turn.
    while (cin >> moveX >> moveY >> msLeft) {
        Move opponentsMove = NULL_MOVE(!side);
        if (moveX >= 0 && moveY >= 0) {
            opponentsMove = Move(moveX, moveY, !side);
        }

        // Get player's move and output to java wrapper.
        Move playersMove = player->doMove(opponentsMove, msLeft);
        if (playersMove.isNull()) {
            cout << "-1 -1" << endl;
        } else {
            cout << playersMove.getX() << " " << playersMove.getY() << endl;
        }
        cout.flush();
        cerr.flush();

    }

    return 0;
}
