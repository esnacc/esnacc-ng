/*
 * asn_bits.h
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
 * $Header: /baseline/SNACC/c-lib/inc/asn-bits.h,v 1.9 2004/01/22 20:02:58 nicholar Exp $
 */


#ifndef _asn_bits_h_
#define _asn_bits_h_

#ifdef __cplusplus
extern "C" {
#endif



typedef struct AsnBits
{
  int	bitLen;
  char	*bits;
} AsnBits;

extern const char numToHexCharTblG[];


/* 
 * BER encode/decode routines
 */
AsnLen BEncAsnBits PROTO ((GenBuf *b, AsnBits *data));

void BDecAsnBits PROTO ((GenBuf *b, AsnBits *result, AsnLen *bytesDecoded, ENV_TYPE env));

AsnLen BEncAsnBitsContent PROTO ((GenBuf *b, AsnBits *bits));

void BDecAsnBitsContent PROTO ((GenBuf *b, AsnTag tagId, AsnLen len, AsnBits *result, AsnLen *bytesDecoded, ENV_TYPE env));

/* 
 * DER encode/decode routines
 *
 * Same as BER except that the bits are zero padded (however BEncxx does this)
 */
#define DEncAsnBits BEncAsnBits

#define DDecAsnBits BDecAsnBits

#define DEncAsnBitsContent BEncAsnBitsContent

#define DDecAsnBitsContent BDecAsnBitsContent

/* Print and Free routines */
void FreeAsnBits PROTO ((AsnBits *v));

void PrintAsnBits PROTO ((FILE *f, AsnBits *b, unsigned int indent));




/* Utility routines */
#define TO_HEX( fourBits)	(numToHexCharTblG[(fourBits) & 0x0f])


#define ASNBITS_PRESENT( abits)	((abits)->bits != NULL)

int AsnBitsEquiv PROTO ((AsnBits *b1, AsnBits *b2));

void SetAsnBit PROTO ((AsnBits *b1, size_t bit));

void ClrAsnBit PROTO ((AsnBits *b1, size_t bit));

int GetAsnBit PROTO ((AsnBits *b1, size_t bit));

#ifdef __cplusplus
} 
#endif /* extern 'C' */

#endif /* conditional include */

