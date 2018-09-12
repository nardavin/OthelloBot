#include "boardNode.hpp"

int nodeCount = 0;
int maxNodeCount = 0;
time_t startTime;

/**
 * Constructs a child node
 * @param parentBoard Board from parent node
 * @param m Move made to get to this node from parent
 */
BoardNode::BoardNode(Board* parentBoard, Move m) : lastMove(BLACK) {
    board = parentBoard->copy();
    board->doMove(m);
    lastMove = m;
    sideToMove = !m.getSide();

    children = vector<BoardNode*>();
    nodeCount += 1;
    if(nodeCount > maxNodeCount){
        maxNodeCount = nodeCount;
    }

}

/**
 * Constructs a base node
 * @param board   Current board for the game
 * @param ourSide The side that our player is on
 */
BoardNode::BoardNode(Board* board, bool ourSide) :
                        BoardNode(board, NULL_MOVE(!ourSide)) {
    nodeCount = 0;
    maxNodeCount = 0;
    startTime = time(nullptr);
}

/**
 * Deconstructs node
 */
BoardNode::~BoardNode(){
    if (board) delete board;
    for(int i = 0; i < (int)children.size(); i++){
        delete children[i];
    }
    children.clear();
    nodeCount -= 1;
}

/**
 * Gets the move that was used to get from the previous board to this one
 * @return Move that was used to get from previous board to this one
 */
Move BoardNode::getMove(){
    return lastMove;
}

/**
 * Gets the children of a node
 * @return Vector of node's children
 */
vector<BoardNode*> BoardNode::getChildren(){
    return children;
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
                                    Heuristic* heuristic){
    if(depth == 0){
        return heuristic->getScore(board, sideToMove);
    }

    vector<Move> possibleMoves = board->possibleMoves(sideToMove);
    for(int i = 0; i < (int)possibleMoves.size(); i++){
        children.push_back(new BoardNode(board, possibleMoves[i]));
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
float BoardNode::searchTreePVS(int depth, float alpha, float beta,
                                Heuristic* heuristic, TransTableEntry* tTable){
    if(depth == 0){
        return heuristic->getScore(board, sideToMove);
    }
    vector<Move> possibleMoves = board->possibleMoves(sideToMove);

    if(depth > 4){
        possibleMoves = sortMoves(possibleMoves, heuristic, 1);
    }

    if (tTable) {
        TransTableEntry entry = tTable[board->getHash() % NUM_T_TABLE_ENTRIES];
        if (entry.hash == board->getHash() && entry.move.getSide() == sideToMove) {
            //if (depth == 9) cerr << "match" << endl;
            for(int i = 0; i < (int)possibleMoves.size(); i++){
                if (possibleMoves[i] == entry.move) {
                    //if (depth == 9) cerr << "move found at index " << i << " with depth " << (int)entry.depth << endl;
                    possibleMoves.erase(possibleMoves.begin() + i);
                    possibleMoves.insert(possibleMoves.begin(), entry.move);
                    break;
                }
            }
        }
        else {
            //if (depth == 9) cerr << entry.hash << " " << (int)entry.depth << endl;
        }
    }

    Move bestMove = possibleMoves[0];
    for(int i = 0; i < (int)possibleMoves.size(); i++){
        children.push_back(new BoardNode(board, possibleMoves[i]));
        float score;
        if(i == 0){
            score = -children[i]->searchTreePVS(depth - 1, -beta, -alpha, heuristic, tTable);
        }
        else{
            score = -children[i]->searchTreePVS(depth - 1, -alpha-PVS_WINDOW, -alpha, heuristic, tTable);
            if(alpha < score && score < beta){
                score = -children[i]->searchTreePVS(depth - 1, -beta, -alpha, heuristic, tTable);
            }
        }
        if (score > alpha) {
            alpha = score;
            bestMove = possibleMoves[i];
        }
        if(alpha >= beta) break;
    }

    if (tTable) {
        int index = board->getHash() % NUM_T_TABLE_ENTRIES;
        if (depth >= tTable[index].depth-2) {
            tTable[index].hash = board->getHash();
            tTable[index].depth = depth;
            tTable[index].move = bestMove;
        }
    }

    for(int i = 0; i < (int)children.size(); i++){
        delete children[i];
    }
    children.clear();
    possibleMoves.clear();
    return alpha;
}

/**
 * Searches a tree using a modified minimax algorithm to completely map out
 * winning moveset
 * @param  heuristic Heuristic to describe boardstate. Should give 1 for win and
 *                   -1 for loss.
 * @param  ourSide   Side of the board to scan for winning moveset for
 * @return           Worst case score for this node
 */
float BoardNode::searchTreeEndGame(Heuristic* heuristic, bool ourSide){
    if(maxNodeCount >= 2500000 || difftime(time(nullptr), startTime) > 60){
        return -1;
    }

    if(board->isDone()){
        return heuristic->getScore(board, ourSide);
    }
    vector<Move> possibleMoves = board->possibleMoves(sideToMove);

    if(sideToMove == ourSide){
        for(int i = 0; i < (int)possibleMoves.size(); i++){
            BoardNode *testNode = new BoardNode(board, possibleMoves[i]);
            float score = testNode->searchTreeEndGame(heuristic, ourSide);
            if(score > 0){
                children.push_back(testNode);
                return 1;
            }
            else{
                delete testNode;
            }
        }
        return -1;
    }
    else{
        for(int i = 0; i < (int)possibleMoves.size(); i++){
            BoardNode *testNode = new BoardNode(board, possibleMoves[i]);
            float score = testNode->searchTreeEndGame(heuristic, ourSide);
            children.push_back(testNode);
            if(score <= 0){
                for(int j = 0; j <= i; j++){
                    delete children[j];
                }
                children.clear();
                return -1;
            }
        }
        return 1;
    }

}


/**
 * Finds the best move to make this round using minimax and A/B pruning
 * @param  depth     Depth to search in the node tree
 * @param  heuristic Heuristic function that defines the score for each position
 * @param  ourSide   The side that you are playing on. Passed into heuristic function.
 * @return           The most optimal move based on the heuristic function
 */
Move BoardNode::getBestChoice(int depth, Heuristic* heuristic,
                                        TransTableEntry* tTable){
    vector<Move> possibleMoves = board->possibleMoves(sideToMove);
    if(possibleMoves.size() == 1){
        return possibleMoves[0];
    }
    for(int i = 0; i < (int)possibleMoves.size(); i++){
        children.push_back(new BoardNode(board, possibleMoves[i]));
    }
    float alpha = -numeric_limits<float>::max();
    float beta = numeric_limits<float>::max();

    Move ret = NULL_MOVE(sideToMove);
    for(int i = 0; i < (int)children.size(); i++){
        float score = -children[i]->searchTreePVS(depth - 1, -beta, -alpha, heuristic, tTable);
        if(score > alpha){
            alpha = score;
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
vector<Move> BoardNode::sortMoves(vector<Move> moves, Heuristic* heuristic,
                                            int depth){
    vector<pair<float, int>> indexScores;
    float alpha = -numeric_limits<float>::max();
    float beta = numeric_limits<float>::max();

    for(int i = 0; i < (int)moves.size(); i++){
        BoardNode node = BoardNode(board, moves[i]);
        indexScores.push_back(make_pair(node.searchTreeAB(depth, alpha, beta, heuristic), i));
    }
    std::sort(indexScores.begin(), indexScores.end());
    vector<Move> sorted;
    for(int i = 0; i < (int)indexScores.size(); i++){
        sorted.push_back(moves[indexScores[i].second]);
    }

    return sorted;
}
