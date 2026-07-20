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
            RubixCube::WHITE, 0, 0,
            RubixCube::RED, 2, 0,
            RubixCube::BLUE, 0, 2,
            CornerOrientation::CORRECT},

        // Front-Up-Right
        {
            RubixCube::WHITE, 0, 2,
            RubixCube::RED, 2, 2,
            RubixCube::GREEN, 0, 0,
            CornerOrientation::CORRECT},

        // Front-Down-Left
        {
            RubixCube::WHITE, 2, 0,
            RubixCube::ORANGE, 0, 0,
            RubixCube::BLUE, 2, 2,
            CornerOrientation::CORRECT},

        // Front-Down-Right
        {
            RubixCube::WHITE, 2, 2,
            RubixCube::ORANGE, 0, 2,
            RubixCube::GREEN, 2, 0,
            CornerOrientation::CORRECT},

        // Back-Up-Left
        {
            RubixCube::YELLOW, 0, 2,
            RubixCube::RED, 0, 0,
            RubixCube::BLUE, 0, 0,
            CornerOrientation::CORRECT},

        // Back-Up-Right
        {
            RubixCube::YELLOW, 0, 0,
            RubixCube::RED, 0, 2,
            RubixCube::GREEN, 0, 2,
            CornerOrientation::CORRECT},

        // Back-Down-Left
        {
            RubixCube::YELLOW, 2, 2,
            RubixCube::ORANGE, 2, 0,
            RubixCube::BLUE, 2, 0,
            CornerOrientation::CORRECT},

        // Back-Down-Right
        {
            RubixCube::YELLOW, 2, 0,
            RubixCube::ORANGE, 2, 2,
            RubixCube::GREEN, 2, 2,
            CornerOrientation::CORRECT}};

bool isEdge(int a, int b, RubixCube::Faces c1, RubixCube::Faces c2)
{
    return (a == c1 && b == c2) || (a == c2 && b == c1);
}

bool isCorner(int a, int b, int c, RubixCube::Faces x, RubixCube::Faces y, RubixCube::Faces z)
{
    return (a == x || a == y || a == z) && (b == x || b == y || b == z) && (c == x || c == y || c == z);
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

    throw runtime_error("Edge not found.");
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

bool CubeAnalyzer::crossSolved() const
{
    EdgePosition e = findEdge(RubixCube::WHITE, RubixCube::RED);

    if (e.face1 != RubixCube::WHITE || e.face2 != RubixCube::RED || e.flipped)
        return false;

    e = findEdge(RubixCube::WHITE, RubixCube::BLUE);

    if (e.face1 != RubixCube::WHITE || e.face2 != RubixCube::BLUE || e.flipped)
        return false;

    e = findEdge(RubixCube::WHITE, RubixCube::GREEN);

    if (e.face1 != RubixCube::WHITE || e.face2 != RubixCube::GREEN || e.flipped)
        return false;

    e = findEdge(RubixCube::WHITE, RubixCube::ORANGE);

    if (e.face1 != RubixCube::WHITE || e.face2 != RubixCube::ORANGE || e.flipped)
        return false;

    return true;
}

bool CubeAnalyzer::f2lSolved() const
{
    if (!crossSolved())
        return false;

    // Edge pieces

    EdgePosition e = findEdge(RubixCube::BLUE, RubixCube::RED);

    if (e.face1 != RubixCube::RED || e.face2 != RubixCube::BLUE || e.flipped)
        return false;

    e = findEdge(RubixCube::RED, RubixCube::GREEN);

    if (e.face1 != RubixCube::RED || e.face2 != RubixCube::GREEN || e.flipped)
        return false;

    e = findEdge(RubixCube::ORANGE, RubixCube::BLUE);

    if (e.face1 != RubixCube::ORANGE || e.face2 != RubixCube::BLUE || e.flipped)
        return false;

    e = findEdge(RubixCube::GREEN, RubixCube::ORANGE);

    if (e.face1 != RubixCube::ORANGE || e.face2 != RubixCube::GREEN || e.flipped)
        return false;

    // Corner pieces

    CornerPosition c = findCorner(RubixCube::WHITE, RubixCube::BLUE, RubixCube::RED);

    if (c.face1 != RubixCube::WHITE || c.face2 != RubixCube::RED || c.face3 != RubixCube::BLUE ||
        c.orientation != CornerOrientation::CORRECT)
        return false;

    c = findCorner(RubixCube::WHITE, RubixCube::RED, RubixCube::GREEN);

    if (c.face1 != RubixCube::WHITE || c.face2 != RubixCube::RED || c.face3 != RubixCube::GREEN ||
        c.orientation != CornerOrientation::CORRECT)
        return false;

    c = findCorner(RubixCube::WHITE, RubixCube::ORANGE, RubixCube::BLUE);

    if (c.face1 != RubixCube::WHITE || c.face2 != RubixCube::ORANGE || c.face3 != RubixCube::BLUE ||
        c.orientation != CornerOrientation::CORRECT)
        return false;

    c = findCorner(RubixCube::WHITE, RubixCube::GREEN, RubixCube::ORANGE);

    if (c.face1 != RubixCube::WHITE || c.face2 != RubixCube::ORANGE || c.face3 != RubixCube::GREEN ||
        c.orientation != CornerOrientation::CORRECT)
        return false;

    return true;
}

bool CubeAnalyzer::ollSolved() const
{
    if (!f2lSolved())
        return false;

    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            if (cube.cube[RubixCube::YELLOW][i][j] != RubixCube::YELLOW)
                return false;
        }
    }
    return true;
}

bool CubeAnalyzer::pllSolved() const
{
    if (!ollSolved())
        return false;

    return ((cube.cube[RubixCube::RED][0] == vector<int>{1, 1, 1}) &&
            (cube.cube[RubixCube::BLUE][0] == vector<int>{2, 2, 2}) &&
            (cube.cube[RubixCube::GREEN][0] == vector<int>{3, 3, 3}) &&
            (cube.cube[RubixCube::ORANGE][0] == vector<int>{4, 4, 4}));
}