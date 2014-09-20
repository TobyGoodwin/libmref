/*
 * Copyright (C) 2014 Free Software Foundation, Inc.
 */

#ifndef _MREF_H
#define _MREF_H

typedef unsigned char *mref;

#define MREF_NUM_FIELDS 6

typedef int mref_field_t[MREF_NUM_FIELDS];
typedef int mref_err_t;

mref_err_t mref_parse(mref, mref_field_t);
mref_err_t mref_split(mref, mref_field_t);
int mref_valid(mref);

const char *mref_strerr(mref_err_t);

#endif
