#include "F2LSolver.h"

#include "Color.h"
#include "RubixCube.h"

namespace
{
bool stickerIsSolved(const RubixCube &cube, int face, int row, int col)
{
    return cube.cube[face][row][col] == face;
}
}

bool F2LSolver::isSolved(const RubixCube &cube) const
{
    // First layer: all White-face pieces, including the four corner pairs.
    for (int row = 0; row < 3; ++row)
        for (int col = 0; col < 3; ++col)
            if (!stickerIsSolved(cube, WHITE, row, col)) return false;

    // The side stickers belonging to White's four corners/cross edges.
    for (int col = 0; col < 3; ++col)
    {
        if (!stickerIsSolved(cube, RED, 2, col) ||
            !stickerIsSolved(cube, ORANGE, 0, col)) return false;
    }
    for (int row = 0; row < 3; ++row)
    {
        if (!stickerIsSolved(cube, BLUE, row, 2) ||
            !stickerIsSolved(cube, GREEN, row, 0)) return false;
    }

    // The four equatorial (non-White/non-Yellow) edges are the F2L second layer.
    return stickerIsSolved(cube, RED, 1, 0) && stickerIsSolved(cube, BLUE, 0, 1) &&
           stickerIsSolved(cube, BLUE, 2, 1) && stickerIsSolved(cube, ORANGE, 1, 0) &&
           stickerIsSolved(cube, ORANGE, 1, 2) && stickerIsSolved(cube, GREEN, 2, 1) &&
           stickerIsSolved(cube, GREEN, 0, 1) && stickerIsSolved(cube, RED, 1, 2);
}
