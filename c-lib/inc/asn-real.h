/*
 * asn_real.h
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
 * $Header: /baseline/SNACC/c-lib/inc/asn-real.h,v 1.9 2004/01/22 20:02:58 nicholar Exp $
 */


#ifndef _asn_real_h_
#define _asn_real_h_

#ifdef __cplusplus
extern "C" {
#endif

typedef double AsnReal;

extern AsnReal PLUS_INFINITY;
extern AsnReal MINUS_INFINITY;


void InitAsnInfinity();
unsigned long SignedIntOctetLen PROTO ((long  val));
    
AsnLen BEncAsnReal PROTO ((GenBuf *b, AsnReal *data));

void BDecAsnReal PROTO ((GenBuf *b, AsnReal *result, AsnLen *bytesDecoded, ENV_TYPE env));

AsnLen BEncAsnRealContent PROTO ((GenBuf *b, AsnReal *data));

void BDecAsnRealContent PROTO ((GenBuf *b, AsnTag tag, AsnLen len, AsnReal *result, AsnLen *bytesDecoded, ENV_TYPE env));

/* do nothing */
#define FreeAsnReal( v)

void PrintAsnReal PROTO ((FILE *f, AsnReal *b, unsigned int indent));

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif

