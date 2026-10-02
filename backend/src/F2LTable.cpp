#include "F2LTable.h"

#include <stdexcept>

using namespace std;

string F2LTable::getAlgorithm(int caseID)
{
    switch (caseID)
    {
    // =========================================================
    // F2L CASES 1 - 4
    // =========================================================

    case 1:
        return "U R U' R'";

    case 2:
        return "F R' F' R";

    case 3:
        return "U' R U R' U2 R U' R'";

    case 4:
        return "R U R'";


    // =========================================================
    // F2L CASES 5 - 8
    // =========================================================

    case 5:
        return "U' R U2 R' U2 R U' R'";

    case 6:
        return "R' F R F'";

    case 7:
        return "R U' R' U2 F' U' F";

    case 8:
        return "R U' R' U2 F' U' F";


    // =========================================================
    // F2L CASES 9 - 12
    // =========================================================

    case 9:
        return "R U R'";

    case 10:
        return "U' R U R' U R U R'";

    case 11:
        return "U' R U R' U R U' R'";

    case 12:
        return "U' R U R' U R U R'";


    // =========================================================
    // F2L CASES 13 - 16
    // =========================================================

    case 13:
        return "R' U2 R2 U R2 U R";

    case 14:
        return "R U' R' U R U' R' U2 R U' R'";

    case 15:
        return "U' R U' R' U R U R'";

    case 16:
        return "R2 F R F' R U2 R' U R";


    // =========================================================
    // F2L CASES 17 - 20
    // =========================================================

    case 17:
        return "R U2 R' U' R U R'";

    case 18:
        return "R' U2 R U R' U' R";

    case 19:
        return "U R U2 R' U R U' R'";

    case 20:
        return "U' R' U2 R U' R' U R";


    // =========================================================
    // F2L CASES 21 - 24
    // =========================================================

    case 21:
        return "R U' R' U2 R U R'";

    case 22:
        return "R' U2 R U R' U' R";

    case 23:
        return "U R U' R' U' R U' R' U R U' R'";

    case 24:
        return "U' R U2 R' U R' U' R U R";


    // =========================================================
    // F2L CASES 25 - 28
    // =========================================================

    case 25:
        return "U R U' R' D' L' U L";

    case 26:
        return "U R U' R' F R' F' R";

    case 27:
        return "R U' R' U R U' R'";

    case 28:
        return "R U R' U' F R' F' R";


    // =========================================================
    // F2L CASES 29 - 32
    // =========================================================

    case 29:
        return "R' U R U' R' U R";

    case 30:
        return "R U R' U' R U R'";

    case 31:
        return "U' R U' R' U2 R U' R'";

    case 32:
        return "U R U' R' U R U' R' U R U' R'";


    // =========================================================
    // F2L CASES 33 - 36
    // =========================================================

    case 33:
        return "U' R U R' D R' U' R";

    case 34:
        return "U R' U2 R U' R' U' R";

    case 35:
        return "U R' U R U R' U2 R";

    case 36:
        return "R U R' U' R U R' U' R U R'";


    // =========================================================
    // F2L CASES 37 - 41
    // =========================================================

    case 37:
        return "R U' R' U' R U R' U2 R U' R'";

    case 38:
        return "R U' R' U' R U R' U2 R U' R'";

    case 39:
        return "R U' R' U R U2 R' U R U' R'";

    case 40:
        return "F' L' U2 L F R U R'";

    case 41:
        return "R U' R' F' L' U2 L F";


    default:
        throw invalid_argument(
            "Invalid F2L case: " + to_string(caseID)
        );
    }
}