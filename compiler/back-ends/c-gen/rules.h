/*
 * compiler/back-ends/c-gen/rules.h
 *
 * Copyright (C) 1991, 1992 Michael Sample
 *            and the University of British Columbia
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * $Header: /baseline/SNACC/compiler/back-ends/c-gen/rules.h,v 1.2 2003/07/07 14:53:38 nicholar Exp $
 * $Log: rules.h,v $
 * Revision 1.2  2003/07/07 14:53:38  nicholar
 * Eliminated headers and cleaned up include references
 *
 * Revision 1.1.1.1  2000/08/21 20:36:05  leonberp
 * First CVS Version of SNACC.
 *
 * Revision 1.3  1995/07/25 18:46:35  rj
 * file name has been shortened for redundant part: c-gen/c-rules -> c-gen/rules.
 *
 * Revision 1.2  1994/10/08  03:48:16  rj
 * since i was still irritated by cpp standing for c++ and not the C preprocessor, i renamed them to cxx (which is one known suffix for C++ source files). since the standard #define is __cplusplus, cplusplus would have been the more obvious choice, but it is a little too long.
 *
 * Revision 1.1  1994/08/28  09:48:36  rj
 * first check-in. for a list of changes to the snacc-1.1 distribution please refer to the ChangeLog.
 *
 */

/* see asn1module.h for CTDI (C type def info) */

typedef struct CRules
{
    int  maxDigitsToAppend;
    char *choiceIdFieldName;   /* name of choiceId field */
    char *choiceIdEnumSuffix;  /* suffix for choiceId enum def name */
    char *choiceUnionFieldName; /* what the name of the choice's union is */
    char *choiceUnionDefSuffix;  /* suffix for choice union def name */
    int   capitalizeNamedElmts;
    char *printRoutineBaseName;  /* eg if "Print" -> PrintFoo (..) */
    char *encodeRoutineBaseName;
    char *decodeRoutineBaseName;
    char *freeRoutineBaseName;
    CTDI typeConvTbl[BASICTYPE_OBJECTCLASSFIELDTYPE + 1];
}  CRules;

extern CRules cRulesG;
