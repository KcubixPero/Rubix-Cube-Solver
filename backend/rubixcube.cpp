#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;
using vector2d = vector<vector<int>>;
using vector3d = vector<vector<vector<int>>>;

class RubixCube
{
public:
    enum Faces
    {
        WHITE = 0,
        RED = 1,
        BLUE = 2,
        GREEN = 3,
        ORANGE = 4,
        YELLOW = 5
    };

    char faceChar[6]{
        'W',
        'R',
        'B',
        'G',
        'O',
        'Y'};

    vector3d cube;

    RubixCube(const vector3d &sides)
    {
        cube.resize(6, vector2d(3, vector<int>(3)));

        for (int i = 0; i < 6; i++)
        {
            cube[i] = sides[i];
        }
    }

    void rotateClockwise(int face)
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

    void R()
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

    void R_()
    {
        R();
        R();
        R();
    }

    void L()
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

    void L_()
    {
        L();
        L();
        L();
    }

    void U()
    {
        rotateClockwise(RED);

        vector<int> temp = cube[WHITE][0];
        cube[WHITE][0] = cube[GREEN][0];
        cube[GREEN][0] = cube[YELLOW][0];
        cube[YELLOW][0] = cube[BLUE][0];
        cube[BLUE][0] = temp;
    }

    void U_()
    {
        U();
        U();
        U();
    }

    void D()
    {
        rotateClockwise(ORANGE);

        vector<int> temp = cube[WHITE][2];
        cube[WHITE][2] = cube[BLUE][2];
        cube[BLUE][2] = cube[YELLOW][2];
        cube[YELLOW][2] = cube[GREEN][2];
        cube[GREEN][2] = temp;
    }

    void D_()
    {
        D();
        D();
        D();
    }

    void F()
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

    void F_()
    {
        F();
        F();
        F();
    }

    void B()
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

    void B_()
    {
        B();
        B();
        B();
    }

    void print()
    {
        for (int face = 0; face < 6; face++)
        {
            for (int i = 0; i < 3; i++)
            {
                for (int j = 0; j < 3; j++)
                {
                    cout << faceChar [[face][i][j]] << " ";
                }
                cout << endl;
            }

            cout << "-------------\n";
        }
    }
};

int main()
{
    vector3d solved(6, vector2d(3, vector<int>(3)));

    for (int f = 0; f < 6; f++)
    {
        for (int i = 0; i < 3; i++)
        {
            for (int j = 0; j < 3; j++)
                solved[f][i][j] = f;
        }
    }

    RubixCube cube(solved);
    cube.print();

    return 0;
}