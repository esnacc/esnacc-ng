/*
 * asn_any.h
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
 * $Header: /baseline/SNACC/c-lib/inc/asn-any.h,v 1.8 2004/01/22 20:02:58 nicholar Exp $
 */

#ifndef _asn_any_h_
#define _asn_any_h_

#include "hash.h"

#ifdef __cplusplus
extern "C" {
#endif

/*
 * 1 hash table for integer keys
 * 1 hash table for oid keys
 */
extern Table *anyOidHashTblG;
extern Table *anyIntHashTblG;

typedef AsnLen (*EncodeFcn) PROTO ((GenBuf *b, void *value));
typedef void (*DecodeFcn) PROTO ((GenBuf *b, void *value, AsnLen *bytesDecoded, ENV_TYPE env));
typedef void (*FreeFcn)   PROTO ((void *v));
typedef void (*PrintFcn)  PROTO ((FILE *f, void *v, unsigned int indent));

/*
 * this is put into the hash table with the
 * int or oid as the key
 */
typedef struct AnyInfo
{
  int		anyId;  /* will be a value from the AnyId enum */
  AsnOid	oid;    /* will be zero len/null if intId is valid */
  AsnInt	intId;
  unsigned int	size;  /* size of the C data type (ie as ret'd by sizeof) */
  EncodeFcn	Encode;
  DecodeFcn	Decode;
  FreeFcn	Free;
  PrintFcn	Print;
} AnyInfo;


typedef struct AsnAny
{
  AnyInfo	*ai; /* point to entry in hash tbl that has routine ptrs */
  void		*value; /* points to the value */
} AsnAny;

/*
 * Returns anyId value for the given ANY type.
 * Use this to determine to the type of an ANY after decoding
 * it. Returns -1 if the ANY info is not available
 */
#define GetAsnAnyId( a)		(((a)->ai)? (a)->ai->anyId: -1)

/* 
 * This ID is used for those any by oid types for which
 * we have to use our unknown any handlers to decode/encode.
 * The data pointed at by the value field will basically
 * be the ASN.1 encoded value contained in an octs type
 * structure.
 *
 * NOTE: we don't use -1 here since that is used for the
 * GetAsnAnyId() macro.
 *
 */
#define	kUnknownAnyObjectID	-7

/*
 * used before encoding or decoding a type so the proper
 * encode or decode routine is used.
 */
void SetAnyTypeByInt PROTO ((AsnAny *v, AsnInt id));
void SetAnyTypeByOid PROTO ((AsnAny *v, AsnOid *id));
void SetAnyTypeUnknown PROTO ((AsnAny *v));


/*
 * used to initialize the hash table (s)
 */
void InstallAnyByInt PROTO ((int anyId, AsnInt intId, unsigned int size, EncodeFcn encode, DecodeFcn decode, FreeFcn free, PrintFcn print));

void InstallAnyByOid PROTO ((int anyId, AsnOid *oid, unsigned int size, EncodeFcn encode, DecodeFcn decode, FreeFcn free, PrintFcn print));


/*
 * Standard enc, dec, free, & print routines
 * for the AsnAny type.
 * These call the routines referenced from the
 * given value's hash table entry.
 */
void FreeAsnAny PROTO ((AsnAny *v));

AsnLen BEncAsnAny PROTO ((GenBuf *b, AsnAny *v));

void BDecAsnAny PROTO ((GenBuf *b, AsnAny *result, AsnLen *bytesDecoded, ENV_TYPE env));

#define DEncAsnAny BEncAsnAny
#define DDecAsnAny BDecAsnAny

void PrintAsnAny PROTO ((FILE *f, AsnAny *v, unsigned int indent));



/* AnyDefinedBy is currently the same as AsnAny */

typedef AsnAny			AsnAnyDefinedBy;

#define FreeAsnAnyDefinedBy	FreeAsnAny

#define BEncAsnAnyDefinedBy	BEncAsnAny

#define BDecAsnAnyDefinedBy	BDecAsnAny

#define PrintAsnAnyDefinedBy	PrintAsnAny


#ifdef __cplusplus
} /* extern 'C' */
#endif /* extern 'C' */

#endif /* conditional include */

