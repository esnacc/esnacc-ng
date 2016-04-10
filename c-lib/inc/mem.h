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
 * $Header: /baseline/SNACC/c-lib/inc/mem.h,v 1.3 2003/12/17 19:05:03 gronej Exp $
 * $Log: mem.h,v $
 * Revision 1.3  2003/12/17 19:05:03  gronej
 * SNACC baseline merged with PER v1_7 tag
 *
 * Revision 1.2.2.1  2003/11/05 14:58:55  gronej
 * working PER code merged with esnacc_1_6
 *
 * Revision 1.2  2003/02/20 21:07:59  leonberp
 * added #ifdef __cplusplus extern "C" to headers
 *
 * Revision 1.1  2002/05/10 17:22:26  leonberp
 * latest changes for release 2.2
 * includes integrating asn-useful into C & C++ runtime library, the compiler changes that go along with that, SnaccException changes for C++ runtime and compiler
 *
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

#ifndef _snacc_mem_h_
#define _snacc_mem_h_

#ifdef __cplusplus
extern "C" {
#endif

#include "snacc.h"
#include "asn-incl.h"

void	*Malloc PROTO ((int size));
void	*Realloc PROTO ((void *ptr, int newsize));
void	Free PROTO ((void *ptr));

char    *Strdup PROTO ((const char *istring));
    
/* malloc type */
#define MT( type)	(type *)Malloc (sizeof (type))


#ifdef __cplusplus
}
#endif

#endif

