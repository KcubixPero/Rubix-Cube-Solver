#include "Color.h"
#include "CrossSolver.h"
#include "F2LSolver.h"
#include "MoveParser.h"
#include "OLLSolver.h"
#include "PLLSolver.h"
#include "RubixCube.h"
#include "Scrambler.h"

#include <iostream>
#include <string>
#include <vector>

namespace {
RubixCube makeSolvedCube() {
    vector3d faces(6, vector2d(3, std::vector<int>(3)));
    for (int face = 0; face < 6; ++face)
        for (int row = 0; row < 3; ++row)
            for (int col = 0; col < 3; ++col)
                faces[face][row][col] = face;
    return RubixCube(faces);
}

std::string joinMoves(const std::vector<std::string>& moves) {
    std::string result;
    for (const auto& move : moves) {
        if (!result.empty()) result += ' ';
        result += move;
    }
    return result.empty() ? "(none)" : result;
}

bool isSolved(const RubixCube& cube) {
    for (int face = 0; face < 6; ++face)
        for (int row = 0; row < 3; ++row)
            for (int col = 0; col < 3; ++col)
                if (cube.cube[face][row][col] != face) return false;
    return true;
}

void printStage(const char* name, const std::vector<std::string>& moves) {
    std::cout << "\n===== " << name << " =====\n" << joinMoves(moves) << '\n';
}
}

int main() {
    RubixCube cube = makeSolvedCube();
    const std::string scramble = Scrambler::generateScramble(25);
    std::cout << "===== SCRAMBLE =====\n" << scramble << '\n';
    MoveParser::execute(cube, scramble);

    try {
        const auto cross = CrossSolver::solve(cube);
        MoveParser::execute(cube, joinMoves(cross) == "(none)" ? "" : joinMoves(cross));
        if (!CrossSolver::isSolved(cube)) throw std::runtime_error("White Cross verification failed.");
        printStage("CROSS", cross);

        const auto f2l = F2LSolver::solve(cube);
        if (!F2LSolver::isSolved(cube)) throw std::runtime_error("F2L verification failed.");
        printStage("F2L", f2l);
        const auto oll = OLLSolver::solve(cube);
        if (!OLLSolver::isSolved(cube)) throw std::runtime_error("OLL verification failed.");
        printStage("OLL", oll);
        const auto pll = PLLSolver::solve(cube);
        if (!PLLSolver::isSolved(cube)) throw std::runtime_error("PLL verification failed.");
        printStage("PLL", pll);

        std::vector<std::string> full;
        full.insert(full.end(), cross.begin(), cross.end());
        full.insert(full.end(), f2l.begin(), f2l.end());
        full.insert(full.end(), oll.begin(), oll.end());
        full.insert(full.end(), pll.begin(), pll.end());
        printStage("FINAL SOLUTION", full);
        if (!isSolved(cube) || !PLLSolver::isSolved(cube)) {
            std::cerr << "Final cube verification failed.\n";
            return 1;
        }
        std::cout << "\nCube solved and verified.\n";
    } catch (const std::exception& error) {
        std::cerr << "Solver failed: " << error.what() << '\n';
        return 1;
    }
    return 0;
}
