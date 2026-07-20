#ifndef CUBEANALYZER_H
#define CUBEANALYZER_H  

#include "RubixCube.h"

enum class CornerOrientation
{
    CORRECT = 0,
    TWIST_1 = 1,
    TWIST_2 = 2
};

struct Edge
{
    RubixCube::Faces color1;
    RubixCube::Faces color2;
};

struct Corner
{
    RubixCube::Faces color1;
    RubixCube::Faces color2;
    RubixCube::Faces color3;
};

struct EdgePosition
{
    RubixCube::Faces face1;
    int row1;
    int col1;

    RubixCube::Faces face2;
    int row2;
    int col2;

    bool flipped;
};

struct CornerPosition
{
    RubixCube::Faces face1;
    int row1;
    int col1;

    RubixCube::Faces face2;
    int row2;
    int col2;

    RubixCube::Faces face3;
    int row3;
    int col3;

    CornerOrientation orientation;
};

class CubeAnalyzer
{
private: 
    RubixCube &cube;

public:
    CubeAnalyzer(RubixCube &cube);
    EdgePosition findEdge(RubixCube::Faces color1, RubixCube::Faces color2) const;
    CornerPosition findCorner(RubixCube::Faces c1, RubixCube::Faces c2, RubixCube::Faces c3) const;
    bool crossSolved() const;
    bool f2lSolved() const;
    bool ollSolved() const;
    bool pllSolved() const;
};

#endif