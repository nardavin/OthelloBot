#include <iostream>
#include <vector>
#include <Eigen/Dense>
#include "common.hpp"
#include "board.hpp"
#include "linearHeuristic.hpp"
#include "boardNodeLearning.hpp"

#define LEARN_RATE .01
#define LEARN_SLOW_RATE .97
#define LAMBDA 0.7

#define NUM_BATCHES 50
#define BATCH_SIZE 100

#define PREGAME_MOVES 6
#define SEARCH_DEPTH 5

#define INPUT_WEIGHTS "weights/random.weights"
#define OUTPUT_WEIGHTS "weights/learned.weights"

// Use this file to test your minimax implementation (2-ply depth, with a
// heuristic of the difference in number of pieces).
int main(int argc, char *argv[]) {

    cerr << endl;

    LinearHeuristic* heuristic = new LinearHeuristic(INPUT_WEIGHTS);

    for (int batch = 0; batch < NUM_BATCHES; batch++) {

        // Zero delta for new batch
        VectorXd weightDelta = VectorXd::Zero(NUM_LIN_WEIGHTS);
        int numDeltas = 0;

        for (int game = 0; game < BATCH_SIZE; game++) {

            cerr << "\rGame " << game+1 << "/" << BATCH_SIZE;

            // Init Board
            Board *board = new Board();
            bool movingSide = BLACK;

            // Randomly play first few moves to get (hopefully) unique boardstate
            // I estimate there is on the order of 1000 possible states 6 moves in
            for (int i = 0; i < PREGAME_MOVES; i++) {
                vector<Move> moves = board->possibleMoves(movingSide);
                Move chosenMove = moves[rand() % moves.size()];
                board->doMove(chosenMove);
                movingSide = !movingSide;
            }

            // Setup for game to be played
            BoardNodeLearning* root = nullptr;
            vector<Board*> principals[2];
            principals[BLACK] = vector<Board*>();
            principals[WHITE] = vector<Board*>();

            // Play Game
            while (!board->isDone()) {
                root = new BoardNodeLearning(board, movingSide);
                Move move = root->getBestChoice(SEARCH_DEPTH, heuristic);

                board->doMove(move);

                principals[movingSide].push_back(root->getPrincipalBoard());

                delete root;
                movingSide = !movingSide;
            }

            // Setup for principal data extraction
            vector<double> diffs[2];
            diffs[BLACK] = vector<double>();
            diffs[WHITE] = vector<double>();
            vector<VectorXd> derivs[2];
            derivs[BLACK] = vector<VectorXd>();
            derivs[WHITE] = vector<VectorXd>();

            // Pull data from principals
            for (int i = 0; i < (int)principals[BLACK].size() - 1; i++) {
                Board* principal = principals[BLACK][i];
                Board* nextPrincipal = principals[BLACK][i+1];

                derivs[BLACK].push_back(heuristic->getGrad(principal, BLACK));

                diffs[BLACK].push_back(heuristic->getScore(nextPrincipal, BLACK)
                                - heuristic->getScore(principal, BLACK));
            }
            for (int i = 0; i < (int)principals[WHITE].size() - 1; i++) {
                Board* principal = principals[WHITE][i];
                Board* nextPrincipal = principals[WHITE][i+1];

                derivs[WHITE].push_back(heuristic->getGrad(principal, WHITE));

                diffs[WHITE].push_back(heuristic->getScore(nextPrincipal, WHITE)
                                - heuristic->getScore(principal, WHITE));
            }

            // Calculate delta
            for (int side = 0; side <= 1; side++) {
                for (int i = 0; i < (int)derivs[side].size(); i++) {
                    double scalar = 0;
                    for (int j = i; j < (int)diffs[side].size(); j++) {
                        scalar += pow(LAMBDA, j-i) * diffs[side][j];
                    }
                    weightDelta += pow(LEARN_SLOW_RATE, batch) * LEARN_RATE * derivs[side][i] * scalar;
                    numDeltas ++;
                }
            }

            // Cleanup
            for (int i = 0; i < (int)principals[BLACK].size(); i++) {
                delete principals[BLACK][i];
            }
            for (int i = 0; i < (int)principals[WHITE].size(); i++) {
                delete principals[WHITE][i];
            }
            principals[BLACK].clear();
            principals[WHITE].clear();
            delete board;
        }

        weightDelta /= (double)numDeltas;

        cerr << endl;
        cerr << endl;
        cerr << "Batch " << batch+1 << "/" << NUM_BATCHES << " complete. Deltas:" << endl;
        cerr << weightDelta << endl;
        cerr << endl;

        heuristic->updateWeights(weightDelta);

    }

    heuristic->saveWeights(OUTPUT_WEIGHTS);

    delete heuristic;

    return 0;

}
