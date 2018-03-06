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
        moveToMake = minimax(othelloBoard, 2, msLeft);
    }
    else {
        moveToMake = minimax(othelloBoard, 6, msLeft);
    }

    othelloBoard->doMove(moveToMake, ourSide);

    return moveToMake;
}

float naiveHeuristic(Board* board, bool side) {
    // Sample total score heuristic
    float value = board->countWhite() - board->countBlack();
    if (side == BLACK) {
        value *= -1;
    }
    return value;
}

float heuristic(Board* board, bool side) {
    bool otherSide = !side;
    float value = (board->countWhite() - board->countBlack())*0.5;
    if (side == BLACK) {
        value *= -1;

        if (board->getParity() == 1) {
            // this value is arbitrary should be tuned
            value += 1;
        }
        else {
            value -= 1;
        }
    }

    else {
        if (board->getParity() == 0) {
            value += 1;
        } else {
            value -= 1;
        }
    }

    // Maximise stable tiles
    //value += board->countStableHeuristic(side)*2;

    // Maximise possible moves, minimise opponents moves
    vector<Move*> moves = board->possibleMoves(side);
    int posMoves = moves.size();
    for(int i = 0; i < (int)moves.size(); i++){
        delete moves[i];
    }
    moves.clear();
    value += posMoves*2;
    value -= posMoves*2;

    // minimise frontier?

    // These values are super random, maybe write a script to run hundreds of games with different parameters
    // and find the best ones?

    if (board->get(side, 0, 0) == true) {
        value += 10;
    }
    else if (board->get(side, 7, 0) == true) {
        value += 10;
    }
    else if (board->get(side, 0, 7) == true) {
        value += 10;
    }
    else if (board->get(side, 7, 7) == true) {
        value += 10;
    }

    if (board->get(otherSide, 0,0)) {
        value -= 10;
    }
    else if(board->get(otherSide, 7,0)) {
        value -= 10;
    }
    else if(board->get(otherSide, 7,7)) {
        value -= 10;
    }
    else if(board->get(otherSide, 0,7)) {
        value -= 10;
    }


    return value;
}

Move *Player::minimax(Board* board, int depth, int msLeft){
    BoardNode* root = new BoardNode(othelloBoard, ourSide);
    return root->getBestChoice(board, depth, &heuristic, ourSide);
}
