#include "Color.h"
#include "CrossSolver.h"
#include "F2LSolver.h"
#include "MoveParser.h"
#include "MoveSimplifier.h"
#include "OLLSolver.h"
#include "PLLSolver.h"
#include "Scrambler.h"

#include <cstdlib>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

static RubixCube solvedCube() {
    vector3d faces(6, vector2d(3, vector<int>(3)));
    for (int face = 0; face < 6; ++face)
        for (int row = 0; row < 3; ++row)
            for (int col = 0; col < 3; ++col)
                faces[face][row][col] = face;
    return RubixCube(faces);
}

static void require(bool condition, const std::string& message) {
    if (!condition) throw std::runtime_error(message);
}

static void verifyStageMoves(const RubixCube& before, const RubixCube& after,
                             const std::vector<std::string>& moves, const char* stage) {
    RubixCube replay = before;
    for (const auto& move : moves) MoveParser::executeMove(replay, move);
    require(replay.cube == after.cube, std::string(stage) + " returned moves do not match its cube state");
}

static void solveAndVerify(RubixCube cube) {
    const auto cross = CrossSolver::solve(cube);
    RubixCube before = cube;
    for (const auto& move : cross) MoveParser::executeMove(cube, move);
    require(CrossSolver::isSolved(cube), "Cross stage failed");
    verifyStageMoves(before, cube, cross, "Cross");

    before = cube;
    const auto f2l = F2LSolver::solve(cube);
    require(F2LSolver::isSolved(cube), "F2L stage failed");
    verifyStageMoves(before, cube, f2l, "F2L");

    before = cube;
    const auto oll = OLLSolver::solve(cube);
    if (!OLLSolver::isSolved(cube)) {
        std::cerr << "OLL failed (F2L=" << F2LSolver::isSolved(cube) << ") yellow face: ";
        for (const auto& row : cube.cube[YELLOW]) for (int color : row) std::cerr << color;
        std::cerr << '\n';
        require(false, "OLL stage failed");
    }
    verifyStageMoves(before, cube, oll, "OLL");

    before = cube;
    const auto pll = PLLSolver::solve(cube);
    require(PLLSolver::isSolved(cube), "PLL stage failed");
    verifyStageMoves(before, cube, pll, "PLL");
}

int main() {
    try {
        require(MoveSimplifier::simplifyAdjacentFaces({"R", "R"}) == std::vector<std::string>{"R2"}, "R R simplification failed");
        require(MoveSimplifier::simplifyAdjacentFaces({"R", "R'"}).empty(), "inverse move simplification failed");
        require(MoveSimplifier::simplifyAdjacentFaces({"R2", "R"}) == std::vector<std::string>{"R'"}, "half-turn simplification failed");
        require(MoveSimplifier::simplifyAdjacentFaces({"R", "U", "R'"}) == std::vector<std::string>({"R", "U", "R'"}), "simplifier reordered non-adjacent faces");
        solveAndVerify(solvedCube());
        for (const char* scramble : {"R", "R U F", "R U R' U'", "F2 D L' B"}) {
            RubixCube cube = solvedCube();
            MoveParser::execute(cube, scramble);
            solveAndVerify(cube);
        }

        std::srand(20261003);
        for (int index = 0; index < 20; ++index) {
            RubixCube cube = solvedCube();
            const std::string scramble = Scrambler::generateScramble(1);
            MoveParser::execute(cube, scramble);
            solveAndVerify(cube);
            std::cout << "Random scramble " << index + 1 << ": passed\n";
        }
    } catch (const std::exception& error) {
        std::cerr << "Pipeline test failed: " << error.what() << '\n';
        return 1;
    }
    std::cout << "Solved, simple, and 20 random scramble tests passed.\n";
}
