#pragma once

#include "RubixCube.h"

#include <string>
#include <vector>

namespace LegacyStageSupport {
std::vector<std::string> solveF2L(RubixCube& cube);
std::vector<std::string> solveOLL(RubixCube& cube);
std::vector<std::string> solvePLL(RubixCube& cube);
}
