#include <stdio.h>

#include "mref.h"

mref_err_t mref_fetch_handle(mref_field_t f, FILE *h) {
    fprintf(h, "foo!\n");
    fclose(h);
    return 0;
}
