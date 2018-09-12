#include "board.hpp"

#define GET(bits, x, y) ((bool)(bits & (0x1ULL << ( (7-x) + 8 * (7-y) ))))
#define FLIP(bits, x, y) bits ^= (0x1ULL << ( (7-x) + 8 * (7-y) ))

#define BLANK 0x0000000000000000ULL

#define LEFT_MASK 0xfefefefefefefefeULL
#define RIGHT_MASK 0x7f7f7f7f7f7f7f7fULL

static Direction directions[8] = {NW, N, NE, E, SE, S, SW, W};

#define PIECE_HASH(x, y, side) pieceHash[side*64 + y*8 + x]
#define MOVE_HASH(x, y, direction, length) moveHash[direction*384 + (length-1)*64 + y*8 + x]

bool Board::isHashInit = false;
unsigned long long Board::pieceHash[128];
unsigned long long Board::moveHash[3072];
unsigned long long Board::defaultHash;

void Board::initHash(){
    mt19937_64 generator(1337);
    for (int side = 0; side <= 1; side++) {
        for (int y = 0; y < 8; y++) {
            for (int x = 0; x < 8; x++) {
                PIECE_HASH(x, y, side) = generator();
            }
        }
    }

    defaultHash = PIECE_HASH(3, 3, WHITE) ^ PIECE_HASH(4, 3, BLACK)
                ^ PIECE_HASH(3, 4, BLACK) ^ PIECE_HASH(4, 4, WHITE);

    //                        NW,  N, NE, E, SE, S, SW,  W
    int xDirectionDelta[8] = {-1,  0,  1, 1,  1, 0, -1, -1};
    int yDirectionDelta[8] = {-1, -1, -1, 0,  1, 1,  1,  0};

    for (int dir = 0; dir < 8; dir++) {
        for (int len = 1; len <= 6; len++) {
            for (int y = 0; y < 8; y++) {
                for (int x = 0; x < 8; x++) {

                    bool isValidMove = true;
                    unsigned long long hash = BLANK;
                    int length = len;
                    int yPos = y;
                    int xPos = x;

                    while (length > 0) {
                        yPos += yDirectionDelta[dir];
                        xPos += xDirectionDelta[dir];
                        length --;

                        if((yDirectionDelta[dir] != 0 && (yPos <= 0 || yPos >= 7)) ||
                           (xDirectionDelta[dir] != 0 && (xPos <= 0 || xPos >= 7))) {
                            isValidMove = false;
                            break;
                        }

                        hash ^= PIECE_HASH(xPos, yPos, BLACK);
                        hash ^= PIECE_HASH(xPos, yPos, WHITE);
                    }

                    if (isValidMove) {
                        MOVE_HASH(x, y, dir, len) = hash;
                    }
                    else {
                        MOVE_HASH(x, y, dir, len) = BLANK;
                    }
                }
            }
        }
    }
}

/**
 * Constructs a new board
 */
Board::Board(){
    if(!isHashInit) {
        initHash();
        isHashInit = true;
    }
    pieces[WHITE] = 0x0000001008000000ULL;
    pieces[BLACK] = 0x0000000810000000ULL;
    isMovesCalc[WHITE] = false;
    isMovesCalc[BLACK] = false;
    parity = WHITE;
    hash = defaultHash;
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
        newBoard->moves[WHITE][i] = moves[WHITE][i];
        newBoard->moves[BLACK][i] = moves[BLACK][i];
    }
    newBoard->allMoves[WHITE] = allMoves[WHITE];
    newBoard->allMoves[BLACK] = allMoves[BLACK];
    newBoard->isMovesCalc[WHITE] = isMovesCalc[WHITE];
    newBoard->isMovesCalc[BLACK] = isMovesCalc[BLACK];
    newBoard->parity = parity;
    newBoard->hash = hash;
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
        return (bits << 1) & LEFT_MASK;

        case NW:
        return (bits << 9) & LEFT_MASK;

        case N:
        return (bits << 8);

        case NE:
        return (bits << 7) & RIGHT_MASK;

        case E:
        return (bits >> 1) & RIGHT_MASK;

        case SE:
        return (bits >> 9) & RIGHT_MASK;

        case S:
        return (bits >> 8);

        case SW:
        return (bits >> 7) & LEFT_MASK;

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
        moves[side][i] = BLANK;
    }
    allMoves[side] = BLANK;
    for(int i = 0; i < 8; i++){
        unsigned long long candidates = pieces[!side] & shiftBits(pieces[side], directions[i]);
        while(candidates != BLANK){
            moves[side][(i + 4) % 8] |= empty & shiftBits(candidates, directions[i]);
            allMoves[side] |= empty & shiftBits(candidates, directions[i]);
            candidates = pieces[!side] & shiftBits(candidates, directions[i]);
        }
    }
    isMovesCalc[side] = true;
}

/**
 * Determines whether the specified side has any moves
 * @param  side Side to check for moves on
 * @return      True if there are >0 possible moves, false otherwise
 */
bool Board::hasMoves(bool side){
    if(!isMovesCalc[side]){
        calcMoves(side);
    }
    return (bool)allMoves[side];
}

/**
 * Counts the number of moves available to the given side
 * @param  side Side to check for moves on
 * @return      Number of available moves
 */
int Board::countMoves(bool side){
    if(!isMovesCalc[side]){
        calcMoves(side);
    }
    return __builtin_popcountll(allMoves[side]);
}

/**
 * Returns a vector of possible moves that a given side can make
 * @param side Side to calculate moves for
 */
vector<Move> Board::possibleMoves(bool side){
    if(!isMovesCalc[side]){
        calcMoves(side);
    }
    vector<Move> ret = vector<Move>();
    for(int y = 0; y < 8; y++){
        for(int x = 0; x < 8; x++){
            if(GET(allMoves[side], x, y)){
                ret.push_back(Move(x, y, side));
            }
        }
    }
    if(ret.size() == 0) {
        ret.push_back(NULL_MOVE(side));
    }
    return ret;
}

/**
 * Checks if a certain move is valid for the specified side
 * @param  m    Move to check
 * @return      True if move is valid, false otherwise
 */
bool Board::checkMove(Move m){
    bool side = m.getSide();
    if(!isMovesCalc[side]){
        calcMoves(side);
    }
    return GET(allMoves[side], m.getX(), m.getY());
}

/**
 * Does a given move on the board
 * @param m    Move to do on the board
 */
void Board::doMove(Move m){
    if (m.isNull()) {
        parity = !parity;
        return;
    }
    bool side = m.getSide();
    int x = m.getX();
    int y = m.getY();
    if (!checkMove(m)) {return;}
    unsigned long long move = BLANK;
    FLIP(move, x, y);
    FLIP(pieces[side], x, y);
    hash ^= PIECE_HASH(x, y, side);
    for(int i = 0; i < 8; i++){
        if(move & moves[side][i]){
            int len = 0;
            unsigned long long target = shiftBits(move, directions[i]);
            while(!(target & pieces[side])){
                len ++;
                pieces[side] |= target;
                pieces[!side] ^= target;
                target = shiftBits(target, directions[i]);
            }
            hash ^= MOVE_HASH(x, y, i, len);
        }
    }
    isMovesCalc[WHITE] = false;
    isMovesCalc[BLACK] = false;
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
    if ((pieces[BLACK] | pieces[WHITE]) == ~BLANK) {
        return true;
    }
    return !(hasMoves(BLACK) || hasMoves(WHITE));
}

/**
 * Overrides the board state to match given data. Used for testsuites
 * @param data Data to change board state to
 */
void Board::setBoard(char data[]){
    pieces[BLACK] = BLANK;
    pieces[WHITE] = BLANK;
    isMovesCalc[WHITE] = false;
    isMovesCalc[BLACK] = false;

    for(int i = 0; i < 64; i++){
        if(data[i] == 'b'){
            FLIP(pieces[BLACK], i % 8, i/8);
        }
        else if(data[i] == 'w'){
            FLIP(pieces[WHITE], i % 8, i/8);
        }
    }

    hash = BLANK;

    for (int y = 0; y < 8; y++) {
        for (int x = 0; x < 8; x++) {
            if(GET(pieces[BLACK], x, y)) {
                hash ^= PIECE_HASH(x, y, BLACK);
            }
            else if(GET(pieces[WHITE], x, y)) {
                hash ^= PIECE_HASH(x, y, WHITE);
            }
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

unsigned long long Board::getHash() {
    return hash;
}
