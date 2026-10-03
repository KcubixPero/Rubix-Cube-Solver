#include "PLLSolver.h"

#include "LegacyStageSupport.h"
#include "MoveSimplifier.h"
#include "OLLSolver.h"

#include <stdexcept>

std::vector<std::string> PLLSolver::solve(RubixCube &cube)
{
    if (!OLLSolver::isSolved(cube))
        throw std::invalid_argument("PLL requires Cross, F2L, and OLL to be solved first.");
    return MoveSimplifier::simplifyAdjacentFaces(LegacyStageSupport::solvePLL(cube));
}

bool PLLSolver::isSolved(const RubixCube &cube)
{
    for (int face = 0; face < 6; ++face)
        for (int row = 0; row < 3; ++row)
            for (int col = 0; col < 3; ++col)
                if (cube.cube[face][row][col] != face)
                    return false;
    return true;
}
