#include <errno.h>
#include <stdio.h>

#include "mref.h"

mref_err_t mref_fetch_filename(mref_field_t f, const char *n) {
    FILE *h;

    h = fopen(n, "w");
    if (!h) return 100 + errno;
    return mref_fetch_handle(f, h);
}
