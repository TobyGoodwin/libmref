/*
 * Copyright (C) 2014 Free Software Foundation, Inc.
 */

#ifndef _MREF_H
#define _MREF_H

typedef unsigned char *mref;

#define MREF_NUM_FIELDS 6

typedef int mref_fields[MREF_NUM_FIELDS];
typedef int mref_error;

mref_error mref_parse(mref, mref_fields);
mref_error mref_split(mref, mref_fields);
int mref_valid(mref);

const char *mref_strerr(mref_error);

#endif
