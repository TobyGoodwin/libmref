#include <netinet/ip.h>
#include <stdio.h>
#include <sys/socket.h>
#include <time.h>

#include "mref.h"

/* XXX ipv6 */
static char *peer_address(int s) {
    static char r[16];
    socklen_t l;
    struct sockaddr_in sa;
    unsigned char *ip;

    l = sizeof(struct sockaddr_in);
    if (getpeername(s, (struct sockaddr *)&sa, &l) == -1) return 0;

    ip = (unsigned char *)&sa.sin_addr;
    sprintf(r, "%d.%d.%d.%d", ip[0], ip[1], ip[2], ip[3]);
    return r;
}

static char date[50];

int std11date(void) {
    struct timespec ts;
    struct tm *t;

    if (clock_gettime(CLOCK_REALTIME, &ts) == -1)
        return 0;
    t = localtime(&ts.tv_sec);
    if (strftime(date, 50, "%a, %d %b %Y %T %z", t) == 0)
        return 0;
    return 1;
}

mref_err_t mref_trace_headers(struct mref *m, FILE *h, int s, const char *me) {
    char *pa, *pn;

    if (fputs("Source-MRef: ", h) == EOF ||
            fwrite(m->x, m->length, 1, h) != 1 ||
            fputc('\n', h) == EOF)
        return MREF_ERR_SYS;

    pn = mref_field_alloc(m, MREF_FLD_STORE);
    if (!pn) return MREF_ERR_NOMEM;

    pa = peer_address(s);
    if (!pa) return MREF_ERR_NOMEM;

    if (!std11date()) return MREF_ERR_NOMEM;

    if (fprintf(h, "Retrieved: from %s ([%s]) by %s\n", pn, pa, me) < 0 ||
           fprintf(h, "    with TMSP; %s\n", date) < 0)
        return MREF_ERR_SYS;

    return 0;
}
