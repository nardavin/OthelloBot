#include <iostream>
#include "common.hpp"
//#include "player.hpp"
#include "board.hpp"
#include "boardNode.hpp"

float heuristic2(Board* board, bool ourSide) {
    bool theirSide = !ourSide;
    float numPiecesMult = 0.1;
    float numMovesMult = 2.0;
    float numStableMult = 5.0;
    float frontierSizeMult = 1.0;
    float parityMult = 2.0;
    float gameEndMult = 100000.0;
        
    float value = 0;

    int ourCount = board->count(ourSide);
    int ourMoves = board->countMoves(ourSide);
    

    
    value += ourMoves * numMovesMult;
    value += board->countStable(ourSide) * numStableMult;
    value -= board->getFrontierSize(ourSide) * frontierSizeMult;
    value += (board->getParity() == ourSide) ? parityMult : -parityMult;

    int theirCount = board->count(theirSide);
    int theirMoves = board->countMoves(theirSide);
    value -= theirMoves * numMovesMult;
    value -= board->countStable(theirSide) * numStableMult;
    value += board->getFrontierSize(theirSide) * frontierSizeMult;

    int movesIn = ourCount+theirCount-4; 
    if (movesIn < 10) {
        numPiecesMult *= -1;
    }

    value += ourCount * numPiecesMult;
    value -= theirCount * numPiecesMult;
    
    if(ourMoves == 0 && theirMoves == 0){
        value += (ourCount > theirCount) ? gameEndMult : -gameEndMult;
    }


    
    return value;
}


// Use this file to test your minimax implementation (2-ply depth, with a
// heuristic of the difference in number of pieces).
int main(int argc, char *argv[]) {

    /*
    Player *black = new Player(BLACK);
    Player *white = new Player(WHITE);
    Move *blackMove = nullptr;
    Move *whiteMove = nullptr;
    for(int i = 0; i < 32; i++){
        blackMove = black->doMove(whiteMove, 0);
        delete whiteMove;
        whiteMove = white->doMove(blackMove, 0);
        delete blackMove;
    }
    delete whiteMove;
    delete black;
    delete white;
    */



//     char boardData[64] = {
//         ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
//         ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
//         ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
//         ' ', ' ', ' ', 'b', 'w', ' ', ' ', ' ',
//         ' ', ' ', ' ', 'w', 'b', ' ', ' ', ' ',
//         ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
//         ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
//         ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '
//     }; // Basic board
//     
//     char boardData[64] = {
//         ' ', 'b', 'b', 'b', 'b', ' ', ' ', ' ',
//         'w', ' ', 'b', 'w', 'b', ' ', ' ', ' ',
//         'w', 'w', 'b', 'b', 'b', 'b', 'b', 'w',
//         'w', 'w', 'b', 'w', 'b', 'b', 'b', 'w',
//         ' ', 'b', 'w', 'w', 'w', 'w', 'b', 'w',
//         'b', 'b', 'w', 'w', 'w', 'b', 'w', 'w',
//         ' ', ' ', ' ', ' ', 'b', 'b', ' ', 'w',
//         ' ', ' ', ' ', ' ', ' ', 'b', ' ', ' '
//     }; // White to move (World chamption picked (0, 4)

//     char boardData[64] = {
//         'b', 'w', 'w', 'w', 'w', 'w', 'w', 'b',
//         'b', 'w', 'w', 'w', 'w', 'w', 'w', 'w',
//         'b', 'w', 'w', 'w', 'w', 'w', 'w', 'w',
//         'b', 'w', 'b', 'w', 'b', 'w', 'w', 'w',
//         'b', 'w', 'b', 'w', 'b', 'w', 'w', 'w',
//         'b', 'w', 'b', 'w', 'w', 'w', 'w', 'w',
//         'b', 'b', 'b', 'b', 'b', 'w', 'w', 'w',
//         'b', 'b', 'b', 'b', 'b', 'b', ' ', ' '
//     }; // Black to move
    
    char boardData[64] = {
        ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
        'w', ' ', 'b', 'w', ' ', ' ', ' ', ' ',
        'w', 'w', 'w', 'w', 'w', 'w', ' ', ' ',
        'w', 'b', 'w', 'w', 'b', 'b', 'b', ' ',
        'w', 'b', 'w', 'b', 'b', 'b', ' ', ' ',
        'w', 'w', 'b', 'b', 'b', ' ', ' ', ' ',
        ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
        ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '
    }; // White to move


   /*
   Board *board = new Board();
   board->setBoard(boardData);
   board->test();
   board->possibleMoves(WHITE);
   board->test();
   Board *two = board->copy();
   board->doMove(new Move(2, 3), WHITE);
   board->test();
   two->test(); */

    Board *board = new Board(); 
    board->setBoard(boardData);
    board->printBoard();
    BoardNode k(board, WHITE);
    Move* p = k.getBestChoice(8, &heuristic2, WHITE);
    cerr << '(' << p->getX() << ',' << p->getY() << ')' << endl;
    

    /*
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
    */
}
