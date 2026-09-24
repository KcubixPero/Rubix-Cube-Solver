#ifndef PHASE_SOLVER_H
#define PHASE_SOLVER_H

#include "BFSSolver.h"

class RubixCube;

class PhaseSolver
{
public:
    explicit PhaseSolver(BFSConfig config = {});
    virtual ~PhaseSolver() = default;

    BFSResult solve(const RubixCube &cube) const;
    virtual bool isSolved(const RubixCube &cube) const = 0;

protected:
    BFSConfig config_;
};

#endif
