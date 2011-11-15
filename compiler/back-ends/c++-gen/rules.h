/*
 * compiler/back_ends/c++_gen/rules.h
 *
 * Copyright (C) 1991, 1992 Michael Sample
 *           and the University of British Columbia
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * $Header: /baseline/SNACC/compiler/back-ends/c++-gen/rules.h,v 1.8 2004/01/29 21:21:44 gronej Exp $
 * $Log: rules.h,v $
 * Revision 1.8  2004/01/29 21:21:44  gronej
 * Took all Init() calls out of Clear() functions in generated code
 *
 * Revision 1.7  2004/01/14 19:07:53  gronej
 * Updated Compiler to accept and process relative-oid's
 *
 * Revision 1.6  2002/10/28 20:50:58  leonberp
 * Ok.. I think this is the final fix for OCTET/BIT STRING CONTAINING
 *
 * Revision 1.5  2002/10/24 21:07:21  leonberp
 * latest fixing for OCTET CONTAINING
 *
 * Revision 1.4  2002/05/10 16:39:39  leonberp
 * latest changes for release 2.2
 * includes integrating asn-useful into C & C++ runtime library, the compiler changes that go along with that, SnaccException changes for C++ runtime and compiler
 *
 * Revision 1.3  2001/07/12 19:34:22  leonberp
 * Changed namespace to SNACC and added compiler options: -ns and -nons.  Also removed dead code.
 *
 * Revision 1.2  2000/10/24 14:54:46  rwc
 * Updated to remove high-level warnings (level 4 on MSVC++) for an easier build.
 * SOME warnings persist due to difficulty in modifying the SNACC compiler to
 * properly build clean source; also some files are built by Lex/Yacc.
 *
 * Revision 1.1.1.1  2000/08/21 20:36:06  leonberp
 * First CVS Version of SNACC.
 *
 * Revision 1.3  1995/07/25 18:24:21  rj
 * file name has been shortened for redundant part: c++-gen/c++-rules -> c++-gen/rules.
 *
 * Revision 1.2  1994/10/08  03:47:50  rj
 * since i was still irritated by cpp standing for c++ and not the C preprocessor, i renamed them to cxx (which is one known suffix for C++ source files). since the standard #define is __cplusplus, cplusplus would have been the more obvious choice, but it is a little too long.
 *
 * Revision 1.1  1994/08/28  09:47:55  rj
 * first check-in. for a list of changes to the snacc-1.1 distribution please refer to the ChangeLog.
 *
 */
#ifndef _RulesInclude
#define _RulesInclude
/* see asn1module.h for CxxTDI (C++ type def info) */

typedef struct CxxRules
{
    int  maxDigitsToAppend;
    char *choiceIdFieldName;   /* name of choiceId field */
    char *choiceIdEnumName;  /* name (tag) for choiceId enum def name */
    char *choiceUnionFieldName; /* what the name of the choice's union is */
    char *choiceUnionName;  /* name (tag) for choice union def name */
    int   capitalizeNamedElmts;
    char *encodeBaseName;
    char *decodeBaseName;
    char *encodeContentBaseName;
    char *decodeContentBaseName;
    char *encodePduBaseName;
    char *decodePduBaseName;
    CxxTDI typeConvTbl[BASICTYPE_EXTENSION + 1];
}  CxxRules;

extern CxxRules cxxRulesG;
#endif
