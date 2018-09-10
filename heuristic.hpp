#ifndef __HEURISTIC_H__
#define __HEURISTIC_H__

#include "common.hpp"
#include "board.hpp"
#include <Eigen/Dense>
#include <string>
using namespace std;
using namespace Eigen;

class Heuristic {
public:
    Heuristic() {}
    virtual ~Heuristic() {}
    virtual double getScore(Board* board, bool side) = 0;
    virtual VectorXd getGrad(Board* board, bool side) = 0;
    virtual void updateWeights(VectorXd& deltaWeights) = 0;
    virtual void saveWeights(const char* filename) = 0;
};

#endif
