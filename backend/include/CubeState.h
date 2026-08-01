#ifndef CUBESTATE_H
#define CUBESTATE_H

#include <cstdint>

class CubeState
{
private:

    uint64_t edgeState;
    uint64_t cornerState;

public:

    CubeState();

    uint64_t getEdgeState() const;
    uint64_t getCornerState() const;

    void setEdgeState(uint64_t state);
    void setCornerState(uint64_t state);

    bool isSolved() const;
};

#endif