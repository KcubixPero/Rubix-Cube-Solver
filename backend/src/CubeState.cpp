#include "CubeState.h"
#include "Edge.h"
#include "Corner.h"

CubeState::CubeState()
{
    edgeState = 0;
    cornerState = 0;
}

void CubeState::setEdge(int position, EdgeID id, int orientation)
{
    int shift = position * EDGE_BITS;

    // Clear the old 5 bits
    edgeState &= ~(EDGE_MASK << shift);

    // Pack the new edge
    uint64_t packed =
        (static_cast<uint64_t>(orientation) << 4) |
        static_cast<uint64_t>(id);

    // Insert into edgeState
    edgeState |= (packed << shift);
}

EdgeCubie CubeState::getEdge(int position) const
{
    int shift = position * EDGE_BITS;

    uint64_t packed = (edgeState >> shift) & EDGE_MASK;

    EdgeCubie edge;

    edge.id = static_cast<EdgeID>(packed & 0b1111);

    edge.flipped = (packed >> 4) & 1;

    return edge;
}

void CubeState::setCorner(int position, CornerID id, int orientation)
{
    int shift = position * CORNER_BITS;

    // Clear the old 5 bits
    cornerState &= ~(CORNER_MASK << shift);

    // Pack the corner
    uint64_t packed =
        (static_cast<uint64_t>(orientation) << 3) |
        static_cast<uint64_t>(id);

    // Store it
    cornerState |= (packed << shift);
}

CornerCubie CubeState::getCorner(int position) const
{
    int shift = position * CORNER_BITS;

    uint64_t packed = (cornerState >> shift) & CORNER_MASK;

    CornerCubie corner;

    corner.id = static_cast<CornerID>(packed & 0b111);

    corner.orientation = (packed >> 3) & 0b11;

    return corner;
}

uint64_t CubeState::getEdgeState() const
{
    return edgeState;
}

uint64_t CubeState::getCornerState() const
{
    return cornerState;
}