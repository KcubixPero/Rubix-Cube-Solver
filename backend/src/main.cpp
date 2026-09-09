#include <iostream>
#include <vector>
#include <string>

#include "RubixCube.h"
#include "MoveParser.h"
#include "Scrambler.h"
#include "LegacySolver.h"

using namespace std;

int main()
{
    // ================= CREATE SOLVED CUBE =================

    vector3d solvedCube(6, vector2d(3, vector<int>(3)));

    for (int face = 0; face < 6; face++)
    {
        for (int row = 0; row < 3; row++)
        {
            for (int col = 0; col < 3; col++)
            {
                solvedCube[face][row][col] = face;
            }
        }
    }

    RubixCube cube(solvedCube);

    cout << "================ SOLVED CUBE ================\n";
    cube.print();


    // ================= SCRAMBLE =================

    string scramble = "R R2 B F";

    cout << "\n================ SCRAMBLE ================\n";
    cout << scramble << "\n";

    MoveParser::execute(cube, scramble);

    cout << "\n================ OUR SCRAMBLED CUBE ================\n";
    cube.print();


    // ================= SOLVE =================

    cout << "\n================ STARTING LEGACY SOLVER ================\n";

    LegacySolver solver;

    vector<string> solution = solver.solve(cube);


    // ================= PRINT SOLUTION =================

    string solutionString;

    for (const string &move : solution)
    {
        if (!solutionString.empty())
            solutionString += " ";

        solutionString += move;
    }

    cout << "\n================ SOLUTION ================\n";
    cout << solutionString << "\n";


    // ================= VERIFY =================

    cout << "\n================ VERIFYING SOLUTION ================\n";

    MoveParser::execute(cube, solutionString);

    cube.print();

    return 0;
}