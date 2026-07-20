#include "../include/CubeAnalyzer.h"

const EdgePosition edgeTable[12] =
    {
        {RubixCube::WHITE, 0, 1, RubixCube::RED, 2, 1, false},
        {RubixCube::WHITE, 1, 2, RubixCube::GREEN, 1, 0, false},
        {RubixCube::WHITE, 2, 1, RubixCube::ORANGE, 0, 1, false},
        {RubixCube::WHITE, 1, 0, RubixCube::BLUE, 1, 2, false},

        {RubixCube::RED, 1, 2, RubixCube::GREEN, 0, 1, false},
        {RubixCube::RED, 1, 0, RubixCube::BLUE, 0, 1, false},

        {RubixCube::ORANGE, 1, 2, RubixCube::GREEN, 2, 1, false},
        {RubixCube::ORANGE, 1, 0, RubixCube::BLUE, 2, 1, false},

        {RubixCube::YELLOW, 0, 1, RubixCube::RED, 0, 1, false},
        {RubixCube::YELLOW, 1, 0, RubixCube::GREEN, 1, 2, false},
        {RubixCube::YELLOW, 2, 1, RubixCube::ORANGE, 2, 1, false},
        {RubixCube::YELLOW, 1, 2, RubixCube::BLUE, 1, 0, false}};

const CornerPosition cornerTable[8] =
{
    // Front-Up-Left
    {
        RubixCube::WHITE, 0,0,
        RubixCube::RED,   2,0,
        RubixCube::BLUE,  0,2,
        CornerOrientation::CORRECT
    },

    // Front-Up-Right
    {
        RubixCube::WHITE, 0,2,
        RubixCube::RED,   2,2,
        RubixCube::GREEN, 0,0,
        CornerOrientation::CORRECT
    },

    // Front-Down-Left
    {
        RubixCube::WHITE, 2,0,
        RubixCube::ORANGE,0,0,
        RubixCube::BLUE,  2,2,
        CornerOrientation::CORRECT
    },

    // Front-Down-Right
    {
        RubixCube::WHITE, 2,2,
        RubixCube::ORANGE,0,2,
        RubixCube::GREEN, 2,0,
        CornerOrientation::CORRECT
    },

    // Back-Up-Left
    {
        RubixCube::YELLOW,0,2,
        RubixCube::RED,   0,0,
        RubixCube::BLUE,  0,0,
        CornerOrientation::CORRECT
    },

    // Back-Up-Right
    {
        RubixCube::YELLOW,0,0,
        RubixCube::RED,   0,2,
        RubixCube::GREEN, 0,2,
        CornerOrientation::CORRECT
    },

    // Back-Down-Left
    {
        RubixCube::YELLOW,2,2,
        RubixCube::ORANGE,2,0,
        RubixCube::BLUE,  2,0,
        CornerOrientation::CORRECT
    },

    // Back-Down-Right
    {
        RubixCube::YELLOW,2,0,
        RubixCube::ORANGE,2,2,
        RubixCube::GREEN, 2,2,
        CornerOrientation::CORRECT
    }
};

bool isEdge(int a, int b, RubixCube::Faces c1, RubixCube::Faces c2)
{
    return (a == c1 && b == c2) || (a == c2 && b == c1);
}

bool isCorner(int a, int b, int c, RubixCube::Faces x, RubixCube::Faces y, RubixCube::Faces z)
{
    return
        (a == x || a == y || a == z) && (b == x || b == y || b == z) && (c == x || c == y || c == z);
}

EdgePosition CubeAnalyzer::findEdge(RubixCube::Faces color1, RubixCube::Faces color2) const
{
    for (const auto &edge : edgeTable)
    {
        if (isEdge(
                cube.cube[edge.face1][edge.row1][edge.col1],
                cube.cube[edge.face2][edge.row2][edge.col2],
                color1, color2))
        {
            EdgePosition result = edge;

            result.flipped = (cube.cube[edge.face1][edge.row1][edge.col1] == color2);

            return result;
        }
    }
}

CornerPosition CubeAnalyzer::findCorner(RubixCube::Faces color1, RubixCube::Faces color2, RubixCube::Faces color3) const
{
    for (const auto &corner : cornerTable)
    {
        int a = cube.cube[corner.face1][corner.row1][corner.col1];
        int b = cube.cube[corner.face2][corner.row2][corner.col2];
        int c = cube.cube[corner.face3][corner.row3][corner.col3];

        if (isCorner(a, b, c, color1, color2, color3))
        {
            CornerPosition result = corner;

            if (a == color1)
                result.orientation = CornerOrientation::CORRECT;
            else if (b == color1)
                result.orientation = CornerOrientation::TWIST_1;
            else
                result.orientation = CornerOrientation::TWIST_2;

            return result;
        }
    }

    throw runtime_error("Corner not found.");
}

bool CubeAnalyzer::crossSolved()
{
}

bool CubeAnalyzer::f2lSolved()
{
}

bool CubeAnalyzer::ollSolved()
{
}

bool CubeAnalyzer::pllSolved()
{
}