#include "RubixCube.h"
#include "CubeState.h"
#include "MoveParser.h"
#include "Scrambler.h"
#include "Solver.h"
#include "Color.h"

int main()
{
    vector3d solved(6, vector2d(3, vector<int>(3)));

    for (int f = 0; f < 6; f++)
        for (int i = 0; i < 3; i++)
            for (int j = 0; j < 3; j++)
                solved[f][i][j] = f;

    RubixCube cube(solved);

    cout << "================ SOLVED CUBE ================\n";
    cube.print();

    cout << "\n========== CubeState ==========\n";
    cube.toCubeState();

    cout << "\n=============================================\n";

    cube.R();

    cout << "\n================ AFTER R ====================\n";
    cube.print();

    cout << "\n========== CubeState ==========\n";
    cube.toCubeState();

    cout << "\n=============================================\n";

    cube.F();

    cout << "\n================ AFTER F ====================\n";
    cube.print();

    cout << "\n========== CubeState ==========\n";
    cube.toCubeState();

    cout << "\n=============================================\n";

    return 0;
}