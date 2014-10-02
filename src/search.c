#include "mref.h"

mref_err_t mref_search(struct mref *m, void *x, size_t length) {
    unsigned char *p = x;
    size_t i, end, start;

    start = -1;
    for (i = 0; i < length; ++i) {
        if (i + 1 < length && p[i] == '\n' && p[i + 1] == '\n')
            break;
        /* XXX */
        if (i + 6 < length &&
                (p[i] == 'M' || p[i] == 'm') &&
                (p[i + 1] == 'R' || p[i + 1] == 'r') &&
                (p[i + 2] == 'E' || p[i + 2] == 'e') &&
                (p[i + 3] == 'F' || p[i + 3] == 'f') &&
                p[i + 4] == ':' &&
                p[i + 5] == ' ') {
            start = i + 6;
            break;
        }
    }
    if (start == -1) return MREF_ERR_HDR_MISSING;

    end = length;
    for (i = start; i < length; ++i) {
fprintf(stderr, "i is %d, length is %d, p[i] is %c\n", i, length, p[i]);
        if (i+1 < length && p[i] == '\n' &&
                !( p[i+1] == ' ' || p[i+1] == '\t')) {
            end = i;
            break;
        }
        if (i+1 == length && p[i] == '\n') {
            end = i;
            break;
        }
    }
    m->x = x + start;
    m->length = end - start;
fprintf(stderr, "m->length %d, x[last] is %c\n", m->length, m->x[m->length-1]);

    return 0;
}
