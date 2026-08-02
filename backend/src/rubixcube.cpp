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
    R(); R(); R();
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
    L(); L(); L();
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
    U(); U(); U();
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
    D(); D(); D();
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
    F(); F(); F();
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
    B(); B(); B();
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

    EdgeCubie edges[12];
    CornerCubie corners[8];

    // ---------- EDGES ----------

    Color up, down, front, back, left, right;


    // UF
    up = static_cast<Color>(cube[RED][2][1]);
    front = static_cast<Color>(cube[WHITE][0][1]);
    edges[UF].id = identifyEdge(up, front);
    edges[UF].flipped = edgeOrientation(up, front, RED, WHITE);

    // UR
    up = static_cast<Color>(cube[RED][1][2]);
    right = static_cast<Color>(cube[GREEN][0][1]);
    edges[UR].id = identifyEdge(up, right);
    edges[UR].flipped = edgeOrientation(up, right, RED, GREEN);

    // UB
    up = static_cast<Color>(cube[RED][0][1]);
    back = static_cast<Color>(cube[YELLOW][0][1]);
    edges[UB].id = identifyEdge(up, back);
    edges[UB].flipped = edgeOrientation(up, back, RED, YELLOW);

    // UL
    up = static_cast<Color>(cube[RED][1][0]);
    left = static_cast<Color>(cube[BLUE][0][1]);
    edges[UL].id = identifyEdge(up, left);
    edges[UL].flipped = edgeOrientation(up, left, RED, BLUE);

    // FR
    front = static_cast<Color>(cube[WHITE][1][2]);
    right = static_cast<Color>(cube[GREEN][1][0]);
    edges[FR].id = identifyEdge(front, right);
    edges[FR].flipped = edgeOrientation(front, right, WHITE, GREEN);

    // BR
    back = static_cast<Color>(cube[YELLOW][1][2]);
    right = static_cast<Color>(cube[GREEN][1][2]);
    edges[BR].id = identifyEdge(back, right);
    edges[BR].flipped = edgeOrientation(back, right, YELLOW, GREEN);

    // BL
    back = static_cast<Color>(cube[YELLOW][1][0]);
    left = static_cast<Color>(cube[BLUE][1][2]);
    edges[BL].id = identifyEdge(back, left);
    edges[BL].flipped = edgeOrientation(back, left, YELLOW, BLUE);

    // FL
    front = static_cast<Color>(cube[WHITE][1][0]);
    left = static_cast<Color>(cube[BLUE][1][0]);
    edges[FL].id = identifyEdge(front, left);
    edges[FL].flipped = edgeOrientation(front, left, WHITE, BLUE);

    // DF
    down = static_cast<Color>(cube[ORANGE][0][1]);
    front = static_cast<Color>(cube[WHITE][2][1]);
    edges[DF].id = identifyEdge(down, front);
    edges[DF].flipped = edgeOrientation(down, front, ORANGE, WHITE);

    // DR
    down = static_cast<Color>(cube[ORANGE][1][2]);
    right = static_cast<Color>(cube[GREEN][2][1]);
    edges[DR].id = identifyEdge(down, right);
    edges[DR].flipped = edgeOrientation(down, right, ORANGE, GREEN);

    // DB
    down = static_cast<Color>(cube[ORANGE][2][1]);
    back = static_cast<Color>(cube[YELLOW][2][1]);
    edges[DB].id = identifyEdge(down, back);
    edges[DB].flipped = edgeOrientation(down, back, ORANGE, YELLOW);

    // DL
    down = static_cast<Color>(cube[ORANGE][1][0]);
    left = static_cast<Color>(cube[BLUE][2][1]);
    edges[DL].id = identifyEdge(down, left);
    edges[DL].flipped = edgeOrientation(down, left, ORANGE, BLUE);

    // ---------- CORNERS ----------


    // UFR
    up = static_cast<Color>(cube[RED][2][2]);
    front = static_cast<Color>(cube[WHITE][0][2]);
    right = static_cast<Color>(cube[GREEN][0][0]);

    corners[UFR].id = identifyCorner(up, front, right);
    corners[UFR].orientation = cornerOrientation(
        up,
        front,
        right,
        RED,
        WHITE,
        GREEN);

    // URB
    up = static_cast<Color>(cube[RED][0][2]);
    right = static_cast<Color>(cube[GREEN][0][2]);
    back = static_cast<Color>(cube[YELLOW][0][0]);

    corners[URB].id = identifyCorner(up, right, back);
    corners[URB].orientation = cornerOrientation(
        up,
        right,
        back,
        RED,
        GREEN,
        YELLOW);

    // UBL
    up = static_cast<Color>(cube[RED][0][0]);
    back = static_cast<Color>(cube[YELLOW][0][2]);
    left = static_cast<Color>(cube[BLUE][0][0]);

    corners[UBL].id = identifyCorner(up, back, left);
    corners[UBL].orientation = cornerOrientation(
        up,
        back,
        left,
        RED,
        YELLOW,
        BLUE);

    // ULF
    up = static_cast<Color>(cube[RED][2][0]);
    left = static_cast<Color>(cube[BLUE][0][2]);
    front = static_cast<Color>(cube[WHITE][0][0]);

    corners[ULF].id = identifyCorner(up, left, front);
    corners[ULF].orientation = cornerOrientation(
        up,
        left,
        front,
        RED,
        BLUE,
        WHITE);

    // DFR
    down = static_cast<Color>(cube[ORANGE][0][2]);
    front = static_cast<Color>(cube[WHITE][2][2]);
    right = static_cast<Color>(cube[GREEN][2][0]);

    corners[DFR].id = identifyCorner(down, front, right);
    corners[DFR].orientation = cornerOrientation(
        down,
        front,
        right,
        ORANGE,
        WHITE,
        GREEN);

    // DRB
    down = static_cast<Color>(cube[ORANGE][2][2]);
    right = static_cast<Color>(cube[GREEN][2][2]);
    back = static_cast<Color>(cube[YELLOW][2][0]);

    corners[DRB].id = identifyCorner(down, right, back);
    corners[DRB].orientation = cornerOrientation(
        down,
        right,
        back,
        ORANGE,
        GREEN,
        YELLOW);

    // DBL
    down = static_cast<Color>(cube[ORANGE][2][0]);
    back = static_cast<Color>(cube[YELLOW][2][2]);
    left = static_cast<Color>(cube[BLUE][2][0]);

    corners[DBL].id = identifyCorner(down, back, left);
    corners[DBL].orientation = cornerOrientation(
        down,
        back,
        left,
        ORANGE,
        YELLOW,
        BLUE);

    // DLF
    down = static_cast<Color>(cube[ORANGE][0][0]);
    left = static_cast<Color>(cube[BLUE][2][2]);
    front = static_cast<Color>(cube[WHITE][2][0]);

    corners[DLF].id = identifyCorner(down, left, front);
    corners[DLF].orientation = cornerOrientation(
        down,
        left,
        front,
        ORANGE,
        BLUE,
        WHITE);

    cout << "\n----- Edges -----\n";

for (int i = 0; i < 12; i++)
{
    cout << i
         << " : ID = " << edges[i].id
         << "  Flip = " << edges[i].flipped
         << '\n';
}

cout << "\n----- Corners -----\n";

for (int i = 0; i < 8; i++)
{
    cout << i
         << " : ID = " << corners[i].id
         << "  Ori = " << corners[i].orientation
         << '\n';
}

    return state;
}