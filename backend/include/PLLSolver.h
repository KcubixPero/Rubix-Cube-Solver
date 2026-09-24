#ifndef PLL_SOLVER_H
#define PLL_SOLVER_H

#include "PhaseSolver.h"

class PLLSolver : public PhaseSolver
{
public:
    using PhaseSolver::PhaseSolver;
    bool isSolved(const RubixCube &cube) const override;
};

#endif
