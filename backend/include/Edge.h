#ifndef EDGE_H
#define EDGE_H

#include "Color.h"

enum EdgeID
{
    UF = 0,
    UR,
    UB,
    UL,

    FR,
    BR,
    BL,
    FL,

    DF,
    DR,
    DB,
    DL
};

struct EdgeInfo
{
    EdgeID id;
    Color c1;
    Color c2;
};

extern const EdgeInfo EDGE_INFO[12];

struct EdgeCubie
{
    EdgeID id;
    bool flipped;
};

EdgeID identifyEdge(Color a, Color b);

#endif