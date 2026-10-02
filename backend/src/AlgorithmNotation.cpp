#include "AlgorithmNotation.h"

#include <array>
#include <sstream>
#include <stdexcept>

namespace {
using Frame = std::array<char,6>; // R,L,U,D,F,B labels -> physical face letters.

int faceIndex(char face) {
    switch (face) {
        case 'R': return 0; case 'L': return 1; case 'U': return 2;
        case 'D': return 3; case 'F': return 4; case 'B': return 5;
        default: throw std::invalid_argument("Unsupported algorithm face: " + std::string(1,face));
    }
}

std::string invertSuffix(const std::string& suffix) {
    if (suffix.empty()) return "'";
    if (suffix == "'") return "";
    if (suffix == "2" || suffix == "2'") return "2";
    throw std::invalid_argument("Unsupported algorithm suffix: " + suffix);
}

void regrip(Frame& frame, char axis, bool inverse) {
    // Each row maps a face label in the new view to its face label before x/y/z.
    static const int xMap[6] = {0,1,4,5,3,2};
    static const int yMap[6] = {5,4,2,3,0,1};
    static const int zMap[6] = {2,3,1,0,4,5};
    const int* mapping = axis=='x' ? xMap : axis=='y' ? yMap : zMap;
    int turns = inverse ? 3 : 1;
    while (turns--) {
        const Frame old = frame;
        for (int i=0; i<6; ++i) frame[i] = old[mapping[i]];
    }
}

void turn(Frame& frame, std::vector<std::string>& output, char face, const std::string& suffix) {
    output.emplace_back(std::string(1,frame[faceIndex(face)]) + suffix);
}

void parseToken(Frame& frame, std::vector<std::string>& output, std::string token);

void parseExpanded(Frame& frame, std::vector<std::string>& output,
                   const std::vector<std::string>& actions) {
    for (const auto& action : actions) parseToken(frame,output,action);
}

void parseToken(Frame& frame, std::vector<std::string>& output, std::string token) {
    if (token.empty()) return;
    for (char& ch : token) if (ch=='(' || ch==')' || ch=='[' || ch==']') ch=' ';
    if (token.find_first_not_of(' ') == std::string::npos) return;
    if (token.find(' ') != std::string::npos) {
        std::istringstream parts(token);
        std::string part;
        while (parts >> part) parseToken(frame,output,part);
        return;
    }

    std::string suffix = token.substr(1);
    if (suffix == "2'") suffix = "2";
    const bool half = suffix == "2";
    const bool prime = suffix == "'";
    if (!suffix.empty() && !half && !prime)
        throw std::invalid_argument("Unsupported algorithm token: " + token);

    char face = token[0];
    if (face=='x' || face=='y' || face=='z') {
        const int count = half ? 2 : 1;
        for (int i=0; i<count; ++i) regrip(frame,face,prime);
        return;
    }

    // A slice turn is expressed as a rigid regrip plus ordinary outer turns.
    if (face=='M' || face=='E' || face=='S') {
        std::vector<std::string> actions;
        if (face=='M') actions = {"x'","R","L'"};
        else if (face=='E') actions = {"y'","U","D'"};
        else actions = {"z","F'","B"};
        if (prime) {
            std::vector<std::string> inverse;
            for (auto it=actions.rbegin(); it!=actions.rend(); ++it) {
                std::string action=*it;
                if (action.size()>1 && action[1]=='2') inverse.push_back(action);
                else if (action.size()>1 && action[1]=='\'') inverse.push_back(action.substr(0,1));
                else inverse.push_back(action+"'");
            }
            actions=std::move(inverse);
        }
        const int count=half ? 2 : 1;
        for (int i=0;i<count;++i) parseExpanded(frame,output,actions);
        return;
    }

    const bool wide = face>='a' && face<='z';
    if (wide) {
        face = static_cast<char>(face - 'a' + 'A');
        char axis = (face=='R' || face=='L') ? 'x' :
                    (face=='U' || face=='D') ? 'y' : 'z';
        char opposite = face=='R' ? 'L' : face=='L' ? 'R' :
                        face=='U' ? 'D' : face=='D' ? 'U' :
                        face=='F' ? 'B' : 'F';
        // Wide turn = cube rotation followed by the opposite outer face turn.
        std::vector<std::string> actions;
        if (!prime) actions = {std::string(1,axis),std::string(1,opposite)};
        else actions = {std::string(1,opposite)+"'",std::string(1,axis)+"'"};
        const int count=half ? 2 : 1;
        for (int i=0;i<count;++i) parseExpanded(frame,output,actions);
        return;
    }

    const std::string physicalSuffix=half ? "2" : prime ? "'" : "";
    turn(frame,output,face,physicalSuffix);
}
}

std::vector<std::string> AlgorithmNotation::toPhysicalMoves(const std::string& algorithm) {
    Frame frame{{'R','L','B','F','U','D'}};
    std::vector<std::string> result;
    std::istringstream input(algorithm);
    std::string token;
    while (input >> token) parseToken(frame,result,token);
    return result;
}

std::string AlgorithmNotation::inverse(const std::vector<std::string>& moves) {
    std::ostringstream out;
    for (auto it=moves.rbegin(); it!=moves.rend(); ++it) {
        if (out.tellp()>0) out << ' ';
        const std::string& move=*it;
        out << move[0] << invertSuffix(move.substr(1));
    }
    return out.str();
}
