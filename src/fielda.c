#include <stdlib.h>
#include <string.h>

#include "mref.h"

char *mref_field_alloc(struct mref *m, int f) {
    char *x;
    size_t l;

    if (f >= MREF_NUM_FIELDS) return 0;
    l = m->fend[f] - m->fbeg[f] + 1;
    x = malloc(l);
    if (!x) return 0;
    memcpy(x, m->x + m->fbeg[f], l);
    x[l - 1] = '\0';
    return x;
}
