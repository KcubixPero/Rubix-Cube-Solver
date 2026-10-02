#include "Color.h"
#include "CrossSolver.h"
#include "F2LSolver.h"
#include "MoveParser.h"

#include <cassert>
#include <iostream>

static RubixCube solvedCube() {
    vector3d faces(6, vector2d(3, vector<int>(3)));
    for (int f=0; f<6; ++f) for (int r=0; r<3; ++r) for (int c=0; c<3; ++c) faces[f][r][c]=f;
    return RubixCube(faces);
}

int main() {
    RubixCube cube = solvedCube();
    assert(CrossSolver::isSolved(cube));
    assert(F2LSolver::isSolved(cube));

    // One and two virtual U turns (physical Yellow/Back) leave the front
    // white cross intact while moving all four F2L pairs.
    for (const char* scramble : {
             "B", "B2", "B'",
             "R B R' B'",
             "R B R' B' R B2 R' B2"}) {
        std::cerr << "F2L test case: " << scramble << std::endl;
        cube = solvedCube();
        MoveParser::execute(cube, scramble);
        assert(CrossSolver::isSolved(cube));
        F2LSolver solver(cube);
        if (!solver.solve()) {
            std::cerr << "F2L failed for " << scramble << ": " << solver.getError() << '\n';
            return 1;
        }
        assert(F2LSolver::isSolved(cube));
        RubixCube replay = solvedCube();
        MoveParser::execute(replay, scramble);
        MoveParser::execute(replay, solver.getMoves());
        assert(F2LSolver::isSolved(replay));
        assert(replay.cube == cube.cube);
    }

    // Short end-to-end cross-to-F2L scrambles exercise the protected A* solver
    // and then verify that the new F2L stage receives its actual resulting cube.
    for (const char* scramble : {"R", "F'", "L2"}) {
        cube = solvedCube();
        MoveParser::execute(cube, scramble);
        const auto cross = CrossSolver::solve(cube);
        for (const auto& move : cross) MoveParser::executeMove(cube, move);
        assert(CrossSolver::isSolved(cube));
        F2LSolver solver(cube);
        if (!solver.solve()) {
            std::cerr << "Cross-to-F2L failed for " << scramble << ": " << solver.getError() << '\n';
            return 1;
        }
        assert(F2LSolver::isSolved(cube));
    }

    std::cout << "F2L tests passed\n";
}
