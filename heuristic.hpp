#ifndef __HEURISTIC_H__
#define __HEURISTIC_H__

#include "common.hpp"
#include "board.hpp"
#include <Eigen/Dense>
#include <string>
using namespace std;
using namespace Eigen;

class Heuristic {
protected:
    VectorXd weights;
public:
    Heuristic(int numWeights) : weights(numWeights) {}
    virtual ~Heuristic() {}
    virtual double getScore(Board* board, bool side) = 0;
    virtual VectorXd getGrad(Board* board, bool side) = 0;
    virtual void updateWeights(void* deltaWeights) = 0;
};

#endif
