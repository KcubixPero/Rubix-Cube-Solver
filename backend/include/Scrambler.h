#ifndef SCRAMBLER_H
#define SCRAMBLER_H

#include <iostream>
#include <string>

class RubixCube;

using namespace std;

class Scrambler{
public:
    // length <= 0 keeps the original random 20-25 turn behaviour.
    static string generateScramble(int length = 0);
};

#endif
