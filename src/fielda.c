#include <stdlib.h>
#include <string.h>

#include "mref.h"

char *mref_field_alloc(struct mref *m, int f) {
    char *x;
    size_t l;

    if (f >= MREF_NUM_FIELDS) return 0;
    l = mref_field_length(m, f);
    x = malloc(l + 1);
    if (!x) return 0;
    memcpy(x, m->x + m->fbeg[f], l);
    x[l] = '\0';
    return x;
}
