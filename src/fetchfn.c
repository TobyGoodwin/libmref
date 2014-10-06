#include <errno.h>
#include <stdio.h>

#include "mref.h"

/* XXX not using this in mref_fetch() atm; what headers should it write? */
mref_err_t mref_fetch_filename(struct mref *m, const char *n, const char *me) {
    FILE *h;

    h = fopen(n, "w");
    if (!h) MREF_ERR_SYS;
    
    return mref_fetch_handle(m, h, me);
}
