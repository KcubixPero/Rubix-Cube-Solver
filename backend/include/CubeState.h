#ifndef CUBESTATE_H
#define CUBESTATE_H

#include <cstdint>
#include "Edge.h"
#include "Corner.h"

class CubeState
{
private:
    uint64_t edgeState;
    uint64_t cornerState;

    static constexpr int EDGE_BITS = 5;
    static constexpr uint64_t EDGE_MASK = 0b11111ULL;

    static constexpr int CORNER_BITS = 5;
    static constexpr uint64_t CORNER_MASK = 0b11111ULL;

public:
    CubeState();

    uint64_t getEdgeState() const;
    uint64_t getCornerState() const;

    void setEdgeState(uint64_t state);
    void setCornerState(uint64_t state);

    void setEdge(int position, EdgeID id, int orientation);
    EdgeCubie getEdge(int position) const;

    void setCorner(int position, CornerID id, int orientation);
    CornerCubie getCorner(int position) const;
    
    bool isSolved() const;
};

#endif