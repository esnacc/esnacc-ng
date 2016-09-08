/*
 * .../c-lib/src/asn-bits.c - BER encode, decode, print and free routines for ASN.1 BIT STRING type
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
 * $Header: /baseline/SNACC/c-lib/src/asn-bits.c,v 1.15 2004/03/23 18:49:21 gronej Exp $
 */

#include "asn-config.h"

#if STDC_HEADERS || HAVE_STRING_H
#include <string.h>
#else
#include <strings.h>
#endif

#include "asn-len.h"
#include "asn-tag.h"
#include "str-stk.h"
#include "asn-bits.h"

static unsigned short unusedBitsG;

const char numToHexCharTblG[16] = {'0', '1', '2', '3', '4', '5', '6', '7', '8',
                                   '9', 'a', 'b', 'c', 'd', 'e', 'f'};


/*
 * encodes universal TAG LENGTH and Contents of and ASN.1 BIT STRING
 */
AsnLen
BEncAsnBits PARAMS ((b, data),
    GenBuf *b _AND_
    AsnBits *data)
{
    AsnLen len;

    len =  BEncAsnBitsContent (b, data);
    len += BEncDefLen (b, len);
    len += BEncTag1 (b, UNIV, PRIM, BITSTRING_TAG_CODE);
    return len;
}  /* BEncAsnInt */


/*
 * decodes universal TAG LENGTH and Contents of and ASN.1 BIT STRING
 */
void
BDecAsnBits PARAMS ((b, result, bytesDecoded, env),
    GenBuf *b _AND_
    AsnBits    *result _AND_
    AsnLen *bytesDecoded _AND_
    jmp_buf env)
{
    AsnTag tag;
    AsnLen elmtLen;

    if (((tag =BDecTag (b, bytesDecoded, env)) !=
         MAKE_TAG_ID (UNIV, PRIM, BITSTRING_TAG_CODE)) &&
        (tag != MAKE_TAG_ID (UNIV, CONS, BITSTRING_TAG_CODE)))
    {
         Asn1Error ("BDecAsnBits: ERROR - wrong tag on BIT STRING.\n");
         longjmp (env, -40);
    }

    elmtLen = BDecLen (b, bytesDecoded, env);
    BDecAsnBitsContent (b, tag, elmtLen, result, bytesDecoded, env);

}  /* BDecAsnBits */



/*
 * Encodes the BIT STRING value (including the unused bits
 * byte) to the given buffer.
 */
AsnLen
BEncAsnBitsContent PARAMS ((b, bits),
    GenBuf *b _AND_
    AsnBits *bits)
{
    unsigned long unusedBits;
    unsigned long byteLen;

    if (!b || !bits)
        return 0;

    /* Calculate number of bytes and number of unused bits */
    byteLen = (bits->bitLen + 7) / 8;
    unusedBits = (unsigned char)(bits->bitLen % 8);

    if (unusedBits != 0)
        unusedBits = 8 - unusedBits;

    BufPutSegRvs(b, bits->bits, byteLen);
    BufPutByteRvs(b, unusedBits);
    return byteLen + 1;

} /* BEncAsnBitsContent */


/*
 * Used when decoding to combine constructed pieces into one
 * contiguous block.
 * Fills string stack with references to the pieces of a
 * construced bit string. sets unusedBitsG appropriately.
 * and strStkG.totalByteLenG to bytelen needed to hold the bitstring
 */
static void
FillBitStringStk PARAMS ((b, elmtLen0, bytesDecoded, env),
    GenBuf *b _AND_
    AsnLen elmtLen0 _AND_
    AsnLen *bytesDecoded _AND_
    jmp_buf env)
{
    unsigned long refdLen;
    char *strPtr;
    unsigned long totalElmtsLen1 = 0;
    unsigned long tagId1;
    unsigned long elmtLen1;
    unsigned long lenToRef;

    for (; (totalElmtsLen1 < elmtLen0) || (elmtLen0 == INDEFINITE_LEN); )
    {
        tagId1 = BDecTag (b, &totalElmtsLen1, env);

        if ((tagId1 == EOC_TAG_ID) && (elmtLen0 == INDEFINITE_LEN))
        {
            BDEC_2ND_EOC_OCTET (b, &totalElmtsLen1, env);
            break;
        }

        elmtLen1 = BDecLen (b, &totalElmtsLen1, env);
        if (tagId1 == MAKE_TAG_ID (UNIV, PRIM, BITSTRING_TAG_CODE))
        {
            /*
             * primitive part of string, put references to piece (s) in
             * str stack
             */

            if (elmtLen1 == 0) {
                Asn1Error("BDecAsnBitsContent: ERROR - invalid length on "
                          "primitive BIT STRING\n");
                longjmp(env, -6);
            }

            /*
             * get unused bits octet
             */
            if (unusedBitsG != 0)
            {
                /*
                 *  whoa - only allowed non-octet aligned bits on
                 *  on last piece of bits string
                 */
                Asn1Error ("FillBitStringStk: ERROR - a component of a constructed BIT STRING that is not the last has non-zero unused bits\n");
                longjmp (env, -1);
            }

            unusedBitsG = BufGetByte(b);

            lenToRef =elmtLen1-1; /* remove one octet for the unused bits oct*/
            while (lenToRef > 0)
            {
                refdLen = lenToRef;
                strPtr = (char *)BufGetSeg (b, &refdLen);

                if (refdLen == 0) { /* end of data */
                    Asn1Error(
                        "FillBitStringStk: ERROR - expecting more data\n");
                    longjmp(env, -2);
                }

                PUSH_STR (strPtr, refdLen, env);
                lenToRef -= refdLen;
            }
            totalElmtsLen1 += elmtLen1;
        } else if (tagId1 == MAKE_TAG_ID (UNIV, CONS, BITSTRING_TAG_CODE)) {
            /*
             * constructed octets string embedding in this constructed
             * octet string. decode it.
             */
            FillBitStringStk (b, elmtLen1, &totalElmtsLen1, env);
        }
        else  /* wrong tag */
        {
            Asn1Error ("FillBitStringStk: ERROR - decoded non-BIT STRING tag inside a constructed BIT STRING\n");
            longjmp (env, -3);
        }
    } /* end of for */

    *bytesDecoded += totalElmtsLen1;

}  /* FillBitStringStk */


/*
 * Decodes a seq of universally tagged bits until either EOC is
 * encountered or the given len decoded.  Returns them in a
 * single concatenated bit string
 */
static void
BDecConsAsnBits PARAMS ((b, len, result, bytesDecoded, env),
    GenBuf *b _AND_
    AsnLen len _AND_
    AsnBits *result _AND_
    AsnLen *bytesDecoded _AND_
    jmp_buf env)
{
    char *bufCurr;
    unsigned long curr;

    RESET_STR_STK();

    /*
     * decode each piece of the octet string, puting
     * an entry in the octet/bit string stack for each
     */
    FillBitStringStk (b, len, bytesDecoded, env);

    /* alloc single str long enough for combined bitstring */
    result->bitLen = strStkG.totalByteLen*8 - unusedBitsG;

    bufCurr = result->bits = Asn1Alloc (strStkG.totalByteLen);
    CheckAsn1Alloc (result->bits, env);

    /* copy bit string pieces (buffer refs) into single block */
    for (curr = 0; curr < strStkG.nextFreeElmt; curr++)
    {
        memcpy (bufCurr, strStkG.stk[curr].str, strStkG.stk[curr].len);
        bufCurr += strStkG.stk[curr].len;
    }

}  /* BDecConsAsnBits */

/*
 * Decodes the content of a BIT STRING (including the unused bits octet)
 * Always returns a single contiguous bit string
 */
void
BDecAsnBitsContent PARAMS ((b, tagId, len, result, bytesDecoded, env),
    GenBuf *b _AND_
    AsnTag tagId _AND_
    AsnLen len _AND_
    AsnBits *result _AND_
    AsnLen *bytesDecoded _AND_
    jmp_buf env)
{
    /*
     * tagId is encoded tag shifted into long int.
     * if CONS bit is set then constructed bit string
     */
    if (TAG_IS_CONS (tagId))
        BDecConsAsnBits (b, len, result, bytesDecoded, env);
    else /* primitive octet string */
    {
        if (len == INDEFINITE_LEN)
        {
            Asn1Error(
                "BDecAsnBitsContent: ERROR - indefinite len on primitive\n");
            longjmp(env, -5);
        } else if (len == 0) {
            Asn1Error(
                "BDecAsnBitsContent: ERROR - invalid len on primitive BIT STRING\n");
            longjmp(env, -6);
        }

        (*bytesDecoded) += len;
        len--;
        result->bitLen = (len * 8) - (unsigned int)BufGetByte (b);
        result->bits =  Asn1Alloc (len);
        CheckAsn1Alloc (result->bits, env);
        BufCopy (result->bits, b, len);
        if (BufReadError (b)) {
            Asn1Error(
                "BDecAsnBitsContent: ERROR - decoded past end of data\n");
            longjmp(env, -4);
        }
    }
}  /* BDecAsnBitsContent */



/*
 * Frees the string part of a BIT STRING
 */
void
FreeAsnBits PARAMS ((v),
    AsnBits *v)
{
    Asn1Free (v->bits);
} /* FreeAsnBits  */


/*
 * Prints the contents of the given BIT STRING to the
 * given file. indent is ignored.  Always uses ASN.1 Value Notaion
 * Hex format. (Should be binary versions in some cases)
 */
void
PrintAsnBits PARAMS ((f,v, indent),
    FILE *f _AND_
    AsnBits *v _AND_
    unsigned int indent ESNACC_UNUSED)
{
    int i;
    unsigned long octetLen;

    if (v->bitLen == 0)
        octetLen = 0;
    else
        octetLen = (v->bitLen-1)/8 +1;

    fprintf (f,"'");
    for (i = 0; i < (int)octetLen; i++)
        fprintf (f,"%c%c", TO_HEX (v->bits[i] >> 4), TO_HEX (v->bits[i]));
    fprintf (f,"'H");
} /* PrintAsnBits  */

/*
 * Returns TRUE if the given BIT STRINGs are identical.
 * Otherwise returns FALSE.
 */
int
AsnBitsEquiv PARAMS ((b1, b2),
    AsnBits *b1 _AND_
    AsnBits *b2)
{
    int octetsLessOne;
    int unusedBits;

    if ((b1->bitLen == 0) && (b2->bitLen == 0))
        return TRUE;

    octetsLessOne = (b1->bitLen-1)/8;
    unusedBits = b1->bitLen % 8;
    if (unusedBits != 0)
       unusedBits = 8 - unusedBits;

    /* trailing bits may not be significant  */
    return b1->bitLen == b2->bitLen && !memcmpeq (b1->bits, b2->bits, octetsLessOne) && ((b1->bits[octetsLessOne] & (0xFF << unusedBits)) == (b1->bits[octetsLessOne] & (0xFF << unusedBits)));

} /* AsnBitsEquiv */


/*
 * Set given bit to 1.  Most significant bit is bit 0, least significant
 * is bit (v1->bitLen -1)
 */
void
SetAsnBit PARAMS ((b1, bit),
    AsnBits *b1 _AND_
    size_t bit)
{
    unsigned long octet;
    unsigned long octetsBit;

    if (bit < (unsigned long)b1->bitLen)
    {
        octet = bit/8;
        octetsBit = 7 - (bit % 8);/* bit zero is first/most sig bit in octet */
        b1->bits[octet] |= 1 << octetsBit;
    }
}  /* SetAsnBit */


/*
 * Set given bit to 0.  Most significant bit is bit 0, least significant
 * is bit (v1->bitLen -1)
 */
void
ClrAsnBit PARAMS ((b1, bit),
    AsnBits *b1 _AND_
    size_t bit)
{
    unsigned long octet;
    unsigned long octetsBit;

    if (bit <  (unsigned long)b1->bitLen)
    {
        octet = bit/8;
        octetsBit = 7 - (bit % 8);/* bit zero is first/most sig bit in octet */
        b1->bits[octet] &= ~(1 << octetsBit);
    }

}  /* ClrAsnBit */


/*
 * Get given bit.  Most significant bit is bit 0, least significant
 * is bit (v1->bitLen -1).  Returns TRUE if the bit is 1. Returns FALSE
 * if the bit is 0.  if the bit is out of range then returns 0.
 */
int
GetAsnBit PARAMS ((b1, bit),
    AsnBits *b1 _AND_
    size_t bit)
{
    unsigned long octet;
    unsigned long octetsBit;

    if (bit < (unsigned long)b1->bitLen)
    {
        octet = bit/8;
        octetsBit = 7 - (bit % 8); /* bit zero is first/most sig bit in octet*/
        return b1->bits[octet] & (1 << octetsBit);
    }
    return 0;
}  /* AsnBits::GetBit */


