#ifndef __LINHEURISTIC_H__
#define __LINHEURISTIC_H__

#include "common.hpp"
#include "board.hpp"
#include "heuristic.hpp"
#include <Eigen/Dense>
#include <fstream>
#include <math.h>
#include <cstdlib>

using namespace std;
using namespace Eigen;

#define NUM_LIN_WEIGHTS 5
#define LEARNING_RATE 1
#define TANH_MAX 0.999
#define TANH_SLOPE 1

class LinearHeuristic : public Heuristic{
private:
    VectorXd getInputs(Board* board, bool side);
public:
    LinearHeuristic(const char* filename);
    ~LinearHeuristic();
    double getScore(Board* board, bool side);
    VectorXd getGrad(Board* board, bool side);
    void updateWeights(VectorXd& deltaWeights);
    void saveWeights(const char* filename);
};

#endif
