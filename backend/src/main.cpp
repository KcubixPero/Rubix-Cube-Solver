#include <iostream>
#include <bitset>

#include "RubixCube.h"
#include "CubeState.h"
#include "Color.h"

using namespace std;

int main()
{
    // ---------- Create solved cube ----------
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

    // ---------- Test sequence ----------
    cube.R();
    cube.F();

    cout << "================ STICKER CUBE ================\n";
    cube.print();

    CubeState state = cube.toCubeState();

    cout << "\n================ EDGES ================\n";

    for (int i = 0; i < 12; i++)
    {
        EdgeCubie edge = state.getEdge(i);

        cout << "Position "
             << i
             << " : ID = "
             << edge.id
             << "  Flip = "
             << edge.flipped
             << '\n';
    }

    cout << "\n================ CORNERS ================\n";

    for (int i = 0; i < 8; i++)
    {
        CornerCubie corner = state.getCorner(i);

        cout << "Position "
             << i
             << " : ID = "
             << corner.id
             << "  Orientation = "
             << corner.orientation
             << '\n';
    }

    cout << "\n================ PACKED STATE ================\n";

    cout << "\nEdge State (decimal)\n";
    cout << state.getEdgeState() << '\n';

    cout << "\nEdge State (binary)\n";
    cout << bitset<64>(state.getEdgeState()) << '\n';

    cout << "\nCorner State (decimal)\n";
    cout << state.getCornerState() << '\n';

    cout << "\nCorner State (binary)\n";
    cout << bitset<64>(state.getCornerState()) << '\n';

    return 0;
}