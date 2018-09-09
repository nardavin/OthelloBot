#include <iostream>
#include <vector>
#include <Eigen/Dense>
#include "common.hpp"
#include "board.hpp"
#include "linearHeuristic.hpp"
#include "boardNodeLearning.hpp"

#define LEARN_RATE .01
#define LAMBDA 0.7

#define NUM_BATCHES 5
#define BATCH_SIZE 100

#define PREGAME_MOVES 6
#define SEARCH_DEPTH 5

// Use this file to test your minimax implementation (2-ply depth, with a
// heuristic of the difference in number of pieces).
int main(int argc, char *argv[]) {

    LinearHeuristic* heuristic = new LinearHeuristic("weights/handmade.weights");

    for (int batch = 0; batch < NUM_BATCHES; batch++) {

        // Zero delta for new batch
        VectorXd weightDelta = VectorXd::Zero(NUM_LIN_WEIGHTS);
        int numDeltas = 0;

        for (int game = 0; game < BATCH_SIZE; game++) {

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

            // Display Score
            int whiteScore = board->count(WHITE);
            int blackScore = board->count(BLACK);
            cerr << "Game " << game+1 << "/" << BATCH_SIZE <<
            " Score: Black-" << blackScore << " White-" << whiteScore << endl;

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
            for (int i = 0; i < (int)derivs[BLACK].size(); i++) {
                double scalar = 0;
                for (int j = i; j < (int)diffs[BLACK].size(); j++) {
                    scalar += pow(LAMBDA, j-i) * diffs[BLACK][j];
                }
                weightDelta += LEARN_RATE * derivs[BLACK][i] * scalar;
                numDeltas ++;
            }
            for (int i = 0; i < (int)derivs[WHITE].size(); i++) {
                double scalar = 0;
                for (int j = i; j < (int)diffs[WHITE].size(); j++) {
                    scalar += pow(LAMBDA, j-i) * diffs[WHITE][j];
                }
                weightDelta += LEARN_RATE * derivs[WHITE][i] * scalar;
                numDeltas ++;
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
        cerr << "Batch " << batch+1 << "/" << NUM_BATCHES << " complete. Deltas:" << endl;
        cerr << weightDelta << endl;
        cerr << endl;

        heuristic->updateWeights(weightDelta);

    }

    heuristic->saveWeights("weights/out.weights");

    delete heuristic;

    return 0;

}
