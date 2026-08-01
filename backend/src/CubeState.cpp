#include "CubeState.h"
#include "Edge.h"
#include "Corner.h"

CubeState::CubeState()
{
    edgeState = 0;
    cornerState = 0;
}

uint64_t CubeState::getEdgeState() const
{
    return edgeState;
}

uint64_t CubeState::getCornerState() const
{
    return cornerState;
}

void CubeState::setEdgeState(uint64_t state)
{
    edgeState = state;
}

void CubeState::setCornerState(uint64_t state)
{
    cornerState = state;
}

bool CubeState::isSolved() const
{
    return false;
}