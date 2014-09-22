#include "mref.h"

int mref_valid(struct mref m) {
    return mref_parse(m) == 0;
}
