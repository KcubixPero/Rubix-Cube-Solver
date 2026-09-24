#ifndef CROSS_SOLVER_H
#define CROSS_SOLVER_H

#include "PhaseSolver.h"

class CrossSolver : public PhaseSolver
{
public:
    using PhaseSolver::PhaseSolver;
    bool isSolved(const RubixCube &cube) const override;
};

#endif
