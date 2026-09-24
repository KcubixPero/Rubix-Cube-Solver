#include "PhaseSolver.h"

#include "RubixCube.h"

PhaseSolver::PhaseSolver(BFSConfig config) : config_(config) {}

BFSResult PhaseSolver::solve(const RubixCube &cube) const
{
    return BFSSolver(config_).solve(cube, [this](const RubixCube &state)
    {
        return isSolved(state);
    });
}
