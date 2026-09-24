#include "OLLSolver.h"

#include "Color.h"
#include "RubixCube.h"

bool OLLSolver::isSolved(const RubixCube &cube) const
{
    // With F2L preserved, a uniformly Yellow last face means every last-layer
    // corner and edge is oriented, regardless of its permutation.
    for (int row = 0; row < 3; ++row)
        for (int col = 0; col < 3; ++col)
            if (cube.cube[YELLOW][row][col] != YELLOW) return false;
    return true;
}
