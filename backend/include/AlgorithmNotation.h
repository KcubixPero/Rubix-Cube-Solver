#pragma once

#include <string>
#include <vector>

// Compiles CFOP notation (including regrips, wide turns and slices) to the
// existing six-face move vocabulary in the project's fixed physical frame.
class AlgorithmNotation {
public:
    static std::vector<std::string> toPhysicalMoves(const std::string& algorithm);
    static std::string inverse(const std::vector<std::string>& moves);
};
