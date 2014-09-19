#include "mref.h"

int mref_parse(mref m, int *field) {
    int i, p, q;

    if (!mref_split(m, field)) return 1;

    /* version number */
    p = 0; q = field[0] - 1;
    if (m[p] != '0') return 2;

    /* flags */
    for (i = p + 1; i < q; ++i)
        if (m[i] != 'L') return 3;

    /* sender - not much to say? */
    p = field[0]; q = field[1] - 1;
    for (i = p; i < q; ++i)
        ;

    /* recipient */
    p = field[1]; q = field[2] - 1;
    /* not empty */
    if (p - q == 0) return 4;

    /* store */
    p = field[2]; q = field[3] - 1;
    /* not empty */
    if (q - p == 0) return 5;

    /* message hash */
    p = field[3]; q = field[4] - 1;
    /* not empty */
    if (q - p != 44) return 6;

    /* mref hash */
    p = field[4]; q = field[5] - 1;
    /* not empty */
    if (q - p != 44) return 7;

    return 0;
}
