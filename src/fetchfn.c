#include <errno.h>
#include <stdio.h>

#include "mref.h"

mref_err_t mref_fetch_filename(struct mref *m, const char *n) {
    FILE *h;

    h = fopen(n, "w");
    if (!h) return 100 + errno;
    return mref_fetch_handle(m, h);
}
