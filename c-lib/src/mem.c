/*
 * compiler/core/mem.c - used for allocating the components of the Module
 *         data structure.  The program expects 0'ed memory
 *         to be returned by Malloc - this initializes ptrs
 *         to NULL.
 *
 *         If there is not enough memory the Malloc exits
 *         (Callers of Malloc will never get a NULL return value)
 *
 * Copyright (C) 1991, 1992 Michael Sample
 *            and the University of British Columbia
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * $Header: /baseline/SNACC/c-lib/src/mem.c,v 1.2 2003/12/17 19:05:04 gronej Exp $
 * $Log: mem.c,v $
 * Revision 1.2  2003/12/17 19:05:04  gronej
 * SNACC baseline merged with PER v1_7 tag
 *
 * Revision 1.1.2.1  2003/11/05 14:58:54  gronej
 * working PER code merged with esnacc_1_6
 *
 * Revision 1.1  2002/05/10 17:32:16  leonberp
 * latest changes for release 2.2
 * includes integrating asn-useful into C & C++ runtime library, the compiler changes that go along with that, SnaccException changes for C++ runtime and compiler
 *
 * Revision 1.2  2000/10/24 14:54:53  rwc
 * Updated to remove high-level warnings (level 4 on MSVC++) for an easier build.
 * SOME warnings persist due to difficulty in modifying the SNACC compiler to
 * properly build clean source; also some files are built by Lex/Yacc.
 *
 * Revision 1.1.1.1  2000/08/21 20:36:00  leonberp
 * First CVS Version of SNACC.
 *
 * Revision 1.4  1995/07/25  19:11:50  rj
 * use memzero that is defined in .../snacc.h to use either memset or bzero.
 *
 * Realloc() now checks realloc(3)'s return value.
 *
 * Revision 1.3  1994/10/08  03:48:49  rj
 * since i was still irritated by cpp standing for c++ and not the C preprocessor, i renamed them to cxx (which is one known suffix for C++ source files). since the standard #define is __cplusplus, cplusplus would have been the more obvious choice, but it is a little too long.
 *
 * Revision 1.2  1994/09/01  00:39:27  rj
 * snacc_config.h removed; more portable .h file inclusion.
 *
 * Revision 1.1  1994/08/28  09:49:21  rj
 * first check-in. for a list of changes to the snacc-1.1 distribution please refer to the ChangeLog.
 */

#include "config.h"
#include "snacc.h"

#if STDC_HEADERS
#include <stdlib.h>
#else
#if HAVE_STRING_H
#include <string.h>
#endif
#if HAVE_MALLOC_H
#include <malloc.h>
#endif
#endif

#include <stdio.h>

#include "mem.h"
#include <memory.h>

void*
Malloc PARAMS ((size), int size)
{
    void *retVal = malloc (size);

    if (retVal == NULL)
    {
        fprintf (stderr, "out of memory! bye!\n");
        fprintf (stderr, "tried to allocate %d byes\n", size);
        exit (1);
    }

    memzero (retVal, size);
    return retVal;

}  /* Malloc */

void *Realloc PARAMS ((ptr, newsize),
    void *ptr _AND_
    int newsize)
{
    void *retval = realloc (ptr, newsize);

    if (retval == NULL)
    {
        fprintf (stderr, "out of memory! bye!\n");
        fprintf (stderr, "tried to reallocate %d byes\n", newsize);
        exit (1);
    }

    return retval;
}

void Free PARAMS ((ptr),
    void *ptr)
{
    free (ptr);
}

char *
Strdup PARAMS ((istring),
     const char *istring)
{
    char *ret = strdup(istring);
    if (ret == NULL)
    {
        fprintf (stderr, "out of memory! bye!\n");
        fprintf (stderr, "tried to strdup too many bytes\n");
        exit (1);
    }
    return ret;
}
