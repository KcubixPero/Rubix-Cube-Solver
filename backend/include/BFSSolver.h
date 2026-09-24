#ifndef BFS_SOLVER_H
#define BFS_SOLVER_H

#include <cstddef>
#include <functional>
#include <string>
#include <vector>

class RubixCube;

struct Move
{
    std::string notation;
    char face;
};

struct BFSConfig
{
    int maxDepth = 7;
    std::size_t maxStates = 500000;
    bool logProgress = false;
};

struct BFSResult
{
    bool found = false;
    bool limitReached = false;
    std::vector<Move> moves;
    std::size_t statesExplored = 0;
    int solutionDepth = 0;
    long long elapsedMilliseconds = 0;
};

class BFSSolver
{
public:
    explicit BFSSolver(BFSConfig config = {});

    BFSResult solve(const RubixCube &start,
                    const std::function<bool(const RubixCube &)> &goal) const;

    static const std::vector<Move> &moveSet();
    static void applyMove(RubixCube &cube, const Move &move);
    static std::string stateKey(const RubixCube &cube);

private:
    BFSConfig config_;
};

#endif
