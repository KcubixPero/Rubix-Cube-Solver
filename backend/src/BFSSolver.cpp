#include "BFSSolver.h"

#include "RubixCube.h"

#include <chrono>
#include <algorithm>
#include <iostream>
#include <queue>
#include <unordered_map>

namespace
{
struct Node
{
    RubixCube cube;
    int parent;
    int moveIndex;
    int depth;
};
}

BFSSolver::BFSSolver(BFSConfig config) : config_(config) {}

const std::vector<Move> &BFSSolver::moveSet()
{
    static const std::vector<Move> moves = {
        {"U", 'U'}, {"U'", 'U'}, {"U2", 'U'},
        {"D", 'D'}, {"D'", 'D'}, {"D2", 'D'},
        {"L", 'L'}, {"L'", 'L'}, {"L2", 'L'},
        {"R", 'R'}, {"R'", 'R'}, {"R2", 'R'},
        {"F", 'F'}, {"F'", 'F'}, {"F2", 'F'},
        {"B", 'B'}, {"B'", 'B'}, {"B2", 'B'}};
    return moves;
}

void BFSSolver::applyMove(RubixCube &cube, const Move &move)
{
    const std::string &m = move.notation;
    if (m == "U") cube.U(); else if (m == "U'") cube.U_(); else if (m == "U2") { cube.U(); cube.U(); }
    else if (m == "D") cube.D(); else if (m == "D'") cube.D_(); else if (m == "D2") { cube.D(); cube.D(); }
    else if (m == "L") cube.L(); else if (m == "L'") cube.L_(); else if (m == "L2") { cube.L(); cube.L(); }
    else if (m == "R") cube.R(); else if (m == "R'") cube.R_(); else if (m == "R2") { cube.R(); cube.R(); }
    else if (m == "F") cube.F(); else if (m == "F'") cube.F_(); else if (m == "F2") { cube.F(); cube.F(); }
    else if (m == "B") cube.B(); else if (m == "B'") cube.B_(); else if (m == "B2") { cube.B(); cube.B(); }
}

std::string BFSSolver::stateKey(const RubixCube &cube)
{
    std::string key;
    key.reserve(54);
    for (const auto &face : cube.cube)
        for (const auto &row : face)
            for (int color : row)
                key.push_back(static_cast<char>(color));
    return key;
}

BFSResult BFSSolver::solve(const RubixCube &start,
                           const std::function<bool(const RubixCube &)> &goal) const
{
    BFSResult result;
    const auto began = std::chrono::steady_clock::now();
    const auto &moves = moveSet();
    if (config_.logProgress) std::cout << "[BFS] Starting search\n";

    try
    {
        std::vector<Node> nodes;
        nodes.reserve(std::min<std::size_t>(config_.maxStates, 50000));
        nodes.push_back({start, -1, -1, 0});
        std::unordered_map<std::string, int> visited;
        visited.emplace(stateKey(start), 0);
        std::queue<int> pending;
        pending.push(0);
        int lastReportedDepth = -1;

        while (!pending.empty())
        {
            const int currentIndex = pending.front();
            pending.pop();
            const Node current = nodes[currentIndex];
            ++result.statesExplored;
            if (config_.logProgress && current.depth != lastReportedDepth)
            {
                lastReportedDepth = current.depth;
                std::cout << "[BFS] Depth " << current.depth << ": " << pending.size() + 1 << " queued states\n";
            }
            if (goal(current.cube))
            {
                result.found = true;
                result.solutionDepth = current.depth;
                for (int node = currentIndex; nodes[node].parent != -1; node = nodes[node].parent)
                    result.moves.push_back(moves[nodes[node].moveIndex]);
                std::reverse(result.moves.begin(), result.moves.end());
                break;
            }
            if (current.depth == config_.maxDepth) continue;

            const char previousFace = current.moveIndex < 0 ? '\0' : moves[current.moveIndex].face;
            for (int moveIndex = 0; moveIndex < static_cast<int>(moves.size()); ++moveIndex)
            {
                if (moves[moveIndex].face == previousFace) continue;
                RubixCube next = current.cube;
                applyMove(next, moves[moveIndex]);
                const std::string key = stateKey(next);
                if (visited.find(key) != visited.end()) continue;
                if (nodes.size() >= config_.maxStates)
                {
                    result.limitReached = true;
                    break;
                }
                const int nextIndex = static_cast<int>(nodes.size());
                visited.emplace(key, nextIndex);
                nodes.push_back({next, currentIndex, moveIndex, current.depth + 1});
                pending.push(nextIndex);
            }
            if (result.limitReached) break;
        }
    }
    catch (const std::bad_alloc &) { result.limitReached = true; }
    result.elapsedMilliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::steady_clock::now() - began).count();
    if (config_.logProgress)
    {
        if (result.found) std::cout << "[BFS] Goal found\n";
        else if (result.limitReached) std::cout << "[BFS] Search limit reached.\n";
        std::cout << "[BFS] States explored: " << result.statesExplored
                  << ", time: " << result.elapsedMilliseconds << " ms\n";
    }
    return result;
}
