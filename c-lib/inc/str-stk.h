/*
 * str_stk.h  - maintains a stack of the components of a bit string
 * or octet string so they can be copied into a single chunk
 *
 *
 *  CONSTRUCTED BIT AND OCTET STRINGS SUCK. They should be
 *  specified in the application's ASN.1 spec as SEQUENCE OF OCTET STRING
 *
 * this stack stuff is for decoding constructed bit/octet strings
 * so the user gets a single contiguous bit/octet str instead of
 * irritating little pieces.  This does not cost a lot more than
 * a linked octet/bit string type since we're copying from the
 * buffer anyway, not referencing it directly (even in simple case).
 * It will cost more if the string stk overflows and
 * needs to be enlarged via realloc - set the values of
 * initialStkSizeG, and stkGrowSize carefully for your application.
 * Once the StkSize grows, it doesn't shrink back ever.
 *
 * Only three routine use/deal with this stack garbage
 *  BDecConsAsnOcts
 *  BDecConsAsnBits
 *  SetupConsBitsOctsStringStk
 *
 * Copyright (C) 1992 Michael Sample and the University of British Columbia
 *
 * This library is free software; you can redistribute it and/or
 * modify it provided that this copyright/license information is retained
 * in original form.
 *
 * If you modify this file, you must clearly indicate your changes.
 *
 * This source code is distributed in the hope that it will be
 * useful, but WITHOUT ANY WARRANTY; without even the implied warranty
 * of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 *
 * $Header: /baseline/SNACC/c-lib/inc/str-stk.h,v 1.6 2004/03/25 19:20:16 gronej Exp $
 * $Log: str-stk.h,v $
 * Revision 1.6  2004/03/25 19:20:16  gronej
 * fixed some linux warnings
 *
 * Revision 1.5  2003/12/17 19:05:03  gronej
 * SNACC baseline merged with PER v1_7 tag
 *
 * Revision 1.4.2.1  2003/11/05 14:58:55  gronej
 * working PER code merged with esnacc_1_6
 *
 * Revision 1.4  2003/02/20 21:07:59  leonberp
 * added #ifdef __cplusplus extern "C" to headers
 *
 * Revision 1.3  2002/10/18 13:10:32  mcphersc
 * took out long int to unsigned long
 *
 * Revision 1.2  2001/07/12 19:34:06  leonberp
 * Changed namespace to SNACC and added compiler options: -ns and -nons.  Also removed dead code.
 *
 * Revision 1.1.1.1  2000/08/21 20:35:55  leonberp
 * First CVS Version of SNACC.
 *
 * Revision 1.2  1995/07/24 21:01:24  rj
 * changed `_' to `-' in file names.
 *
 * Revision 1.1  1994/08/28  09:45:41  rj
 * first check-in. for a list of changes to the snacc-1.1 distribution please refer to the ChangeLog.
 *
 */

#ifndef _str_stk_h
#ifdef __cpluscplus
extern "C" {
#endif

typedef struct StrStkElmt
{
    char *str;
    unsigned long len;
} StrStkElmt;

typedef struct StrStk
{
    StrStkElmt *stk; /* ptr to array of SSElmts with 'size' elmts */
    unsigned long initialNumElmts;
    unsigned long numElmts;  /* total # of elements in str stk */
    unsigned long growElmts; /* # elmts to increase size by when nec */
    unsigned long nextFreeElmt; /* index of next free element */
    unsigned long totalByteLen; /* octet len of string stored in stk */
} StrStk;


extern StrStk strStkG;

/*
 * initializes stk (Allocates if nec.)
 * once stk is enlarged, it doesn't shrink
 */
#define RESET_STR_STK()\
{\
    strStkG.nextFreeElmt = 0;\
    strStkG.totalByteLen = 0;\
    if (strStkG.stk == NULL){\
       strStkG.stk = (StrStkElmt*) malloc ((strStkG.initialNumElmts) *sizeof (StrStkElmt));\
       strStkG.numElmts = strStkG.initialNumElmts;}\
}


/*
 * add a char*,len pair to top of stack.
 * grows stack if necessary using realloc (!)
 */
#define PUSH_STR(strPtr, strsLen, env)\
{\
    if (strStkG.nextFreeElmt >= strStkG.numElmts)\
    {\
       strStkG.stk = (StrStkElmt*) realloc (strStkG.stk, (strStkG.numElmts + strStkG.growElmts) *sizeof (StrStkElmt));\
       strStkG.numElmts += strStkG.growElmts;\
    }\
    strStkG.totalByteLen += strsLen;\
    strStkG.stk[strStkG.nextFreeElmt].str = strPtr;\
    strStkG.stk[strStkG.nextFreeElmt].len = strsLen;\
    strStkG.nextFreeElmt++;\
}


/*
 * Set up size values for the stack that is used for merging constructed
 * octet or bit string into single strings.
 * ****  Call this before decoding anything. *****
 * Note: you don't have to call this if the default values
 * for initialStkSizeG and stkGrowSizeG are acceptable
 */
#define SetupConsBitsOctsStringStk (initialNumberOfElmts, numberOfElmtsToGrowBy)\
{\
    strStkG.initialNumElmts = initialNumberOfElmts; \
    strStkG.growElmts = numberOfElmtsToGrowBy;\
}


#ifdef __cplusplus
}
#endif /* extern "C" */
#endif /* _str_stk__h */


