#include <stdio.h>
#include <string.h>

#include "mref.h"

int main(int argc, char **argv) {
    mref_err_t e;
    struct mref m;

    if (argc != 2) return 99;
    e = mref_search(&m, argv[1], strlen(argv[1]));
    if (e != 0) { fprintf(stderr, "search returns %d\n", e); return 1; }
    e = mref_parse(&m);
    if (e != 0) { fprintf(stderr, "parse returns %d\n", e); return 1; }
    printf("sender = %s\n", mref_field_alloc(&m, MREF_FLD_SENDER));
    return 0;
}
