#include <iostream>
#include "common.hpp"
#include "board.hpp"
#include "linearHeuristic.hpp"
#include "boardNodeLearning.hpp"

// Use this file to test your minimax implementation (2-ply depth, with a
// heuristic of the difference in number of pieces).
int main(int argc, char *argv[]) {

    char boardData[64] = {
        ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
        ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
        ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
        ' ', ' ', ' ', 'w', 'b', ' ', ' ', ' ',
        ' ', ' ', ' ', 'b', 'w', ' ', ' ', ' ',
        ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
        ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
        ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '
    };

    Board *board = new Board();
    board->setBoard(boardData);

    LinearHeuristic* heuristic = new LinearHeuristic("weights/handmade.weights");

    board->printBoard();

    BoardNodeLearning* root = new BoardNodeLearning(board, WHITE);
    Move move = root->getBestChoice(10, heuristic);
    board->doMove(move);
    board->printBoard();


    Board* principal = root->getPrincipalBoard();
    principal->printBoard();

    cerr << heuristic->getScore(principal, WHITE) << endl;

    delete principal;
    delete board;
    delete heuristic;
    delete root;

    return 0;

}
