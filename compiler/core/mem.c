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
 * $Header: /baseline/SNACC/compiler/core/mem.c,v 1.3 2003/07/07 14:50:13 nicholar Exp $
 * $Log: mem.c,v $
 * Revision 1.3  2003/07/07 14:50:13  nicholar
 * Eliminated headers and cleaned up include references
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


#include <stdlib.h>
#include <stdio.h>
#include <memory.h>
#include "snacc.h"


extern FILE* errFileG;		// Defined in snacc.c


void*
Malloc PARAMS ((size), int size)
{
    void *retVal = malloc (size);

    if (retVal == NULL)
    {
        fprintf (errFileG, "out of memory! bye!\n");
        fprintf (errFileG, "tried to allocate %d byes\n", size);
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
        fprintf (errFileG, "out of memory! bye!\n");
        fprintf (errFileG, "tried to reallocate %d byes\n", newsize);
        exit (1);
    }

    return retval;
}

void Free PARAMS ((ptr),
    void *ptr)
{
    free (ptr);
}
