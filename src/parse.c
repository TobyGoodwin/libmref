#include <gcrypt.h>
#include <stdio.h>

#include "mref.h"

int mref_parse(mref m, int *field) {
    int i, p, q;

    if (!mref_split(m, field)) return 1;

    /* version number */
    p = 0; q = field[0] - 1;
    if (m[p] != '0') return 2;

    /* flags */
    for (i = p + 1; i < q; ++i)
        if (m[i] != 'L') return 3;

    /* sender - not much to say? */
    p = field[0]; q = field[1] - 1;
    for (i = p; i < q; ++i)
        ;

    /* recipient */
    p = field[1]; q = field[2] - 1;
    /* not empty */
    if (p - q == 0) return 4;

    /* store */
    p = field[2]; q = field[3] - 1;
    /* not empty */
    if (q - p == 0) return 5;

    /* message hash */
    p = field[3]; q = field[4] - 1;
    /* not empty */
    if (q - p != 44) return 6;

    /* mref hash */
    p = field[4]; q = field[5] - 1;
    /* not empty */
    if (q - p != 44) return 7;

    {
        gcry_error_t gerr;
        gcry_md_hd_t ghd;
        unsigned char h[32], h64[45];
        int i, j;
        unsigned char map[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

        gcry_check_version(0);
        gerr = gcry_md_open(&ghd, GCRY_MD_SHA256, 0);
        if (!ghd) return 8;
        gcry_md_write(ghd, m, field[4] - 1);
        memcpy(h, gcry_md_read(ghd, 0), 32);
        gcry_md_close(ghd);
        for (i = 0, j = 0; i < 30; i += 3) {
            h64[j++] = map[ h[i] >> 2 ];
            h64[j++] = map[ (h[i] & 3) << 4 | h[i+1] >> 4 ];
            h64[j++] = map[ (h[i+1] & 0xf) << 2 | h[i+2] >> 6 ];
            h64[j++] = map[ h[i+2] & 0x3f ];
        }
        h64[j++] = map[ h[i] >> 2 ];
        h64[j++] = map[ (h[i] & 3) << 4 | h[i+1] >> 4 ];
        h64[j++] = map[ (h[i+1] & 0xf) << 2 ];
        h64[j++] = '=';
        h64[j++] = '\0';
        printf("hash is %s\n", h64);
        if (memcmp(m + p, h64, 44) != 0) return 9;
    }

    return 0;
}
