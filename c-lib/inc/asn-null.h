/*
 * asn_null.h
 *
 * MS 92
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
 * $Header: /baseline/SNACC/c-lib/inc/asn-null.h,v 1.9 2004/04/21 13:27:33 gronej Exp $
 */

#ifndef _asn_null_h_
#define _asn_null_h_

#ifdef __cplusplus
extern "C" {
#endif


typedef char AsnNull;


AsnLen BEncAsnNull PROTO ((GenBuf *b, AsnNull *data));

void BDecAsnNull PROTO ((GenBuf *b, AsnNull *result, AsnLen *bytesDecoded, ENV_TYPE env));


/* 'return' length of encoded NULL value, 0 */
#define BEncAsnNullContent(b, data) 0

void BDecAsnNullContent PROTO ((GenBuf *b, AsnTag tag, AsnLen len, AsnNull *result, AsnLen *bytesDecoded, ENV_TYPE env));

 /* do nothing */
void FreeAsnNull PROTO ((AsnNull *b));
#define FreeAsnNull( v)

void PrintAsnNull PROTO ((FILE *f, AsnNull * b, unsigned int indent));

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* conditional include */




