#include "player.hpp"

/*
 * Constructor for the player; initialize everything here. The side your AI is
 * on (BLACK or WHITE) is passed in as "side". The constructor must finish
 * within 30 seconds.
 */
Player::Player(bool side) {
    // Will be set to true in test_minimax.cpp.
    testingMinimax = false;

    othelloBoard = new Board();

    ourSide = side;
    otherSide = !side;
}

/*
 * Destructor for the player.
 */
Player::~Player() {
    delete othelloBoard;
}

void Player::setBoard(Board* b){
    othelloBoard = b;
}

float naiveHeuristic(Board* board, bool side) {
    // Sample total score heuristic
    return board->count(side) - board->count(!side);
}

float heuristic(Board* board, bool ourSide) {
    bool theirSide = !ourSide;
    float numPiecesMult = 0.5;
    float numMovesMult = 2.0;
    float numStableMult = 5.0;
    float frontierSizeMult = 1.0;
    float parityMult = 2.0;

    float value = 0;

    int ourCount = board->count(ourSide);
    int theirCount = board->count(theirSide);
//     if (!(board->hasMoves(ourSide) || board->hasMoves(theirSide))) {
//         if (ourCount - theirCount > 0) {
//             return 10000;
//         }
//         else {
//             return -10000;
//         }
//     }
//     
    value += ourCount * numPiecesMult;
    value += board->countMoves(ourSide) * numMovesMult;
    value += board->countStable(ourSide) * numStableMult;
    value -= board->getFrontierSize(ourSide) * frontierSizeMult;
    value += (board->getParity() == ourSide) ? parityMult : -parityMult;

    value -= theirCount * numPiecesMult;
    value -= board->countMoves(theirSide) * numMovesMult;
    value -= board->countStable(theirSide) * numStableMult;
    value += board->getFrontierSize(theirSide) * frontierSizeMult;

    return value;
}

/*
 * Compute the next move given the opponent's last move. Your AI is
 * expected to keep track of the board on its own. If this is the first move,
 * or if the opponent passed on the last move, then opponentsMove will be
 * nullptr.
 *
 * msLeft represents the time your AI has left for the total game, in
 * milliseconds. doMove() must take no longer than msLeft, or your AI will
 * be disqualified! An msLeft value of -1 indicates no time limit.
 *
 * The move returned must be legal; if there are no valid moves for your side,
 * return nullptr.
 */
Move *Player::doMove(Move *opponentsMove, int msLeft) {

    othelloBoard->doMove(opponentsMove, otherSide);

    Move* moveToMake;

    if (testingMinimax == true) {
        moveToMake = minimax(&naiveHeuristic, 2, msLeft);
    }
    else {
        moveToMake = minimax(&heuristic, 2, msLeft);
    }
    othelloBoard->doMove(moveToMake, ourSide);
    if(moveToMake == nullptr){
        cerr << "null" << endl;
    }
    else{
        cerr << moveToMake->getX() << " " << moveToMake->getY() << endl;
    }
    return moveToMake;
}

Move *Player::minimax(float (*heuristic)(Board*, bool), int depth, int msLeft){
    BoardNode* root = new BoardNode(othelloBoard, ourSide);
    Move* test = root->getBestChoice(depth, heuristic, ourSide);
    delete root;
    return test;
}
