#include "RubixCube.h"
#include "Color.h"

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
