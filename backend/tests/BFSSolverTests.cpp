#include <cassert>
#include <iostream>

#include "CrossSolver.h"
#include "F2LSolver.h"
#include "MoveParser.h"
#include "OLLSolver.h"
#include "PLLSolver.h"
#include "RubixCube.h"

namespace {
RubixCube solvedCube() {
    vector3d stickers(6, vector2d(3, std::vector<int>(3)));
    for (int f = 0; f < 6; ++f) for (int r = 0; r < 3; ++r) for (int c = 0; c < 3; ++c) stickers[f][r][c] = f;
    return RubixCube(stickers);
}
void applySolution(const BFSResult &result, RubixCube &cube) { for (const Move &move : result.moves) MoveParser::execute(cube, move.notation); }
}

int main() {
    BFSConfig config{5, 200000, false};
    CrossSolver cross(config); F2LSolver f2l(config); OLLSolver oll(config); PLLSolver pll(config);
    RubixCube cube = solvedCube();
    for (const PhaseSolver *solver : {static_cast<const PhaseSolver *>(&cross), static_cast<const PhaseSolver *>(&f2l), static_cast<const PhaseSolver *>(&oll), static_cast<const PhaseSolver *>(&pll)}) {
        BFSResult result = solver->solve(cube); assert(result.found && result.moves.empty());
    }
    MoveParser::execute(cube, "R R'"); assert(pll.isSolved(cube));
    MoveParser::execute(cube, "R");
    for (const PhaseSolver *solver : {static_cast<const PhaseSolver *>(&cross), static_cast<const PhaseSolver *>(&f2l), static_cast<const PhaseSolver *>(&oll), static_cast<const PhaseSolver *>(&pll)}) {
        RubixCube copy = cube; BFSResult result = solver->solve(copy); assert(result.found && result.moves.size() == 1); applySolution(result, copy); assert(solver->isSolved(copy));
    }
    cube = solvedCube(); MoveParser::execute(cube, "R U F");
    for (const PhaseSolver *solver : {static_cast<const PhaseSolver *>(&cross), static_cast<const PhaseSolver *>(&f2l), static_cast<const PhaseSolver *>(&oll), static_cast<const PhaseSolver *>(&pll)}) {
        BFSResult result = solver->solve(cube); assert(result.found); applySolution(result, cube); assert(solver->isSolved(cube));
    }
    assert(pll.isSolved(cube)); std::cout << "BFS solver tests passed.\n";
}
