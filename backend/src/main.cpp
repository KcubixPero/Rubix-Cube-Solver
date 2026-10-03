#include "CrossSolver.h"
#include "F2LSolver.h"
#include "HttpApi.h"
#include "MoveParser.h"
#include "OLLSolver.h"
#include "PLLSolver.h"
#include "RubixCube.h"
#include "Scrambler.h"

#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

namespace {

RubixCube makeSolvedCube()
{
    vector3d faces(6, vector2d(3, std::vector<int>(3)));

    for (int face = 0; face < 6; ++face)
        for (int row = 0; row < 3; ++row)
            for (int col = 0; col < 3; ++col)
                faces[face][row][col] = face;

    return RubixCube(faces);
}

std::string joinMoves(const std::vector<std::string>& moves)
{
    std::string result;

    for (const auto& move : moves)
    {
        if (!result.empty())
            result += ' ';

        result += move;
    }

    return result;
}

void applyMoves(RubixCube& cube, const std::vector<std::string>& moves)
{
    if (!moves.empty())
        MoveParser::execute(cube, joinMoves(moves));
}

void printStage(
    const std::string& name,
    const std::vector<std::string>& moves,
    RubixCube& cube)
{
    std::cout << "\n===== " << name << " =====\n";
    std::cout << "Moves: " << joinMoves(moves) << '\n';
    std::cout << "Count: " << moves.size() << "\n\n";

    std::cout << "Cube after " << name << ":\n";
    cube.print();
}

} // namespace

int runDemo()
{
    RubixCube cube = makeSolvedCube();

    const std::string scramble = Scrambler::generateScramble(25);

    std::cout << "========================================\n";
    std::cout << "              KCUBIXLAB\n";
    std::cout << "========================================\n\n";

    std::cout << "Scramble:\n" << scramble << "\n";

    MoveParser::execute(cube, scramble);

    try
    {
        // ==================== CROSS ====================

        const auto cross = CrossSolver::solve(cube);
        applyMoves(cube, cross);

        if (!CrossSolver::isSolved(cube))
            throw std::runtime_error("White Cross verification failed.");

        printStage("WHITE CROSS", cross, cube);

        // ==================== F2L ====================

        const auto f2l = F2LSolver::solve(cube);

        if (!F2LSolver::isSolved(cube))
            throw std::runtime_error("F2L verification failed.");

        printStage("F2L", f2l, cube);

        // ==================== OLL ====================

        const auto oll = OLLSolver::solve(cube);

        if (!OLLSolver::isSolved(cube))
            throw std::runtime_error("OLL verification failed.");

        printStage("OLL", oll, cube);

        // ==================== PLL ====================

        const auto pll = PLLSolver::solve(cube);

        if (!PLLSolver::isSolved(cube))
            throw std::runtime_error("PLL verification failed.");

        printStage("PLL", pll, cube);

        // ==================== FINAL ====================

        const int totalMoves =
            static_cast<int>(cross.size()) +
            static_cast<int>(f2l.size()) +
            static_cast<int>(oll.size()) +
            static_cast<int>(pll.size());

        std::cout << "\n========================================\n";
        std::cout << "             FINAL RESULT\n";
        std::cout << "========================================\n";

        std::cout << "Cross : " << cross.size() << '\n';
        std::cout << "F2L   : " << f2l.size() << '\n';
        std::cout << "OLL   : " << oll.size() << '\n';
        std::cout << "PLL   : " << pll.size() << '\n';
        std::cout << "----------------------\n";
        std::cout << "Total : " << totalMoves << "\n\n";

        if (!PLLSolver::isSolved(cube))
            throw std::runtime_error("Final cube verification failed.");

        std::cout << "CUBE SOLVED\n";
    }
    catch (const std::exception& error)
    {
        std::cerr << "\nSolver failed: " << error.what() << '\n';
        return 1;
    }

    return 0;
}

int main(int argc, char** argv)
{
    if (argc > 1 && std::string(argv[1]) == "--server")
    {
        try
        {
            unsigned short port = 8080;

            if (argc > 2)
                port = static_cast<unsigned short>(std::stoi(argv[2]));

            return runHttpApi(port);
        }
        catch (const std::exception& error)
        {
            std::cerr << "API server failed: " << error.what() << '\n';
            return 1;
        }
    }

    return runDemo();
}