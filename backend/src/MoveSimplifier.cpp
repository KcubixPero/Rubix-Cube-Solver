#include "MoveSimplifier.h"

#include <stdexcept>

namespace {
int quarterTurns(const std::string& move) {
    if (move.size() < 1 || move.size() > 2 || std::string("RLUDFB").find(move[0]) == std::string::npos)
        throw std::invalid_argument("Invalid face move: " + move);
    if (move.size() == 1) return 1;
    if (move[1] == '\'') return 3;
    if (move[1] == '2') return 2;
    throw std::invalid_argument("Invalid face move: " + move);
}
}

std::vector<std::string> MoveSimplifier::simplifyAdjacentFaces(const std::vector<std::string>& moves) {
    std::vector<std::string> result;
    for (const std::string& move : moves) {
        const int turns = quarterTurns(move);
        if (!result.empty() && result.back()[0] == move[0]) {
            const int combined = (quarterTurns(result.back()) + turns) % 4;
            result.pop_back();
            if (combined == 1) result.push_back(std::string(1, move[0]));
            else if (combined == 2) result.push_back(std::string(1, move[0]) + "2");
            else if (combined == 3) result.push_back(std::string(1, move[0]) + "'");
        } else {
            result.push_back(move);
        }
    }
    return result;
}
