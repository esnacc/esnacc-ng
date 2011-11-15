/*
 * compiler/back_ends/c++_gen/kwd.c - routines for determining whether a given str is a C++
 *             keyword
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
 * $Header: /baseline/SNACC/compiler/back-ends/c++-gen/kwd.c,v 1.3 2004/02/12 18:55:09 gronej Exp $
 * $Log: kwd.c,v $
 * Revision 1.3  2004/02/12 18:55:09  gronej
 * Stable SNACC
 * non optional choices, sets and sequences are now not pointers
 * merged with list code
 * all memory leaks within SNACC fixed
 *
 * Revision 1.2  2003/07/07 14:52:35  nicholar
 * Eliminated headers and cleaned up include references
 *
 * Revision 1.1.1.1  2000/08/21 20:36:06  leonberp
 * First CVS Version of SNACC.
 *
 * Revision 1.7  1995/09/07 19:05:01  rj
 * where the C++ compiler has got the bool type built-in, let the C++ backend avoid conflicts with the new keywords.
 *
 * Revision 1.6  1995/08/17  14:56:12  rj
 * bool added to the list of reserved words
 *
 * Revision 1.5  1995/07/25  18:22:40  rj
 * file name has been shortened for redundant part: c++-gen/c++-kwd -> c++-gen/kwd.
 *
 * Revision 1.4  1995/02/17  20:10:40  rj
 * inclusion order fixed.
 *
 * Revision 1.3  1994/10/08  03:48:04  rj
 * since i was still irritated by cpp standing for c++ and not the C preprocessor, i renamed them to cxx (which is one known suffix for C++ source files). since the standard #define is __cplusplus, cplusplus would have been the more obvious choice, but it is a little too long.
 *
 * Revision 1.2  1994/09/01  00:19:44  rj
 * snacc_config.h removed; more portable .h file inclusion.
 *
 * Revision 1.1  1994/08/28  09:48:11  rj
 * first check-in. for a list of changes to the snacc-1.1 distribution please refer to the ChangeLog.
 *
 */

#include "snacc.h"

#if STDC_HEADERS || HAVE_STRING_H
#include <string.h>
#else
#include <strings.h>
#endif


/*
 * last elmt will be NULL.
 * key words grabbed from C++/Stroustrup 2.0
 */
static char *cxxKWdG[] =
{
    "asm", "auto", "break", "case", "catch", "char", "class", "const",
    "continue", "default", "delete", "do", "double", "else", "enum",
    "extern", "float", "for", "friend", "goto", "if", "inline", "int",
    "long", "new", "operator", "private", "protected", "public",
    "register", "return", "short", "signed", "sizeof", "static",
    "struct", "switch", "template", "this", "throw", "try", "typedef",
    "union", "unsigned", "virtual", "void", "volatile", "while", 
#if BOOL_BUILTIN
    "bool", "true", "false",
#endif
    NULL
};


/*
 * returns non-zero if the given str is a C++ key word
 */
int
IsCxxKeyWord PARAMS ((str),
    char *str)
{
    int i;

    for (i=0; (cxxKWdG[i] != NULL) && (strcmp (cxxKWdG[i],str) != 0); i++)
	;

    return cxxKWdG[i] != NULL;
}
