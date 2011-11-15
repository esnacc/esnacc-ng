/*
 * asn_int.h
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
 * $Header: /baseline/SNACC/c-lib/inc/asn-int.h,v 1.9 2004/04/21 13:27:09 gronej Exp $
 */

#ifndef _asn_int_h_
#define _asn_int_h_

#if SIZEOF_INT == 4
#  define I		int
#else
#  if SIZEOF_LONG == 4
#    define I		long
#  else
#    if SIZEOF_SHORT == 4
#      define I		short
#    endif
#  endif
# endif

#ifdef I
  typedef I		AsnInt;
  typedef unsigned I	UAsnInt;
#else
  #error "can't find integer type which is 4 bytes in size"
#endif
#undef I


#ifdef __cplusplus
extern "C" {
#endif


AsnLen BEncAsnInt PROTO ((GenBuf *b, AsnInt *data));

void BDecAsnInt PROTO ((GenBuf *b, AsnInt *result, AsnLen *bytesDecoded, ENV_TYPE env));

AsnLen BEncAsnIntContent PROTO ((GenBuf *b, AsnInt *data));

void BDecAsnIntContent PROTO ((GenBuf *b, AsnTag tag, AsnLen elmtLen, AsnInt  *result, AsnLen *bytesDecoded, ENV_TYPE env));

/* do nothing  */
#define FreeAsnInt( v)

void PrintAsnInt PROTO ((FILE *f, AsnInt *v, unsigned int indent));




AsnLen BEncUAsnInt PROTO ((GenBuf *b, UAsnInt *data));

void BDecUAsnInt PROTO ((GenBuf *b, UAsnInt *result, AsnLen *bytesDecoded, ENV_TYPE env));

AsnLen BEncUAsnIntContent PROTO ((GenBuf *b, UAsnInt *data));

void BDecUAsnIntContent PROTO ((GenBuf *b, AsnTag tagId, AsnLen len, UAsnInt *result, AsnLen *bytesDecoded, ENV_TYPE env));

/* do nothing  */
#define FreeUAsnInt( v)

void PrintUAsnInt PROTO ((FILE *f, UAsnInt *v, unsigned int indent));

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* conditional include */





