#include <stdio.h>
#include <stdlib.h>

#include "mref.h"

mref_err_t mref_delivery_headers(struct mref *m, FILE *h) {
    char *r, *s;
    mref_err_t e = 0;

    r = mref_field_alloc(m, MREF_FLD_RCPT);
    if (!r) return MREF_ERR_NOMEM;

    s = mref_field_alloc(m, MREF_FLD_SENDER);
    if (!s) { free(r); return MREF_ERR_NOMEM; }

    if (fprintf(h, "Return-Path: <%s>\n", s) < 0 ||
            fprintf(h, "Delivered-To: %s\n", r) < 0)
        e = MREF_ERR_SYS;

    free(s); free(r);

    return e;
}
