#include "RubixCube.h"
#include "F2LSolver.h"
#include "MoveParser.h"

#include <iostream>
#include <vector>

using namespace std;

int main()
{
    vector3d solved(
        6,
        vector2d(3, vector<int>(3))
    );

    for (int face = 0; face < 6; face++)
    {
        for (int row = 0; row < 3; row++)
        {
            for (int col = 0; col < 3; col++)
            {
                solved[face][row][col] = face;
            }
        }
    }

    RubixCube cube(solved);

    cout << "=== SOLVED ===\n";

    F2LSolver::printPairState(
        cube,
        WHITE,
        GREEN,
        ORANGE,
        WHITE,
        GREEN
    );

    cout << "\n=== AFTER U ===\n";

    MoveParser::execute(cube, "U");

    F2LSolver::printPairState(
        cube,
        WHITE,
        GREEN,
        ORANGE,
        WHITE,
        GREEN
    );

    cout << "\n=== AFTER U R ===\n";

    MoveParser::execute(cube, "R");

    F2LSolver::printPairState(
        cube,
        WHITE,
        GREEN,
        ORANGE,
        WHITE,
        GREEN
    );

    return 0;
}