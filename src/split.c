#include <limits.h>
#include <string.h>

#include "mref.h"

mref_err_t mref_split(struct mref *m) {
    int c, n;
    size_t l;

    if (!m->x) return 0;
    if (m->length == 0) {
        l = strlen(m->x);
        if (l > USHRT_MAX) return 0;
        m->length = l;
    }

    /* find all 5 commas */
    c = 0;
    m->fbeg[c] = 0;
    for (n = 0; n < m->length; ++n)
        if (m->x[n] == ',') {
            m->fend[c++] = n;
            if (c == MREF_NUM_FIELDS) return 0;
            n++;
            if (n >= m->length) return 0;
            while (isspace(m->x[n]) && n < m->length) n++;
            if (n >= m->length) return 0;
            m->fbeg[c] = n;
        }
    if (c != MREF_NUM_FIELDS-1) return 0;
    m->fend[c] = m->length;

    return 1;
}
