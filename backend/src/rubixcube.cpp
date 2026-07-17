#include "../include/RubixCube.h"

RubixCube::RubixCube(const vector3d &sides)
{
    cube.resize(6, vector2d(3, vector<int>(3)));

    for (int i = 0; i < 6; i++)
    {
        cube[i] = sides[i];
    }
}

void RubixCube::rotateClockwise(int face)
{
    int temp = cube[face][0][0];
    cube[face][0][0] = cube[face][2][0];
    cube[face][2][0] = cube[face][2][2];
    cube[face][2][2] = cube[face][0][2];
    cube[face][0][2] = temp;

    temp = cube[face][0][1];
    cube[face][0][1] = cube[face][1][0];
    cube[face][1][0] = cube[face][2][1];
    cube[face][2][1] = cube[face][1][2];
    cube[face][1][2] = temp;
}

void RubixCube::R()
{
    rotateClockwise(GREEN);

    for (int i = 0; i < 3; i++)
    {
        int temp = cube[WHITE][i][2];
        cube[WHITE][i][2] = cube[ORANGE][i][2];
        cube[ORANGE][i][2] = cube[YELLOW][2 - i][0];
        cube[YELLOW][2 - i][0] = cube[RED][i][2];
        cube[RED][i][2] = temp;
    }
}

void RubixCube::R_()
{
    R();
    R();
    R();
}

void RubixCube::L()
{
    rotateClockwise(BLUE);

    for (int i = 0; i < 3; i++)
    {
        int temp = cube[WHITE][i][0];
        cube[WHITE][i][0] = cube[RED][i][0];
        cube[RED][i][0] = cube[YELLOW][2 - i][0];
        cube[YELLOW][2 - i][0] = cube[ORANGE][i][0];
        cube[ORANGE][i][0] = temp;
    }
}

void RubixCube::L_()
{
    L();
    L();
    L();
}

void RubixCube::U()
{
    rotateClockwise(RED);

    vector<int> temp = cube[WHITE][0];
    cube[WHITE][0] = cube[GREEN][0];
    cube[GREEN][0] = cube[YELLOW][0];
    cube[YELLOW][0] = cube[BLUE][0];
    cube[BLUE][0] = temp;
}

void RubixCube::U_()
{
    U();
    U();
    U();
}

void RubixCube::D()
{
    rotateClockwise(ORANGE);

    vector<int> temp = cube[WHITE][2];
    cube[WHITE][2] = cube[BLUE][2];
    cube[BLUE][2] = cube[YELLOW][2];
    cube[YELLOW][2] = cube[GREEN][2];
    cube[GREEN][2] = temp;
}

void RubixCube::D_()
{
    D();
    D();
    D();
}

void RubixCube::F()
{
    rotateClockwise(WHITE);

    for (int i = 0; i < 3; i++)
    {
        int temp = cube[BLUE][i][2];
        cube[BLUE][i][2] = cube[ORANGE][i][2];
        cube[ORANGE][i][2] = cube[GREEN][2 - i][0];
        cube[GREEN][2 - i][0] = cube[RED][i][2];
        cube[RED][i][2] = temp;
    }
}

void RubixCube::F_()
{
    F();
    F();
    F();
}

void RubixCube::B()
{
    rotateClockwise(YELLOW);

    for (int i = 0; i < 3; i++)
    {
        int temp = cube[BLUE][i][0];
        cube[BLUE][i][0] = cube[RED][i][0];
        cube[RED][i][0] = cube[GREEN][2 - i][0];
        cube[GREEN][2 - i][0] = cube[ORANGE][i][0];
        cube[ORANGE][i][0] = temp;
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