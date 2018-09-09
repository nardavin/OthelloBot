#include "player.hpp"

/*
 * Constructor for the player; initialize everything here. The side your AI is
 * on (BLACK or WHITE) is passed in as "side". The constructor must finish
 * within 30 seconds.
 */
Player::Player(bool side, char* weightName) {

    othelloBoard = new Board();

    ourSide = side;
    otherSide = !side;

    movesPlayed = 0;
    endGameHead = nullptr;

    mainHeuristic = new LinearHeuristic(weightName);
    endgameHeuristic = new LinearHeuristic("weights/endgame.weights");
}

/*
 * Destructor for the player.
 */
Player::~Player() {
    delete othelloBoard;
    delete mainHeuristic;
    delete naiveHeuristic;
    delete endgameHeuristic;
}

/**
 * Sets the player's board to a given board. Used for testing
 * @param b Board to set the board to
 */
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
Move Player::doMove(Move opponentsMove, int msLeft) {

    if(!opponentsMove.isNull()){
        movesPlayed ++;
    }

    othelloBoard->doMove(opponentsMove);

    Move moveToMake = NULL_MOVE(ourSide);

    if(movesPlayed < 40){
        moveToMake = minimax(mainHeuristic, 10, msLeft);
    }
    else if(movesPlayed >= 60){
        moveToMake = NULL_MOVE(ourSide);
    }
    else{
        moveToMake = endGameSolve(opponentsMove, msLeft);
    }

    othelloBoard->doMove(moveToMake);

    cerr << "sudormrf-" << (ourSide==BLACK ? "Black" : "White") << ": ";
    if(moveToMake.isNull()){
        cerr << "pass" << endl;
    }
    else{
        cerr << moveToMake.getX() << " " << moveToMake.getY() << endl;
    }

    if(!moveToMake.isNull()){
        movesPlayed ++;
    }

    return moveToMake;
}

/**
 * Determines the move to make during the early/mid game using minimax
 * @param  heuristic Heuristic function to use for this search
 * @param  depth     Depth to search into minimax tree
 * @param  msLeft    Time remaining to make moves
 * @return           Move to make
 */
Move Player::minimax(Heuristic* heuristic, int depth, int msLeft){
    BoardNode* root = new BoardNode(othelloBoard, ourSide);
    Move test = root->getBestChoice(depth, heuristic);
    delete root;
    return test;
}

/**
 * Determines the move to make during endgame using an endgame solver
 * @param  opponentsMove Move that the opponent made last round
 * @param  msLeft        Time remaining to make moves
 * @return               Move to make
 */
Move Player::endGameSolve(Move opponentsMove, int msLeft){
    if(endGameHead == nullptr){
        endGameHead = new BoardNode(othelloBoard, ourSide);
        int score = endGameHead->searchTreeEndGame(endgameHeuristic, ourSide);
        if(score < 1){
            cerr << "sudormrf-" << (ourSide==BLACK ? "Black" : "White") << ": "
                << "No solution found" << endl;
            delete endGameHead;
            endGameHead = nullptr;
            return minimax(mainHeuristic, 10, msLeft);
        }
        cerr << "sudormrf-" << (ourSide==BLACK ? "Black" : "White") << ": "
            << "CHOKEHOLD SOLUTION FOUND" << endl;
        endGameTracker = endGameHead->getChildren()[0];
        return endGameTracker->getMove();
    }
    else{
        vector<BoardNode*> nodes = endGameTracker->getChildren();
        if(opponentsMove.isNull()){
            endGameTracker = endGameTracker->getChildren()[0];
        }
        else{
            for(int i = 0; i < (int)nodes.size(); i++){

                Move temp = nodes[i]->getMove();
                if(temp.getX() == opponentsMove.getX() && temp.getY() == opponentsMove.getY()){
                    endGameTracker = endGameTracker->getChildren()[i];
                    break;
                }
            }
        }
        if(endGameTracker->getChildren().size() == 0){
            return NULL_MOVE(ourSide);
        }
        else{
            endGameTracker = endGameTracker->getChildren()[0];
            return endGameTracker->getMove();
        }
    }
}
