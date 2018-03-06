#include "board.hpp"

/*
 * Make a standard 8x8 othello board and initialize it to the standard setup.
 */
Board::Board() {
    taken.set(3 + 8 * 3);
    taken.set(3 + 8 * 4);
    taken.set(4 + 8 * 3);
    taken.set(4 + 8 * 4);
    black.set(4 + 8 * 3);
    black.set(3 + 8 * 4);
    parity = 0;
}

/*
 * Destructor for the board.
 */
Board::~Board() {
}

/*
 * Returns a copy of this board.
 */
Board *Board::copy() {
    Board *newBoard = new Board();
    newBoard->black = black;
    newBoard->taken = taken;
    return newBoard;
}

bool Board::occupied(int x, int y) {
    return taken[x + 8*y];
}

bool Board::get(bool side, int x, int y) {
    return occupied(x, y) && (black[x + 8*y] == (side == BLACK));
}

void Board::set(bool side, int x, int y) {
    taken.set(x + 8*y);
    black.set(x + 8*y, side == BLACK);
}

bool Board::onBoard(int x, int y) {
    return(0 <= x && x < 8 && 0 <= y && y < 8);
}


/*
 * Returns true if the game is finished; false otherwise. The game is finished
 * if neither side has a legal move.
 */
bool Board::isDone() {
    return !(hasMoves(BLACK) || hasMoves(WHITE));
}

/*
 * Returns true if there are legal moves for the given side.
 */
bool Board::hasMoves(bool side) {
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            Move move(i, j);
            if (checkMove(&move, side)) return true;
        }
    }
    return false;
}

/*
 * Returns true if a move is legal for the given side; false otherwise.
 */
bool Board::checkMove(Move *m, bool side) {
    // Passing is only legal if you have no moves.
    if (m == nullptr) return !hasMoves(side);

    int X = m->getX();
    int Y = m->getY();

    // Make sure the square hasn't already been taken.
    if (occupied(X, Y)) return false;

    bool other = !side;
    for (int dx = -1; dx <= 1; dx++) {
        for (int dy = -1; dy <= 1; dy++) {
            if (dy == 0 && dx == 0) continue;

            // Is there a capture in that direction?
            int x = X + dx;
            int y = Y + dy;
            if (onBoard(x, y) && get(other, x, y)) {
                do {
                    x += dx;
                    y += dy;
                } while (onBoard(x, y) && get(other, x, y));

                if (onBoard(x, y) && get(side, x, y)) return true;
            }
        }
    }
    return false;
}

/*
 * Modifies the board to reflect the specified move.
 */
void Board::doMove(Move *m, bool side) {
    // A nullptr move means pass.
    if (m == nullptr){
        parity += 1;
        parity %= 2;
        return;
    }

    // Ignore if move is invalid.
    if (!checkMove(m, side)) return;

    int X = m->getX();
    int Y = m->getY();
    bool other = !side;
    for (int dx = -1; dx <= 1; dx++) {
        for (int dy = -1; dy <= 1; dy++) {
            if (dy == 0 && dx == 0) continue;

            int x = X;
            int y = Y;
            do {
                x += dx;
                y += dy;
            } while (onBoard(x, y) && get(other, x, y));

            if (onBoard(x, y) && get(side, x, y)) {
                x = X;
                y = Y;
                x += dx;
                y += dy;
                while (onBoard(x, y) && get(other, x, y)) {
                    set(side, x, y);
                    x += dx;
                    y += dy;
                }
            }
        }
    }
    set(side, X, Y);
}

// Check if an x, y is a corner, assuming the x,y is a valid input. 
bool Board::isCorner(int x, int y) {
    if ((x % 7 == 0) && (y*8 % 7 == 0)) {
        return true;
    }
    return false;
}

// checks if a position x,y for a side is stable
// necessary conditions are either that it is a corner,
// That it is next to a side and a corner
// Or that it is next to 4 stable pieces
bool Board::isStable(int x, int y, bool side) {
    if (isCorner(x,y)) {
        return true;
    }
    // Check next to side and a corner
    if ((x+1 > 7 || y+1 > 7 || x-1 < 0 || y-1 < 0) 
        && ((isCorner(x+1,y) && get(side, x+1, y))
        || (isCorner(x-1,y) && get(side, x-1, y))
        || (isCorner(x,y+1) && get(side, x, y+1)) 
        || (isCorner(x, y-1) && get(side, x, y-1))))  
        {
            return true;
    }

    // Also if it is next to 4 stable pieces this applies, but it is expensive to compute
    // This can be optimised VERY easily by keeping track of which ones are stable on the board
    // Also we should keep a board variable that keeps track of stability so we don't compute it every heuristic
//     int next_to = 0;
//     if (isStable(x-1, y-1, side) && onBoard(x-1, y-1)) {
//         next_to += 1;
//     }
//     if (isStable(x, y-1, side) && onBoard(x, y-1)) {
//         next_to += 1;
//     }
//     if (isStable(x+1, y-1, side) && onBoard(x+1, y-1)) {
//         next_to += 1;
//     }
//     if (isStable(x-1, y, side) && onBoard(x-1, y)) {
//         next_to += 1;
//     }
//     if (isStable(x+1, y, side) && onBoard(x+1, y)) {
//         next_to += 1;
//     }
//     if (isStable(x-1, y+1, side) && onBoard(x-1, y+1)) {
//         next_to += 1;
//     }
//     if (isStable(x, y+1, side) && onBoard(x, y+1)) {
//         next_to += 1;
//     }
//     if (isStable(x+1, y+1, side) && onBoard(x+1, y+1)) {
//         next_to += 1;
//     }
//     if (next_to >= 4) {
//         return true;
//     }
    
    return false;
}
    
// Count the number of stable tiles and scale for the Heuristic
int Board::countStableHeuristic(bool side) {
    int value;
    for(int i = 0; i < 8; i++){
        for(int j = 0; j < 8; j++){
            bool stable = isStable(i,j,side);
            if (stable && get(side, i, j) && isCorner(i,j)) {
                value += 10;
            }
            else if (stable && get(side, i, j)) {
                value += 3;
            }
        }
    }
    return value;
}

// Find possible moves
vector<Move*> Board::possibleMoves(bool side) {
    vector<Move*> moves;
    for(int i = 0; i < 8; i++){
        for(int j = 0; j < 8; j++){
            Move* move = new Move(i, j);
            if(checkMove(move, side)) {
                moves.push_back(move);
            }
            else{
                delete move;
            }
        }
    }
    return moves;
}

int Board::getParity() {
    return parity;
}

/*
 * Current count of given side's stones.
 */
int Board::count(bool side) {
    return (side == BLACK) ? countBlack() : countWhite();
}

/*
 * Current count of black stones.
 */
int Board::countBlack() {
    return black.count();
}

/*
 * Current count of white stones.
 */
int Board::countWhite() {
    return taken.count() - black.count();
}

/*
 * Sets the board state given an 8x8 char array where 'w' indicates a white
 * piece and 'b' indicates a black piece. Mainly for testing purposes.
 */
void Board::setBoard(char data[]) {
    taken.reset();
    black.reset();
    for (int i = 0; i < 64; i++) {
        if (data[i] == 'b') {
            taken.set(i);
            black.set(i);
        } if (data[i] == 'w') {
            taken.set(i);
        }
    }
}
