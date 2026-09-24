#include "PLLSolver.h"

#include "RubixCube.h"

bool PLLSolver::isSolved(const RubixCube &cube) const
{
    for (int face = 0; face < 6; ++face)
        for (int row = 0; row < 3; ++row)
            for (int col = 0; col < 3; ++col)
                if (cube.cube[face][row][col] != face) return false;
    return true;
}
