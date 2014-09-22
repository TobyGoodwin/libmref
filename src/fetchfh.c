#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "mref.h"

mref_err_t mref_fetch_handle(struct mref m, FILE *h) {
    char *store;
    int fd = fileno(h);
    size_t store_len;
    fprintf(h, "foo!\n");

    /* want this in an accessor function */
    store_len = m.field[MREF_FIELD_STORE_END] - m.field[MREF_FIELD_STORE_START];
    store = malloc(store_len);
    if (!store) return MREF_ERR_NOMEM;
    memcpy(store, m.x + MREF_FIELD_STORE_START, store_len);
    store[store_len] = '\0';

    fprintf(stderr, "store is: %s\n", store);
    if (fflush(h) != 0) return MREF_ERR_SYS;
    if (fsync(fd) != 0) return MREF_ERR_SYS;
    fclose(h); /* cannot fail */
    return 0;
}
