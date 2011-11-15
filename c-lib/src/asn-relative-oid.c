/*
 * asn_relative_oid.c - BER encode, decode, print and free routines for the
 *             ASN.1 RELATIVE OBJECT IDENTIFIER type.
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
*/


#include "asn-config.h"
#include "asn-len.h"
#include "asn-tag.h"
#include "asn-octs.h"
#include "asn-relative-oid.h"


/*
 * encodes universal TAG LENGTH and Contents of and ASN.1 OBJECT ID
 */
AsnLen
BEncAsnRelativeOid PARAMS ((b, data),
    GenBuf *b _AND_
    AsnRelativeOid *data)
{
    AsnLen len;

    len =  BEncAsnRelativeOidContent (b, data);
    len += BEncDefLen (b, len);
    len += BEncTag1 (b, UNIV, PRIM, RELATIVE_OID_TAG_CODE);
    return len;
}  /* BEncAsnRelativeOid */


/*
 * decodes universal TAG LENGTH and Contents of and ASN.1 OBJECT ID
 */
void
BDecAsnRelativeOid PARAMS ((b, result, bytesDecoded, env),
    GenBuf *b _AND_
    AsnRelativeOid    *result _AND_
    AsnLen *bytesDecoded _AND_
    jmp_buf env)
{
    AsnTag tag;
    AsnLen elmtLen;

    if ((tag = BDecTag (b, bytesDecoded, env)) != MAKE_TAG_ID (UNIV, PRIM, RELATIVE_OID_TAG_CODE))
    {
         Asn1Error ("BDecAsnRelativeOid: ERROR - wrong tag on OBJECT IDENTIFIER.\n");
         longjmp (env, -40);
    }

    elmtLen = BDecLen (b, bytesDecoded, env);
    BDecAsnRelativeOidContent (b, tag, elmtLen, result, bytesDecoded, env);

}  /* BDecAsnRelativeOid */



/*
 * Decodes just the content of the RELATIVE_OID.
 * AsnRelativeOid is handled the same as a primtive octet string
 */
void
BDecAsnRelativeOidContent PARAMS ((b, tagId, len, result, bytesDecoded, env),
    GenBuf *b _AND_
    AsnTag tagId _AND_
    AsnLen len _AND_
    AsnRelativeOid *result _AND_
    AsnLen *bytesDecoded _AND_
    jmp_buf env)
{
    if (len == INDEFINITE_LEN)
    {
         Asn1Error ("BDecAsnRelativeOidContent: ERROR - indefinite length on primitive\n");
         longjmp (env, -66);
    }
    result->octetLen = len;
    result->octs =  Asn1Alloc (len);
    CheckAsn1Alloc (result->octs, env);
    BufCopy (result->octs, b, len);
    if (BufReadError (b))
    {
         Asn1Error ("BDecAsnRelativeOidContent: ERROR - decoded past end of data\n");
         longjmp (env, -21);
    }
    (*bytesDecoded) += len;
    tagId=tagId;  /* referenced to avoid compiler warning. */
}  /* BDecAsnRelativeOidContent */



/*
 * Prints the given RELATIVE_OID to the given FILE * in ASN.1 Value Notation.
 * Since the internal rep of an RELATIVE_OID is 'encoded', this routine
 * decodes each individual arc number to print it.
 */
void
PrintAsnRelativeOid PARAMS ((f,v, indent),
    FILE *f _AND_
    AsnRelativeOid *v _AND_
    unsigned int indent)
{
    unsigned int arcNum;
    int i;

    fprintf (f,"{");

    for (i = 0; i < (int)(v->octetLen); )
    {
        for (arcNum = 0; (i < (int)(v->octetLen)) && (v->octs[i] & 0x80);i++)
            arcNum = (arcNum << 7) + (v->octs[i] & 0x7f);

        arcNum = (arcNum << 7) + (v->octs[i] & 0x7f);
        i++;
        fprintf (f," %u", arcNum);
    }
    fprintf (f,"}");
    indent=indent; /* referenced to avoid compiler warning. */

} /* PrintAsnRelativeOid */



/*
 * given an RELATIVE_OID, figures out the length for the encoded version
 */
AsnLen
EncodedRelativeOidLen PARAMS ((oid),
    RELATIVE_OID *oid)
{
    AsnLen totalLen;
    unsigned long tmpArcNum;
    RELATIVE_OID *tmpOid;

    totalLen = 0;

    /*
     * oid must have at least 2 elmts
     */
    if (oid == NULL)
       return 0;

    for (tmpOid = oid; tmpOid != NULL; tmpOid = tmpOid->next)
    {
        totalLen++;
        tmpArcNum = tmpOid->arcNum;
        for (; (tmpArcNum >>= 7) != 0; totalLen++)
	    ;
    }

    return totalLen;

}  /* EncodedOidLen */


/*
 * given an oid list and a pre-allocated ENC_RELATIVE_OID
 * (use EncodedOidLen to figure out byte length needed)
 * fills the ENC_RELATIVE_OID with a BER encoded version
 * of the oid.
 */
void
BuildEncodedRelativeOid PARAMS ((oid, result),
    RELATIVE_OID *oid _AND_
    AsnRelativeOid *result)
{
    unsigned long len;
    char         *buf;
    int           i;
    unsigned long tmpArcNum;
    RELATIVE_OID          *tmpOid;

    buf = result->octs;

    /*
     * oid must have at least 2 elmts
     */
    if (oid->next == NULL)
       return;
    /*
     * munge together first two arcNum
     * note first arcnum must be <= 2
     * and second must be < 39 if first = 0 or 1
     * see (X.209) for ref to this stupidity
     */
  
   
    for (tmpOid = oid; tmpOid != NULL; tmpOid = tmpOid->next)
    {
        /*
         * figure out encoded length -1 of this arcNum
         */
        tmpArcNum = tmpOid->arcNum;
        for (len = 0; (tmpArcNum >>= 7) != 0; len++)
	    ;


        /*
         * write more signifcant bytes (if any)
         * with 'more' bit set
         */
        for (i=0; i < (int)len; i++)
            *(buf++) = (char)(0x80 | (tmpOid->arcNum >> ((len-i)*7)));

        /*
         * write least significant byte
         */
        *(buf++) = (char)(0x7f & tmpOid->arcNum);
    }
    result->octetLen = (buf - result->octs);
} /* BuildEncodedOid */


/*
 * convert an AsnRelativeOid into an RELATIVE_OID (linked list)
 * NOT RECOMMENDED for use in protocol implementations
 */
void
UnbuildEncodedRelativeOid PARAMS ((eoid, result),
    AsnRelativeOid *eoid _AND_
    RELATIVE_OID **result)
{
    RELATIVE_OID **nextOid;
    RELATIVE_OID *headOid;
    int arcNum;
    int i;
   
    for (arcNum = 0, i=0; (i < (int)(eoid->octetLen)) && (eoid->octs[i] & 0x80);i++)
        arcNum = (arcNum << 7) + (eoid->octs[i] & 0x7f);

    arcNum = (arcNum << 7) + (eoid->octs[i] & 0x7f);
    i++;

    headOid = (RELATIVE_OID*)malloc (sizeof (RELATIVE_OID));
    headOid->arcNum = arcNum;
    headOid->next = (RELATIVE_OID*)malloc (sizeof (RELATIVE_OID));
    nextOid = &headOid->next;

    
    for (i = 1; i < (int)(eoid->octetLen); )
    {
        for (arcNum = 0; (i < (int)(eoid->octetLen)) && (eoid->octs[i] & 0x80); i++)
            arcNum = (arcNum << 7) + (eoid->octs[i] & 0x7f);

        arcNum = (arcNum << 7) + (eoid->octs[i] & 0x7f);
        i++;
        *nextOid = (RELATIVE_OID*)malloc (sizeof (RELATIVE_OID));
        (*nextOid)->arcNum = arcNum;
        (*nextOid)->next = NULL;
        nextOid = &(*nextOid)->next;
        
    }

    *result = headOid;

} /* UnbuildEncodedOid */
