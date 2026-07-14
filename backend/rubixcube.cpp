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

    void R()
    {
    }

    void R_()
    {
        R();
        R();
        R();
    }

    void L()
    {
    }

    void L_()
    {
        L();
        L();
        L();
    }

    void U()
    {
    }

    void U_()
    {
        U();
        U();
        U();
    }

    void D()
    {
    }

    void D_()
    {
        D();
        D();
        D();
    }

    void F()
    {
    }

    void F_()
    {
        F();
        F();
        F();
    }

    void B()
    {
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
                    cout << faceChar[cube[face][i][j]] << " ";
                }
                cout << endl;
            }

            cout << "-------------\n";
        }
    }
};
