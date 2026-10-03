#pragma once

#include "RubixCube.h"

#include <string>
#include <vector>

class PLLSolver {
public:
    static std::vector<std::string> solve(RubixCube& cube);
    static bool isSolved(const RubixCube& cube);
};
