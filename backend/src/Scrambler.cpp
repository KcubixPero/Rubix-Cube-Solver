#include "../include/Scrambler.h"
#include <random>

struct Move
{
public:
    char face;
    string suffix;
};

const vector<Move> moves = {
        {'R', ""},
        {'R', "'"},
        {'R', "2"},
        {'L', ""},
        {'L', "'"},
        {'L', "2"},
        {'U', ""},
        {'U', "'"},
        {'U', "2"},
        {'D', ""},
        {'D', "'"},
        {'D', "2"},
        {'F', ""},
        {'F', "'"},
        {'F', "2"},
        {'B', ""},
        {'B', "'"},
        {'B', "2"}
};

int randomMove(int a, int b)
{
    static random_device rd;
    static mt19937 gen(rd());

    uniform_int_distribution<int> distrib(a, b);
    return distrib(gen);
}

string Scrambler::generateScramble()
{
    int random_num = randomMove(20, 25);

    string scramble = "";

    int moves_size = moves.size();

    Move prev = {'\0', ""};
    Move curr;

    while (random_num--)
    {
        int move = randomMove(0, moves_size - 1);
        curr = moves[move];

        while (curr.face == prev.face)
        {
            move = randomMove(0, moves_size - 1);
            curr = moves[move];
        }

        prev = curr;
        scramble += curr.face + curr.suffix + " ";
    }

    return scramble;
}