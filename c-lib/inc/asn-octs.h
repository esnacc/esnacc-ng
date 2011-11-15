/*
 * asn_octs.h
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
 * $Header: /baseline/SNACC/c-lib/inc/asn-octs.h,v 1.8 2004/01/22 20:02:58 nicholar Exp $
 */


#ifndef _asn_octs_h_
#define _asn_octs_h_

#ifdef __cplusplus
extern "C" {
#endif


typedef struct AsnOcts
{
  unsigned long octetLen;
  char			*octs;
} AsnOcts;

#define ASNOCTS_PRESENT( aocts)	((aocts)->octs != NULL)



AsnLen BEncAsnOcts PROTO ((GenBuf *b, AsnOcts *data));

void BDecAsnOcts PROTO ((GenBuf *b, AsnOcts *result, AsnLen *bytesDecoded, ENV_TYPE env));

AsnLen BEncAsnOctsContent PROTO ((GenBuf *b, AsnOcts *octs));

void BDecAsnOctsContent PROTO ((GenBuf *b, AsnTag tagId, AsnLen len, AsnOcts *result, AsnLen *bytesDecoded, ENV_TYPE env));

void FreeAsnOcts PROTO ((AsnOcts *o));

void PrintAsnOcts PROTO ((FILE *f, AsnOcts *o, unsigned int indent));

int AsnOctsEquiv PROTO ((AsnOcts *o1, AsnOcts *o2));

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* conditional include */

