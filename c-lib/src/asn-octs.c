/*
 * .../c-lib/src/asn-octs.c - BER encode, decode, print and free routines for the ASN.1 OCTET STRING type.
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
 * $Header: /baseline/SNACC/c-lib/src/asn-octs.c,v 1.13 2004/03/23 18:49:21 gronej Exp $
 * $Log: asn-octs.c,v $
 * Revision 1.13  2004/03/23 18:49:21  gronej
 * fixed linux warnings
 *
 * Revision 1.12  2004/01/22 20:03:12  nicholar
 * Changed indent variable to an unsigned int from an unsigned short.  Indent is  used in library print functions and generated print functions
 *
 * Revision 1.11  2003/12/17 19:05:03  gronej
 * SNACC baseline merged with PER v1_7 tag
 *
 * Revision 1.10.2.1  2003/11/05 14:58:53  gronej
 * working PER code merged with esnacc_1_6
 *
 * Revision 1.10  2002/10/23 10:23:37  mcphersc
 * Changed BUF_TYPE to AsnBuf
 *
 * Revision 1.9  2002/10/18 13:09:32  mcphersc
 * took out long int to unsigned long
 *
 * Revision 1.8  2002/10/01 14:13:52  mcphersc
 * ASN "C" Buf modifications
 *
 * Revision 1.7  2002/09/03 19:43:10  vracarl
 * fixed a typo
 *
 * Revision 1.6  2002/09/03 19:33:19  vracarl
 * got rid of c++ comments
 *
 * Revision 1.5  2002/07/25 11:05:48  sfl
 * added indent=indent line 316 to get rid of warnings
 *
 * Revision 1.3  2002/03/05 18:34:15  vracarl
 * added CheckAsn1Alloc check and a check for INDEFINITE_LEN
 *
 * Revision 1.2  2000/10/16 18:10:40  rwc
 * removed most warnings from C++-lib, some C-lib.
 *
 * Revision 1.1.1.1  2000/08/21 20:35:52  leonberp
 * First CVS Version of SNACC.
 *
 * Revision 1.3  1995/07/27 09:00:32  rj
 * use memcmpeq that is defined in .../snacc.h to use either memcmp or bcmp.
 *
 * changed `_' to `-' in file names.
 *
 * Revision 1.2  1994/09/01  00:06:15  rj
 * reduce the risk of unwanted surprises with macro expansion by properly separating the C tokens.
 *
 * Revision 1.1  1994/08/28  09:45:58  rj
 * first check-in. for a list of changes to the snacc-1.1 distribution please refer to the ChangeLog.
 *
 */

#include <ctype.h>

#include "asn-config.h"
#include "asn-len.h"
#include "asn-tag.h"
#include "str-stk.h"
#include "asn-bits.h" /* for TO_HEX macro */
#include "asn-octs.h"
#include <memory.h>


/*
 * encodes universal TAG LENGTH and Contents of and ASN.1 OCTET STRING
 */
AsnLen
BEncAsnOcts PARAMS ((b, data),
    GenBuf *b _AND_
    AsnOcts *data)
{
    AsnLen len;

    len =  BEncAsnOctsContent (b, data);
    len += BEncDefLen (b, len);
    len += BEncTag1 (b, UNIV, PRIM, OCTETSTRING_TAG_CODE);
    return len;
}  /* BEncAsnOcts */


/*
 * decodes universal TAG LENGTH and Contents of and ASN.1 OCTET STRING
 */
void
BDecAsnOcts PARAMS ((b, result, bytesDecoded, env),
    GenBuf *b _AND_
    AsnOcts    *result _AND_
    AsnLen *bytesDecoded _AND_
    jmp_buf env)
{
    AsnTag tag;
    AsnLen elmtLen;

    if (((tag = BDecTag (b, bytesDecoded, env)) != MAKE_TAG_ID (UNIV, PRIM, OCTETSTRING_TAG_CODE)) && (tag != MAKE_TAG_ID (UNIV, CONS, OCTETSTRING_TAG_CODE)))
    {
         Asn1Error ("BDecAsnOcts: ERROR - wrong tag on OCTET STRING.\n");
         longjmp (env, -40);
    }

    elmtLen = BDecLen (b, bytesDecoded, env);
    BDecAsnOctsContent (b, tag, elmtLen, result, bytesDecoded, env);

}  /* BDecAsnOcts */

/*
 * BER encodes just the content of an OCTET STRING.
 */
AsnLen
BEncAsnOctsContent PARAMS ((b, o),
    GenBuf *b _AND_
    AsnOcts *o)
{
    BufPutSegRvs (b, o->octs, o->octetLen);
    return o->octetLen;
}  /* BEncAsnOctsContent */



/*
 * Used for decoding constructed OCTET STRING values into
 * a contiguous local rep.
 * fills string stack with references to the pieces of a
 * construced octet string
 */
static void
FillOctetStringStk PARAMS ((b, elmtLen0, bytesDecoded, env),
    GenBuf *b _AND_
    AsnLen elmtLen0 _AND_
    AsnLen *bytesDecoded _AND_
    jmp_buf env)
{
    unsigned long refdLen;
    unsigned long totalRefdLen;
    char *strPtr;
    unsigned long totalElmtsLen1 = 0;
    unsigned long tagId1;
    unsigned long elmtLen1;

    for (; (totalElmtsLen1 < elmtLen0) || (elmtLen0 == INDEFINITE_LEN); )
    {
        tagId1 = BDecTag (b, &totalElmtsLen1, env);

        if ((tagId1 == EOC_TAG_ID) && (elmtLen0 == INDEFINITE_LEN))
        {
            BDEC_2ND_EOC_OCTET (b, &totalElmtsLen1, env);
            break;
        }

        elmtLen1 = BDecLen (b, &totalElmtsLen1, env);
        if (tagId1 == MAKE_TAG_ID (UNIV, PRIM, OCTETSTRING_TAG_CODE))
        {
            /*
             * primitive part of string, put references to piece (s) in
             * str stack
             */
            totalRefdLen = 0;
            refdLen = elmtLen1;
            while (1)
            {
                strPtr = (char *)BufGetSeg (b, &refdLen);

                PUSH_STR (strPtr, refdLen, env);
                totalRefdLen += refdLen;
                if (totalRefdLen == elmtLen1)
                    break; /* exit this while loop */

                if (refdLen == 0) /* end of data */
                {
                    Asn1Error ("BDecConsOctetString: ERROR - attempt to decode past end of data\n");
                    longjmp (env, -18);
                }
                refdLen = elmtLen1 - totalRefdLen;
            }
            totalElmtsLen1 += elmtLen1;
        }


        else if (tagId1 == MAKE_TAG_ID (UNIV, CONS, OCTETSTRING_TAG_CODE))
        {
            /*
             * constructed octets string embedding in this constructed
             * octet string. decode it.
             */
            FillOctetStringStk (b, elmtLen1, &totalElmtsLen1, env);
        }
        else  /* wrong tag */
        {
            Asn1Error ("BDecConsOctetString: ERROR - decoded non-OCTET STRING tag inside a constructed OCTET STRING\n");
            longjmp (env, -19);
        }
    } /* end of for */

    (*bytesDecoded) += totalElmtsLen1;

}  /* FillOctetStringStk */


/*
 * Decodes a seq of universally tagged octets strings until either EOC is
 * encountered or the given len is decoded.  Merges them into a single
 * string. puts a NULL terminator on the string but does not include
 * this in the length.
 */
static void
BDecConsAsnOcts PARAMS ((b, len, result, bytesDecoded, env),
    GenBuf *b _AND_
    AsnLen len _AND_
    AsnOcts *result _AND_
    AsnLen *bytesDecoded _AND_
    jmp_buf env)
{
    char *bufCurr;
    unsigned long curr;

    RESET_STR_STK();

    /*
     * decode each piece of the octet string, puting
     * an entry in the octet string stack for each
     */
    FillOctetStringStk (b, len, bytesDecoded, env);

    result->octetLen = strStkG.totalByteLen;

    /* alloc str for all octs pieces with extra byte for null terminator */
    bufCurr = result->octs = Asn1Alloc (strStkG.totalByteLen +1);
    CheckAsn1Alloc (result->octs, env);

    /* copy octet str pieces into single blk */
    for (curr = 0; curr < strStkG.nextFreeElmt; curr++)
    {
        memcpy (bufCurr, strStkG.stk[curr].str, strStkG.stk[curr].len);
        bufCurr += strStkG.stk[curr].len;
    }

    /* add null terminator - this is not included in the str's len */
    *bufCurr = '\0';

}  /* BDecConsAsnOcts */

/*
 * Decodes the content of a BER OCTET STRING value
 */
void
BDecAsnOctsContent PARAMS ((b, tagId, len, result, bytesDecoded, env),
    GenBuf *b _AND_
    AsnTag tagId _AND_
    AsnLen len _AND_
    AsnOcts *result _AND_
    AsnLen *bytesDecoded _AND_
    jmp_buf env)
{
    /*
     * tagId is encoded tag shifted into long int.
     * if CONS bit is set then constructed octet string
     */
    if (TAG_IS_CONS (tagId))
        BDecConsAsnOcts (b, len, result, bytesDecoded, env);

    else /* primitive octet string */
    {
        if (len == INDEFINITE_LEN)
        {
             Asn1Error ("BDecAsnOctsContent: ERROR - indefinite length on primitive\n");
             longjmp (env, -67);
        }
        result->octetLen = len;
        result->octs =  Asn1Alloc (len+1);
        CheckAsn1Alloc (result->octs, env);
        BufCopy (result->octs, b, len);

        if (BufReadError (b))
        {
            Asn1Error ("BDecAsnOctsContent: ERROR - decoded past end of data\n");
            longjmp (env, -20);
        }

        /* add null terminator - this is not included in the str's len */
        result->octs[len] = '\0';
        (*bytesDecoded) += len;
    }
}  /* BDecAsnOctsContent */


/*
 * Frees the string part of the given OCTET STRING
 */
void
FreeAsnOcts PARAMS ((v),
    AsnOcts *v)
{
    Asn1Free (v->octs);
}  /* FreeAsnOcts */

/*
 * Prints the given OCTET STRING value to the given FILE * in ASN.1
 * Value Notation.  Since the value notation uses the hard to read
 * hex format, the ASCII version is included in an ASN.1 comment.
 */
void
PrintAsnOcts PARAMS ((f,v, indent),
    FILE *f _AND_
    AsnOcts *v _AND_
    unsigned int indent)
{
    int i;

    /* print hstring value */
    fprintf (f,"'");

    for (i = 0; i < (int)(v->octetLen); i++)
	  fprintf (f,"%c%c", TO_HEX (v->octs[i] >> 4), TO_HEX (v->octs[i]));

    fprintf (f,"'H");

    /* show printable chars in comment */
    fprintf (f,"  -- \"");

    for (i = 0; i < (int)(v->octetLen); i++)
    {
/*	if (isprint (v->octs[i]))		DAD - temp removing for now*/
	if(1)
	    fprintf (f,"%c", v->octs[i]);
	else
	  fprintf (f,".");
    }
    fprintf (f,"\" --");
    indent=indent; /* referenced to avoid compiler warning. */
}


/*
 * Returns TRUE if the given OCTET STRING values are identical.
 * Returns FALSE otherwise.
 */
int
AsnOctsEquiv PARAMS ((o1, o2),
    AsnOcts *o1 _AND_
    AsnOcts *o2)
{
    return o1->octetLen == o2->octetLen &&
        !memcmp(o1->octs, o2->octs, o1->octetLen);
} /* AsnOctsEquiv */



