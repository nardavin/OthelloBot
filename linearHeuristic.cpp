#include "linearHeuristic.hpp"

LinearHeuristic::LinearHeuristic(const char* filename) :
            Heuristic::Heuristic(NUM_LIN_WEIGHTS) {

    ifstream ifile(filename);
    if(!ifile.is_open()) {
        cerr << "Error opening file: " << filename << endl;
        exit(1);
    }

    int numWeights;
    ifile >> numWeights;
    if (numWeights != NUM_LIN_WEIGHTS) {
        cerr << "Weights file " << filename << " does not have the correct number of weights" << endl;
        exit(1);
    }

    double value;
    for (int i = 0; i < NUM_LIN_WEIGHTS; i++) {
        ifile >> value;
        weights[i] = value;
    }

    ifile.close();
}

LinearHeuristic::~LinearHeuristic() {
}

VectorXd LinearHeuristic::getInputs(Board* board, bool side) {
    VectorXd inputs(NUM_LIN_WEIGHTS);

    inputs[0] = board->count(side) - board->count(!side);
    inputs[1] = board->countMoves(side) - board->countMoves(!side);
    inputs[2] = board->countStable(side) - board->countStable(!side);
    inputs[3] = board->getFrontierSize(side) - board->getFrontierSize(!side);
    inputs[4] = (board->getParity() == side) ? 1 : -1;

    return inputs;
}

double LinearHeuristic::getScore(Board* board, bool side){
    if (board->isDone()) {
        int difference = board->count(side) - board->count(!side);
        if (difference > 0) {
            return 1;
        }
        else if (difference == 0) {
            return 0;
        }
        else {
            return -1;
        }
    }

    VectorXd inputs = getInputs(board, side);

    return TANH_MAX * tanh(TANH_SLOPE * weights.dot(inputs));
}


VectorXd LinearHeuristic::getGrad(Board* board, bool side){
    if (board->isDone()) {
        return VectorXd::Zero(NUM_LIN_WEIGHTS);
    }

    VectorXd inputs = getInputs(board, side);
    double score = TANH_MAX * tanh(TANH_SLOPE * weights.dot(inputs));

    return inputs * (TANH_MAX * TANH_SLOPE * (1 - pow(tanh(TANH_SLOPE * score), 2)));
}

void LinearHeuristic::updateWeights(VectorXd& deltaWeights){
    weights += deltaWeights;
}

void LinearHeuristic::saveWeights(const char* filename) {
    ofstream ofile(filename);

    if(!ofile.is_open()) {
        cerr << "Error opening file: " << filename << endl;
        exit(1);
    }

    ofile << NUM_LIN_WEIGHTS << endl;

    for (int i = 0; i < NUM_LIN_WEIGHTS; i++) {
        ofile << weights[i] << endl;
    }

    ofile.close();
}
