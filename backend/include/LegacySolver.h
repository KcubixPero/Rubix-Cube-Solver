#ifndef LEGACY_SOLVER_H
#define LEGACY_SOLVER_H

#include <string>
#include <vector>

class RubixCube;

class LegacySolver
{
public:
    std::vector<std::string> solve(RubixCube &cube);
};

#endif