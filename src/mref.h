/*
 * Copyright (C) 2014 Free Software Foundation, Inc.
 */

#ifndef _MREF_H
#define _MREF_H

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
mref_err_t mref_fetch_filename(struct mref *, const char *);
/* fetch an mref's referent into an open file handle */
mref_err_t mref_fetch_handle(struct mref *, FILE *);

/* search an mref from a hunk of memory containing an RFC 5322 message */
mref_err_t mref_search(struct mref *, void *, size_t);


mref_err_t mref_read_handle_filename(FILE *, const char *);

const char *mref_strerr(mref_err_t);

#define MREF_FIELD_VERSION        0
#define MREF_FIELD_SENDER         1
#define MREF_FIELD_RCPT           2
#define MREF_FIELD_STORE          3
#define MREF_FIELD_MESSAGE_HASH   4
#define MREF_FIELD_MREF_HASH      5

#define MREF_FIELD_VERSION_END        0
#define MREF_FIELD_SENDER_START       0
#define MREF_FIELD_SENDER_END         1
#define MREF_FIELD_RCPT_START         1
#define MREF_FIELD_RCPT_END           2
#define MREF_FIELD_STORE_START        2
#define MREF_FIELD_STORE_END          3
#define MREF_FIELD_MESSAGE_HASH_START 3
#define MREF_FIELD_MESSAGE_HASH_END   4
#define MREF_FIELD_MREF_HASH_START    4
#define MREF_FIELD_MREF_HASH_END      5

#define MREF_ERR_NOMEM                1
#define MREF_ERR_NOT_FIELDS           2
#define MREF_ERR_HDR_MISSING         23

#define MREF_ERR_SYS (100+errno)
#define MREF_ERR_TLS(x) (200+x)

#endif
