/*
 * Copyright (C) 2014 Free Software Foundation, Inc.
 */

#ifndef _MREF_H
#define _MREF_H

#include <stdio.h>

#define MREF_NUM_FIELDS 6
struct mref {
    unsigned char *x;
    int field[MREF_NUM_FIELDS];
};

typedef int mref_err_t;

mref_err_t mref_parse(struct mref *);
mref_err_t mref_split(struct mref *);
int mref_valid(struct mref *);

mref_err_t mref_fetch_filename(struct mref *, const char *n);
mref_err_t mref_fetch_handle(struct mref *, FILE *h);

const char *mref_strerr(mref_err_t);

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

#define MREF_ERR_SYS (100+errno)

#endif
