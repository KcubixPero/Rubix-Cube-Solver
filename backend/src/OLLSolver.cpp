#include "OLLSolver.h"

#include "Color.h"
#include "F2LSolver.h"
#include "LegacyStageSupport.h"

#include <stdexcept>

std::vector<std::string> OLLSolver::solve(RubixCube& cube) {
    if (!F2LSolver::isSolved(cube))
        throw std::invalid_argument("OLL requires Cross and F2L to be solved first.");
    return LegacyStageSupport::solveOLL(cube);
}

bool OLLSolver::isSolved(const RubixCube& cube) {
    if (!F2LSolver::isSolved(cube)) return false;
    for (int row = 0; row < 3; ++row)
        for (int col = 0; col < 3; ++col)
            if (cube.cube[YELLOW][row][col] != YELLOW) return false;
    return true;
}
