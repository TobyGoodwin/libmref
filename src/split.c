#include "mref.h"

int mref_split(mref m, int *field) {
    int c, n;

    if (!m) return 0;

    /* find all 5 commas */
    c = 0;
    for (n = 0; m[n] != '\0'; ++n)
        if (m[n] == ',') {
            if (c == 5) return 0;
            if (field) field[c++] = n + 1;
        }
    if (c != 5) return 0;
    if (field) field[c] = n + 1; /* pointer to 1 past terminating \0 */

    return 1;
}
