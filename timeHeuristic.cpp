#include "timeHeuristic.hpp"

TimeHeuristic::TimeHeuristic(const char* filename) :
        constWeights(NUM_EACH_WEIGHTS), linearWeights(NUM_EACH_WEIGHTS) {

    if (strcmp(filename, "weights/random.weights") == 0) {
        for (int i = 0; i < NUM_EACH_WEIGHTS; i++) {
            double randVal = ((float)rand())/RAND_MAX;
            randVal *= 2;
            randVal -= 1;
            randVal *= .05;
            constWeights[i] = randVal;
        }
        for (int i = 0; i < NUM_EACH_WEIGHTS; i++) {
            double randVal = ((float)rand())/RAND_MAX;
            randVal *= 2;
            randVal -= 1;
            randVal *= .05;
            linearWeights[i] = randVal;
        }
    }
    else{
        ifstream ifile(filename);
        if(!ifile.is_open()) {
        cerr << "Error opening file: " << filename << endl;
            exit(1);
        }

        string weightsType;
        ifile >> weightsType;
        if (weightsType.compare("time") != 0) {
            cerr << "Weights file " << filename << " does not support this heuristic type" << endl;
            exit(1);
        }

        double value;
        for (int i = 0; i < NUM_EACH_WEIGHTS; i++) {
            ifile >> value;
            constWeights[i] = value;
        }
        for (int i = 0; i < NUM_EACH_WEIGHTS; i++) {
            ifile >> value;
            linearWeights[i] = value;
        }

        ifile.close();
    }
}

TimeHeuristic::~TimeHeuristic() {
}

VectorXd TimeHeuristic::getInputs(Board* board, bool side) {
    VectorXd inputs(NUM_EACH_WEIGHTS);

    inputs[0] = board->count(side) - board->count(!side);
    inputs[1] = board->countMoves(side) - board->countMoves(!side);
    inputs[2] = board->countStable(side) - board->countStable(!side);
    inputs[3] = board->getFrontierSize(side) - board->getFrontierSize(!side);
    inputs[4] = (board->getParity() == side) ? 1 : -1;

    return inputs;
}

double TimeHeuristic::getScore(Board* board, bool side){
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

    double progress = (double)(board->count(BLACK) + board->count(WHITE)) / 64.0;
    VectorXd weights = constWeights + (linearWeights * progress);

    return TANH_MAX * tanh(TANH_SLOPE * weights.dot(inputs));
}


VectorXd TimeHeuristic::getGrad(Board* board, bool side){
    if (board->isDone()) {
        return VectorXd::Zero(NUM_TOTAL_WEIGHTS);
    }

    VectorXd inputs = getInputs(board, side);
    double progress = (double)(board->count(BLACK) + board->count(WHITE)) / 64.0;

    VectorXd preTanGrad(NUM_TOTAL_WEIGHTS);
    preTanGrad << inputs, inputs * progress;

    VectorXd weights = constWeights + (linearWeights * progress);
    double score = TANH_MAX * tanh(TANH_SLOPE * weights.dot(inputs));

    return preTanGrad * (TANH_MAX * TANH_SLOPE * (1 - pow(tanh(TANH_SLOPE * score), 2)));
}

void TimeHeuristic::updateWeights(VectorXd& deltaWeights){
    constWeights += deltaWeights.head(NUM_EACH_WEIGHTS);
    linearWeights += deltaWeights.tail(NUM_EACH_WEIGHTS);
}

void TimeHeuristic::saveWeights(const char* filename) {
    ofstream ofile(filename);

    if(!ofile.is_open()) {
        cerr << "Error opening file: " << filename << endl;
        exit(1);
    }

    ofile << "time" << endl;

    for (int i = 0; i < NUM_EACH_WEIGHTS; i++) {
        ofile << constWeights[i] << endl;
    }
    for (int i = 0; i < NUM_EACH_WEIGHTS; i++) {
        ofile << linearWeights[i] << endl;
    }

    ofile.close();
}
