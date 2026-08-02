#include "RubixCube.h"
#include "CubeState.h"
#include "Edge.h"
#include "Corner.h"

#include "RubixCube.h"
#include "CubeState.h"
#include "Edge.h"
#include "Corner.h"

/*
  FACE COLOR / ORIENTATION MAP:
            [RED (U)]
  [BLUE (L)] [WHITE (F)] [GREEN (R)] [YELLOW (B)]
            [ORANGE (D)]
*/

RubixCube::RubixCube(const vector3d &sides)
{
    cube.resize(6, vector2d(3, vector<int>(3)));

    for (int i = 0; i < 6; i++)
    {
        cube[i] = sides[i];
    }
}

// Rotates a 3x3 face matrix 90 degrees clockwise in-place
void RubixCube::rotateClockwise(int face)
{
    // 1. Swap Corners Clockwise
    int temp = cube[face][0][0];
    cube[face][0][0] = cube[face][2][0];
    cube[face][2][0] = cube[face][2][2];
    cube[face][2][2] = cube[face][0][2];
    cube[face][0][2] = temp;

    // 2. Swap Edges Clockwise
    temp = cube[face][0][1];
    cube[face][0][1] = cube[face][1][0];
    cube[face][1][0] = cube[face][2][1];
    cube[face][2][1] = cube[face][1][2];
    cube[face][1][2] = temp;
}

// --- RIGHT MOVE (R) ---
void RubixCube::R()
{
    rotateClockwise(GREEN);

    for (int i = 0; i < 3; i++)
    {
        int temp = cube[RED][i][2];
        cube[RED][i][2] = cube[WHITE][i][2];
        cube[WHITE][i][2] = cube[ORANGE][i][2];
        cube[ORANGE][i][2] = cube[YELLOW][2 - i][0];
        cube[YELLOW][2 - i][0] = temp;
    }
}

void RubixCube::R_()
{
    R();
    R();
    R();
}

// --- LEFT MOVE (L) ---
void RubixCube::L()
{
    rotateClockwise(BLUE);

    for (int i = 0; i < 3; i++)
    {
        int temp = cube[RED][i][0];
        cube[RED][i][0] = cube[YELLOW][2 - i][2];
        cube[YELLOW][2 - i][2] = cube[ORANGE][i][0];
        cube[ORANGE][i][0] = cube[WHITE][i][0];
        cube[WHITE][i][0] = temp;
    }
}

void RubixCube::L_()
{
    L();
    L();
    L();
}

// --- UP MOVE (U) ---
void RubixCube::U()
{
    rotateClockwise(RED);

    for (int i = 0; i < 3; i++)
    {
        int temp = cube[WHITE][0][i];
        cube[WHITE][0][i] = cube[GREEN][0][i];
        cube[GREEN][0][i] = cube[YELLOW][0][i];
        cube[YELLOW][0][i] = cube[BLUE][0][i];
        cube[BLUE][0][i] = temp;
    }
}

void RubixCube::U_()
{
    U();
    U();
    U();
}

// --- DOWN MOVE (D) ---
void RubixCube::D()
{
    rotateClockwise(ORANGE);

    for (int i = 0; i < 3; i++)
    {
        int temp = cube[WHITE][2][i];
        cube[WHITE][2][i] = cube[BLUE][2][i];
        cube[BLUE][2][i] = cube[YELLOW][2][i];
        cube[YELLOW][2][i] = cube[GREEN][2][i];
        cube[GREEN][2][i] = temp;
    }
}

void RubixCube::D_()
{
    D();
    D();
    D();
}

// --- FRONT MOVE (F) ---
void RubixCube::F()
{
    rotateClockwise(WHITE);

    for (int i = 0; i < 3; i++)
    {
        int temp = cube[RED][2][i];
        cube[RED][2][i] = cube[BLUE][2 - i][2];
        cube[BLUE][2 - i][2] = cube[ORANGE][0][2 - i];
        cube[ORANGE][0][2 - i] = cube[GREEN][i][0];
        cube[GREEN][i][0] = temp;
    }
}

void RubixCube::F_()
{
    F();
    F();
    F();
}

// --- BACK MOVE (B) ---
void RubixCube::B()
{
    rotateClockwise(YELLOW);

    for (int i = 0; i < 3; i++)
    {
        int temp = cube[RED][0][i];
        cube[RED][0][i] = cube[GREEN][i][2];
        cube[GREEN][i][2] = cube[ORANGE][2][2 - i];
        cube[ORANGE][2][2 - i] = cube[BLUE][2 - i][0];
        cube[BLUE][2 - i][0] = temp;
    }
}

void RubixCube::B_()
{
    B();
    B();
    B();
}

void RubixCube::print()
{
    for (int face = 0; face < 6; face++)
    {
        for (int i = 0; i < 3; i++)
        {
            for (int j = 0; j < 3; j++)
            {
                cout << faceChar[cube[face][i][j]] << " ";
            }
            cout << endl;
        }

        cout << "-------------\n";
    }
}

int faceRank(Color face)
{
    switch (face)
    {
    case RED:
    case ORANGE:
        return 3;

    case WHITE:
    case YELLOW:
        return 2;

    case BLUE:
    case GREEN:
        return 1;
    }

    return -1;
}

int colorRank(Color color)
{
    switch (color)
    {
    case RED:
    case ORANGE:
        return 3;

    case WHITE:
    case YELLOW:
        return 2;

    case BLUE:
    case GREEN:
        return 1;
    }

    return -1;
}

bool edgeOrientation(Color color1, Color color2, Color face1, Color face2)
{
    if (colorRank(color1) > colorRank(color2))
    {
        return faceRank(face1) < faceRank(face2);
    }

    return faceRank(face2) < faceRank(face1);
}

int cornerOrientation(Color sticker1, Color sticker2, Color sticker3, Color face1, Color face2, Color face3)
{
    if (sticker1 == RED || sticker1 == ORANGE)
    {
        if (face1 == RED || face1 == ORANGE)
            return 0;

        if (face1 == WHITE || face1 == YELLOW)
            return 1;

        return 2;
    }

    if (sticker2 == RED || sticker2 == ORANGE)
    {
        if (face2 == RED || face2 == ORANGE)
            return 0;

        if (face2 == WHITE || face2 == YELLOW)
            return 1;

        return 2;
    }

    if (face3 == RED || face3 == ORANGE)
        return 0;

    if (face3 == WHITE || face3 == YELLOW)
        return 1;

    return 2;
}

CubeState RubixCube::toCubeState() const
{
    CubeState state;

    Color up, down, front, back, left, right;

    // ---------- EDGES ----------

    // UF
    up = static_cast<Color>(cube[RED][2][1]);
    front = static_cast<Color>(cube[WHITE][0][1]);
    state.setEdge(UF,
                  identifyEdge(up, front),
                  edgeOrientation(up, front, RED, WHITE));

    // UR
    up = static_cast<Color>(cube[RED][1][2]);
    right = static_cast<Color>(cube[GREEN][0][1]);
    state.setEdge(UR,
                  identifyEdge(up, right),
                  edgeOrientation(up, right, RED, GREEN));

    // UB
    up = static_cast<Color>(cube[RED][0][1]);
    back = static_cast<Color>(cube[YELLOW][0][1]);
    state.setEdge(UB,
                  identifyEdge(up, back),
                  edgeOrientation(up, back, RED, YELLOW));

    // UL
    up = static_cast<Color>(cube[RED][1][0]);
    left = static_cast<Color>(cube[BLUE][0][1]);
    state.setEdge(UL,
                  identifyEdge(up, left),
                  edgeOrientation(up, left, RED, BLUE));

    // FR
    front = static_cast<Color>(cube[WHITE][1][2]);
    right = static_cast<Color>(cube[GREEN][1][0]);
    state.setEdge(FR,
                  identifyEdge(front, right),
                  edgeOrientation(front, right, WHITE, GREEN));

    // BR
    back = static_cast<Color>(cube[YELLOW][1][0]);
    right = static_cast<Color>(cube[GREEN][1][2]);
    state.setEdge(BR,
                  identifyEdge(back, right),
                  edgeOrientation(back, right, YELLOW, GREEN));

    // BL
    back = static_cast<Color>(cube[YELLOW][1][2]);
    left = static_cast<Color>(cube[BLUE][1][0]);
    state.setEdge(BL,
                  identifyEdge(back, left),
                  edgeOrientation(back, left, YELLOW, BLUE));

    // FL
    front = static_cast<Color>(cube[WHITE][1][0]);
    left = static_cast<Color>(cube[BLUE][1][2]);
    state.setEdge(FL,
                  identifyEdge(front, left),
                  edgeOrientation(front, left, WHITE, BLUE));

    // DF
    down = static_cast<Color>(cube[ORANGE][0][1]);
    front = static_cast<Color>(cube[WHITE][2][1]);
    state.setEdge(DF,
                  identifyEdge(down, front),
                  edgeOrientation(down, front, ORANGE, WHITE));

    // DR
    down = static_cast<Color>(cube[ORANGE][1][2]);
    right = static_cast<Color>(cube[GREEN][2][1]);
    state.setEdge(DR,
                  identifyEdge(down, right),
                  edgeOrientation(down, right, ORANGE, GREEN));

    // DB
    down = static_cast<Color>(cube[ORANGE][2][1]);
    back = static_cast<Color>(cube[YELLOW][2][1]);
    state.setEdge(DB,
                  identifyEdge(down, back),
                  edgeOrientation(down, back, ORANGE, YELLOW));

    // DL
    down = static_cast<Color>(cube[ORANGE][1][0]);
    left = static_cast<Color>(cube[BLUE][2][1]);
    state.setEdge(DL,
                  identifyEdge(down, left),
                  edgeOrientation(down, left, ORANGE, BLUE));

    // ---------- CORNERS ----------

    // UFR
    up = static_cast<Color>(cube[RED][2][2]);
    front = static_cast<Color>(cube[WHITE][0][2]);
    right = static_cast<Color>(cube[GREEN][0][0]);
    state.setCorner(UFR,
                    identifyCorner(up, front, right),
                    cornerOrientation(up, front, right,
                                      RED, WHITE, GREEN));

    // URB
    up = static_cast<Color>(cube[RED][0][2]);
    right = static_cast<Color>(cube[GREEN][0][2]);
    back = static_cast<Color>(cube[YELLOW][0][0]);
    state.setCorner(URB,
                    identifyCorner(up, right, back),
                    cornerOrientation(up, right, back,
                                      RED, GREEN, YELLOW));

    // UBL
    up = static_cast<Color>(cube[RED][0][0]);
    back = static_cast<Color>(cube[YELLOW][0][2]);
    left = static_cast<Color>(cube[BLUE][0][0]);
    state.setCorner(UBL,
                    identifyCorner(up, back, left),
                    cornerOrientation(up, back, left,
                                      RED, YELLOW, BLUE));

    // ULF
    up = static_cast<Color>(cube[RED][2][0]);
    left = static_cast<Color>(cube[BLUE][0][2]);
    front = static_cast<Color>(cube[WHITE][0][0]);
    state.setCorner(ULF,
                    identifyCorner(up, left, front),
                    cornerOrientation(up, left, front,
                                      RED, BLUE, WHITE));

    // DFR
    down = static_cast<Color>(cube[ORANGE][0][2]);
    front = static_cast<Color>(cube[WHITE][2][2]);
    right = static_cast<Color>(cube[GREEN][2][0]);
    state.setCorner(DFR,
                    identifyCorner(down, front, right),
                    cornerOrientation(down, front, right,
                                      ORANGE, WHITE, GREEN));

    // DRB
    down = static_cast<Color>(cube[ORANGE][2][2]);
    right = static_cast<Color>(cube[GREEN][2][2]);
    back = static_cast<Color>(cube[YELLOW][2][0]);
    state.setCorner(DRB,
                    identifyCorner(down, right, back),
                    cornerOrientation(down, right, back,
                                      ORANGE, GREEN, YELLOW));

    // DBL
    down = static_cast<Color>(cube[ORANGE][2][0]);
    back = static_cast<Color>(cube[YELLOW][2][2]);
    left = static_cast<Color>(cube[BLUE][2][0]);
    state.setCorner(DBL,
                    identifyCorner(down, back, left),
                    cornerOrientation(down, back, left,
                                      ORANGE, YELLOW, BLUE));

    // DLF
    down = static_cast<Color>(cube[ORANGE][0][0]);
    left = static_cast<Color>(cube[BLUE][2][2]);
    front = static_cast<Color>(cube[WHITE][2][0]);
    state.setCorner(DLF,
                    identifyCorner(down, left, front),
                    cornerOrientation(down, left, front,
                                      ORANGE, BLUE, WHITE));

    return state;
}