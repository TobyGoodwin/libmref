#include <stdio.h>

#include "mref.h"

int main(int argc, char **argv) {
    mref_err_t e, ee;
    int i;

    e = mref_init();
    if (e != 0) return e;
    ee = 0;
    for (i = 1; i < argc; ++i) {
//printf("fetching %s\n", argv[i]);
        e = mref_fetch(argv[i], "me");
        if (e != 0) {
            printf("mref_fetch returned %d\n", e);
            ee = e;
        }
    }
    mref_finish();
    return ee;
}
