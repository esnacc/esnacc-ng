/*
 * nibble_alloc.h - handles buffer allocation
 * MS 91
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
 * $Header: /baseline/SNACC/c-lib/inc/nibble-alloc.h,v 1.6 2003/12/17 19:05:03 gronej Exp $
 * $Log: nibble-alloc.h,v $
 * Revision 1.6  2003/12/17 19:05:03  gronej
 * SNACC baseline merged with PER v1_7 tag
 *
 * Revision 1.5.2.1  2003/11/05 14:58:55  gronej
 * working PER code merged with esnacc_1_6
 *
 * Revision 1.5  2003/02/20 21:07:59  leonberp
 * added #ifdef __cplusplus extern "C" to headers
 *
 * Revision 1.4  2002/10/21 17:13:24  mcphersc
 * fixed long int
 *
 * Revision 1.3  2002/10/18 13:10:32  mcphersc
 * took out long int to unsigned long
 *
 * Revision 1.2  2001/07/12 19:34:06  leonberp
 * Changed namespace to SNACC and added compiler options: -ns and -nons.  Also removed dead code.
 *
 * Revision 1.1.1.1  2000/08/21 20:35:54  leonberp
 * First CVS Version of SNACC.
 *
 * Revision 1.2  1995/07/24 21:01:22  rj
 * changed `_' to `-' in file names.
 *
 * Revision 1.1  1994/08/28  09:21:43  rj
 * first check-in. for a list of changes to the snacc-1.1 distribution please refer to the ChangeLog.
 *
 */

#ifndef _nibble_alloc_h_
#define _nibble_alloc_h_


#ifdef __cplusplus
extern "C" {
#endif

typedef struct NibbleBuf
{
    char *start;
    char *end;
    char *curr;
    struct NibbleBuf *next;
} NibbleBuf;


typedef struct NibbleMem
{
    NibbleBuf *firstNibbleBuf;
    NibbleBuf *currNibbleBuf;
    unsigned long incrementSize;
} NibbleMem;



void InitNibbleMem PROTO ((unsigned long initialSize, unsigned long incrementSize));

void ShutdownNibbleMem();

void ServiceNibbleFault PROTO ((unsigned long size));

void *NibbleAlloc PROTO ((unsigned long size));

void ResetNibbleMem();


#ifdef __cplusplus
}
#endif


#endif /* conditional include */

