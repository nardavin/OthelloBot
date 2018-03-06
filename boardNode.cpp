#include "boardNode.hpp"

/**
 * Constructs node
 * @param b Board to load into mode
 * @param m Move made to get to this node from last
 */
BoardNode::BoardNode(Board* parentBoard, Move* m, bool s){
    board = parentBoard->copy();
    move = m;
    side = s;
    board->doMove(move, side);
    children = vector<BoardNode*>();

}

BoardNode::BoardNode(Board* board, bool ourSide){
    BoardNode(board, nullptr, !ourSide);
}

/**
 * Deconstructs node
 */
BoardNode::~BoardNode(){
    delete board;
    if (move) delete move;
    for(int i = 0; i < (int)children.size(); i++){
        delete children[i];
    }
    children.clear();
}

/**
 * Gets the move that was used to get from the previous board to this one
 * @return Move that was used to get from previous board to this one
 */

Move BoardNode::getMove(){
    return *move;
}


/**
 * [BoardNode::searchTree description]
 * @param  depth     [description]
 * @param  alpha     [description]
 * @param  beta      [description]
 * @param  heuristic [description]
 * @param  ourSide   [description]
 * @return           [description]
 */
float BoardNode::searchTree(int depth, float alpha, float beta,
            float (*heuristic)(Board*, bool), bool ourSide){
    if(depth == 0){
        return (*heuristic)(board, ourSide);
    }
    bool movingSide = !side;
    bool isMaxing = (ourSide == movingSide);
    vector<Move*> possibleMoves = board->possibleMoves(movingSide);
    if(possibleMoves.size() == 0){
        return (*heuristic)(board, ourSide);
    }
    for(int i = 0; i < (int)possibleMoves.size(); i++){
        children.push_back(new BoardNode(board, possibleMoves[i], movingSide));
    }
    for(int i = 0; i < (int)children.size(); i++){
        float score = children[i]->searchTree(depth - 1, alpha, beta, heuristic, ourSide);
        if(isMaxing){
            alpha = max(alpha, score);
        }
        else{
            beta = min(beta, score);
        }
        if(alpha >= beta) break;
    }
    for(int i = 0; i < (int)children.size(); i++){
        delete children[i];
    }
    children.clear();
    return (isMaxing) ? alpha : beta;
}

/**
 * Finds the best move to make this round
 * @param  heuristic Heuristic function that defines the score for each position
 * @param  side      The side thet you are playing on. Passed into heuristic function.
 * @return           The most optimal move based on the heuristic function
 */

Move* BoardNode::getBestChoice(Board* board, int depth, float (*heuristic)(Board*, bool), bool ourSide){
    vector<Move*> possibleMoves = board->possibleMoves(ourSide);
    if(possibleMoves.size() == 0){
        return nullptr;
    }
    for(int i = 0; i < (int)possibleMoves.size(); i++){
        children.push_back(new BoardNode(board, possibleMoves[i], ourSide));
    }
    float alpha = -numeric_limits<float>::max();
    float beta = numeric_limits<float>::max();
    Move ret = Move(0, 0);
    for(int i = 0; i < (int)children.size(); i++){
        float score = children[i]->searchTree(depth - 1, alpha, beta, heuristic, ourSide);
        if(score > alpha){
            alpha = score;
            ret = children[i]->getMove();
        }
    }
    for(int i = 0; i < (int)children.size(); i++){
        delete children[i];
    }
    children.clear();
    return new Move(ret.getX(), ret.getY());
}


/**
 * Recursively builds the node tree by searching for all possible moves
 * @param side  Side that is making the move at this level
 * @param depth How deep the tree should be
 */
/*
void BoardNode::buildTree(bool side, int depth){
    if(depth == 0){
        isBottom = true;
        return;
    }
    for(int i = 0; i < 8; i++){
        for(int j = 0; j < 8; j++){
            Move* move = new Move(i, j);
            if(board->checkMove(move, side)){
                Board* b = board->copy();
                b->doMove(move, side);
                children.push_back(new BoardNode(b, move));
            }
            else{
                delete move;
            }
        }
    }
    if(children.size() == 0){
        isBottom = true;
        return;
    }
    bool newSide = !side;
    for(int i = 0; i < (int)children.size(); i++){
        children[i]->buildTree(newSide, depth - 1);
    }
}
*/

/**
 * Recursively finds the best score for each path
 * @param  heuristic Heuristic function that defines the score for each position
 * @param  side      The side thet you are playing on. Passed into heuristic function.
 * @param  isMaxing  True if this node layer is meant to maximize the value, false if minimize.
 * @return           Float of the "best" score for this parent node
 */
/*
float BoardNode::getNodeBestScore(float (*heuristic)(Board*, bool), bool side, bool isMaxing){
    if(isBottom){
        return (*heuristic)(board, side);
    }
    float bestScore = children[0]->getNodeBestScore(heuristic, side, !isMaxing);
    for(int i = 1; i < (int)children.size(); i++){
        float tempScore = children[i]->getNodeBestScore(heuristic, side, !isMaxing);
        if(isMaxing ^ (tempScore < bestScore)){
            bestScore = tempScore;
        }
    }
    return bestScore;
}
*/
