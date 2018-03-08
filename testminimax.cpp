#include <iostream>
#include "common.hpp"
#include "player.hpp"
#include "board.hpp"
#include "boardNode.hpp"
#include "newBoard.hpp"

// Use this file to test your minimax implementation (2-ply depth, with a
// heuristic of the difference in number of pieces).
int main(int argc, char *argv[]) {



    // Create board with example state. You do not necessarily need to use
    // this, but it's provided for convenience.
    char boardData[64] = {
        ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
        ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
        ' ', 'b', ' ', ' ', ' ', ' ', ' ', ' ',
        'b', 'w', 'b', 'b', 'b', 'b', ' ', ' ',
        ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
        ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
        ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
        ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '
    };
    char boardData2[64] = {
        ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
        ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
        ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
        ' ', ' ', 'b', 'w', 'b', ' ', ' ', ' ',
        ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
        ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
        ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
        ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '
    };
    NewBoard test = NewBoard();
    test.setBoard(boardData2);
    test.printBoard();
    vector<Move*> moves = test.possibleMoves(WHITE);
    for(int i = 0; i < (int)moves.size(); i++){
        cout << moves[i]->getX() << " " << moves[i]->getY() << endl;
    }


    Board *board = new Board();
    board->setBoard(boardData);


    // Initialize player as the white player, and set testing_minimax flag.
    Player *player = new Player(WHITE);
    player->testingMinimax = true;

    player->setBoard(board);


    // Get player's move and check if it's right.
    Move *move = player->doMove(nullptr, 0);

    if (move != nullptr && move->getX() == 1 && move->getY() == 1) {
        std::cout << "Correct move: (1, 1)" << std::endl;;
    } else {
        std::cout << "Wrong move: got ";
        if (move == nullptr) {
            std::cout << "PASS";
        } else {
            std::cout << "(" << move->getX() << ", " << move->getY() << ")";
        }
        std::cout << ", expected (1, 1)" << std::endl;
    }

    return 0;
}
