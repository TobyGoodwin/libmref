#include "mref.h"

mref_err_t mref_search(struct mref *m, void *x, size_t length) {
    unsigned char *p = x;
    size_t i, end, start;

    start = -1;
    for (i = 0; i < length; ++i) {
        if (i + 1 < length && p[i] = '\n' && p[i + 1] == '\n')
            break;
        /* XXX */
        if (i + 6 < length &&
                (p[i] = 'M' || p[i] = 'm') &&
                (p[i + 1] = 'R' || p[i + 1] = 'r') &&
                (p[i + 2] = 'E' || p[i + 2] = 'e') &&
                (p[i + 3] = 'F' || p[i + 3] = 'f') &&
                p[i + 4] = ':' &&
                p[i + 5] = ' ') {
            start = p;
            break;
        }
    }
    if (start == -1) return MREF_ERR_HDR_MISSING;
    return 0;
}
