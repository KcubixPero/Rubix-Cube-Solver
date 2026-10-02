#ifndef F2LSOLVER_H
#define F2LSOLVER_H

#include "RubixCube.h"
#include "Color.h"
#include <vector>

using namespace std;

struct PiecePosition
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

    int count;
};

enum class F2LCaseType
{
    UNKNOWN,

    // Corner/edge in U layer
    CORNER_UFR,
    EDGE_UF,

    // Other basic locations will be added later
};

class F2LSolver
{
public:
    static PiecePosition findEdge(
        const RubixCube& cube,
        Color color1,
        Color color2
    );

    static PiecePosition findCorner(
        const RubixCube& cube,
        Color color1,
        Color color2,
        Color color3
    );

    static void printPairState(
        const RubixCube& cube,
        Color corner1,
        Color corner2,
        Color corner3,
        Color edge1,
        Color edge2
    );

private:
    static bool containsColor(
        const vector<int>& colors,
        Color color
    );
};

#endif