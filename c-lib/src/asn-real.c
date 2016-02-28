/*
 * asn_real.c - BER encode, decode, print and free routines for ASN.1 REAL type.
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
 * $Header: /baseline/SNACC/c-lib/src/asn-real.c,v 1.16 2004/01/22 20:03:12 nicholar Exp $
 * $Log: asn-real.c,v $
 * Revision 1.16  2004/01/22 20:03:12  nicholar
 * Changed indent variable to an unsigned int from an unsigned short.  Indent is  used in library print functions and generated print functions
 *
 * Revision 1.15  2003/12/17 19:05:03  gronej
 * SNACC baseline merged with PER v1_7 tag
 *
 * Revision 1.14.2.1  2003/11/05 14:58:53  gronej
 * working PER code merged with esnacc_1_6
 *
 * Revision 1.14  2002/10/23 10:23:37  mcphersc
 * Changed BUF_TYPE to AsnBuf
 *
 * Revision 1.13  2002/10/21 17:13:16  mcphersc
 * fixed long int
 *
 * Revision 1.12  2002/10/21 17:05:56  mcphersc
 * fixed long int
 *
 * Revision 1.11  2002/10/21 17:01:29  mcphersc
 * fixed unsigned short int
 *
 * Revision 1.10  2002/10/18 13:09:32  mcphersc
 * took out long int to unsigned long
 *
 * Revision 1.9  2002/10/01 14:14:24  mcphersc
 * ASN "C" Buf modifications
 *
 * Revision 1.8  2002/09/25 12:11:00  mcphersc
 * fixed warnings
 *
 * Revision 1.7  2002/09/03 19:39:19  vracarl
 * got rid of c++ comments
 *
 * Revision 1.6  2002/07/25 10:51:28  sfl
 * Got rid of compiler warnings about indent and missint include reference to math.h
 *
 * Revision 1.4  2001/07/20 11:54:34  mcphersc
 * Get rid of compiler warning on windows for function  prototype
 * extern double frexp( double x, int *expptr )
 *
 * Revision 1.3  2000/10/24 14:54:43  rwc
 * Updated to remove high-level warnings (level 4 on MSVC++) for an easier build.
 * SOME warnings persist due to difficulty in modifying the SNACC compiler to
 * properly build clean source; also some files are built by Lex/Yacc.
 *
 * Revision 1.2  2000/10/16 18:10:40  rwc
 * removed most warnings from C++-lib, some C-lib.
 *
 * Revision 1.1.1.1  2000/08/21 20:35:52  leonberp
 * First CVS Version of SNACC.
 *
 * Revision 1.5  1997/02/28 13:39:50  wan
 * Modifications collected for new version 1.3: Bug fixes, tk4.2.
 *
 * Revision 1.4  1995/07/24 21:04:54  rj
 * changed `_' to `-' in file names.
 *
 * Revision 1.3  1995/02/18  16:25:13  rj
 * added support for CPU/compiler combination presenting 64 bit little endian long integers
 * (in addition to the aforesupported 32 bit big endian long ints).
 *
 * Revision 1.2  1994/09/01  00:06:28  rj
 * reduce the risk of unwanted surprises with macro expansion by properly separating the C tokens.
 *
 * Revision 1.1  1994/08/28  09:46:00  rj
 * first check-in. for a list of changes to the snacc-1.1 distribution please refer to the ChangeLog.
 *
 */

#include "math.h"
#include "asn-config.h"
#include "asn-len.h"
#include "asn-tag.h"
#include "asn-real.h"

double pow PROTO ((double base, double exp));
#ifdef WIN32
/* Get rid of frexp warning on windows */
//extern double frexp( double x, int *expptr );
#endif

#ifdef IEEE_REAL_LIB
/* ieee functions (in case not in math.h)*/
extern int iszero (double);
extern int isinf (double);
extern int signbit (double);
extern int ilogb (double);
extern double scalbn (double, int);
#endif


/*
 *  You must call InitAsnInfinity() to initailize these values
 *  (necessary if you deal with REAL values.)
 */
AsnReal PLUS_INFINITY;
AsnReal MINUS_INFINITY;


#define ENC_PLUS_INFINITY 0x40
#define ENC_MINUS_INFINITY 0x41

#define REAL_BINARY 0x80
#define REAL_SIGN   0x40
#define REAL_EXPLEN_MASK 0x03
#define REAL_EXPLEN_1 0x00
#define REAL_EXPLEN_2 0x01
#define REAL_EXPLEN_3 0x02
#define REAL_EXPLEN_LONG 0x03
#define REAL_FACTOR_MASK 0x0c
#define REAL_BASE_MASK 0x30
#define REAL_BASE_2 0x00
#define REAL_BASE_8 0x10
#define REAL_BASE_16 0x20


/*
 * Returns the smallest octet length needed to
 * hold the given long int value
 */
unsigned long
SignedIntOctetLen PARAMS ((val),
    long  val)
{
    unsigned long mask = (0x7f80L << ((sizeof (unsigned long ) - 2) * 8));
    unsigned int retVal = sizeof (long);

    if (val < 0)
        val = val ^ (~0L);  /* XOR val with all 1's */

    while ((retVal > 1) && ((val & mask) == 0))
    {
        mask >>= 8;
        retVal--;
    }

    return retVal;

} /* SignedIntOctetLen */


/*
 * encodes universal TAG LENGTH and Contents of and ASN.1 REAL
 */
AsnLen
BEncAsnReal PARAMS ((b, data),
    GenBuf *b _AND_
    AsnReal *data)
{
    AsnLen len;

    len =  BEncAsnRealContent (b, data);
    len += BEncDefLen (b, len);
    len += BEncTag1 (b, UNIV, PRIM, REAL_TAG_CODE);
    return len;
}  /* BEncAsnReal */


/*
 * decodes universal TAG LENGTH and Contents of and ASN.1 REAL
 */
void
BDecAsnReal PARAMS ((b, result, bytesDecoded, env),
    GenBuf *b _AND_
    AsnReal *result _AND_
    AsnLen *bytesDecoded _AND_
    jmp_buf env)
{
    AsnTag tag;
    AsnLen elmtLen;

    if ((tag = BDecTag (b, bytesDecoded, env)) != MAKE_TAG_ID (UNIV, PRIM, REAL_TAG_CODE))
    {
         Asn1Error ("BDecAsnReal: ERROR wrong tag on REAL.\n");
         longjmp (env, -40);
    }

    elmtLen = BDecLen (b, bytesDecoded, env);
    BDecAsnRealContent (b, tag, elmtLen, result, bytesDecoded, env);

}  /* BDecAsnReal */



#ifdef IEEE_REAL_FMT

/*
 * Inits the PLUS_INFINITY and MINUS_INFINITY globals assuming
 * that the double is an IEEE DOUBLE
 * The bits for MINUS_INFINITY are 0xfff0000000000000
 * The bits for PLUS_INFINITY are  0x7ff0000000000000
 */
void
InitAsnInfinity()
{
    unsigned char *c = (unsigned char *)&PLUS_INFINITY;
    int i;

    if (sizeof (double) != 8)
        Asn1Error ("InitAsnInfinity: ERROR expected sizeof (AsnReal) to be 8");

#if WORDS_BIGENDIAN				/* Big endian */
    c[0] = 0x7f;
    c[1] = 0xf0;
    for (i = 2; i < sizeof (double); i++)
        c[i] = 0;
#else						/* Little endian */
    c[7] = 0x7f;
    c[6] = 0xf0;
    for (i = 0; i < 6; i++)
        c[i] = 0;
#endif

    MINUS_INFINITY = -PLUS_INFINITY;
} /* InitAsnInfinity */

/*
 * Encodes the content of an ASN.1 REAL value to the given buffer.
 * This version of the routine ASSUMES that the C rep. of a double
 * is the same as the IEEE std.
 */
AsnLen
BEncAsnRealContent PARAMS ((b, value),
    GenBuf  *b _AND_
    AsnReal  *value)
{
    int	exponent;
    int isNeg;
#if SIZEOF_LONG == 4
    unsigned char *dbl;
    unsigned long *first4;
    unsigned long *second4;
#else
#if SIZEOF_LONG == 8
    unsigned long mantissa, val, *p;
    int i;
#endif
#endif

    /* no contents for 0.0 reals */
    if (*value == 0.0) /* all bits zero, disregarding top/sign bit */
        return 0;

#if SIZEOF_LONG == 4
#if !WORDS_BIGENDIAN
  #error sorry! this 32 bit code requires big endianess.
#endif

    /* this code is designed to work were longs are 32 bit wide and big endian */

    dbl = (unsigned char *) value;

    first4 = (unsigned long *) dbl;
    second4 = (unsigned long *) (dbl + sizeof (long ));

    isNeg = dbl[0] & 0x80;

    /* special real values for +/- oo */
    if (((*first4 & 0x7fffffff) == 0x7ff00000) && (*second4 == 0))
    {
        if (isNeg)
	    {
            BufPutByteRvs (b, ENC_MINUS_INFINITY);
	    }
        else
	    {
            BufPutByteRvs (b, ENC_PLUS_INFINITY);
	    }
        return 1;
    }
    else  /* encode a binary real value */
    {
        exponent = (((*first4) >> 20) & 0x07ff);

        /* write the mantissa (N value) */
        BufPutSegRvs (b, (char *)(dbl+2), sizeof (double)-2);

        /*
         * The rightmost 4 bits of a double 2nd octet are the
         * most sig bits of the mantissa.
         * write the most signficant byte of the asn1 real mantissa,
         * adding implicit bit to 'left of decimal' if not de-normalized
         * (de normalized if exponent == 0)
         *
         * if the double is not in de-normalized form subtract 1023
         * from the exponent to get proper signed exponent.
         *
         * for both the normalized and de-norm forms
         * correct the exponent by subtracting 52 since:
         *   1. mantissa is 52 bits in the double (56 in ASN.1 REAL form)
         *   2. implicit decimal at the beginning of double's mantissa
         *   3. ASN.1 REAL's implicit decimal is after its mantissa
         * so converting the double mantissa to the ASN.1 form has the
         * effect of multiplying it by 2^52. Subtracting 52 from the
         * exponent corrects this.
         */
        if (exponent == 0) /* de-normalized - no implicit 1 to left of dec.*/
        {
            BufPutByteRvs (b, dbl[1] & 0x0f);
            exponent -= 52;
        }
        else
        {
            BufPutByteRvs (b, (dbl[1] & 0x0f) | 0x10); /* 0x10 adds implicit bit */
            exponent -= (1023 + 52);
        }

#else
#if SIZEOF_LONG == 8
#if WORDS_BIGENDIAN
  #error sorry! this 64 bit code requires little endianess.
#endif

    /* this code is designed to work on Alpha under OSF/1 (64 bit longs, little endian) */

    p = (unsigned long *) value;
    val = *p;

    isNeg = (val >> 63) & 1;
    /* special real values for +/- oo */
    if (!finite (*value))
    {
        if (isNeg)
	    {
            BufPutByteRvs (b, ENC_MINUS_INFINITY);
	    }
        else
	    {
            BufPutByteRvs (b, ENC_PLUS_INFINITY);
	    }
        return 1;
    }
    else /* encode a binary real value */
    {
	exponent = (val >> 52) & 0x7ff;
	mantissa = (val & 0xfffffffffffffL) | 0x10000000000000L;

	for (i = 0; i < 7; i++)
	{
	    BufPutByteRvs (b, mantissa & 0xff);
	    mantissa >>= 8;
        }
        exponent -= (1023 + 52);

#else
  #error long neither 8 nor 4 bytes in size?
#endif
#endif

        /*  write the exponent  */
        BufPutByteRvs (b, exponent & 0xff);
        BufPutByteRvs (b, exponent >> 8);

        /* write format octet */
        /* bb is 00 since base is 2 so do nothing */
        /* ff is 00 since no other shifting is nec */
        if (isNeg)
	    {
            BufPutByteRvs (b, REAL_BINARY | REAL_EXPLEN_2 | REAL_SIGN);
	    }
        else
	    {
            BufPutByteRvs (b, REAL_BINARY | REAL_EXPLEN_2);
	    }

        return sizeof (double) + 2;
    }

    /* not reached */

} /* BEncAsnRealContent */

#else  /* IEEE_REAL_FMT not def */

#ifdef IEEE_REAL_LIB


/*
 * Inits the PLUS_INFINITY and MINUS_INFINITY globals assuming
 * that the ieee_values library is present
 */
void
InitAsnInfinity()
{
    PLUS_INFINITY = infinity();
    MINUS_INFINITY = -PLUS_INFINITY;
} /* InitAsnInfinity */

/*
 * Encodes the content of an ASN.1 REAL value to the given buffer.
 * This version of the routine does not assume an IEEE double rep.
 * ieee library conversion routine are used instead.
 */
AsnLen
BEncAsnRealContent PARAMS ((b, value),
    GenBuf *b _AND_
    AsnReal  *value)
{
    unsigned long encLen;
    double mantissa;
    double tmpMantissa;
    unsigned int truncatedMantissa;
    int	exponent;
    unsigned int expLen;
    int	sign;
    unsigned char buf[sizeof (double)];
    int i, mantissaLen;
    unsigned char firstOctet;

    /* no contents for 0.0 reals */
    if (iszero (*value))
        return 0;

    /* special real values for +/- oo */
    if (isinf (*value))
    {
        if (signbit (*value)) /* neg */
            BufPutByteRvs (b, ENC_MINUS_INFINITY);
        else
            BufPutByteRvs (b, ENC_PLUS_INFINITY);
        encLen = 1;
    }
    else  /* encode a binary real value */
    {
        if (signbit (*value))
            sign = -1;
        else
            sign = 1;

        exponent =  ilogb (*value);

        /* get the absolute value of the mantissa (subtract 1 to make < 1) */
        mantissa = scalbn (fabs (*value), -exponent-1);


        tmpMantissa = mantissa;

        /* convert mantissa into an unsigned integer */
        for (i = 0; i < sizeof (double); i++)
        {
            /* normalizied so shift 8 bits worth to the left of the decimal */
            tmpMantissa *= (1<<8);

            /* grab only (octet sized) the integer part */
            truncatedMantissa = (unsigned int) tmpMantissa;

            /* remove part to left of decimal now for next iteration */
            tmpMantissa -= truncatedMantissa;

            /* write into tmp buffer */
            buf[i] = truncatedMantissa;

            /* keep track of last non zero octet so can zap trailing zeros */
            if (truncatedMantissa)
                mantissaLen = i+1;
        }

        /*
         * write format octet  (first octet of content)
         *  field  1 S bb ff ee
         *  bit#   8 7 65 43 21
         *
         * 1 in bit#1 means binary rep
         * 1 in bit#2 means the mantissa is neg, 0 pos
         * bb is the base:    65  base
         *                    00    2
         *                    01    8
         *                    10    16
         *                    11    future ext.
         *
         * ff is the Value of F where  Mantissa = sign x N x 2^F
         *    FF can be one of 0 to 3 inclusive. (used to save re-alignment)
         *
         * ee is the length of the exponent:  21   length
         *                                    00     1
         *                                    01     2
         *                                    10     3
         *                                    11     long form
         *
         *
         * encoded binary real value looks like
         *
         *     fmt oct
         *   --------------------------------------------------------
         *   |1Sbbffee|  exponent (2's comp)  |   N (unsigned int)  |
         *   --------------------------------------------------------
         *    87654321
         */
        firstOctet = REAL_BINARY;
        if (signbit (*value))
            firstOctet |= REAL_SIGN;

        /* bb is 00 since base is 2 so do nothing */
        /* ff is 00 since no other shifting is nec */

        /*
         * get exponent calculate its encoded length
         * Note that the process of converting the mantissa
         * double to an int shifted the decimal mantissaLen * 8
         * to the right - so correct that here
         */
        exponent++; /* compensate for trick to put mantissa < 1 */
        exponent -= (mantissaLen * 8);
        expLen = SignedIntOctetLen (exponent);

        switch (expLen)
        {
            case 1:
                firstOctet |= REAL_EXPLEN_1;
                break;
            case 2:
                firstOctet |= REAL_EXPLEN_2;
                break;
            case 3:
                firstOctet |= REAL_EXPLEN_3;
                break;
            default:
                firstOctet |= REAL_EXPLEN_LONG;
                break;
        }

        encLen = mantissaLen + expLen + 1;

        /* write the mantissa (N value) */
        BufPutSegRvs (b, (char*)buf, mantissaLen);

        /* write the exponent */
        for (i = expLen; i > 0; i--)
        {
            BufPutByteRvs (b, exponent);
            exponent >>= 8;
        }

        /* write the exponents length if nec */
        if (expLen > 3)
        {
            encLen++;
            BufPutByteRvs (b, expLen);
        }

        /* write the format octet */
        BufPutByteRvs (b, firstOctet);

    }
    return encLen;

}  /*  BEncAsnRealContent */

#else  /* neither IEEE_REAL_FMT or IEEE_REAL_LIB are def */

/*
 * Inits the PLUS_INFINITY and MINUS_INFINITY globals assuming
 * that the double is an IEEE DOUBLE.  This should be changed
 * for the target architecture (if it is not IEEE)
 */
void
InitAsnInfinity()
{
    unsigned char *c;
    int i;

    if (sizeof (double) != 8)
        Asn1Error ("InitAsnInfinity: ERROR expected sizeof (AsnReal) to be 8");

    c = (unsigned char*)&PLUS_INFINITY;
    c[0] = 0x7f;
    c[1] = 0xf0;
    for (i = 2; i < sizeof (double); i++)
        c[i] = 0;

    MINUS_INFINITY = -PLUS_INFINITY;
} /* InitAsnInfinity */

/*
 * Encodes the content of an ASN.1 REAL value to the given buffer.
 * This version of the routine does not assume an IEEE double rep.
 * or the existence of the IEEE library routines.  Uses old style
 * UNIX frexp etc.
 */
AsnLen
BEncAsnRealContent PARAMS ((b, value),
    GenBuf *b _AND_
    AsnReal  *value)
{
    unsigned long encLen;
    double mantissa;
    double tmpMantissa;
    unsigned int truncatedMantissa;
    int	exponent;
    unsigned int expLen;
    int	sign;
    unsigned char buf[sizeof (double)];
    int i, mantissaLen=0;
    unsigned char firstOctet;

    /* no contents for 0.0 reals */
    if (*value == 0.0)
        return 0;

    /* special real values for +/- oo */
    if (*value == MINUS_INFINITY)
    {
        BufPutByteRvs (b, ENC_MINUS_INFINITY);
        encLen = 1;
    }
    else if (*value == PLUS_INFINITY)
    {
        BufPutByteRvs (b, ENC_PLUS_INFINITY);
        encLen = 1;
    }
    else  /* encode a binary real value */
    {
        /*
         * this is what frexp gets from *value
         * *value == mantissa * 2^exponent
         * where 0.5 <= |manitissa| < 1.0
         */
        mantissa = frexp (*value, &exponent);

        /* set sign and make mantissa = | mantissa | */
        if (mantissa < 0.0)
        {
            sign = -1;
            mantissa *= -1;
        }
        else
            sign = 1;


        tmpMantissa = mantissa;

        /* convert mantissa into an unsigned integer */
        for (i = 0; i < sizeof (double); i++)
        {
            /* normalizied so shift 8 bits worth to the left of the decimal */
            tmpMantissa *= (1<<8);

            /* grab only (octet sized) the integer part */
            truncatedMantissa = (unsigned int) tmpMantissa;

            /* remove part to left of decimal now for next iteration */
            tmpMantissa -= truncatedMantissa;

            /* write into tmp buffer */
            buf[i] = (unsigned char)truncatedMantissa;

            /* keep track of last non zero octet so can zap trailing zeros */
            if (truncatedMantissa)
                mantissaLen = i+1;
        }

        /*
         * write format octet  (first octet of content)
         *  field  1 S bb ff ee
         *  bit#   8 7 65 43 21
         *
         * 1 in bit#1 means binary rep
         * 1 in bit#2 means the mantissa is neg, 0 pos
         * bb is the base:    65  base
         *                    00    2
         *                    01    8
         *                    10    16
         *                    11    future ext.
         *
         * ff is the Value of F where  Mantissa = sign x N x 2^F
         *    FF can be one of 0 to 3 inclusive. (used to save re-alignment)
         *
         * ee is the length of the exponent:  21   length
         *                                    00     1
         *                                    01     2
         *                                    10     3
         *                                    11     long form
         *
         *
         * encoded binary real value looks like
         *
         *     fmt oct
         *   --------------------------------------------------------
         *   |1Sbbffee|  exponent (2's comp)  |   N (unsigned int)  |
         *   --------------------------------------------------------
         *    87654321
         */
        firstOctet = REAL_BINARY;
        if (sign == -1)
            firstOctet |= REAL_SIGN;

        /* bb is 00 since base is 2 so do nothing */
        /* ff is 00 since no other shifting is nec */

        /*
         * get exponent calculate its encoded length
         * Note that the process of converting the mantissa
         * double to an int shifted the decimal mantissaLen * 8
         * to the right - so correct that here
         */
        exponent -= (mantissaLen * 8);
        expLen = SignedIntOctetLen (exponent);

        switch (expLen)
        {
            case 1:
                firstOctet |= REAL_EXPLEN_1;
                break;
            case 2:
                firstOctet |= REAL_EXPLEN_2;
                break;
            case 3:
                firstOctet |= REAL_EXPLEN_3;
                break;
            default:
                firstOctet |= REAL_EXPLEN_LONG;
                break;
        }

        encLen = mantissaLen + expLen + 1;

        /* write the mantissa (N value) */
        GenBufPutSegRvs (b, (char*)buf, mantissaLen);

        /* write the exponent */
        for (i = expLen; i > 0; i--)
        {
            BufPutByteRvs (b, (unsigned char)exponent);
            exponent >>= 8;
        }

        /* write the exponents length if nec */
        if (expLen > 3)
        {
            encLen++;
            BufPutByteRvs (b, (unsigned char)expLen);
        }

        /* write the format octet */
        BufPutByteRvs (b, firstOctet);

    }
    return encLen;

}  /*  BEncAsnRealContent */

#endif /* IEEE_REAL_LIB */
#endif /* IEEE_REAL_FMT */



/*
 * Decodes the content of a BER REAL value.
 * This only supports the binary REAL encoding.  The decimal encoding
 * is left as an exercise to the reader.
 */
void
BDecAsnRealContent PARAMS ((b, tagId, len, result, bytesDecoded, env),
    GenBuf *b _AND_
    AsnTag    tagId _AND_
    AsnLen    len _AND_
    AsnReal  *result _AND_
    AsnLen *bytesDecoded _AND_
    jmp_buf env)
{
    unsigned char firstOctet;
    unsigned char firstExpOctet;
    int i;
    unsigned int expLen;
    double mantissa;
    unsigned short base;
    long exponent = 0;

    if (len == 0)
    {
        *result = 0.0;
        return;
    }
    else if (len == INDEFINITE_LEN)
    {
        Asn1Error ("BDecAsnRealContent: ERROR - indefinite length on primitive.\n");
        longjmp (env, -67);
    }

    firstOctet = BufGetByte (b);
    if (len == 1)
    {
        (*bytesDecoded) += 1;
        if (firstOctet == ENC_PLUS_INFINITY)
            *result = PLUS_INFINITY;
        else if (firstOctet == ENC_MINUS_INFINITY)
            *result = MINUS_INFINITY;
        else
        {
            Asn1Error ("BDecAsnRealContent: ERROR - unrecognized real number of length 1 octet.\n");
            longjmp (env, -22);
        }
    }
    else
    {
        if (firstOctet & REAL_BINARY)
        {
            firstExpOctet = BufGetByte (b);
            if (firstExpOctet & 0x80)
                exponent = -1;
            switch (firstOctet & REAL_EXPLEN_MASK)
            {
                case REAL_EXPLEN_1:
                    expLen = 1;
                    exponent =  (exponent << 8)| firstExpOctet;
                    break;

                case REAL_EXPLEN_2:
                    expLen = 2;
                    exponent =  (exponent << 16) |
                        (((unsigned long) firstExpOctet) << 8) |
                        BufGetByte (b);
                    break;

                case REAL_EXPLEN_3:
                    expLen = 3;
                    exponent =  (exponent << 16) |
                        (((unsigned long) firstExpOctet) << 8) |
                            BufGetByte (b);
                    exponent =  (exponent << 8) | BufGetByte (b);
                    break;

                default:  /* long form */
                    expLen = firstExpOctet +1;
                    i = firstExpOctet-1;
                    firstExpOctet =  BufGetByte (b);
                    if (firstExpOctet & 0x80)
                        exponent = (-1 <<8) | firstExpOctet;
                    else
                        exponent = firstExpOctet;
                    for (;i > 0; firstExpOctet--)
                        exponent = (exponent << 8) | BufGetByte (b);
                    break;
            }

            mantissa = 0.0;
            for (i = 1 + expLen; i < (int)len; i++)
            {
                mantissa *= (1<<8);
                mantissa +=  BufGetByte (b);
            }

            /* adjust N by scaling factor */
            mantissa *= (1<<((firstOctet & REAL_FACTOR_MASK) >> 2));

            switch (firstOctet & REAL_BASE_MASK)
            {
                case REAL_BASE_2:
                    base = 2;
                    break;

                case REAL_BASE_8:
                    base = 8;
                    break;

                case REAL_BASE_16:
                    base = 16;
                    break;

                default:
                    Asn1Error ("BDecAsnRealContent: ERROR - unsupported base for a binary real number.\n");
                    longjmp (env, -23);
                    break;

            }

            *result =  mantissa * pow ((double)base, (double)exponent);

            if (firstOctet & REAL_SIGN)
                *result = -*result;

            (*bytesDecoded) += len;
        }
        else /* decimal version */
        {
            Asn1Error ("BDecAsnRealContent: ERROR - decimal REAL form is not currently supported\n");
            longjmp (env, -24);
        }
    }
    tagId = tagId;  /* referenced to avoid compiler warning. */

}  /* BDecAsnRealContent */


/*
 * Prints given REAL value to the given FILE * in ASN.1 Value Notation.
 * indent is ignored.
 */
void
PrintAsnReal PARAMS ((f, v, indent),
    FILE *f _AND_
    AsnReal *v _AND_
    unsigned int indent)
{
    fprintf (f, "%.17E", *v);
    indent = indent; /* referenced to avoid compiler warning. */
}
