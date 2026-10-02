#include "Color.h"
#include "CrossSolver.h"
#include "F2LSolver.h"
#include "MoveParser.h"
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
    return result;
}

std::size_t countMoves(const std::string& sequence) {
    return MoveParser::tokenize(sequence).size();
}
}

int main() {
    // Start solved, scramble through the same move parser used for solutions.
    RubixCube cube = makeSolvedCube();
    const std::string scramble = Scrambler::generateScramble(25);

    std::cout << "========== WHITE CROSS + F2L TEST ==========\n\n";
    std::cout << "Initial state: SOLVED\n";
    std::cout << "Scramble (25 moves): " << scramble << "\n";
    MoveParser::execute(cube, scramble);
    std::cout << "\nCube after scramble:\n";
    cube.print();

    // Solve and apply the white cross.
    std::cout << "\n========== WHITE CROSS ==========\n";
    const std::vector<std::string> crossMoves = CrossSolver::solve(cube);
    if (crossMoves.empty() && !CrossSolver::isSolved(cube)) {
        std::cerr << "WHITE CROSS: FAILED (solver returned no solution)\n";
        return 1;
    }
    MoveParser::execute(cube, joinMoves(crossMoves));
    if (!CrossSolver::isSolved(cube)) {
        std::cerr << "WHITE CROSS: FAILED (verification failed)\n";
        cube.print();
        return 1;
    }
    std::cout << "Moves: " << (crossMoves.empty() ? "(none)" : joinMoves(crossMoves)) << "\n";
    std::cout << "Move count: " << crossMoves.size() << "\n";
    std::cout << "Status: SOLVED\n";

    // F2LSolver applies its solution directly to the cube after a solved cross.
    std::cout << "\n========== F2L ==========\n";
    F2LSolver f2l(cube);
    if (!f2l.solve()) {
        std::cerr << "F2L: FAILED - " << f2l.getError() << "\n";
        cube.print();
        return 1;
    }
    if (!CrossSolver::isSolved(cube) || !F2LSolver::isSolved(cube)) {
        std::cerr << "F2L: FAILED (cross/F2L verification failed)\n";
        cube.print();
        return 1;
    }
    std::cout << "Moves: " << (f2l.getMoves().empty() ? "(none)" : f2l.getMoves()) << "\n";
    std::cout << "Move count: " << countMoves(f2l.getMoves()) << "\n";
    std::cout << "Status: SOLVED\n";

    std::cout << "\n========== RESULT ==========\n";
    std::cout << "Cross + F2L moves: " << crossMoves.size() + countMoves(f2l.getMoves()) << "\n";
    std::cout << "White cross: " << (CrossSolver::isSolved(cube) ? "SOLVED" : "FAILED") << "\n";
    std::cout << "F2L: " << (F2LSolver::isSolved(cube) ? "SOLVED" : "FAILED") << "\n";
    std::cout << "Cube after Cross + F2L:\n";
    cube.print();
    return 0;
}
