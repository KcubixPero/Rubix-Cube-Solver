#include "../include/RubixCube.h"

int main()
{
    vector3d solved(6, vector2d(3, vector<int>(3)));

    for(int f=0; f<6; f++)
        for(int i=0;i<3;i++)
            for(int j=0;j<3;j++)
                solved[f][i][j]=f;

    RubixCube cube(solved);

    cube.print();

    return 0;
}