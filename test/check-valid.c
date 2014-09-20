#include <stdio.h>

#include <mref.h>

int main(int argc, char **argv) {
    mref_err_t r;
    mref_field_t field;
    if (argc != 2) return 99;
    r = mref_parse(argv[1], field);
    if (r == 0)
        return 0;
    fprintf(stderr, "mref_parse(%s) ==> %d\n", argv[1], r);
    return 1;
}
