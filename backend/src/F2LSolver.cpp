#include "F2LSolver.h"

#include "Color.h"
#include "CrossSolver.h"
#include "MoveParser.h"

#include <algorithm>
#include <array>
#include <sstream>
#include <unordered_map>

namespace {
// CFOP frame mapped onto the repository's fixed physical frame:
// virtual U/D/F/B = physical Yellow/White/Red/Orange. R/L are unchanged.
// Each entry is the physical move applied for one virtual face turn.
struct Turn { const char* name; };
const std::array<Turn, 18> kTurns{{
    {"R"},{"R'"},{"R2"},
    {"L"},{"L'"},{"L2"},
    {"U"},{"U'"},{"U2"},
    {"D"},{"D'"},{"D2"},
    {"F"},{"F'"},{"F2"},
    {"B"},{"B'"},{"B2"}
}};

// Internal physical face indices: W=0, R=1, B=2, G=3, O=4, Y=5.
// Distinguish the two virtual faces sharing physical face turns: virtual D is
// physical White (F), while virtual F is physical Red (U).
const char* physicalName(const Turn& t) {
    const char face = t.name[0];
    const char suffix = t.name[1];
    if (face == 'R' || face == 'L') return t.name;
    const char mapped = face == 'U' ? 'B' : face == 'D' ? 'F' : face == 'F' ? 'U' : 'D';
    static thread_local char token[3];
    token[0] = mapped;
    token[1] = suffix;
    token[suffix == '\0' ? 1 : 2] = '\0';
    return token;
}

void apply(RubixCube& c, int move) {
    const Turn& t = kTurns[move];
    MoveParser::executeMove(c, physicalName(t));
}

struct Cell { int face, row, col; };
// Convert a physical corner/edge coordinate to the sticker cell on a face.
Cell cellOnFace(int face, int x, int y, int z) {
    switch (face) {
        case WHITE:  return {face, 1-y, x+1};       // z = +1
        case YELLOW: return {face, 1-y, 1-x};       // z = -1
        case RED:    return {face, z+1, x+1};       // y = +1
        case ORANGE: return {face, 1-z, x+1};       // y = -1
        case GREEN:  return {face, 1-y, 1-z};       // x = +1
        default:     return {face, 1-y, z+1};       // BLUE, x = -1
    }
}

using Colors = std::vector<int>;

struct PieceLocation {
    std::array<int,3> position{};
    std::array<int,3> stickers{};
    bool found = false;
};

PieceLocation findPiece(const RubixCube& c, const Colors& piece) {
    Colors wanted = piece;
    std::sort(wanted.begin(), wanted.end());
    for (int x=-1; x<=1; ++x) for (int y=-1; y<=1; ++y) for (int z=-1; z<=1; ++z) {
        const int surfaceCount = (x != 0) + (y != 0) + (z != 0);
        if (surfaceCount != static_cast<int>(piece.size())) continue;
        std::array<int,3> ids{};
        Colors actual;
        for (int f=0; f<6; ++f) {
            const bool onFace = (f==WHITE && z==1) || (f==YELLOW && z==-1) ||
                                (f==RED && y==1) || (f==ORANGE && y==-1) ||
                                (f==GREEN && x==1) || (f==BLUE && x==-1);
            if (!onFace) continue;
            const Cell cell = cellOnFace(f,x,y,z);
            const int color = c.cube[f][cell.row][cell.col];
            actual.push_back(color);
            for (std::size_t i=0; i<piece.size(); ++i)
                if (piece[i] == color) ids[i] = f*9 + cell.row*3 + cell.col;
        }
        std::sort(actual.begin(), actual.end());
        if (actual == wanted) return {{x,y,z},ids,true};
    }
    return {};
}

std::string pieceKey(const RubixCube& c, const Colors& piece) {
    std::string key;
    const PieceLocation location = findPiece(c,piece);
    if (!location.found) return key;
    for (std::size_t i=0; i<piece.size(); ++i)
        key.push_back(static_cast<char>(location.stickers[i]));
    return key;
}

std::array<int,3> piecePosition(const RubixCube& c, const Colors& piece) {
    return findPiece(c,piece).position;
}

struct Requirement {
    Colors colors;
    std::string goalKey;
    std::array<int,3> goalPosition;
};

bool slotSolved(const RubixCube& c, int x, int y, int z,
                int edgeX, int edgeY, int edgeZ) {
    for (int f = 0; f < 6; ++f) {
        const bool cornerSticker = (f == WHITE ? z == 1 : f == YELLOW ? z == -1 :
                                    f == RED ? y == 1 : f == ORANGE ? y == -1 :
                                    f == GREEN ? x == 1 : x == -1);
        if (cornerSticker) {
            Cell p = cellOnFace(f, x, y, z);
            if (c.cube[p.face][p.row][p.col] != f) return false;
        }
        const bool edgeSticker = (f == WHITE ? edgeZ == 1 : f == YELLOW ? edgeZ == -1 :
                                  f == RED ? edgeY == 1 : f == ORANGE ? edgeY == -1 :
                                  f == GREEN ? edgeX == 1 : edgeX == -1);
        if (edgeSticker) {
            Cell p = cellOnFace(f, edgeX, edgeY, edgeZ);
            if (c.cube[p.face][p.row][p.col] != f) return false;
        }
    }
    return true;
}

std::string protectedState(const RubixCube& c,
                           const std::vector<Requirement>& reqs) {
    std::string key;
    for (const auto& req : reqs) key += pieceKey(c, req.colors);
    return key;
}

int heuristic(const RubixCube& c, const std::vector<Requirement>& reqs) {
    int h = 0;
    for (const auto& req : reqs) {
        const int misplacedAxes = [&] {
            const auto p = piecePosition(c, req.colors);
            return (p[0] != req.goalPosition[0]) +
                   (p[1] != req.goalPosition[1]) +
                   (p[2] != req.goalPosition[2]);
        }();
        // A face turn moves a corner/edge through at most two coordinate
        // axes. This coordinate bound is admissible and needs no PDB/table.
        int lowerBound = (misplacedAxes + 1) / 2;
        if (lowerBound == 0 && pieceKey(c, req.colors) != req.goalKey) lowerBound = 1;
        h = std::max(h, lowerBound);
    }
    return h;
}

bool allGoals(const RubixCube& c, const std::array<bool,4>& locked) {
    // Goal coordinates are physical coordinates for virtual FR, BR, BL, FL.
    static const int corners[4][3] = {{1,1,1},{1,-1,1},{-1,-1,1},{-1,1,1}};
    static const int edges[4][3] = {{1,1,0},{1,-1,0},{-1,-1,0},{-1,1,0}};
    if (!CrossSolver::isSolved(c)) return false;
    for (int i = 0; i < 4; ++i) if (locked[i])
        if (!slotSolved(c, corners[i][0], corners[i][1], corners[i][2],
                        edges[i][0], edges[i][1], edges[i][2])) return false;
    return true;
}

struct SearchContext {
    const std::vector<Requirement>& requirements;
    const std::array<bool,4>& locked;
    int bound;
    std::vector<int> path;
    std::unordered_map<std::string, int> visited;
    std::size_t nodes = 0;
};

bool dfs(const RubixCube& state, int depth, int lastFace, SearchContext& ctx) {
    ++ctx.nodes;
    const int h = heuristic(state, ctx.requirements);
    if (depth + h > ctx.bound) return false;
    if (allGoals(state, ctx.locked)) return true;

    std::string key = protectedState(state, ctx.requirements);
    key.push_back(static_cast<char>(lastFace + 1));
    auto it = ctx.visited.find(key);
    if (it != ctx.visited.end() && it->second <= depth) return false;
    ctx.visited[std::move(key)] = depth;

    struct Candidate { int estimate; int move; };
    std::vector<Candidate> candidates;
    const std::string currentPieces = protectedState(state, ctx.requirements);
    for (int m = 0; m < 18; ++m) {
        const int face = m / 3;
        if (face == lastFace) continue;
        // Opposite faces commute. Keep one canonical order to avoid exploring
        // both equivalent paths (L,R and B,F) at the same depth.
        if ((lastFace == 1 && face == 0) || (lastFace == 5 && face == 4)) continue;
        if (face == 3) continue; // virtual D turns disturb the white cross
        RubixCube next = state;
        apply(next, m);
        // Turns that move none of the protected or target pieces cannot help
        // this projected search, so discard them before recursing.
        if (protectedState(next, ctx.requirements) == currentPieces) continue;
        const int childH = heuristic(next, ctx.requirements);
        if (depth + 1 + childH > ctx.bound) continue;
        candidates.push_back({childH, m});
    }
    std::stable_sort(candidates.begin(), candidates.end(), [](const Candidate& a, const Candidate& b) {
        if (a.estimate != b.estimate) return a.estimate < b.estimate;
        return a.move < b.move;
    });
    for (const Candidate& candidate : candidates) {
        const int m = candidate.move;
        const int face = m / 3;
        RubixCube next = state;
        apply(next, m);
        ctx.path.push_back(m);
        if (dfs(next, depth + 1, face, ctx)) return true;
        ctx.path.pop_back();
    }
    return false;
}

RubixCube solvedCube() {
    vector3d sides(6, vector2d(3, vector<int>(3)));
    for (int f=0; f<6; ++f) for (int r=0; r<3; ++r) for (int c=0; c<3; ++c) sides[f][r][c]=f;
    return RubixCube(sides);
}

const std::vector<Requirement>& requirementTables() {
    static const std::vector<Requirement> tables = [] {
        const RubixCube solved = solvedCube();
        std::vector<Requirement> result;
        const std::array<Colors,4> crosses{{Colors{WHITE,RED},Colors{WHITE,GREEN},Colors{WHITE,ORANGE},Colors{WHITE,BLUE}}};
        const std::array<std::array<int,3>,4> crossPositions{{{{0,1,1}},{{1,0,1}},{{0,-1,1}},{{-1,0,1}}}};
        for (int i=0; i<4; ++i) result.push_back({crosses[i],pieceKey(solved,crosses[i]),crossPositions[i]});
        const int sideA[4] = {RED, ORANGE, ORANGE, RED};
        const int sideB[4] = {GREEN, GREEN, BLUE, BLUE};
        const std::array<std::array<int,3>,4> corners{{{{1,1,1}},{{1,-1,1}},{{-1,-1,1}},{{-1,1,1}}}};
        const std::array<std::array<int,3>,4> edges{{{{1,1,0}},{{1,-1,0}},{{-1,-1,0}},{{-1,1,0}}}};
        for (int slot=0; slot<4; ++slot) {
            Colors corner{WHITE,sideA[slot],sideB[slot]};
            Colors edge{sideA[slot],sideB[slot]};
            result.push_back({corner,pieceKey(solved,corner),corners[slot]});
            result.push_back({edge,pieceKey(solved,edge),edges[slot]});
        }
        return result;
    }();
    return tables;
}
}

F2LSolver::F2LSolver(RubixCube& cube) : cube_(cube) {}

bool F2LSolver::isSolved(const RubixCube& cube) {
    if (!CrossSolver::isSolved(cube)) return false;
    static const int corners[4][3] = {{1,1,1},{1,-1,1},{-1,-1,1},{-1,1,1}};
    static const int edges[4][3] = {{1,1,0},{1,-1,0},{-1,-1,0},{-1,1,0}};
    for (int i=0; i<4; ++i)
        if (!slotSolved(cube,corners[i][0],corners[i][1],corners[i][2],edges[i][0],edges[i][1],edges[i][2])) return false;
    return true;
}

bool F2LSolver::solve() {
    moves_.clear(); error_.clear();
    if (!CrossSolver::isSolved(cube_)) {
        error_ = "F2L requires a solved white cross.";
        return false;
    }

    const auto& tables = requirementTables();
    std::vector<Requirement> requirements(tables.begin(), tables.begin() + 4);

    std::vector<int> combined;
    std::array<bool,4> solvedSlots{};
    for (int step=0; step<4; ++step) {
        int slot = -1;
        int bestEstimate = 100;
        std::vector<Requirement> chosenRequirements;
        for (int candidate=0; candidate<4; ++candidate) {
            if (solvedSlots[candidate]) continue;
            std::vector<Requirement> candidateRequirements(tables.begin(), tables.begin() + 4);
            for (int previous=0; previous<4; ++previous) if (solvedSlots[previous]) {
                candidateRequirements.push_back(tables[4 + previous * 2]);
                candidateRequirements.push_back(tables[5 + previous * 2]);
            }
            candidateRequirements.push_back(tables[4 + candidate * 2]);
            candidateRequirements.push_back(tables[5 + candidate * 2]);
            const int estimate = heuristic(cube_, candidateRequirements);
            if (estimate < bestEstimate) {
                slot = candidate;
                bestEstimate = estimate;
                chosenRequirements = std::move(candidateRequirements);
            }
        }
        if (slot < 0) {
            error_ = "No unsolved F2L slot remained during deterministic slot selection.";
            return false;
        }
        requirements = std::move(chosenRequirements);
        std::array<bool,4> locked = solvedSlots;
        locked[slot] = true;
        bool found = false;
        for (int bound=heuristic(cube_,requirements); bound<=12 && !found; ++bound) {
            SearchContext ctx{requirements,locked,bound,{}, {}, 0};
            found = dfs(cube_,0,-1,ctx);
            if (found) {
                for (int m : ctx.path) {
                    apply(cube_,m);
                    combined.push_back(m);
                }
            }
        }
        if (!found) {
            error_ = "Bounded pair search failed for slot " + std::to_string(slot) + " (depth limit 12).";
            return false;
        }
        if (!allGoals(cube_,locked)) {
            error_ = "Pair verification failed after insertion for slot " + std::to_string(slot) + ".";
            return false;
        }
        solvedSlots[slot] = true;
    }

    std::ostringstream out;
    for (std::size_t i=0; i<combined.size(); ++i) {
        if (i) out << ' ';
        out << physicalName(kTurns[combined[i]]);
    }
    moves_ = out.str();
    return isSolved(cube_);
}
