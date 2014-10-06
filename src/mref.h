/*
 * Copyright (C) 2014 Free Software Foundation, Inc.
 */

#ifndef _MREF_H
#define _MREF_H

#include <errno.h>
#include <stdio.h>

typedef unsigned short mref_offset;
#define MREF_NUM_FIELDS 6
struct mref {
    unsigned char *x;
    mref_offset length;
    mref_offset fbeg[MREF_NUM_FIELDS];
    mref_offset fend[MREF_NUM_FIELDS];
};

typedef int mref_err_t;

mref_err_t mref_parse(struct mref *);
mref_err_t mref_split(struct mref *);
int mref_valid(struct mref *);

/* fetch an mref's referent into a named file */
mref_err_t mref_fetch_filename(struct mref *, const char *, const char *);

/* fetch an mref's referent into an open file handle */
mref_err_t mref_fetch_handle(struct mref *, FILE *, const char *);

/* search an mref from a hunk of memory containing an RFC 5322 message */
mref_err_t mref_search(struct mref *, void *, size_t);

/* return a newly allocated, NUL-terminated copy of a field */
char *mref_field_alloc(struct mref *, int);
size_t mref_field_length(struct mref *, int);

/* headers */
mref_err_t mref_delivery_headers(struct mref *, FILE *);
mref_err_t mref_trace_headers(struct mref *, FILE *, int, const char *);

mref_err_t mref_read_handle_filename(FILE *, const char *);

/* open file, search mref, download */
mref_err_t mref_fetch(const char *, const char *);

const char *mref_strerr(mref_err_t);

#define MREF_FLD_VERSION        0
#define MREF_FLD_SENDER         1
#define MREF_FLD_RCPT           2
#define MREF_FLD_STORE          3
#define MREF_FLD_MESSAGE_HASH   4
#define MREF_FLD_MREF_HASH      5

#define MREF_ERR_NOMEM                   1
#define MREF_ERR_NOT_FIELDS              2
#define MREF_ERR_HDR_MISSING            23
#define MREF_ERR_STORE_PROTO            57

#define MREF_ERR_SYS (100+errno)
#define MREF_ERR_TLS(x) (200+x)

#endif
