#ifndef CORNER_H
#define CORNER_H

#include "Color.h"

enum CornerID
{
    UFR = 0,
    URB,
    UBL,
    ULF,

    DFR,
    DRB,
    DBL,
    DLF
};

struct CornerCubie
{
    CornerID id;
    int orientation;
};

struct CornerInfo
{
    CornerID id;
    Color c1;
    Color c2;
    Color c3;
};

extern const CornerInfo CORNER_INFO[8];

CornerID identifyCorner(Color a, Color b, Color c);

#endif