#pragma once

#include "RubixCube.h"

#include <string>
#include <vector>

class F2LSolver {
public:
    explicit F2LSolver(RubixCube& cube);

    bool solve();
    const std::string& getMoves() const { return moves_; }
    const std::string& getError() const { return error_; }
    static bool isSolved(const RubixCube& cube);

private:
    RubixCube& cube_;
    std::string moves_;
    std::string error_;
};
