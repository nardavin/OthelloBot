#ifndef __TIMEHEURISTIC_H__
#define __TIMEHEURISTIC_H__

#include "common.hpp"
#include "board.hpp"
#include "heuristic.hpp"
#include <Eigen/Dense>
#include <fstream>
#include <math.h>
#include <cstdlib>
#include <string>

using namespace std;
using namespace Eigen;

#define NUM_TOTAL_WEIGHTS 10
#define NUM_EACH_WEIGHTS 5
#define TANH_MAX 0.999
#define TANH_SLOPE 1

class TimeHeuristic : public Heuristic{
private:
    VectorXd constWeights;
    VectorXd linearWeights;

    VectorXd getInputs(Board* board, bool side);
public:
    TimeHeuristic(const char* filename);
    ~TimeHeuristic();
    double getScore(Board* board, bool side);
    VectorXd getGrad(Board* board, bool side);
    void updateWeights(VectorXd& deltaWeights);
    void saveWeights(const char* filename);
};

#endif
