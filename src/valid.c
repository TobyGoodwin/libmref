#include "mref.h"

int mref_valid(mref m) {
    if (!m) return 0;

    /* version number */
    if (*m != '0') return 0;
    ++m;

    /* flags */
    while (*m != '\0' && *m != ',') {
        if (*m != 'L') return 0;
        ++m;
    }
    if (*m != ',') return 0;

    return 1;
}
