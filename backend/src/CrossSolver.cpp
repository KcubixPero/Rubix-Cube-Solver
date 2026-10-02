#include "CrossSolver.h"
#include "MoveParser.h"
#include "Color.h"
#include <queue>
#include <unordered_map>

using namespace std;

namespace
{
    const int MOVE_COUNT = 18;

    struct Node
    {
        RubixCube cube;

        int g;
        int h;

        vector<string> path;

        Node(
            const RubixCube& cube,
            int g,
            int h,
            const vector<string>& path
        )
            : cube(cube), g(g), h(h), path(path)
        {
        }
    };

    struct Compare
    {
        bool operator()(const Node& a, const Node& b) const
        {
            return (a.g + a.h) > (b.g + b.h);
        }
    };
}

// ============================================================
// Encode cube state
// ============================================================

string CrossSolver::encode(const RubixCube& cube)
{
    string state;

    for (int face = 0; face < 6; face++)
    {
        for (int row = 0; row < 3; row++)
        {
            for (int col = 0; col < 3; col++)
            {
                state += char('0' + cube.cube[face][row][col]);
            }
        }
    }

    return state;
}

// ============================================================
// White Cross goal
// ============================================================

bool CrossSolver::isSolved(const RubixCube& cube)
{
    // White-Red
    if (cube.cube[WHITE][0][1] != WHITE ||
        cube.cube[RED][2][1] != RED)
        return false;

    // White-Green
    if (cube.cube[WHITE][1][2] != WHITE ||
        cube.cube[GREEN][1][0] != GREEN)
        return false;

    // White-Orange
    if (cube.cube[WHITE][2][1] != WHITE ||
        cube.cube[ORANGE][0][1] != ORANGE)
        return false;

    // White-Blue
    if (cube.cube[WHITE][1][0] != WHITE ||
        cube.cube[BLUE][1][2] != BLUE)
        return false;

    return true;
}

// ============================================================
// Heuristic
// ============================================================

int CrossSolver::heuristic(const RubixCube& cube)
{
    int unsolved = 0;

    // White-Red
    if (cube.cube[WHITE][0][1] != WHITE ||
        cube.cube[RED][2][1] != RED)
        unsolved++;

    // White-Green
    if (cube.cube[WHITE][1][2] != WHITE ||
        cube.cube[GREEN][1][0] != GREEN)
        unsolved++;

    // White-Orange
    if (cube.cube[WHITE][2][1] != WHITE ||
        cube.cube[ORANGE][0][1] != ORANGE)
        unsolved++;

    // White-Blue
    if (cube.cube[WHITE][1][0] != WHITE ||
        cube.cube[BLUE][1][2] != BLUE)
        unsolved++;

    return unsolved;
}

// ============================================================
// Apply move
// ============================================================

void CrossSolver::applyMove(RubixCube& cube, int move)
{
    static const string moves[MOVE_COUNT] =
    {
        "R", "R'", "R2",
        "L", "L'", "L2",
        "U", "U'", "U2",
        "D", "D'", "D2",
        "F", "F'", "F2",
        "B", "B'", "B2"
    };

    MoveParser::execute(cube, moves[move]);
}

// ============================================================
// Move → notation
// ============================================================

string CrossSolver::moveToString(int move)
{
    static const string moves[MOVE_COUNT] =
    {
        "R", "R'", "R2",
        "L", "L'", "L2",
        "U", "U'", "U2",
        "D", "D'", "D2",
        "F", "F'", "F2",
        "B", "B'", "B2"
    };

    return moves[move];
}

// ============================================================
// A*
// ============================================================

vector<string> CrossSolver::solve(const RubixCube& start)
{
    priority_queue<Node, vector<Node>, Compare> open;

    unordered_map<string, int> bestG;

    open.emplace(
        start,
        0,
        heuristic(start),
        vector<string>()
    );

    bestG[encode(start)] = 0;

    while (!open.empty())
    {
        Node current = open.top();
        open.pop();

        string currentKey = encode(current.cube);

        if (current.g > bestG[currentKey])
            continue;

        if (isSolved(current.cube))
            return current.path;

        for (int move = 0; move < MOVE_COUNT; move++)
        {
            RubixCube nextCube = current.cube;

            applyMove(nextCube, move);

            string nextKey = encode(nextCube);

            int newG = current.g + 1;

            auto it = bestG.find(nextKey);

            if (it != bestG.end() &&
                it->second <= newG)
            {
                continue;
            }

            bestG[nextKey] = newG;

            vector<string> newPath = current.path;

            newPath.push_back(
                moveToString(move)
            );

            open.emplace(
                nextCube,
                newG,
                heuristic(nextCube),
                newPath
            );
        }
    }

    return {};
}