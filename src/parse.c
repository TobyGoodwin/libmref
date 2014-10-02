#include <gcrypt.h>
#include <stdio.h>

#include "mref.h"

mref_err_t mref_parse(struct mref *m) {
    int i, p, q;

    if (!mref_split(m)) return MREF_ERR_NOT_FIELDS;

    /* version number */
    p = 0; q = m->fend[MREF_FLD_VERSION];
    if (m->x[p] != '0') return 2;

    /* flags */
    for (i = p + 1; i < q; ++i)
        if (m->x[i] != 'L') return 3;

    /* sender - not much to say? */
    p = m->fbeg[MREF_FLD_SENDER]; q = m->fend[MREF_FLD_SENDER];
    for (i = p; i < q; ++i)
        ;

    /* recipient */
    p = m->fbeg[MREF_FLD_RCPT]; q = m->fend[MREF_FLD_RCPT];
    /* not empty */
    if (p - q == 0) return 4;

    /* store */
    p = m->fbeg[MREF_FLD_STORE]; q = m->fend[MREF_FLD_STORE];
    /* not empty */
    if (q - p == 0) return 5;

    /* message hash */
    p = m->fbeg[MREF_FLD_MESSAGE_HASH];
    q = m->fend[MREF_FLD_MESSAGE_HASH];
printf("p is %d, q is %d\n", p, q);
    /* b64 sha256 */
    if (q - p != 44) return 6;

    /* mref hash */
    p = m->fbeg[MREF_FLD_MREF_HASH]; q = m->fend[MREF_FLD_MREF_HASH];
printf("p is %d, q is %d\n", p, q);
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
        /* XXX doesn't work if there are spaces (easy to fix) */
        gcry_md_write(ghd, m->x, m->fend[MREF_FLD_MESSAGE_HASH]);
        memcpy(h, gcry_md_read(ghd, 0), 32);
        gcry_md_close(ghd);
        /* probably better to decode the stated hash, and compare with the 32
         * byte calculated hash */
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
        if (memcmp(m->x + p, h64, 44) != 0) return 9;
    }

    return 0;
}
