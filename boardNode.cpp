#include "boardNode.hpp"

using namespace std::this_thread;
using namespace std::chrono;

BoardNode::BoardNode(Board* b, Move* m){
    board = b;
    move = m;
    children = vector<BoardNode*>();
    isBottom = false;
}

BoardNode::~BoardNode(){
    delete board;
    delete move;
    for(int i = 0; i < (int)children.size(); i++){
        delete children[i];
    }
    children.clear();
}

Move BoardNode::getMove(){
    return *move;
}

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
