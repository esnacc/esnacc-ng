/*
 * asn_bool.h
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
 * $Header: /baseline/SNACC/c-lib/inc/asn-bool.h,v 1.9 2004/04/21 13:26:24 gronej Exp $
 */

#ifndef _asn_bool_h_
#define _asn_bool_h_

#ifdef __cplusplus
extern "C" {
#endif


typedef unsigned char AsnBool;


AsnLen BEncAsnBool PROTO ((GenBuf *b, AsnBool *data));

void BDecAsnBool PROTO ((GenBuf *b, AsnBool *result, AsnLen *bytesDecoded, ENV_TYPE env));

AsnLen BEncAsnBoolContent PROTO ((GenBuf *b, AsnBool *data));

void BDecAsnBoolContent PROTO ((GenBuf *b, AsnTag tag, AsnLen len, AsnBool *result, AsnLen *bytesDecoded, ENV_TYPE env));

/* do nothing  */
void FreeAsnBool PROTO ((AsnBool *b));
#define FreeAsnBool( v)

void PrintAsnBool PROTO ((FILE *f, AsnBool *b, unsigned int indent));

#ifdef __cplusplus
}
#endif /* extern 'C' */

#endif




