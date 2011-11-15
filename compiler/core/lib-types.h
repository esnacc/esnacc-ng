/*
 * compiler/core/lib_types.h
 *
 * Copyright (C) 1991, 1992 Michael Sample
 *            and the University of British Columbia
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * $Header: /baseline/SNACC/compiler/core/lib-types.h,v 1.3 2001/07/12 19:34:30 leonberp Exp $
 * $Log: lib-types.h,v $
 * Revision 1.3  2001/07/12 19:34:30  leonberp
 * Changed namespace to SNACC and added compiler options: -ns and -nons.  Also removed dead code.
 *
 * Revision 1.2  2000/10/24 14:54:52  rwc
 * Updated to remove high-level warnings (level 4 on MSVC++) for an easier build.
 * SOME warnings persist due to difficulty in modifying the SNACC compiler to
 * properly build clean source; also some files are built by Lex/Yacc.
 *
 * Revision 1.1.1.1  2000/08/21 20:36:00  leonberp
 * First CVS Version of SNACC.
 *
 * Revision 1.3  1995/07/25 19:41:34  rj
 * changed `_' to `-' in file names.
 *
 * Revision 1.2  1994/10/08  03:48:46  rj
 * since i was still irritated by cpp standing for c++ and not the C preprocessor, i renamed them to cxx (which is one known suffix for C++ source files). since the standard #define is __cplusplus, cplusplus would have been the more obvious choice, but it is a little too long.
 *
 * Revision 1.1  1994/08/28  09:49:15  rj
 * first check-in. for a list of changes to the snacc-1.1 distribution please refer to the ChangeLog.
 *
 */
#ifndef _LibTypeInclude
#define _LibTypeInclude

typedef struct LibType
{
    enum BasicTypeChoiceId typeId;
    BER_UNIV_CODE          univTagCode;
    BER_FORM               tagForm;
    AnyRefList            *anyRefs;  /* these may be filled in do_macros.c*/
}  LibType;

#ifndef NO_EXTN_LIBTYPE
extern LibType* libTypesG;
extern LibType libBERTypesG[];
extern LibType libDERTypesG[];
#endif

#define LIBTYPE_GET_UNIV_TAG_CODE( tId)		(libTypesG[tId].univTagCode)
#define LIBTYPE_GET_TAG_FORM( tId)		(libTypesG[tId].tagForm)
#define LIBTYPE_GET_ANY_REFS( tId)		(libTypesG[tId].anyRefs)
#define LIBTYPE_GET_ANY_REFS_HNDL( tId)		(&libTypesG[tId].anyRefs)
#define LIBTYPE_GET_ANY_REFS( tId)		(libTypesG[tId].anyRefs)
#define SET_BER_LIBTYPE() (libTypesG = libBERTypesG)
#define SET_DER_LIBTYPE() (libTypesG = libDERTypesG)

#endif
