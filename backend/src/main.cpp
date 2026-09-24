#include <iostream>
#include <string>
#include <vector>

#include "CrossSolver.h"
#include "F2LSolver.h"
#include "MoveParser.h"
#include "OLLSolver.h"
#include "PLLSolver.h"
#include "RubixCube.h"
#include "Scrambler.h"

using namespace std;

int main()
{
    // 1. Create solved cube
    vector3d solvedCube(6, vector2d(3, vector<int>(3)));
    for (int face = 0; face < 6; ++face)
        for (int row = 0; row < 3; ++row)
            for (int col = 0; col < 3; ++col)
                solvedCube[face][row][col] = face;

    RubixCube cube(solvedCube);

    cout << "========== INITIAL CUBE ==========\n";
    cube.print();

    // 2. Generate and apply a random scramble through MoveParser
    string scramble = Scrambler::generateScramble(3);
    cout << "\n========== SCRAMBLE ==========\n" << scramble << "\n";

    vector<string> scrambleMoves = MoveParser::tokenize(scramble);
    for (const string &move : scrambleMoves)
        MoveParser::execute(cube, move);

    cout << "\n========== SCRAMBLED CUBE ==========\n";
    cube.print();

    // 3. Solve White Cross
    CrossSolver cross;
    BFSResult whiteResult = cross.solve(cube);
    vector<Move> whiteMoves = whiteResult.moves;
    cout << "\n========== WHITE CROSS ==========\n";
    for (const Move &move : whiteMoves) cout << move.notation << " ";
    cout << "\n";
    for (const Move &move : whiteMoves) MoveParser::execute(cube, move.notation);

    // 4. Solve F2L
    F2LSolver f2l;
    BFSResult f2lResult = f2l.solve(cube);
    vector<Move> f2lMoves = f2lResult.moves;
    cout << "\n========== F2L ==========\n";
    for (const Move &move : f2lMoves) cout << move.notation << " ";
    cout << "\n";
    for (const Move &move : f2lMoves) MoveParser::execute(cube, move.notation);

    // 5. Solve OLL
    OLLSolver oll;
    BFSResult ollResult = oll.solve(cube);
    vector<Move> ollMoves = ollResult.moves;
    cout << "\n========== OLL ==========\n";
    for (const Move &move : ollMoves) cout << move.notation << " ";
    cout << "\n";
    for (const Move &move : ollMoves) MoveParser::execute(cube, move.notation);

    // 6. Solve PLL
    PLLSolver pll;
    BFSResult pllResult = pll.solve(cube);
    vector<Move> pllMoves = pllResult.moves;
    cout << "\n========== PLL ==========\n";
    for (const Move &move : pllMoves) cout << move.notation << " ";
    cout << "\n";
    for (const Move &move : pllMoves) MoveParser::execute(cube, move.notation);

    // 7. Concatenate the phase solutions
    vector<Move> finalMoves;
    finalMoves.insert(finalMoves.end(), whiteMoves.begin(), whiteMoves.end());
    finalMoves.insert(finalMoves.end(), f2lMoves.begin(), f2lMoves.end());
    finalMoves.insert(finalMoves.end(), ollMoves.begin(), ollMoves.end());
    finalMoves.insert(finalMoves.end(), pllMoves.begin(), pllMoves.end());

    cout << "\n========== FINAL SOLUTION ==========\n";
    for (const Move &move : finalMoves) cout << move.notation << " ";
    cout << "\n";

    // 8. Print and verify final cube
    cout << "\n========== FINAL CUBE ==========\n";
    cube.print();
    cout << "\nCube solved: " << (pll.isSolved(cube) ? "YES" : "NO") << "\n";

    return 0;
}
