#include "boardNode.hpp"

int nodes = 0;

/**
 * Constructs node
 * @param b Board to load into mode
 * @param m Move made to ged to this node from last
 */
BoardNode::BoardNode(Board* b, Move* m){
    board = b;
    move = m;
    children = vector<BoardNode*>();
    isBottom = false;
    nodes ++;
    cerr << nodes << endl;
}

/**
 * Deconstructs node
 */
BoardNode::~BoardNode(){
    delete board;
    delete move;
    for(int i = 0; i < (int)children.size(); i++){
        delete children[i];
    }
    children.clear();
    nodes --;
}

/**
 * Gets the move that was used to get from the previous board to this one
 * @return Move that was used to get from perbious board to this one
 */
Move BoardNode::getMove(){
    return *move;
}

/**
 * Recursively builds the node tree by searching for all possible moves
 * @param side  Side that is making the move at this level
 * @param depth How deep the tree should be
 */
void BoardNode::buildTree(Side side, int depth){
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
    Side newSide = (side == BLACK) ? WHITE : BLACK;
    for(int i = 0; i < (int)children.size(); i++){
        children[i]->buildTree(newSide, depth - 1);
    }
}

/**
 * Recursively finds the best score for each path
 * @param  heuristic Heuristic function that defines the score for each position
 * @param  side      The side thet you are playing on. Passed into heuristic function.
 * @param  isMaxing  True if this node layer is meant to maximize the value, false if minimize.
 * @return           Float of the "best" score for this parent node
 */
float BoardNode::getNodeBestScore(float (*heuristic)(Board*, Side), Side side, bool isMaxing){
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

/**
 * Finds the best move to make this round
 * @param  heuristic Heuristic function that defines the score for each position
 * @param  side      The side thet you are playing on. Passed into heuristic function.
 * @return           The most optimal move based on the heuristic function
 */
Move* BoardNode::getBestChoice(float (*heuristic)(Board*, Side), Side side){
    if(children.size() == 0){
        return nullptr;
    }
    Move bestMove = children[0]->getMove();
    float bestScore = children[0]->getNodeBestScore(heuristic, side, false);
    for(int i = 1; i < (int)children.size(); i++){
        float tempScore = children[i]->getNodeBestScore(heuristic, side, false);
        if(tempScore > bestScore){
            bestScore = tempScore;
            bestMove = children[i]->getMove();
        }
    }
    return new Move(bestMove.x, bestMove.y);
}
