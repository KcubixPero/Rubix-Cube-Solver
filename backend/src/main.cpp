#include "RubixCube.h"
#include "MoveParser.h"
#include "CrossSolver.h"
#include "Scrambler.h"
#include "Color.h"
#include <iostream>
#include <string>
#include <vector>

using namespace std;

int main()
{
    // ============================================================
    // 1. Create solved cube
    // ============================================================

    vector3d sides(
        6,
        vector2d(
            3,
            vector<int>(3)
        )
    );

    for (int face = 0; face < 6; face++)
    {
        for (int row = 0; row < 3; row++)
        {
            for (int col = 0; col < 3; col++)
            {
                sides[face][row][col] = face;
            }
        }
    }

    RubixCube cube(sides);

    cout << "==============================\n";
    cout << "       A* WHITE CROSS TEST\n";
    cout << "==============================\n\n";

    // ============================================================
    // 2. Generate random scramble
    // ============================================================

    string scramble = Scrambler::generateScramble();

    cout << "Scramble:\n";
    cout << scramble << "\n\n";

    // ============================================================
    // 3. Apply scramble
    // ============================================================

    MoveParser::execute(cube, scramble);

    cout << "Scrambled cube:\n";
    cube.print();

    // ============================================================
    // 4. Solve White Cross using A*
    // ============================================================

    cout << "\n==============================\n";
    cout << "       RUNNING A*\n";
    cout << "==============================\n\n";

    vector<string> solution =
        CrossSolver::solve(cube);

    // ============================================================
    // 5. Print solution
    // ============================================================

    cout << "White Cross solution:\n";

    if (solution.empty())
    {
        cout << "(No moves needed)\n";
    }
    else
    {
        for (const string& move : solution)
        {
            cout << move << " ";
        }

        cout << "\n";
    }

    cout << "\nNumber of moves: "
         << solution.size()
         << "\n";

    // ============================================================
    // 6. Apply White Cross solution
    // ============================================================

    string solutionString;

    for (const string& move : solution)
    {
        solutionString += move + " ";
    }

    MoveParser::execute(cube, solutionString);

    // ============================================================
    // 7. Print resulting cube
    // ============================================================

    cout << "\nCube after White Cross solution:\n";
    cube.print();

    // ============================================================
    // 8. Verify White Cross
    // ============================================================

    bool crossSolved = true;

    // White-Red
    if (cube.cube[WHITE][0][1] != WHITE ||
        cube.cube[RED][2][1] != RED)
    {
        crossSolved = false;
    }

    // White-Green
    if (cube.cube[WHITE][1][2] != WHITE ||
        cube.cube[GREEN][1][0] != GREEN)
    {
        crossSolved = false;
    }

    // White-Orange
    if (cube.cube[WHITE][2][1] != WHITE ||
        cube.cube[ORANGE][0][1] != ORANGE)
    {
        crossSolved = false;
    }

    // White-Blue
    if (cube.cube[WHITE][1][0] != WHITE ||
        cube.cube[BLUE][1][2] != BLUE)
    {
        crossSolved = false;
    }

    cout << "\n==============================\n";

    if (crossSolved)
    {
        cout << "WHITE CROSS: SOLVED\n";
    }
    else
    {
        cout << "WHITE CROSS: FAILED\n";
    }

    cout << "==============================\n";

    return 0;
}