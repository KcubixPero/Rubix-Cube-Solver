#include "F2LSolver.h"

#include <stdexcept>

using namespace std;

bool F2LSolver::containsColor(
    const vector<int>& colors,
    Color color)
{
    for (int c : colors)
    {
        if (c == color)
            return true;
    }

    return false;
}

PiecePosition F2LSolver::findEdge(
    const RubixCube& cube,
    Color color1,
    Color color2)
{
    struct EdgeLocation
    {
        int face1;
        int row1;
        int col1;

        int face2;
        int row2;
        int col2;
    };

    const EdgeLocation edges[] =
    {
        // =========================
        // U LAYER
        // =========================

        // U-F
        {
            RED, 2, 1,
            WHITE, 0, 1
        },

        // U-R
        {
            RED, 1, 2,
            GREEN, 0, 1
        },

        // U-B
        {
            RED, 0, 1,
            YELLOW, 0, 1
        },

        // U-L
        {
            RED, 1, 0,
            BLUE, 0, 1
        },


        // =========================
        // MIDDLE LAYER
        // =========================

        // F-R
        {
            WHITE, 1, 2,
            GREEN, 1, 0
        },

        // F-L
        {
            WHITE, 1, 0,
            BLUE, 1, 2
        },

        // B-R
        //
        // IMPORTANT:
        // On the back face, left/right are visually reversed.
        //
        {
            YELLOW, 1, 0,
            GREEN, 1, 2
        },

        // B-L
        {
            YELLOW, 1, 2,
            BLUE, 1, 0
        },


        // =========================
        // D LAYER
        // =========================

        // F-D
        {
            WHITE, 2, 1,
            ORANGE, 0, 1
        },

        // R-D
        {
            GREEN, 2, 1,
            ORANGE, 1, 2
        },

        // B-D
        {
            YELLOW, 2, 1,
            ORANGE, 2, 1
        },

        // L-D
        {
            BLUE, 2, 1,
            ORANGE, 1, 0
        }
    };

    for (const auto& edge : edges)
    {
        vector<int> colors =
        {
            cube.cube[edge.face1][edge.row1][edge.col1],
            cube.cube[edge.face2][edge.row2][edge.col2]
        };

        if (containsColor(colors, color1) &&
            containsColor(colors, color2))
        {
            PiecePosition result;

            result.face1 = edge.face1;
            result.row1 = edge.row1;
            result.col1 = edge.col1;

            result.face2 = edge.face2;
            result.row2 = edge.row2;
            result.col2 = edge.col2;

            result.face3 = -1;
            result.row3 = -1;
            result.col3 = -1;

            result.count = 2;

            return result;
        }
    }

    throw runtime_error("Edge piece not found");
}

PiecePosition F2LSolver::findCorner(
    const RubixCube& cube,
    Color color1,
    Color color2,
    Color color3)
{
    struct CornerLocation
    {
        int face1;
        int row1;
        int col1;

        int face2;
        int row2;
        int col2;

        int face3;
        int row3;
        int col3;
    };

    const CornerLocation corners[] =
    {
        // =========================
        // U LAYER
        // =========================

        // U-F-R
        {
            RED, 2, 2,
            WHITE, 0, 2,
            GREEN, 0, 0
        },

        // U-F-L
        {
            RED, 2, 0,
            WHITE, 0, 0,
            BLUE, 0, 2
        },

        // U-B-R
        {
            RED, 0, 2,
            YELLOW, 0, 0,
            GREEN, 0, 2
        },

        // U-B-L
        {
            RED, 0, 0,
            YELLOW, 0, 2,
            BLUE, 0, 0
        },


        // =========================
        // D LAYER
        // =========================

        // D-F-R
        {
            ORANGE, 0, 2,
            WHITE, 2, 2,
            GREEN, 2, 0
        },

        // D-F-L
        {
            ORANGE, 0, 0,
            WHITE, 2, 0,
            BLUE, 2, 2
        },

        // D-B-R
        {
            ORANGE, 2, 2,
            YELLOW, 2, 0,
            GREEN, 2, 2
        },

        // D-B-L
        {
            ORANGE, 2, 0,
            YELLOW, 2, 2,
            BLUE, 2, 0
        }
    };

    for (const auto& corner : corners)
    {
        vector<int> colors =
        {
            cube.cube[corner.face1][corner.row1][corner.col1],
            cube.cube[corner.face2][corner.row2][corner.col2],
            cube.cube[corner.face3][corner.row3][corner.col3]
        };

        if (containsColor(colors, color1) &&
            containsColor(colors, color2) &&
            containsColor(colors, color3))
        {
            PiecePosition result;

            result.face1 = corner.face1;
            result.row1 = corner.row1;
            result.col1 = corner.col1;

            result.face2 = corner.face2;
            result.row2 = corner.row2;
            result.col2 = corner.col2;

            result.face3 = corner.face3;
            result.row3 = corner.row3;
            result.col3 = corner.col3;

            result.count = 3;

            return result;
        }
    }

    throw runtime_error("Corner piece not found");
}

void F2LSolver::printPairState(
    const RubixCube& cube,
    Color corner1,
    Color corner2,
    Color corner3,
    Color edge1,
    Color edge2)
{
    PiecePosition corner =
        findCorner(
            cube,
            corner1,
            corner2,
            corner3
        );

    PiecePosition edge =
        findEdge(
            cube,
            edge1,
            edge2
        );

    cout << "\n========== F2L PAIR ==========\n";

    cout << "Corner:\n";

    cout << "  Sticker 1: "
         << corner.face1 << " ["
         << corner.row1 << "]["
         << corner.col1 << "] = "
         << cube.cube[
                corner.face1
             ][
                corner.row1
             ][
                corner.col1
             ]
         << '\n';

    cout << "  Sticker 2: "
         << corner.face2 << " ["
         << corner.row2 << "]["
         << corner.col2 << "] = "
         << cube.cube[
                corner.face2
             ][
                corner.row2
             ][
                corner.col2
             ]
         << '\n';

    cout << "  Sticker 3: "
         << corner.face3 << " ["
         << corner.row3 << "]["
         << corner.col3 << "] = "
         << cube.cube[
                corner.face3
             ][
                corner.row3
             ][
                corner.col3
             ]
         << '\n';

    cout << "\nEdge:\n";

    cout << "  Sticker 1: "
         << edge.face1 << " ["
         << edge.row1 << "]["
         << edge.col1 << "] = "
         << cube.cube[
                edge.face1
             ][
                edge.row1
             ][
                edge.col1
             ]
         << '\n';

    cout << "  Sticker 2: "
         << edge.face2 << " ["
         << edge.row2 << "]["
         << edge.col2 << "] = "
         << cube.cube[
                edge.face2
             ][
                edge.row2
             ][
                edge.col2
             ]
         << '\n';

    cout << "==============================\n";
}