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

/**
 * Constructs a base node
 * @param board   Current board for the game
 * @param ourSide The side that our player is on
 */
BoardNode::BoardNode(Board* board, bool ourSide) :
                        BoardNode(board, nullptr, !ourSide){
}

/**
 * Deconstructs node
 */
BoardNode::~BoardNode(){
    if (board) delete board;
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
 * Searches a tree using negamax and A/B pruning to find the heuristic score
 * for this board
 * @param  depth     How deep to search the node tree
 * @param  alpha     The highest overall score found so far
 * @param  beta      The opponent's best overall score found so far
 * @param  heuristic Heuristic function that defines the score of a board
 * @return           Score of the board accounting for future possible moves
 */
float BoardNode::searchTreeAB(int depth, float alpha, float beta,
                                    float (*heuristic)(Board*, bool)){
    bool movingSide = !side;
    if(depth == 0){
        return (*heuristic)(board, movingSide);
    }
    vector<Move*> possibleMoves = board->possibleMoves(movingSide);
    if(possibleMoves.size() == 0){
        possibleMoves.push_back(nullptr);
    }
    for(int i = 0; i < (int)possibleMoves.size(); i++){
        children.push_back(new BoardNode(board, possibleMoves[i], movingSide));
    }
    for(int i = 0; i < (int)children.size(); i++){
        float score = -children[i]->searchTreeAB(depth - 1, -beta, -alpha, heuristic);
        alpha = max(alpha, score);
        if(alpha >= beta) break;
    }
    for(int i = 0; i < (int)children.size(); i++){
        delete children[i];
    }
    children.clear();
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
float BoardNode::searchTreePVS(int depth, float alpha, float beta,
                                    float (*heuristic)(Board*, bool)){
    bool movingSide = !side;
    if(depth == 0){
        return (*heuristic)(board, movingSide);
    }
    vector<Move*> possibleMoves = board->possibleMoves(movingSide);
    if(possibleMoves.size() == 0){
        possibleMoves.push_back(nullptr);
    }
    else if(depth > 4){
        possibleMoves = sortMoves(possibleMoves, heuristic, 1);
    }
    for(int i = 0; i < (int)possibleMoves.size(); i++){
        children.push_back(new BoardNode(board, possibleMoves[i], movingSide));
    }
    for(int i = 0; i < (int)children.size(); i++){
        float score;
        if(i == 0){
            score = -children[i]->searchTreePVS(depth - 1, -beta, -alpha, heuristic);
        }
        else{
            score = -children[i]->searchTreePVS(depth - 1, -alpha-1, -alpha, heuristic);
            if(alpha < score && score < beta){
                score = -children[i]->searchTreePVS(depth - 1, -beta, -alpha, heuristic);
            }
        }
        alpha = max(alpha, score);
        if(alpha >= beta) break;
    }
    for(int i = 0; i < (int)children.size(); i++){
        delete children[i];
    }
    children.clear();
    return alpha;
}

/**
 * Finds the best move to make this round using minimax and A/B pruning
 * @param  depth     Depth to search in the node tree
 * @param  heuristic Heuristic function that defines the score for each position
 * @param  ourSide   The side that you are playing on. Passed into heuristic function.
 * @return           The most optimal move based on the heuristic function
 */
Move* BoardNode::getBestChoice(int depth, float (*heuristic)(Board*, bool), bool ourSide){
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
        float score = -children[i]->searchTreePVS(depth - 1, -beta, -alpha, heuristic);
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
 * Sorts a vector of moves by predicted heuristic value through a short negamax scan
 * @param moves     Vector of moves to sort
 * @param heuristic Heuristic function to use while sorting
 * @param depth     How deep the search will go
 * @return          The sorted vector of moves
 */
vector<Move*> BoardNode::sortMoves(vector<Move*> moves, float (*heuristic)(Board*, bool),
                                            int depth){
    vector<pair<float, int>> indexScores;
    float alpha = -numeric_limits<float>::max();
    float beta = numeric_limits<float>::max();

    for(int i = 0; i < (int)moves.size(); i++){
        Move *tempMove = new Move(moves[i]->getX(), moves[i]->getY());
        BoardNode node = BoardNode(board, tempMove, !side);
        indexScores.push_back(make_pair(node.searchTreeAB(depth, alpha, beta, heuristic), i));
    }
    std::sort(indexScores.begin(), indexScores.end());
    vector<Move*> sorted;
    for(int i = 0; i < (int)indexScores.size(); i++){
        sorted.push_back(moves[indexScores[i].second]);
    }

    return sorted;
}
