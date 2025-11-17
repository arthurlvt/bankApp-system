#include <stdio.h>
#include <stdbool.h>
#include "include.h"

bool logicOperation(bool val1, bool val2, bool opp1, bool opp2, bool opp3, bool *resultOR, bool *resultAND, bool *resultXOR) {

    if (opp1) {
        *resultOR = val1 || val2;
    }
    if (opp2) {
        *resultAND = val1 && val2;
    }
    if (opp3) {
        *resultXOR = val1 ^ val2;
    }

    return true;
}