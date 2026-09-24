#include "CrossSolver.h"

#include "Color.h"
#include "RubixCube.h"

bool CrossSolver::isSolved(const RubixCube &cube) const
{
    // The White face is the project's front face.  Each listed edge must
    // match both the front centre and its adjacent side centre.
    return cube.cube[WHITE][0][1] == WHITE && cube.cube[RED][2][1] == RED &&
           cube.cube[WHITE][1][2] == WHITE && cube.cube[GREEN][1][0] == GREEN &&
           cube.cube[WHITE][2][1] == WHITE && cube.cube[ORANGE][0][1] == ORANGE &&
           cube.cube[WHITE][1][0] == WHITE && cube.cube[BLUE][1][2] == BLUE;
}
