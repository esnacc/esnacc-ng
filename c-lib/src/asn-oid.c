/*
 * asn_oid.c - BER encode, decode, print and free routines for the
 *             ASN.1 OBJECT IDENTIFIER type.
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
 * $Header: /baseline/SNACC/c-lib/src/asn-oid.c,v 1.10 2004/01/22 20:03:12 nicholar Exp $
 * $Log: asn-oid.c,v $
 * Revision 1.10  2004/01/22 20:03:12  nicholar
 * Changed indent variable to an unsigned int from an unsigned short.  Indent is  used in library print functions and generated print functions
 *
 * Revision 1.9  2003/12/17 19:05:03  gronej
 * SNACC baseline merged with PER v1_7 tag
 *
 * Revision 1.8.2.1  2003/11/05 14:58:53  gronej
 * working PER code merged with esnacc_1_6
 *
 * Revision 1.8  2002/10/23 10:23:37  mcphersc
 * Changed BUF_TYPE to AsnBuf
 *
 * Revision 1.7  2002/10/21 17:01:29  mcphersc
 * fixed unsigned short int
 *
 * Revision 1.6  2002/09/03 19:39:19  vracarl
 * got rid of c++ comments
 *
 * Revision 1.5  2002/07/25 11:15:52  sfl
 * got rid of warnings
 *
 * Revision 1.3  2001/12/18 20:56:48  vracarl
 * added set of octetLen in BuildEncodedOid
 *
 * Revision 1.2  2000/10/16 18:10:40  rwc
 * removed most warnings from C++-lib, some C-lib.
 *
 * Revision 1.1.1.1  2000/08/21 20:35:52  leonberp
 * First CVS Version of SNACC.
 *
 * Revision 1.3  1995/07/24 21:04:53  rj
 * changed `_' to `-' in file names.
 *
 * Revision 1.2  1994/09/01  00:06:21  rj
 * reduce the risk of unwanted surprises with macro expansion by properly separating the C tokens.
 *
 * Revision 1.1  1994/08/28  09:45:59  rj
 * first check-in. for a list of changes to the snacc-1.1 distribution please refer to the ChangeLog.
 *
 */

#include "asn-config.h"
#include "asn-len.h"
#include "asn-tag.h"
#include "asn-octs.h"
#include "asn-oid.h"


/*
 * encodes universal TAG LENGTH and Contents of and ASN.1 OBJECT ID
 */
AsnLen
BEncAsnOid PARAMS ((b, data),
    GenBuf *b _AND_
    AsnOid *data)
{
    AsnLen len;

    len =  BEncAsnOidContent (b, data);
    len += BEncDefLen (b, len);
    len += BEncTag1 (b, UNIV, PRIM, OID_TAG_CODE);
    return len;
}  /* BEncAsnOid */


/*
 * decodes universal TAG LENGTH and Contents of and ASN.1 OBJECT ID
 */
void
BDecAsnOid PARAMS ((b, result, bytesDecoded, env),
    GenBuf *b _AND_
    AsnOid    *result _AND_
    AsnLen *bytesDecoded _AND_
    jmp_buf env)
{
    AsnTag tag;
    AsnLen elmtLen;

    if ((tag = BDecTag (b, bytesDecoded, env)) != MAKE_TAG_ID (UNIV, PRIM, OID_TAG_CODE))
    {
         Asn1Error ("BDecAsnOid: ERROR - wrong tag on OBJECT IDENTIFIER.\n");
         longjmp (env, -40);
    }

    elmtLen = BDecLen (b, bytesDecoded, env);
    BDecAsnOidContent (b, tag, elmtLen, result, bytesDecoded, env);

}  /* BDecAsnOid */



/*
 * Decodes just the content of the OID.
 * AsnOid is handled the same as a primtive octet string
 */
void
BDecAsnOidContent PARAMS ((b, tagId, len, result, bytesDecoded, env),
    GenBuf *b _AND_
    AsnTag tagId _AND_
    AsnLen len _AND_
    AsnOid *result _AND_
    AsnLen *bytesDecoded _AND_
    jmp_buf env)
{
    if (len == INDEFINITE_LEN)
    {
         Asn1Error ("BDecAsnOidContent: ERROR - indefinite length on primitive\n");
         longjmp (env, -66);
    }
    result->octetLen = len;
    result->octs =  Asn1Alloc (len);
    CheckAsn1Alloc (result->octs, env);
    BufCopy (result->octs, b, len);
    if (BufReadError (b))
    {
         Asn1Error ("BDecAsnOidContent: ERROR - decoded past end of data\n");
         longjmp (env, -21);
    }
    (*bytesDecoded) += len;
    tagId=tagId;  /* referenced to avoid compiler warning. */
}  /* BDecAsnOidContent */



/*
 * Prints the given OID to the given FILE * in ASN.1 Value Notation.
 * Since the internal rep of an OID is 'encoded', this routine
 * decodes each individual arc number to print it.
 */
void
PrintAsnOid PARAMS ((f,v, indent),
    FILE *f _AND_
    AsnOid *v _AND_
    unsigned int indent)
{
    unsigned int firstArcNum;
    unsigned int arcNum;
    int i;

    fprintf (f,"{");

    /* un-munge first two arc numbers */
    for (arcNum = 0, i=0; (i < (int)(v->octetLen)) && (v->octs[i] & 0x80);i++)
        arcNum = (arcNum << 7) + (v->octs[i] & 0x7f);

    arcNum = (arcNum << 7) + (v->octs[i] & 0x7f);
    i++;
    firstArcNum = (unsigned short)(arcNum/40);
    if (firstArcNum > 2)
        firstArcNum = 2;

    fprintf (f,"%u %u", (unsigned int)firstArcNum, arcNum - (firstArcNum * 40));

    for (; i < (int)(v->octetLen); )
    {
        for (arcNum = 0; (i < (int)(v->octetLen)) && (v->octs[i] & 0x80);i++)
            arcNum = (arcNum << 7) + (v->octs[i] & 0x7f);

        arcNum = (arcNum << 7) + (v->octs[i] & 0x7f);
        i++;
        fprintf (f," %u", arcNum);
    }
    fprintf (f,"}");
    indent=indent; /* referenced to avoid compiler warning. */

} /* PrintAsnOid */



/*
 * given an OID, figures out the length for the encoded version
 */
AsnLen
EncodedOidLen PARAMS ((oid),
    OID *oid)
{
    AsnLen totalLen;
    unsigned long headArcNum;
    unsigned long tmpArcNum;
    OID *tmpOid;

    /*
     * oid must have at least 2 elmts
     */
    if (oid->next == NULL)
       return 0;

    headArcNum = (oid->arcNum * 40) + oid->next->arcNum;

    /*
     * figure out total encoded length of oid
     */
    tmpArcNum = headArcNum;
    for (totalLen = 1; (tmpArcNum >>= 7) != 0; totalLen++)
	;
    for (tmpOid = oid->next->next; tmpOid != NULL; tmpOid = tmpOid->next)
    {
        totalLen++;
        tmpArcNum = tmpOid->arcNum;
        for (; (tmpArcNum >>= 7) != 0; totalLen++)
	    ;
    }

    return totalLen;

}  /* EncodedOidLen */


/*
 * given an oid list and a pre-allocated ENC_OID
 * (use EncodedOidLen to figure out byte length needed)
 * fills the ENC_OID with a BER encoded version
 * of the oid.
 */
void
BuildEncodedOid PARAMS ((oid, result),
    OID *oid _AND_
    AsnOid *result)
{
    unsigned long len;
    unsigned long headArcNum;
    unsigned long tmpArcNum;
    char         *buf;
    int           i;
    OID          *tmpOid;

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
    headArcNum = (oid->arcNum * 40) + oid->next->arcNum;

    tmpArcNum = headArcNum;

    /*
     * calc # bytes needed for head arc num
     */
    for (len = 0; (tmpArcNum >>= 7) != 0; len++)
	;

    /*
     * write more signifcant bytes (if any) of head arc num
     * with 'more' bit set
     */
    for (i=0; i < (int)len; i++)
        *(buf++) = (char)(0x80 | (headArcNum >> ((len-i)*7)));

    /*
     * write least significant byte of head arc num
     */
    *(buf++) = (char)(0x7f & headArcNum);


    /*
     * write following arc nums, if any
     */
    for (tmpOid = oid->next->next; tmpOid != NULL; tmpOid = tmpOid->next)
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
 * convert an AsnOid into an OID (linked list)
 * NOT RECOMMENDED for use in protocol implementations
 */
void
UnbuildEncodedOid PARAMS ((eoid, result),
    AsnOid *eoid _AND_
    OID **result)
{
    OID **nextOid;
    OID *headOid;
    int arcNum;
    int i;
    int firstArcNum;
    int secondArcNum;

    for (arcNum = 0, i=0; (i < (int)(eoid->octetLen)) && (eoid->octs[i] & 0x80);i++)
        arcNum = (arcNum << 7) + (eoid->octs[i] & 0x7f);

    arcNum = (arcNum << 7) + (eoid->octs[i] & 0x7f);
    i++;

    firstArcNum = arcNum / 40;
    if (firstArcNum > 2)
        firstArcNum = 2;

    secondArcNum = arcNum - (firstArcNum * 40);

    headOid = (OID*)malloc (sizeof (OID));
    headOid->arcNum = firstArcNum;
    headOid->next = (OID*)malloc (sizeof (OID));
    headOid->next->arcNum = secondArcNum;
    nextOid = &headOid->next->next;

    for (; i < (int)(eoid->octetLen); )
    {
        for (arcNum = 0; (i < (int)(eoid->octetLen)) && (eoid->octs[i] & 0x80); i++)
            arcNum = (arcNum << 7) + (eoid->octs[i] & 0x7f);

        arcNum = (arcNum << 7) + (eoid->octs[i] & 0x7f);
        i++;
        *nextOid = (OID*)malloc (sizeof (OID));
        (*nextOid)->arcNum = arcNum;
        (*nextOid)->next = NULL;
        nextOid = &(*nextOid)->next;
        
    }

    *result = headOid;

} /* UnbuildEncodedOid */
