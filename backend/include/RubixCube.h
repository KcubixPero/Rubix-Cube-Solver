#ifndef RUBIXCUBE_H
#define RUBIXCUBE_H

#include <iostream>
#include <vector>

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

    RubixCube(const vector3d &sides);

    void rotateClockwise(int face);

    void R();
    void R_();

    void L();
    void L_();

    void U();
    void U_();

    void D();
    void D_();

    void F();
    void F_();

    void B();
    void B_();

    void print();
};

#endif
