/*
 * hash.h
 *
 * Based on hashing stuff from UBC Raven Code (Terry Coatta & Don Acton)
 *
 * MS 92
 * Copyright (C) 1992 the University of British Columbia
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
 * $Header: /baseline/SNACC/c-lib/inc/hash.h,v 1.5 2003/12/17 19:05:03 gronej Exp $
 * $Log: hash.h,v $
 * Revision 1.5  2003/12/17 19:05:03  gronej
 * SNACC baseline merged with PER v1_7 tag
 *
 * Revision 1.4.2.1  2003/11/05 14:58:55  gronej
 * working PER code merged with esnacc_1_6
 *
 * Revision 1.4  2003/02/20 21:07:59  leonberp
 * added #ifdef __cplusplus extern "C" to headers
 *
 * Revision 1.3  2002/10/21 17:13:24  mcphersc
 * fixed long int
 *
 * Revision 1.2  2001/07/12 19:34:05  leonberp
 * Changed namespace to SNACC and added compiler options: -ns and -nons.  Also removed dead code.
 *
 * Revision 1.1.1.1  2000/08/21 20:35:54  leonberp
 * First CVS Version of SNACC.
 *
 * Revision 1.3  1997/02/28 13:39:49  wan
 * Modifications collected for new version 1.3: Bug fixes, tk4.2.
 *
 * Revision 1.2  1995/07/24 21:01:19  rj
 * changed `_' to `-' in file names.
 *
 * Revision 1.1  1994/08/28  09:21:41  rj
 * first check-in. for a list of changes to the snacc-1.1 distribution please refer to the ChangeLog.
 *
 */

#ifndef _asn_hash_h_
#define _asn_hash_h_

#ifdef __cplusplus
extern "C" {
#endif


#define TABLESIZE 256
#define INDEXMASK 0xFF
#define INDEXSHIFT 8

typedef void *Table[TABLESIZE];

typedef unsigned int Hash;

typedef struct HashSlot
{
  int    leaf;
  Hash   hash;
  void  *value;
  Table *table;
} HashSlot;

Hash MakeHash PROTO ((char *str, unsigned long  len));

Table *InitHash();

int Insert PROTO ((Table *table, void *element, Hash hash));

int CheckFor PROTO ((Table *table, Hash hash));

int CheckForAndReturnValue PROTO ((Table *table, Hash hash, void **value));

#ifdef __cplusplus
}
#endif

#endif /* conditional include */

