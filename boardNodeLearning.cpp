#include "boardNodeLearning.hpp"

/**
 * Constructs a child node
 * @param parentBoard Board from parent node
 * @param m Move made to get to this node from parent
 */
BoardNodeLearning::BoardNodeLearning(Board* parentBoard, Move m) : lastMove(BLACK) {
    board = parentBoard->copy();
    principalBoard = nullptr;
    board->doMove(m);
    lastMove = m;
    sideToMove = !m.getSide();

    children = vector<BoardNodeLearning*>();
}

/**
 * Constructs a base node
 * @param board   Current board for the game
 * @param ourSide The side that our player is on
 */
BoardNodeLearning::BoardNodeLearning(Board* board, bool ourSide) :
                        BoardNodeLearning(board, NULL_MOVE(!ourSide)) {
}

/**
 * Deconstructs node
 */
BoardNodeLearning::~BoardNodeLearning(){
    if (board) delete board;
    if (principalBoard) delete principalBoard;
    for(int i = 0; i < (int)children.size(); i++){
        delete children[i];
    }
    children.clear();
}

/**
 * Gets the move that was used to get from the previous board to this one
 * @return Move that was used to get from previous board to this one
 */
Move BoardNodeLearning::getMove(){
    return lastMove;
}

/**
 * Gets the children of a node
 * @return Vector of node's children
 */
vector<BoardNodeLearning*> BoardNodeLearning::getChildren(){
    return children;
}

Board* BoardNodeLearning::getPrincipalBoard() {
    if (principalBoard) {
        return principalBoard->copy();
    }
    return nullptr;
}

/**
 * Searches a tree using negamax and A/B pruning to find the heuristic score
 * for this board
 * @param  depth     How deep to search the node tree
 * @param  alpha     The highest overall score found so far
 * @param  beta      The opponent's best overall score found so far
 * @param  heuristic Heuristic function that defines the score of a board
 * @return           Score of the board accounting for future possible moves
 */
float BoardNodeLearning::searchTreeAB(int depth, float alpha, float beta,
                                    Heuristic* heuristic){
    if(depth == 0){
        return heuristic->getScore(board, sideToMove);
    }

    vector<Move> possibleMoves = board->possibleMoves(sideToMove);
    for(int i = 0; i < (int)possibleMoves.size(); i++){
        children.push_back(new BoardNodeLearning(board, possibleMoves[i]));
        float score = -children[i]->searchTreeAB(depth - 1, -beta, -alpha, heuristic);
        alpha = max(alpha, score);
        if(alpha >= beta) break;
    }
    for(int i = 0; i < (int)children.size(); i++){
        delete children[i];
    }
    children.clear();
    possibleMoves.clear();
    return alpha;
}

/**
 * Searches a tree using negamax and PVS pruning to find the heuristic score
 * for this board
 * @param  depth     How deep to search the node tree
 * @param  alpha     The highest overall score found so far
 * @param  beta      The opponent's best overall score found so far
 * @param  heuristic Heuristic function that defines the score of a board
 * @return           Score of the board accounting for future possible moves
 */
float BoardNodeLearning::searchTreePVS(int depth, float alpha, float beta,
                                    Heuristic* heuristic){
    if(depth == 0){
        principalBoard = board->copy();
        return heuristic->getScore(board, sideToMove);
    }
    vector<Move> possibleMoves = board->possibleMoves(sideToMove);

    if(depth > 4){
        possibleMoves = sortMoves(possibleMoves, heuristic, 1);
    }
    for(int i = 0; i < (int)possibleMoves.size(); i++){
        children.push_back(new BoardNodeLearning(board, possibleMoves[i]));
        float score;
        if(i == 0){
            score = -children[i]->searchTreePVS(depth - 1, -beta, -alpha, heuristic);
        }
        else{
            score = -children[i]->searchTreePVS(depth - 1, -alpha-PVS_WINDOW, -alpha, heuristic);
            if(alpha < score && score < beta){
                score = -children[i]->searchTreePVS(depth - 1, -beta, -alpha, heuristic);
            }
        }
        if (score > alpha) {
            principalBoard = children[i]->getPrincipalBoard();
            alpha = score;
        }
        if(alpha >= beta) break;
    }
    for(int i = 0; i < (int)children.size(); i++){
        delete children[i];
    }
    children.clear();
    possibleMoves.clear();
    return alpha;
}

/**
 * Finds the best move to make this round using minimax and A/B pruning
 * @param  depth     Depth to search in the node tree
 * @param  heuristic Heuristic function that defines the score for each position
 * @param  ourSide   The side that you are playing on. Passed into heuristic function.
 * @return           The most optimal move based on the heuristic function
 */
Move BoardNodeLearning::getBestChoice(int depth, Heuristic* heuristic){
    vector<Move> possibleMoves = board->possibleMoves(sideToMove);
    if(possibleMoves.size() == 1){
        return possibleMoves[0];
    }
    for(int i = 0; i < (int)possibleMoves.size(); i++){
        children.push_back(new BoardNodeLearning(board, possibleMoves[i]));
    }
    float alpha = -numeric_limits<float>::max();
    float beta = numeric_limits<float>::max();

    Move ret = NULL_MOVE(sideToMove);
    for(int i = 0; i < (int)children.size(); i++){
        float score = -children[i]->searchTreePVS(depth - 1, -beta, -alpha, heuristic);
        if(score > alpha){
            alpha = score;
            principalBoard = children[i]->getPrincipalBoard();
            ret = children[i]->getMove();
        }
    }
    for(int i = 0; i < (int)children.size(); i++){
        delete children[i];
    }
    children.clear();
    return ret;
}

/**
 * Sorts a vector of moves by predicted heuristic value through a short negamax scan
 * @param moves     Vector of moves to sort
 * @param heuristic Heuristic function to use while sorting
 * @param depth     How deep the search will go
 * @return          The sorted vector of moves
 */
vector<Move> BoardNodeLearning::sortMoves(vector<Move> moves, Heuristic* heuristic,
                                            int depth){
    vector<pair<float, int>> indexScores;
    float alpha = -numeric_limits<float>::max();
    float beta = numeric_limits<float>::max();

    for(int i = 0; i < (int)moves.size(); i++){
        BoardNodeLearning node = BoardNodeLearning(board, moves[i]);
        indexScores.push_back(make_pair(node.searchTreeAB(depth, alpha, beta, heuristic), i));
    }
    std::sort(indexScores.begin(), indexScores.end());
    vector<Move> sorted;
    for(int i = 0; i < (int)indexScores.size(); i++){
        sorted.push_back(moves[indexScores[i].second]);
    }

    return sorted;
}
