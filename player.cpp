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
        moveToMake = minimax(&Heuristics::naiveHeuristic, 2, msLeft);
    }
    else {
        moveToMake = minimax(&Heuristics::heuristic, 8, msLeft);
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
