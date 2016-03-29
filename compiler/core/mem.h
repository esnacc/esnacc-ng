/*
 *  compiler/core/mem.h
 *
 * MS 91/08/03
 *
 * Copyright (C) 1991, 1992 Michael Sample
 *            and the University of British Columbia
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * $Header: /baseline/SNACC/compiler/core/mem.h,v 1.2 2001/07/12 19:34:30 leonberp Exp $
 * $Log: mem.h,v $
 * Revision 1.2  2001/07/12 19:34:30  leonberp
 * Changed namespace to SNACC and added compiler options: -ns and -nons.  Also removed dead code.
 *
 * Revision 1.1.1.1  2000/08/21 20:36:00  leonberp
 * First CVS Version of SNACC.
 *
 * Revision 1.2  1994/09/01 00:40:32  rj
 * snacc_config.h's last macro, MT ( ) got here.
 *
 * Revision 1.1  1994/08/28  09:49:22  rj
 * first check-in. for a list of changes to the snacc-1.1 distribution please refer to the ChangeLog.
 *
 */

#include "snacc.h"

#ifndef _snacc_mem_h_
#define _snacc_mem_h_

void	*Malloc PROTO ((int size));
void	*Realloc PROTO ((void *ptr, int newsize));
void	Free PROTO ((void *ptr));

static inline void *Memdup (const char *p, size_t len) {
    char *r = Malloc(len);
    memcpy(r, p, len);
    return r;
}

static inline char *Strdup (const char *str) {
    return Memdup(str, strlen(str) + 1);
}

/* malloc type */
#define MT( type)	(type *)Malloc (sizeof (type))

#endif

