/*
 * compiler/back-ends/c-gen/kwd.c - routines for determining whether a given str is a C keyword
 *
 *    NOTE: this was hacked up really quickly.  It uses a slow linear
 *          search. A much better approach is to use a hash tbl.
 *
 * MS 92
 * Copyright (C) 1991, 1992 Michael Sample
 *            and the University of British Columbia
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * $Header: /baseline/SNACC/compiler/back-ends/c-gen/kwd.c,v 1.2 2003/07/07 14:53:38 nicholar Exp $
 * $Log: kwd.c,v $
 * Revision 1.2  2003/07/07 14:53:38  nicholar
 * Eliminated headers and cleaned up include references
 *
 * Revision 1.1.1.1  2000/08/21 20:36:05  leonberp
 * First CVS Version of SNACC.
 *
 * Revision 1.4  1995/07/25  18:45:38  rj
 * file name has been shortened for redundant part: c-gen/c-kwd -> c-gen/kwd.
 *
 * Revision 1.3  1994/10/08  03:48:06  rj
 * since i was still irritated by cpp standing for c++ and not the C preprocessor, i renamed them to cxx (which is one known suffix for C++ source files). since the standard #define is __cplusplus, cplusplus would have been the more obvious choice, but it is a little too long.
 *
 * Revision 1.2  1994/09/01  00:20:28  rj
 * snacc_config.h removed; more portable .h file inclusion.
 *
 * Revision 1.1  1994/08/28  09:48:13  rj
 * first check-in. for a list of changes to the snacc-1.1 distribution please refer to the ChangeLog.
 *
 */

#include "config.h"
#include "snacc.h"

#if STDC_HEADERS || HAVE_STRING_H
#include <string.h>
#else
#include <strings.h>
#endif


/*
 * last elmt must be NULL.
 * key words grabbed from K&R 2nd Ed
 */
static char *cKWdG[] =
{
    "auto", "break", "case", "char", "const", "continue",
    "default", "do", "double", "else", "enum", "extern",
    "float", "for", "goto", "if", "int", "long", "register",
    "return", "short", "signed", "sizeof", "static", "struct",
    "switch",  "typedef", "union", "unsigned", "void", "volatile",
    "while", NULL
};


/*
 * returns non-zero if the given str is a C key word
 */
int
IsCKeyWord PARAMS ((str),
    char *str)
{
    int i;

    for (i=0; (cKWdG[i] != NULL) && (strcmp (cKWdG[i],str) != 0); i++)
	;

    return cKWdG[i] != NULL;
}
