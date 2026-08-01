#include "MoveParser.h"
#include "RubixCube.h"

#include <sstream>
#include <vector>
#include <string>

using namespace std;

vector<string> MoveParser::tokenize(const string &sequence)
{
    vector<string> moves;

    stringstream ss(sequence);
    string move;

    while (ss >> move)
        moves.push_back(move);

    return moves;
}

void MoveParser::execute(RubixCube &cube, const string &sequence)
{
    auto moves = tokenize(sequence);

    for (const auto &move : moves)
        executeMove(cube, move);
}

void MoveParser::executeMove(RubixCube &cube, const std::string &move)
{
    if (move == "R")
        cube.R();

    else if (move == "R'")
        cube.R_();

    else if (move == "R2")
    {
        cube.R();
        cube.R();
    }

    else if (move == "L")
        cube.L();

    else if (move == "L'")
        cube.L_();

    else if (move == "L2")
    {
        cube.L();
        cube.L();
    }

    else if (move == "U")
        cube.U();

    else if (move == "U'")
        cube.U_();

    else if (move == "U2")
    {
        cube.U();
        cube.U();
    }

    else if (move == "D")
        cube.D();

    else if (move == "D'")
        cube.D_();

    else if (move == "D2")
    {
        cube.D();
        cube.D();
    }

    else if (move == "F")
        cube.F();

    else if (move == "F'")
        cube.F_();

    else if (move == "F2")
    {
        cube.F();
        cube.F();
    }

    else if (move == "B")
        cube.B();

    else if (move == "B'")
        cube.B_();

    else if (move == "B2")
    {
        cube.B();
        cube.B();
    }

    else
    {
        throw std::invalid_argument("Invalid move: " + move);
    }
}