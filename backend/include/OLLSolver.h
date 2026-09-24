#ifndef OLL_SOLVER_H
#define OLL_SOLVER_H

#include "PhaseSolver.h"

class OLLSolver : public PhaseSolver
{
public:
    using PhaseSolver::PhaseSolver;
    bool isSolved(const RubixCube &cube) const override;
};

#endif
