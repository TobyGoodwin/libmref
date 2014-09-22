#include <stdio.h>

#include <mref.h>

int main(int argc, char **argv) {
    mref_err_t r;
    struct mref m = { 0 };
    if (argc != 2) return 99;
    m.x = argv[1];
    r = mref_parse(m);
    if (r == 0)
        return 0;
    fprintf(stderr, "mref_parse(%s) ==> %d\n", argv[1], r);
    return 1;
}
