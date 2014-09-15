/*
 * Copyright (C) 2014 Free Software Foundation, Inc.
 */

#ifndef _MREF_H
#define _MREF_H

typedef unsigned char *mref;

int mref_valid(mref);

const char *mref_error(int);

#endif
