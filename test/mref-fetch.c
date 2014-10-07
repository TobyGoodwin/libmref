#include <stdio.h>

#include "mref.h"

int main(int argc, char **argv) {
    mref_err_t e;

    if (argc != 2) return 99;
    printf("fetching %s\n", argv[1]);
    e = mref_fetch(argv[1], "me");
    if (e != 0) printf("mref_fetch returned %d\n", e);
    return e;
}
