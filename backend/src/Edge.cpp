#include "Edge.h"
#include <stdexcept>

const EdgeInfo EDGE_INFO[12] =
{
    {UF, RED, WHITE},
    {UR, RED, GREEN},
    {UB, RED, YELLOW},
    {UL, RED, BLUE},

    {FR, WHITE, GREEN},
    {BR, YELLOW, GREEN},
    {BL, YELLOW, BLUE},
    {FL, WHITE, BLUE},

    {DF, ORANGE, WHITE},
    {DR, ORANGE, GREEN},
    {DB, ORANGE, YELLOW},
    {DL, ORANGE, BLUE}
};

EdgeID identifyEdge(Color a, Color b)
{
    for (int i = 0; i < 12; i++)
    {
        if ((EDGE_INFO[i].c1 == a && EDGE_INFO[i].c2 == b) || (EDGE_INFO[i].c1 == b && EDGE_INFO[i].c2 == a))
        {
            return EDGE_INFO[i].id;
        }
    }

    throw std::runtime_error("Invalid edge colours");
}