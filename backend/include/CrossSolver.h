#ifndef CROSSSOLVER_H
#define CROSSSOLVER_H

#include "RubixCube.h"

#include <string>
#include <vector>

using namespace std;

class CrossSolver
{
public:
    static vector<string> solve(const RubixCube& cube);

private:
    static bool isSolved(const RubixCube& cube);

    static int heuristic(const RubixCube& cube);

    static string encode(const RubixCube& cube);

    static void applyMove(RubixCube& cube, int move);

    static string moveToString(int move);
};

#endif