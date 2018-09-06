#include "board.hpp"

static unsigned long long leftMask = 0xfefefefefefefefeULL;
static unsigned long long rightMask = 0x7f7f7f7f7f7f7f7fULL;
static Direction directions[8] = {NW, N, NE, E, SE, S, SW, W};

/**
 * Constructs a new board
 */
Board::Board(){
    pieces[WHITE] = 0x0000001008000000ULL;
    pieces[BLACK] = 0x0000000810000000ULL;
    isMovesCalc = false;
    calcSide = BLACK;
    parity = WHITE;
}

/**
 * Deconstructs a board
 */
Board::~Board(){
}

/**
 * Copies this board
 * @return Pointer to copy of board
 */
Board* Board::copy(){
    Board *newBoard = new Board();
    newBoard->pieces[WHITE] = pieces[WHITE];
    newBoard->pieces[BLACK] = pieces[BLACK];
    for(int i = 0; i < 8; i++){
        newBoard->moves[i] = moves[i];
    }
    newBoard->allMoves = allMoves;
    newBoard->isMovesCalc = isMovesCalc;
    newBoard->calcSide = calcSide;
    newBoard->parity = parity;
    return newBoard;
}

/**
 * Prints the current board state. Used for debugging
 */
void Board::printBoard(){
    for(int y = 0; y < 8; y++){
        for(int x = 0; x < 8; x++){
            if(GET(pieces[WHITE], x, y)){
                cerr << "W";
            }
            else if(GET(pieces[BLACK], x, y)){
                cerr << "B";
            }
            else{
                cerr << "-";
            }
        }
        cerr << endl;
    }
    cerr << endl;
}

/**
 * Shifts a grid of bits in the specified direction
 * @param bits Grid of bits to shift
 * @param dir  Direction to shift bits
 */
unsigned long long Board::shiftBits(unsigned long long bits, Direction dir){
    switch(dir){
        case W:
        return (bits << 1) & leftMask;

        case NW:
        return (bits << 9) & leftMask;

        case N:
        return (bits << 8);

        case NE:
        return (bits << 7) & rightMask;

        case E:
        return (bits >> 1) & rightMask;

        case SE:
        return (bits >> 9) & rightMask;

        case S:
        return (bits >> 8);

        case SW:
        return (bits >> 7) & leftMask;

        default:
        return bits;
    }
}

/**
 * Calculates the possible moves for a given side and stores them in private
 * variables for later use.
 * @param side Side to calculate the possible moves for
 */
void Board::calcMoves(bool side){
    unsigned long long empty = ~(pieces[side] | pieces[!side]);
    for(int i = 0; i < 8; i++){
        moves[i] = BLANK;
    }
    allMoves = BLANK;
    for(int i = 0; i < 8; i++){
        unsigned long long candidates = pieces[!side] & shiftBits(pieces[side], directions[i]);
        while(candidates != BLANK){
            moves[(i + 4) % 8] |= empty & shiftBits(candidates, directions[i]);
            allMoves |= empty & shiftBits(candidates, directions[i]);
            candidates = pieces[!side] & shiftBits(candidates, directions[i]);
        }
    }
    calcSide = side;
    isMovesCalc = true;
}

/**
 * Determines whether the specified side has any moves
 * @param  side Side to check for moves on
 * @return      True if there are >0 possible moves, false otherwise
 */
bool Board::hasMoves(bool side){
    if(!isMovesCalc || calcSide != side){
        calcMoves(side);
    }
    return (bool)allMoves;
}

/**
 * Counts the number of moves available to the given side
 * @param  side Side to check for moves on
 * @return      Number of available moves
 */
int Board::countMoves(bool side){
    if(!isMovesCalc || calcSide != side){
        calcMoves(side);
    }
    return __builtin_popcountll(allMoves);
}

/**
 * Returns a vector of possible moves that a given side can make
 * @param side Side to calculate moves for
 */
vector<Move> Board::possibleMoves(bool side){
    if(!isMovesCalc || calcSide != side){
        calcMoves(side);
    }
    vector<Move> ret = vector<Move>();
    for(int y = 0; y < 8; y++){
        for(int x = 0; x < 8; x++){
            if(GET(allMoves, x, y)){
                ret.push_back(Move(x, y));
            }
        }
    }
    return ret;
}

/**
 * Checks if a certain move is valid for the specified side
 * @param  m    Move to check
 * @param  side Side to check the move on
 * @return      True if move is valid, false otherwise
 */
bool Board::checkMove(Move m, bool side){
    if(!isMovesCalc || calcSide != side){
        calcMoves(side);
    }
    return GET(allMoves, m.getX(), m.getY());
}

/**
 * Does a given move on the board
 * @param m    Move to do on the board
 * @param side Side to perform the move from
 */
void Board::doMove(Move m, bool side){
    if (m.isNull()) {
        parity = !parity;
        return;
    }
    if (!checkMove(m, side)) {return;}
    unsigned long long move = BLANK;
    FLIP(move, m.getX(), m.getY());
    FLIP(pieces[side], m.getX(), m.getY());
    for(int i = 0; i < 8; i++){
        if(move & moves[i]){
            unsigned long long target = shiftBits(move, directions[i]);
            while(!(target & pieces[side])){
                pieces[side] |= target;
                pieces[!side] ^= target;
                target = shiftBits(target, directions[i]);
            }
        }
    }
    isMovesCalc = false;
}

/**
 * Counts the number of pieces on a given side
 * @param  side Side to check for pieces on
 * @return      Number of pieces for that side
 */
int Board::count(bool side){
    return __builtin_popcountll(pieces[side]);
}

/**
 * Determines if the game is finished (no moves can be played)
 * @return True if no moves can be played, false otherwise
 */
bool Board::isDone(){
    return !(hasMoves(BLACK) || hasMoves(WHITE));
}

/**
 * Overrides the board state to match given data. Used for testsuites
 * @param data Data to change board state to
 */
void Board::setBoard(char data[]){
    pieces[BLACK] = BLANK;
    pieces[WHITE] = BLANK;
    isMovesCalc = false;

    for(int i = 0; i < 64; i++){
        if(data[i] == 'b'){
            FLIP(pieces[BLACK], i % 8, i/8);
        }
        else if(data[i] == 'w'){
            FLIP(pieces[WHITE], i % 8, i/8);
        }
    }
}

/**
 * Finds the frontier size for a given side
 * @param  side Side to calculate frontier of
 * @return      Frontier of given side
 */
int Board::getFrontierSize(bool side){
    unsigned long long frontier = BLANK;
    unsigned long long empty = ~(pieces[side] | pieces[!side]);

    for(int i = 0; i < 8; i++){
        frontier |= (shiftBits(pieces[side], directions[i]) & empty);
    }

    return __builtin_popcountll(frontier);
}

/**
 * Counts the number of stable pieces for a given side
 * @param  side Side to calculate stability for
 * @return      Number of stable pieces
 */
int Board::countStable(bool side){
    unsigned long long fullStable = BLANK;
    unsigned long long partialStable[4];
    // Add partial stability to edges/corners
    // NW/SE
    partialStable[0] = pieces[side] & 0xff818181818181ffULL;
    // N/S
    partialStable[1] = pieces[side] & 0xff000000000000ffULL;
    // NE/SW
    partialStable[2] = pieces[side] & 0xff818181818181ffULL;
    // E/W
    partialStable[3] = pieces[side] & 0x8181818181818181ULL;

    // Add partial stability to pieces in full rows/columns/diagonals
    // Diagonals NW/SE
    unsigned long long filter = 0x0000000000000080ULL;
    for(int i = 0; i < 15; i++){
        if(filter == (filter & (pieces[BLACK] | pieces[WHITE]))){
            partialStable[0] |= (filter & pieces[side]);
        }
        filter = shiftBits(filter, N) | shiftBits(filter, E);
    }
    // Columns N/S
    filter = 0x8080808080808080ULL;
    for(int i = 0; i < 8; i++){
        if(filter == (filter & (pieces[BLACK] | pieces[WHITE]))){
            partialStable[1] |= (filter & pieces[side]);
        }
        filter = shiftBits(filter, E);
    }
    // Diagonals NE/SW
    filter = 0x8000000000000000ULL;
    for(int i = 0; i < 15; i++){
        if(filter == (filter & (pieces[BLACK] | pieces[WHITE]))){
            partialStable[2] |= (filter & pieces[side]);
        }
        filter = shiftBits(filter, S) | shiftBits(filter, E);
    }
    // Rows E/W
    filter = 0xff00000000000000ULL;
    for(int i = 0; i < 8; i++){
        if(filter == (filter & (pieces[BLACK] | pieces[WHITE]))){
            partialStable[3] |= (filter & pieces[side]);
        }
        filter = shiftBits(filter, S);
    }

    // Define full stable as intersection of partials
    fullStable = partialStable[0] & partialStable[1] & partialStable[2] & partialStable[3];

    // Loop until no changes are made to find all chained stable pieces
    unsigned long long updateStable = fullStable;
    do{
        fullStable = updateStable;
        for(int i = 0; i < 4; i++){
            partialStable[i] |= pieces[side] & shiftBits(fullStable, directions[i]);
            partialStable[i] |= pieces[side] & shiftBits(fullStable, directions[i+4]);
        }
        updateStable = partialStable[0] & partialStable[1] & partialStable[2] & partialStable[3];
    } while(fullStable != updateStable);

    return __builtin_popcountll(fullStable);
}

/**
 * Prints the bits of a 64 bit int as a board. Used for debugging
 * @param bits Set of bits to print
 */
void Board::printBits(unsigned long long bits){
    for(int y = 0; y < 8; y++){
        for(int x = 0; x < 8; x++){
            cerr << GET(bits, x, y);
        }
        cerr << endl;
    }
    cerr << endl;
}

/**
 * Finds the parity of a given board
 * @return Side that the move parity gives advantage to
 */
bool Board::getParity() {
    return parity;
}
