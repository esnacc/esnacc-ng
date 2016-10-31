/*
 * asn_int.c - BER encode, decode, print and free routines for the
 *             ASN.1 INTEGER type
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
 * $Header: /baseline/SNACC/c-lib/src/asn-int.c,v 1.14 2004/04/21 13:29:16 gronej Exp $
 * $Log: asn-int.c,v $
 * Revision 1.14  2004/04/21 13:29:16  gronej
 * v1_7 release fix
 *
 * Revision 1.13  2004/04/21 13:10:35  gronej
 * patch submitted by Oskar to fix free calls
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
 * Revision 1.9  2002/10/21 17:13:16  mcphersc
 * fixed long int
 *
 * Revision 1.8  2002/10/21 17:01:29  mcphersc
 * fixed unsigned short int
 *
 * Revision 1.7  2002/10/18 13:09:02  mcphersc
 * took out long int to unsigned long
 *
 * Revision 1.6  2002/09/03 19:43:11  vracarl
 * fixed a typo
 *
 * Revision 1.5  2002/09/03 19:33:19  vracarl
 * got rid of c++ comments
 *
 * Revision 1.4  2002/07/25 11:00:10  sfl
 * Modified:
 * Line 183
 * tagId=tagId to get rid of warnings
 * Line 199
 * indent=indent to get rid of warnings
 * Line 369
 * tag=tag to get rid of warnings
 * Line 381
 * indent=indent to get rid of warninsg
 *
 * Revision 1.2  2000/10/16 18:10:39  rwc
 * removed most warnings from C++-lib, some C-lib.
 *
 * Revision 1.1.1.1  2000/08/21 20:35:52  leonberp
 * First CVS Version of SNACC.
 *
 * Revision 1.3  1995/07/24 21:04:51  rj
 * changed `_' to `-' in file names.
 *
 * Revision 1.2  1994/09/01  00:05:05  rj
 * reduce the risk of unwanted surprises with macro expansion by properly separating the C tokens.
 *
 * Revision 1.1  1994/08/28  09:45:53  rj
 * first check-in. for a list of changes to the snacc-1.1 distribution please refer to the ChangeLog.
 *
 */

#include "asn-config.h"
#include "asn-len.h"
#include "asn-tag.h"
#include "asn-int.h"

/*
 * encodes universal TAG LENGTH and Contents of and ASN.1 INTEGER
 */
AsnLen
BEncAsnInt PARAMS ((b, data),
    GenBuf *b _AND_
    AsnInt *data)
{
    AsnLen len;

    len =  BEncAsnIntContent (b, data);
    len += BEncDefLen (b, len);
    len += BEncTag1 (b, UNIV, PRIM, INTEGER_TAG_CODE);
    return len;
}  /* BEncAsnInt */


/*
 * decodes universal TAG LENGTH and Contents of and ASN.1 INTEGER
 */
void
BDecAsnInt PARAMS ((b, result, bytesDecoded, env),
    GenBuf *b _AND_
    AsnInt    *result _AND_
    AsnLen *bytesDecoded _AND_
    jmp_buf env)
{
    AsnTag tag;
    AsnLen elmtLen;

    if ((tag = BDecTag (b, bytesDecoded, env)) != MAKE_TAG_ID (UNIV, PRIM, INTEGER_TAG_CODE))
    {
         Asn1Error ("BDecAsnInt: ERROR wrong tag on INTEGER.\n");
         longjmp (env, -40);
    }

    elmtLen = BDecLen (b, bytesDecoded, env);
    BDecAsnIntContent (b, tag, elmtLen, result, bytesDecoded, env);

}  /* BDecAsnInt */


/*
 * encodes signed long integer's contents
 */
AsnLen
BEncAsnIntContent PARAMS ((b, data),
    GenBuf *b _AND_
    AsnInt *data)
{
    int             len;
    int             i;
    unsigned long  mask;
    long dataCpy;

#define INT_MASK (0x7f80 << ((sizeof(AsnInt) - 2) * 8))

    dataCpy = *data;

    /*
     * calculate encoded length of the integer (content)
     */
    mask = INT_MASK;
    if (dataCpy < 0)
         for (len = sizeof (AsnInt); len > 1; --len)
         {
             if ((dataCpy & mask) == mask)
                mask >>= 8;
            else
                break;
        }
    else
        for (len = sizeof (AsnInt); len > 1; --len)
        {
            if ((dataCpy & mask) == 0)
                mask >>= 8;
            else
                break;
        }

    /*
     * write the BER integer
     */
    for (i = 0; i < len; i++)
    {
        BufPutByteRvs (b, (unsigned char)dataCpy);
        dataCpy >>= 8;
    }

    return len;

}  /* BEncAsnIntContent */


/*
 * Decodes content of BER a INTEGER value.  The given tag is ignored.
 */
void
BDecAsnIntContent PARAMS ((b, tagId, len, result, bytesDecoded, env),
    GenBuf *b _AND_
    AsnTag     tagId ESNACC_UNUSED _AND_
    AsnLen     len _AND_
    AsnInt    *result _AND_
    AsnLen *bytesDecoded _AND_
    jmp_buf env)
{
    int   i;
    long retVal;
    unsigned long byte;


    if (len > sizeof (AsnInt))
    {
        Asn1Error ("BDecAsnIntContent: ERROR - integer to big to decode.\n");
        longjmp (env, -7);
    }

    /*
     * look at integer value
     */
    byte = (unsigned long ) BufGetByte (b);

    if (byte & 0x80)   /* top bit of first byte is sign bit */
        retVal = (~0U << 8) | byte;
    else
        retVal = byte;

    /*
     * write from buffer into long int
     */
    for (i = 1; i < (int)len; i++)
        retVal = (retVal << 8) | (unsigned long)(BufGetByte (b));

    if (BufReadError (b))
    {
        Asn1Error ("BDecAsnIntContent: ERROR - decoded past end of data \n");
        longjmp (env, -8);
    }
    (*bytesDecoded) += len;

    *result = retVal;
}  /* BDecAsnIntContent */


/*
 * Prints the given integer to the given FILE * in Value Notation.
 * indent is ignored.
 */
void
PrintAsnInt PARAMS ((f, v, indent),
    FILE *f _AND_
    AsnInt *v _AND_
    unsigned int indent ESNACC_UNUSED)
{
    fprintf (f,"%d", *v);
}


/*
 * The following deal with unsigned longs.
 * They do the same as the above routines for unsigned values.
 *
 * The compiler generated code does not call them. (It should
 * based on subtype info but it does not).
 */


/*
 * encodes universal TAG LENGTH and Contents of and ASN.1 INTEGER
 */
AsnLen
BEncUAsnInt PARAMS ((b, data),
    GenBuf *b _AND_
    UAsnInt *data)
{
    AsnLen len;

    len =  BEncUAsnIntContent (b, data);
    len += BEncDefLen (b, len);
    len += BEncTag1 (b, UNIV, PRIM, INTEGER_TAG_CODE);
    return len;
}  /* BEncUAsnInt */


/*
 * decodes universal TAG LENGTH and Contents of and ASN.1 INTEGER
 */
void
BDecUAsnInt PARAMS ((b, result, bytesDecoded, env),
    GenBuf *b _AND_
    UAsnInt *result _AND_
    AsnLen *bytesDecoded _AND_
    jmp_buf env)
{
    AsnTag tag;
    AsnLen elmtLen;

    if ((tag = BDecTag (b, bytesDecoded, env)) != MAKE_TAG_ID (UNIV, PRIM, INTEGER_TAG_CODE))
    {
         Asn1Error ("BDecAsnInt: ERROR wrong tag on INTGER.\n");
         longjmp (env, -40);
    }

    elmtLen = BDecLen (b, bytesDecoded, env);
    BDecUAsnIntContent (b, tag, elmtLen, result, bytesDecoded, env);

}  /* BDecUAsnInt */


/*
 * encodes unsigned longeger.  This allows you to correctly
 * handle unsiged values that used the most significant (sign) bit.
 */
AsnLen
BEncUAsnIntContent PARAMS ((b, data),
    GenBuf *b _AND_
    UAsnInt *data)
{
    int             len;
    int             i;
    unsigned long   mask;
    unsigned long   dataCpy;

    dataCpy = *data;

    /*
     * calculate encoded length of the integer (content)
     */
    mask = INT_MASK;
    if ((long)dataCpy < 0)
    {
        /*write integer as normal (remember writing in reverse) */
        for (i = 0; i < sizeof (UAsnInt); i++)
        {
            BufPutByteRvs (b, (unsigned char)dataCpy);
            dataCpy >>= 8;
        }
        /*
         * write zero byte at beginning of int, since high bit
         * is set and need to differentiate between sign
         * bit and high bit in unsigned case.
         * (this code follows the prev for loop since writing
         *  in reverse)
         */
        BufPutByteRvs (b, 0);

        return sizeof (UAsnInt)+1;
    }
    else
    {
        for (len = sizeof (UAsnInt); len > 1; --len)
        {
            if ((dataCpy & mask) == 0)
                mask >>= 8;
            else
                break;
        }

        /* write the BER integer */
        for (i = 0; i < len; i++)
        {
            BufPutByteRvs (b, (unsigned char)dataCpy);
            dataCpy >>= 8;
        }
        return len;
    }

}  /* BEncUAsnIntContent */


/*
 * decode integer portion - no tag or length expected or decoded
 * assumes unsigned integer - This routine is useful for
 * integer subtyped to > 0 eg Guage ::= INTEGER (0..4294967295)
 */
void
BDecUAsnIntContent PARAMS ((b, tag, len, result, bytesDecoded, env),
    GenBuf *b _AND_
    AsnTag   tag ESNACC_UNUSED _AND_
    AsnLen   len _AND_
    UAsnInt *result _AND_
    AsnLen  *bytesDecoded _AND_
    jmp_buf  env)
{
    int   i;
    unsigned long retVal;

    retVal =  (unsigned long) BufGetByte (b);

    if (len > (sizeof (UAsnInt)+1))
    {
        Asn1Error ("BDecUAsnIntContent: ERROR - integer to big to decode.\n");
        longjmp (env, -9);
    }
    else if (retVal & 0x80)   /* top bit of first byte is sign bit */
    {
        Asn1Error ("BDecUAsnIntContent: ERROR - integer is negative.\n");
        longjmp (env, -10);
    }
    else if ((len == (sizeof (UAsnInt)+1)) && (retVal != 0))
    {
        /*
         * first octet must be zero 5 octets long - extra 0 octet
         * at beginning is only used for value > 0 that need the
         * high bit
         */
        Asn1Error ("BDecUAsnIntContent: ERROR - integer is negative.\n");
        longjmp (env, -11);
    }

    /*
     * write from buffer into long int
     */
    for (i = 1; i < (int)len; i++)
        retVal = (retVal << 8) | (unsigned long)(BufGetByte (b));

    if (BufReadError (b))
    {
        Asn1Error ("BDecUIntegerContent: ERROR - decoded past end of data\n");
        longjmp (env, -12);
    }
    (*bytesDecoded) += len;

    *result = retVal;
}  /* BDecUAsnIntContent */


void
PrintUAsnInt PARAMS ((f, v, indent),
    FILE *f _AND_
    UAsnInt *v _AND_
    unsigned int indent ESNACC_UNUSED)
{
    fprintf(f, "%u", *v);
}


