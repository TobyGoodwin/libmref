#include "mref.h"

int mref_valid(mref m) {
    int c, n, comma[5];
    if (!m) return 0;

    /* find all 5 commas */
    c = 0;
    for (n = 0; m[n] != '\0'; ++n)
        if (m[n] == ',') {
            if (c == 5) return 0;
            comma[c++] = n;
        }
    if (c != 5) return 0;

    /* version number */
    n = 0;
    if (m[n] != '0') return 0;

    /* flags */
    for (++n; n < comma[0]; ++n)
        if (*m != 'L') return 0;

    /* sender - not much to say? */
    for (++n; n < comma[1]; ++n)
        ;

    /* recipient */
    /* not empty */
    if (comma[2] - comma[1] == 1)
        return 0;

    return 1;
}
