#include "F2LSolver.h"

#include "CrossSolver.h"
#include "Color.h"
#include "LegacyStageSupport.h"
#include "MoveSimplifier.h"

#include <stdexcept>

namespace
{
    struct Cell
    {
        int face;
        int row;
        int col;
    };

    Cell cellOnFace(int face, int x, int y, int z)
    {
        switch (face)
        {
        case WHITE:
            return {face, 1 - y, x + 1};
        case YELLOW:
            return {face, 1 - y, 1 - x};
        case RED:
            return {face, z + 1, x + 1};
        case ORANGE:
            return {face, 1 - z, x + 1};
        case GREEN:
            return {face, 1 - y, 1 - z};
        default:
            return {face, 1 - y, z + 1};
        }
    }

    bool slotSolved(const RubixCube &cube, const int corner[3], const int edge[3])
    {
        for (int face = 0; face < 6; ++face)
        {
            const int x = corner[0], y = corner[1], z = corner[2];
            const bool hasCornerSticker = (face == WHITE && z == 1) || (face == YELLOW && z == -1) ||
                                          (face == RED && y == 1) || (face == ORANGE && y == -1) ||
                                          (face == GREEN && x == 1) || (face == BLUE && x == -1);
            if (hasCornerSticker)
            {
                const Cell cell = cellOnFace(face, x, y, z);
                if (cube.cube[face][cell.row][cell.col] != face)
                    return false;
            }
            const int ex = edge[0], ey = edge[1], ez = edge[2];
            const bool hasEdgeSticker = (face == WHITE && ez == 1) || (face == YELLOW && ez == -1) ||
                                        (face == RED && ey == 1) || (face == ORANGE && ey == -1) ||
                                        (face == GREEN && ex == 1) || (face == BLUE && ex == -1);
            if (hasEdgeSticker)
            {
                const Cell cell = cellOnFace(face, ex, ey, ez);
                if (cube.cube[face][cell.row][cell.col] != face)
                    return false;
            }
        }
        return true;
    }
}

std::vector<std::string> F2LSolver::solve(RubixCube &cube)
{
    if (!CrossSolver::isSolved(cube))
    {
        throw std::invalid_argument("F2L requires the existing CrossSolver to solve the white cross first.");
    }
    return MoveSimplifier::simplifyAdjacentFaces(LegacyStageSupport::solveF2L(cube));
}

bool F2LSolver::isSolved(const RubixCube &cube)
{
    if (!CrossSolver::isSolved(cube))
        return false;
    static const int corners[4][3] = {{1, 1, 1}, {1, -1, 1}, {-1, -1, 1}, {-1, 1, 1}};
    static const int edges[4][3] = {{1, 1, 0}, {1, -1, 0}, {-1, -1, 0}, {-1, 1, 0}};
    for (int slot = 0; slot < 4; ++slot)
        if (!slotSolved(cube, corners[slot], edges[slot]))
            return false;
    return true;
}
