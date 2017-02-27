/*
 * asn-der.h
 *
 * Dean Povey 97/08 
 * Copyright (C) 1997 Dean Povey and the Distributed Systems Technology Centre
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
 * $Header: /baseline/SNACC/c-lib/inc/asn-der.h,v 1.6 2003/12/17 19:05:03 gronej Exp $
 * $Log: asn-der.h,v $
 * Revision 1.6  2003/12/17 19:05:03  gronej
 * SNACC baseline merged with PER v1_7 tag
 *
 * Revision 1.5.2.1  2003/11/05 14:58:55  gronej
 * working PER code merged with esnacc_1_6
 *
 * Revision 1.5  2003/02/20 21:07:59  leonberp
 * added #ifdef __cplusplus extern "C" to headers
 *
 * Revision 1.4  2002/10/18 13:10:32  mcphersc
 * took out long int to unsigned long
 *
 * Revision 1.3  2002/10/15 17:37:34  mcphersc
 * no message
 *
 * Revision 1.2  2001/07/12 19:34:01  leonberp
 * Changed namespace to SNACC and added compiler options: -ns and -nons.  Also removed dead code.
 *
 * Revision 1.1.1.1  2000/08/21 20:35:54  leonberp
 * First CVS Version of SNACC.
 *
 * Revision 1.1  1997/08/28 07:25:50  povey
 * Changes to support DER encoding/decoding
 *
 *
 */
#ifndef _asn_der_h
#define _asn_der_h
#include "c-lib/inc/exp-buf.h"
#include "c-lib/inc/gen-buf.h"

#ifdef __cplusplus
extern "C" {
#endif


typedef struct EncodedElmt {
  GenBuf *b;
  unsigned long len;
} EncodedElmt;

int EncodedElmtCmp PROTO((const void* a1, const void* b));

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif  /* _asn_der_h */

