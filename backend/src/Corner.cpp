#include "Corner.h"
#include <stdexcept>

const CornerInfo CORNER_INFO[8] =
{
    {UFR, RED, WHITE, GREEN},
    {URB, RED, GREEN, YELLOW},
    {UBL, RED, YELLOW, BLUE},
    {ULF, RED, BLUE, WHITE},

    {DFR, ORANGE, WHITE, GREEN},
    {DRB, ORANGE, GREEN, YELLOW},
    {DBL, ORANGE, YELLOW, BLUE},
    {DLF, ORANGE, BLUE, WHITE}
};

CornerID identifyCorner(Color a, Color b, Color c)
{
    for (int i = 0; i < 8; i++)
    {
        int matches = 0;

        if (CORNER_INFO[i].c1 == a || CORNER_INFO[i].c2 == a || CORNER_INFO[i].c3 == a)
            matches++;

        if (CORNER_INFO[i].c1 == b || CORNER_INFO[i].c2 == b || CORNER_INFO[i].c3 == b)
            matches++;

        if (CORNER_INFO[i].c1 == c || CORNER_INFO[i].c2 == c || CORNER_INFO[i].c3 == c)
            matches++;

        if (matches == 3)
            return CORNER_INFO[i].id;
    }

    throw std::runtime_error("Invalid corner colours");
}