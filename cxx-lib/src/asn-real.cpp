// file: .../c++-lib/src/asn-real.C - AsnReal (ASN.1 REAL) type
//
//  Mike Sample
//  92/07/02
// Copyright (C) 1992 Michael Sample and the University of British Columbia
//
// This library is free software; you can redistribute it and/or
// modify it provided that this copyright/license information is retained
// in original form.
//
// If you modify this file, you must clearly indicate your changes.
//
// This source code is distributed in the hope that it will be
// useful, but WITHOUT ANY WARRANTY; without even the implied warranty
// of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
//

// $Log: asn-real.cpp,v $
// Revision 1.21  2004/03/22 20:04:18  gronej
// took IBM references out of the code (to the best of our knowledge, we don't use any of it anymore)
//
// Revision 1.20  2004/02/11 19:08:48  nicholar
// Updated Print() function so no longer uses global indent
//
// Revision 1.19  2004/02/06 00:39:13  nicholar
// Changed AsnList to use std::list<T> instead of List<T>
//
// Revision 1.18  2004/02/04 14:59:27  gronej
// Fixed a TON of memory leaks
//
// Revision 1.17  2004/02/03 14:44:32  gronej
// made all constraint lists static to avoid memory issues
//
// Revision 1.16  2003/12/17 19:05:03  gronej
// SNACC baseline merged with PER v1_7 tag
//

// Revision 1.14.2.5  2003/12/04 20:47:11  gronej
// Moved bAlign out of all PEnc calls and into AsnBufBits as a member
// An AsnBufBits is now invoked with a bAlign parameter defaulted to false
//
// Revision 1.14.2.4  2003/12/03 19:48:08  gronej
// Fixed bitsDecoded to return correct value on decode operations
//
// Revision 1.14.2.3  2003/11/04 14:21:21  gronej
// Update PER compiler with some PERGeneral functionality  11/04/03
//
// Revision 1.14.2.2  2003/10/22 12:45:58  gronej
// Updating PER compiler
//
// Revision 1.14.2.1  2003/10/02 17:15:24  gronej
// Updating PER compiler
//

// Revision 1.14  2003/01/17 01:16:04  leonberp
// FIXED A TON of warnings
//
// Revision 1.13  2003/01/06 16:20:07  leonberp
// Changed BDec() and BDecContent() to use const AsnBufs
//
// Revision 1.12  2002/12/17 20:27:40  leonberp
// made BEnc() and BEncContent() const
//
// Revision 1.11  2002/10/23 21:02:48  leonberp
// fixed AsnBuf references and fixed clock skew problem
//
// Revision 1.10  2002/10/23 10:51:10  mcphersc
// Changed BUF_TYPE to AsnBuf
//
// Revision 1.9  2002/05/10 16:39:36  leonberp
// latest changes for release 2.2
// includes integrating asn-useful into C & C++ runtime library, the compiler changes that go along with that, SnaccException changes for C++ runtime and compiler
//
// Revision 1.8  2002/03/01 14:03:37  vracarl
// added an INDEFINATE_LEN check on the primitive
//
// Revision 1.7  2001/08/29 22:04:19  leonberp
// enchanced Clone() to allocate a new pointe AND COPY the object
//
// Revision 1.6  2001/07/12 19:33:39  leonberp
// Changed namespace to SNACC and added compiler options: -ns and -nons.  Also removed dead code.
//
// Revision 1.5  2001/06/28 15:43:32  rwc
// ADDED "SNACCASN" namespace definition to all SNACC data structures.
// This should not affect most applications since we do not have any name
// conflicts.
// ALSO, combined all ASN primitive data type includes into asn-incl.h.
//
// Revision 1.4  2001/06/19 15:19:47  grafb
// Re-ordered includes and removed redundant includes for g++ 3.0 compile
// Also ifdef-ed out two macro definitions in asn-real.cpp due to conflict
// with this compiler version.
//
// Revision 1.3  2001/06/18 17:47:44  rwc
// Updated to reflect newly added C++ Exception error handling, instead of "C" longjmp and setjmp calls.
// Changes made to both the compiler and the SNACC C++ run-time library.
//
// Revision 1.2  2000/10/16 18:10:37  rwc
// removed most warnings from C++-lib, some C-lib.
//
// Revision 1.1.1.1  2000/08/21 20:36:09  leonberp
// First CVS Version of SNACC.
//
// Revision 1.7  1997/02/28 13:39:46  wan
// Modifications collected for new version 1.3: Bug fixes, tk4.2.
//
// Revision 1.6  1995/08/17 15:27:19  rj
// recognize and return "±inf" for PLUS-INFINITY/MINUS-INFINITY.
//
// Revision 1.5  1995/07/24  20:29:24  rj
// #if TCL ... #endif wrapped into #if META ... #endif
//
// call constructor with additional pdu and create arguments.
//
// changed `_' to `-' in file names.
//
// Revision 1.4  1995/02/18  17:01:49  rj
// denote a long if we want a long.
// make the code work on little endian CPUs.
// ported to work with CPU/compiler combinations providing 64 bit longs.
//
// Revision 1.3  1994/10/08  04:18:29  rj
// code for meta structures added (provides information about the generated code itself).
//
// code for Tcl interface added (makes use of the above mentioned meta code).
//
// virtual inline functions (the destructor, the Clone() function, BEnc(), BDec() and Print()) moved from inc/*.h to src/*.C because g++ turns every one of them into a static non-inline function in every file where the .h file gets included.
//
// made Print() const (and some other, mainly comparison functions).
//
// several `unsigned long int' turned into `size_t'.
//
// Revision 1.2  1994/08/28  10:01:18  rj
// comment leader fixed.
//
// Revision 1.1  1994/08/28  09:21:07  rj
// first check-in. for a list of changes to the snacc-1.1 distribution please refer to the ChangeLog.

#include "asn-incl.h"
#include "math.h"

#include <stdlib.h>
#include <limits>

_BEGIN_SNACC_NAMESPACE

static double AsnPlusInfinity();
static double AsnMinusInfinity();

/*
 * Declare and init the PLUS and MINUS INFINITY values.
 *
 */
const AsnReal PLUS_INFINITY(AsnPlusInfinity());
const AsnReal MINUS_INFINITY(AsnMinusInfinity());

#define ENC_PLUS_INFINITY   0x40
#define ENC_MINUS_INFINITY  0x41

#define REAL_BINARY         0x80
#define REAL_SIGN           0x40
#define REAL_EXPLEN_MASK    0x03
#define REAL_EXPLEN_1       0x00
#define REAL_EXPLEN_2       0x01
#define REAL_EXPLEN_3       0x02
#define REAL_EXPLEN_LONG    0x03
#define REAL_FACTOR_MASK    0x0c
#define REAL_BASE_MASK      0x30
#define REAL_BASE_2         0x00
#define REAL_BASE_8         0x10
#define REAL_BASE_16        0x20

// Returns the smallest octet length needed to hold the given long int value
static unsigned int SignedIntOctetLen(long int val)
{
    unsigned long int mask = (0x7f80L << ((sizeof (long int) - 2) * 8));
    unsigned int retVal = sizeof (long int);

    if (val < 0)
        val = val ^ (~0L);  /* XOR val with all 1's */

    while ((retVal > 1) && ((val & mask) == 0)) {
        mask >>= 8;
        retVal--;
    }

    return retVal;
}

// Returns the PLUS INFINITY in double format
// this assumes you have the IEEE functions in
// the math lib
static double AsnPlusInfinity()
{
    return std::numeric_limits<float>::infinity();
}

static double AsnMinusInfinity()
{
    return -AsnPlusInfinity();
}

static bool iszero(double v)
{
    return (fpclassify(v) == FP_ZERO);
}

// This routine uses the ieee library routines to encode
// this AsnReal's double value
AsnLen AsnReal::BEncContent (AsnBuf &b) const
{
    AsnLen encLen;
    double mantissa;
    double tmpMantissa;
    unsigned int truncatedMantissa;
    int exponent;
    size_t  i, mantissaLen = 1, expLen;
    unsigned char buf[sizeof (double)];
    unsigned char firstOctet;

    /* no contents for 0.0 reals */
    if (iszero(value))
        return 0;

    /* special real values for +/- oo */
    if (isinf (value)) {
        if (signbit (value)) /* neg */
            b.PutByteRvs (ENC_MINUS_INFINITY);
        else
            b.PutByteRvs (ENC_PLUS_INFINITY);
        encLen = 1;
    } else {
        exponent = ilogb(value);

        /* get the absolute value of the mantissa (subtract 1 to make < 1) */
        mantissa = scalbn (fabs (value), -exponent-1);
        tmpMantissa = mantissa;

        /* convert mantissa into an unsigned integer */
        for (i = 0; i < sizeof (double); i++) {
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
        if (signbit (value))
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
        expLen = SignedIntOctetLen(exponent);

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
        b.PutSegRvs ((char*)buf, mantissaLen);

        /* write the exponent */
        for (i = expLen; i > 0; i--) {
            b.PutByteRvs (exponent);
            exponent >>= 8;
        }

        /* write the exponents length if nec */
        if (expLen > 3) {
            encLen++;
            b.PutByteRvs (expLen);
        }

        /* write the format octet */
        b.PutByteRvs (firstOctet);
    }
    return encLen;

}  /*  AsnReal::BEncContent */


static double domainExp(double i, int j)
{
    double exp = 1.0;

    while(j--)
    {
        exp *= 2.0;
    }

    return i * exp;
}

// Decode a REAL value's content from the given buffer.
// places the result in this object.
void AsnReal::BDecContent (const AsnBuf &b, AsnTag /* tagId */, AsnLen elmtLen, AsnLen &bytesDecoded)
{
    FUNC("AsnReal::BDecContent()");

    unsigned char firstOctet;
    unsigned char firstExpOctet;
    int i;
    unsigned int expLen;
    double mantissa;
    unsigned int baseF;
    int exponent = 0;

    if (elmtLen == 0)
    {
        value = 0.0;
        return;
    }
    else if (elmtLen == INDEFINITE_LEN)
       throw EXCEPT("indefinite length on primitive", DECODE_ERROR);
    firstOctet = b.GetByte();
    if (elmtLen == 1)
    {
        bytesDecoded += 1;
        if (firstOctet == ENC_PLUS_INFINITY)
            value = PLUS_INFINITY;
        else if (firstOctet == ENC_MINUS_INFINITY)
            value = MINUS_INFINITY;
        else
        {
            throw EXCEPT("unrecognized 1 octet length real number", DECODE_ERROR);
        }
    }
    else
    {
        if (firstOctet & REAL_BINARY)
        {
            firstExpOctet = b.GetByte();
            if (firstExpOctet & 0x80)
                exponent = -1;
            switch (firstOctet & REAL_EXPLEN_MASK)
            {
                case REAL_EXPLEN_1:
                    expLen = 1;
                    exponent =  (exponent << 8) | firstExpOctet;
                    break;

                case REAL_EXPLEN_2:
                    expLen = 2;
                    exponent =  (exponent << 16) | (((unsigned long int) firstExpOctet) << 8) | b.GetByte();
                    break;

                case REAL_EXPLEN_3:
                    expLen = 3;
                    exponent =  (exponent << 16) | (((unsigned long int) firstExpOctet) << 8) | b.GetByte();
                    exponent =  (exponent << 8) | b.GetByte();
                    break;

                default:  /* long form */
                    expLen = firstExpOctet +1;
                    i = firstExpOctet-1;
                    firstExpOctet =  b.GetByte();
                    if (firstExpOctet & 0x80)
                        exponent = (~0UL << 8) | firstExpOctet;
                    else
                        exponent = firstExpOctet;
                    for (;i > 0; firstExpOctet--, i--)
                        exponent = (exponent << 8) | b.GetByte();
                    break;
            }

            unsigned char cValue;
            mantissa = 0.0;
            for (i = 1 + expLen; i < (int)elmtLen; i++)
            {
                cValue = b.GetByte();
                
                mantissa = domainExp(mantissa, 8) + cValue;
            }

            switch (firstOctet & REAL_BASE_MASK)
            {
            case REAL_BASE_2:
                baseF = 1;
                break;
                
            case REAL_BASE_8:
                baseF = 3;
                break;
                
            case REAL_BASE_16:
                baseF = 4;
                break;
                
            default:
                throw EXCEPT("unsupported base for a binary real number.", DECODE_ERROR);
                break;
                
            }

            unsigned int scaleF = 1<<((firstOctet & REAL_FACTOR_MASK) >> 2);

            //std::cout << "( " << pow(2,baseF) << "," << mantissa << "," << exponent << ") * " << double(scaleF) << " * " << ((firstOctet & REAL_SIGN) ? "-1.0" : "1.0") << std::endl;

            value = mantissa * pow(double(2.0), double(baseF) * double(exponent));
            value *= scaleF;
            
            if (firstOctet & REAL_SIGN)
                value = -value;
            
            bytesDecoded += elmtLen;
        }
        else /* decimal version */
        {
            throw EXCEPT("decimal REAL form is not currently supported" , DECODE_ERROR);
        }
    }
} /* AsnInt::BDecContent */

AsnLen AsnReal::PEnc (AsnBufBits &b) const
{
    AsnLen len=0;
    long templen = 0;
    AsnBuf tempBuf;
    char* seg = NULL;

    templen += BEncContent(tempBuf);
    seg = new char[templen + 1];
    tempBuf.GetSeg(seg, templen);
    len += PEncDefLenTo127(b, templen);

    if (templen > 0) {
        templen *= 8;
        len += b.OctetAlignWrite();
        len += b.PutBits((unsigned char*) seg, templen);
    }

    delete[] seg;
    return len;
}


void AsnReal::PDec (AsnBufBits &b, AsnLen &bitsDecoded)
{
    AsnBuf tempBuf;
    AsnLen bytesDecoded = 0;
    unsigned char* seg;
    unsigned long lseg;

    seg = b.GetBits(8);
    lseg = (unsigned long)seg[0];
    bitsDecoded += 8 + b.OctetAlignRead();
    delete [] seg;

    seg = b.GetBits(lseg * 8);
    tempBuf.PutSegRvs((char*)seg, lseg);

    BDecContent(tempBuf, MAKE_TAG_ID(UNIV, PRIM, REAL_TAG_CODE),
                lseg, bytesDecoded);

    bitsDecoded += (bytesDecoded * 8);
    delete [] seg;
}


AsnLen AsnReal::BEnc (AsnBuf &b) const
{
    AsnLen l;
    l =  BEncContent(b);
    l += BEncDefLen(b, l);
    l += BEncTag1(b, UNIV, PRIM, REAL_TAG_CODE);
    return l;
}

void AsnReal::BDec (const AsnBuf &b, AsnLen &bytesDecoded)
{
    FUNC("AsnReal::BDec()");

    AsnLen elmtLen;
    AsnTag tagId;

    tagId = BDecTag(b, bytesDecoded);
    if (tagId != MAKE_TAG_ID(UNIV, PRIM, REAL_TAG_CODE)) {
        throw InvalidTagException(typeName(), tagId, STACK_ENTRY);
    }

    elmtLen = BDecLen(b, bytesDecoded);
    BDecContent(b, MAKE_TAG_ID(UNIV, PRIM, REAL_TAG_CODE),
                elmtLen, bytesDecoded);
}

void AsnReal::Print(std::ostream& os, unsigned short /*indent*/) const
{
    os << value;
}

void AsnReal::PrintXML (std::ostream &os, const char *lpszTitle) const 
{
    const char *tagName = typeName();
    if (lpszTitle) tagName = lpszTitle;
    os << "<" << tagName << ">\n";
    Print(os);
    os << "</" << tagName << ">\n";
}


char* AsnReal::checkRealValRange(const double m_Lower, const double m_Upper) const
{
    double ltemp = value;
    char* pError = NULL;
    char cTmperr[200];

    if (ltemp > m_Upper) {
        sprintf(cTmperr, "_______\nREAL--Valuerange Constraints:\n_______\nError: --Value out of range--\nValue: %.5f is above the Upper Limit: %.5f \n", ltemp, m_Upper);
        pError = strdup(cTmperr);
    } else if (ltemp < m_Lower) {
        sprintf(cTmperr, "_______\nREAL--Valuerange Constraints:\n_______\nError: --Value out of range--\nValue: %.5f is below the Lower Limit: %.5f \n", ltemp, m_Lower);
        pError = strdup(cTmperr);
    }

    return pError;
}

char* AsnReal::checkRealSingleVal(const double m_SingleVal) const
{
    double ltemp = value;
    char* pError = NULL;
    char cTmperr[200];

    if (ltemp != m_SingleVal) {
        sprintf(cTmperr, "_______\nREAL--SingleValue Constraints:\n_______\nError: --Values must match--\nValue: %.5f is not equal to the Constraint Single Value:  %.5f \n", ltemp, m_SingleVal);
        pError = strdup(cTmperr);
   }

   return pError;
}

_END_SNACC_NAMESPACE
