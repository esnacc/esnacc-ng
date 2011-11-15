/*
 * asn_relative_RELATIVE_RELATIVE_OID.h
 *
 *  this file depends on asn_octs.h
 * 
 *  Joseph Grone
 *  15/01/2004
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
 * $Header: /baseline/SNACC/c-lib/inc/asn-relative-oid.h,v 1.2 2004/01/22 20:03:07 nicholar Exp $
 */

#ifndef _asn_relative_oid_h_
#define _asn_relative_oid_h_

#include "asn-octs.h"

#ifdef __cplusplus
extern "C" {
#endif


typedef AsnOcts AsnRelativeOid;  /* standard oid type  */


#define ASNRELATIVE_OID_PRESENT( aoid)	ASNOCTS_PRESENT (aoid)

AsnLen BEncAsnRelativeOid PROTO ((GenBuf *b, AsnRelativeOid *data));

void BDecAsnRelativeOid PROTO ((GenBuf *b, AsnRelativeOid *result, AsnLen *bytesDecoded, ENV_TYPE env));

#define BEncAsnRelativeOidContent( b, oid)   BEncAsnOctsContent (b, oid)


void BDecAsnRelativeOidContent PROTO ((GenBuf *b, AsnTag tag, AsnLen len, AsnRelativeOid  *result, AsnLen *bytesDecoded, ENV_TYPE env));


#define FreeAsnRelativeOid	FreeAsnOcts

void PrintAsnRelativeOid PROTO ((FILE *f, AsnRelativeOid *b, unsigned int indent));

#define AsnRelativeOidsEquiv( o1, o2) AsnOctsEquiv (o1, o2)

/* linked oid type that may be easier to use in some circumstances */
#define NULL_RELATIVE_OID_ARCNUM	-1
typedef struct RELATIVE_OID
{
  struct RELATIVE_OID	*next;
  long arcNum;
#if COMPILER || TTBL
  struct Value	*valueRef;
#endif
} RELATIVE_OID;

AsnLen EncodedRelativeOidLen PROTO ((RELATIVE_OID *oid));

void BuildEncodedRelativeOid PROTO ((RELATIVE_OID *oid, AsnRelativeOid *result));

void UnbuildEncodedRelativeOid PROTO ((AsnRelativeOid *eoid, RELATIVE_OID **result));

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* conditional include */

