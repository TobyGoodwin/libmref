#include "mref.h"

int mref_valid(mref m) {
    return mref_parse(m, 0) == 0;
}
