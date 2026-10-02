#ifndef MOVEPARSER_H
#define MOVEPARSER_H

#include <string>
#include <vector>

using namespace std;

class RubixCube;

class MoveParser
{
public:
    static vector<string> tokenize(const string &sequence);

    static void execute(RubixCube &cube, const string &sequence);

    static void executeMove(RubixCube &cube, const string &move);
};

#endif