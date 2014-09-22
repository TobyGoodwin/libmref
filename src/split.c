#include "mref.h"

mref_err_t mref_split(struct mref m) {
    int c, n;

    if (!m.x) return 0;

    /* find all 5 commas */
    c = 0;
    for (n = 0; m.x[n] != '\0'; ++n)
        if (m.x[n] == ',') {
            if (c == 5) return 0;
            m.field[c++] = n + 1;
        }
    if (c != 5) return 0;
    m.field[c] = n + 1; /* pointer to 1 past terminating \0 */

    return 1;
}
