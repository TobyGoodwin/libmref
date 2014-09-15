#include <stdio.h>

#include <mref.h>

int main(int argc, char **argv) {
    if (argc != 2) return 99;
    if (mref_valid(argv[1]))
        return 0;
    else
        return 1;
}
