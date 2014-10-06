#include <stdio.h>

#include "mref.h"

int main(int argc, char **argv) {
    mref_err_t e;

    printf("sizeof struct mref is %d\n", sizeof(struct mref));
    struct mref m = { 0 };
    if (argc != 2) return 99;
    m.x = argv[1];
    if (mref_parse(&m) != 0) return 1;
    e = mref_fetch_filename(&m, "/tmp/t0", "me");
    return e;
}
