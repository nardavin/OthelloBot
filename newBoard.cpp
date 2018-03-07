#include "newBoard.hpp"

static bitset<72> boardMask = (bitset<72>(0xff7fbfdfeff7fbfdULL) << 8) | bitset<72>(0xfeULL);
static Direction directions[8] = {NW, N, NE, E, SE, S, SW, W};

/**
 * Constructs a new board
 */
NewBoard::NewBoard(){
    pieces[WHITE].set(POS(3, 3));
    pieces[WHITE].set(POS(4, 4));
    pieces[BLACK].set(POS(3, 4));
    pieces[BLACK].set(POS(4, 3));
    isMovesCalc = false;
    calcSide = BLACK;
}

/**
 * Deconstructs a board
 */
NewBoard::~NewBoard(){
}

/**
 * Copies this board
 * @return Pointer to copy of board
 */
NewBoard* NewBoard::copy(){
    NewBoard *newBoard = new NewBoard();
    newBoard->pieces[WHITE] = pieces[WHITE];
    newBoard->pieces[BLACK] = pieces[BLACK];
    return newBoard;
}

/**
 * Prints the current board state. Used for debugging
 */
void NewBoard::printBoard(){
    for(int y = 0; y < 8; y++){
        for(int x = 0; x < 8; x++){
            if(pieces[WHITE][POS(x, y)]){
                cerr << "W";
            }
            else if(pieces[BLACK][POS(x, y)]){
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
bitset<72> NewBoard::shiftBits(bitset<72> bits, Direction dir){
    switch(dir){
        case W:
        return (bits << 1) & boardMask;

        case NW:
        return (bits << 10) & boardMask;

        case N:
        return (bits << 9) & boardMask;

        case NE:
        return (bits << 8) & boardMask;

        case E:
        return (bits >> 1) & boardMask;

        case SE:
        return (bits >> 10) & boardMask;

        case S:
        return (bits >> 9) & boardMask;

        case SW:
        return (bits >> 8) & boardMask;

        default:
        return bits;
    }
}

/**
 * Calculates the possible moves for a given side and stores them in private
 * variables for later use.
 * @param side Side to calculate the possible moves for
 */
void NewBoard::calcMoves(bool side){
    calcSide = side;
    bitset<72> empty = boardMask ^ (pieces[side] | pieces[!side]);
    for(int i = 0; i < 8; i++){
        moves[i].reset();
    }
    allMoves.reset();
    for(int i = 0; i < 8; i++){
        bitset<72> candidates = pieces[!side] & shiftBits(pieces[side], directions[i]);
        while(!candidates.none()){
            moves[(i + 4) % 8] |= empty & shiftBits(candidates, directions[i]);
            allMoves |= empty & shiftBits(candidates, directions[i]);
            candidates = pieces[!side] & shiftBits(candidates, directions[i]);
        }
    }
    isMovesCalc = true;
}

/**
 * Determines whether the specified side has any moves
 * @param  side Side to check for moves on
 * @return      True if there are >0 possible moves, false otherwise
 */
bool NewBoard::hasMoves(bool side){
    if(!isMovesCalc || calcSide != side){
        calcMoves(side);
    }
    return !allMoves.none();
}

/**
 * Counts the number of moves available to the given side
 * @param  side Side to check for moves on
 * @return      Number of available moves
 */
int NewBoard::countMoves(bool side){
    if(!isMovesCalc || calcSide != side){
        calcMoves(side);
    }
    return allMoves.count();
}

/**
 * Checks if a certain move is valid for the specified side
 * @param  m    Move to check
 * @param  side Side to check the move on
 * @return      True if move is valid, false otherwise
 */
bool NewBoard::checkMove(Move *m, bool side){
    if(!isMovesCalc || calcSide != side){
        calcMoves(side);
    }
    return allMoves[POS(m->getX(), m->getY())];
}

/**
 * Does a given move on the board
 * @param m    Move to do on the board
 * @param side Side to perform the move from
 */
void NewBoard::doMove(Move *m, bool side){
    if (m == nullptr) return;
    isMovesCalc = false;
    if (!checkMove(m, side)) return;
    bitset<72> move;
    move.set(POS(m->getX(), m->getY()));
    pieces[side].set(POS(m->getX(), m->getY()));
    for(int i = 0; i < 8; i++){
        if(!(move & moves[i]).none()){
            bitset<72> target = shiftBits(move, directions[i]);
            while((target & pieces[side]).none()){
                pieces[side] |= target;
                pieces[!side] ^= target;
                target = shiftBits(target, directions[i]);
            }
        }
    }
}

/**
 * Counts the number of pieces on a given side
 * @param  side Side to check for pieces on
 * @return      Number of pieces for that side
 */
int NewBoard::count(bool side){
    return pieces[side].count();
}

/**
 * Overrides the board state to match given data. Used for testsuites
 * @param data Data to change board state to
 */
void NewBoard::setBoard(char data[]){
    pieces[BLACK].reset();
    pieces[WHITE].reset();
    isMovesCalc = false;

    for(int i = 0; i < 64; i++){
        if(data[i] == 'b'){
            pieces[BLACK].set(POS(i % 8, i / 8));
        }
        else if(data[i] == 'w'){
            pieces[WHITE].set(POS(i % 8, i / 8));
        }
    }
}

/**
 * Finds the frontier size for a given size
 * @param side Side to calculate frontier of
 * NOTE: Function not tested and cannot be tested until newBoard is integrated into 
 * the rest of the program. 
 */
int NewBoard::getFrontierSize(bool side) {
    bitset<72> frontier;
    bitset<72> empty = boardMask ^ (pieces[side] | pieces[!side]);

    for(int i = 0; i < 8; i++){
        frontier |= (shiftBits(pieces[side], directions[i]) & empty);
    }
    
    return frontier.count();
}













