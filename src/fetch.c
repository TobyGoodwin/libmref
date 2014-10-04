#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "mref.h"

mref_err_t mref_fetch(const char *f0) {
    char *addr, *f1;
    FILE *fh1;
    int fd0, fd1, r;
    size_t sz;
    struct mref m;
    struct stat sb;

    fd0 = open(f0, O_RDONLY);
    if (fd0 == -1) return MREF_ERR_SYS;

    if (fstat(fd0, &sb) == -1) return MREF_ERR_SYS;

    sz = sb.st_size;

    /* It is an error to mmap nothing. */
    if (sz == 0) return MREF_ERR_NOT_FIELDS;

    addr = (char *)-1;
    addr = mmap(0, sz, PROT_READ, MAP_SHARED, fd0, 0);

    /* XXX eventually, we must try to read the file if we cannot mmap it */
    if (addr == (char *)-1) return MREF_ERR_SYS;

    r = mref_search(&m, addr, sz);
    if (r != 0) return r;

    r = mref_parse(&m);
    if (r != 0) return r;

    f1 = malloc(strlen(f0) + 7);
    if (!f1) return MREF_ERR_NOMEM;
    strcpy(f1, f0); strcat(f1, "XXXXXX");
    fd1 = mkstemp(f1);
    if (fd1 == -1) return MREF_ERR_SYS;

    fh1 = fdopen(fd1, "w");
    if (!fh1) return MREF_ERR_SYS;

    r = mref_fetch_handle(&m, fh1);
    if (r != 0) return r;

    r = rename(f1, f0);
    if (r == -1) return MREF_ERR_SYS;

    return 0;
}
