#ifndef F2L_SOLVER_H
#define F2L_SOLVER_H

#include "PhaseSolver.h"

class F2LSolver : public PhaseSolver
{
public:
    using PhaseSolver::PhaseSolver;
    bool isSolved(const RubixCube &cube) const override;
};

#endif
