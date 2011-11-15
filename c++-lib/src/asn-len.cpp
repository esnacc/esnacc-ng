// file: .../c++-lib/src/asn-len.C - ASN.1 Length manipluation routines
//
// MS 92/06/18
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
// $Header: /baseline/SNACC/c++-lib/src/asn-len.cpp,v 1.18 2004/03/03 20:19:01 gronej Exp $
// $Log: asn-len.cpp,v $
// Revision 1.18  2004/03/03 20:19:01  gronej
// took out readLoc from choice's BDecContent generation, and made new
// put-back logic to put back a tag and a length so you can properly decode
// an unkown any
//
// Revision 1.17  2004/02/09 20:38:50  nicholar
// Updated AsnOid and AsnRelativeOid classes
//
// Revision 1.16  2003/12/17 19:05:03  gronej
// SNACC baseline merged with PER v1_7 tag
//
// Revision 1.15.2.2  2003/11/04 14:21:21  gronej
// Update PER compiler with some PERGeneral functionality  11/04/03
//
// Revision 1.15.2.1  2003/10/22 12:45:58  gronej
// Updating PER compiler
//
// Revision 1.15  2003/01/17 01:16:04  leonberp
// FIXED A TON of warnings
//
// Revision 1.14  2003/01/06 16:20:07  leonberp
// Changed BDec() and BDecContent() to use const AsnBufs
//
// Revision 1.13  2002/11/27 14:29:35  leonberp
// changed to use GetUByte()
//
// Revision 1.12  2002/10/24 14:04:22  leonberp
// fixing support for OCTET STRING CONTAINING
//
// Revision 1.11  2002/10/23 21:02:48  leonberp
// fixed AsnBuf references and fixed clock skew problem
//
// Revision 1.10  2002/10/23 10:51:10  mcphersc
// Changed BUF_TYPE to AsnBuf
//
// Revision 1.9  2002/10/09 19:37:55  leonberp
// new AsnBuf integration
//
// Revision 1.8  2002/10/01 13:43:57  leonberp
// fixed to use new AsnBuf
//
// Revision 1.7  2002/05/10 16:39:35  leonberp
// latest changes for release 2.2
// includes integrating asn-useful into C & C++ runtime library, the compiler changes that go along with that, SnaccException changes for C++ runtime and compiler
//
// Revision 1.6  2002/03/01 14:13:22  vracarl
// fixed incorrect ASN_THROW message text
//
// Revision 1.5  2001/07/12 19:33:37  leonberp
// Changed namespace to SNACC and added compiler options: -ns and -nons.  Also removed dead code.
//
// Revision 1.4  2001/06/28 15:29:47  rwc
// ADDED "SNACCASN" namespace definition to all SNACC data structures.
// This should not affect most applications since we do not have any name
// conflicts.
// ALSO, combined all ASN primitive data type includes into asn-incl.h.
//
// Revision 1.3  2001/06/18 17:47:43  rwc
// Updated to reflect newly added C++ Exception error handling, instead of "C" longjmp and setjmp calls.
// Changes made to both the compiler and the SNACC C++ run-time library.
//
// Revision 1.2  2000/10/16 18:10:37  rwc
// removed most warnings from C++-lib, some C-lib.
//
// Revision 1.1.1.1  2000/08/21 20:36:08  leonberp
// First CVS Version of SNACC.
//
// Revision 1.5  1997/02/16 20:26:04  rj
// check-in of a few cosmetic changes
//
// Revision 1.4  1995/07/24  20:33:15  rj
// changed `_' to `-' in file names.
//
// Revision 1.3  1994/10/08  04:18:24  rj
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
// Revision 1.2  1994/08/28  10:01:13  rj
// comment leader fixed.
//
// Revision 1.1  1994/08/28  09:21:00  rj
// first check-in. for a list of changes to the snacc-1.1 distribution please refer to the ChangeLog.

#include "asn-incl.h"
/*#include "asn-config.h"
#include "asn-len.h"*/

_BEGIN_SNACC_NAMESPACE

/*returns the number of bytes in the encoded length passed in*/
long SNACC::BytesInLen(AsnLen len)
{
    if (len < 128)
    {
        return 1;
    }
    else if (len < 256)
    {
        return 2;
    }
    else if (len < 65536)
    {
        return 3;
    }
    else if (len < 16777126)
    {
        return 4;
    }
    else
    {
        return 5;
    }
}

/*
 * Encodes the given length to the given buffer.
 * returns the number of octets written to the buffer.
 */
AsnLen
BEncDefLen (AsnBuf &b, AsnLen len)
{
    /*
     * unrolled for efficiency
     * (check each possibitlity of the 4 byte integer)
     */
    if (len < 128)
    {
        b.PutByteRvs ((unsigned char)len);
        return 1;
    }
    else if (len < 256)
    {
        b.PutByteRvs ((unsigned char)len);
        b.PutByteRvs ((unsigned char)0x81);
        return 2;
    }
    else if (len < 65536)
    {
        b.PutByteRvs ((unsigned char)len);
        b.PutByteRvs ((unsigned char)(len >> 8));
        b.PutByteRvs ((unsigned char) 0x82);
        return 3;
    }
    else if (len < 16777126)
    {
        b.PutByteRvs ((unsigned char)len);
        b.PutByteRvs ((unsigned char)(len >> 8));
        b.PutByteRvs ((unsigned char)(len >> 16));
        b.PutByteRvs ((unsigned char)0x83);
        return 4;
    }
    else
    {
        b.PutByteRvs ((unsigned char)len);
        b.PutByteRvs ((unsigned char)(len >> 8));
        b.PutByteRvs ((unsigned char)(len >> 16));
        b.PutByteRvs ((unsigned char)(len >> 24));
        b.PutByteRvs ((unsigned char)0x84);
        return 5;
    }
} /*  EncodeDefLen */

/*
 * Decode a BER length from the given buffer. Increments bytesDecoded
 * by the number of octets of the encoded length.  Flags an
 * error if the length is too large or a read error occurs
 */
AsnLen
BDecLen (const AsnBuf &b, AsnLen &bytesDecoded)
{
    FUNC("BDecLen()");
    AsnLen  len;
    unsigned char  byte;
    unsigned long  lenBytes;

    byte = b.GetUByte();

    bytesDecoded++;
    if (byte < 128)   /* short length */
        return byte;

    else if (byte == (unsigned char) 0x080)  /* indef len indicator */
        return INDEFINITE_LEN;

    else  /* long len form */
    {
        /*
         * strip high bit to get # bytes left in len
         */
        lenBytes = byte & (unsigned char) 0x7f;

        if (lenBytes > sizeof (long))
        {
           throw BoundsException("length overflow", STACK_ENTRY);
        }

        bytesDecoded += lenBytes;

        for (len = 0; lenBytes > 0; lenBytes--)
            len = (len << 8) | (unsigned long int) b.GetUByte();

        return len;
    }
    /* not reached */
}


/*
 * Encodes an End of Contents (EOC) to the given buffer.
 * Returns the encoded length.
 */
AsnLen
BEncEoc (AsnBuf &b)
{

    b.PutByteRvs (0);
    b.PutByteRvs (0);
    return 2;
}  /* BEncEoc */

/*
 * Decodes an EOC from the given buffer.  Flags an error if the
 * octets are non-zero or if read error occured.
 */
void
BDecEoc (const AsnBuf &b, AsnLen &bytesDecoded)
{
    FUNC("BDecEoc()");
    if ((b.GetUByte() != 0) || (b.GetUByte() != 0))
    {
       throw EXCEPT("non zero byte in EOC or end of data reached" , DECODE_ERROR);
    }
    bytesDecoded += 2;
}  /* BDecEoc */


/* PER encoding of the length determinant for lengths */
/* zero to 127 inclusive                              */
/* Length will encode in one byte  with bit 8 = 0     */
AsnLen 
PEncDefLenTo127(AsnBufBits &b, int len)
{
	unsigned char cLen = (unsigned char)len;
	b.PutBits(&cLen, 8);
	return 8;
}

/* PER encoding of the length determinant for lengths */
/* in 16k fragments                                   */
/* Length will encode in one byte, bit 7 and 8 = 1    */
/* lower bits (1-4) are multiplied by 16k to give     */
/* length determinant                                 */
AsnLen 
PEncLen_16kFragment(AsnBufBits &b, int len)
{
	AsnLen l= 8;
	unsigned char cLen = 0xC0;
	cLen |= ((unsigned char)len);
	unsigned char *c = &cLen;

	b.PutBits(c, l);
	
	return l;
}

/* PER encoding of the length determinant for lengths */
/* between 127 and 16k non-inclusive                  */
AsnLen 
PEncLen_1to16k(AsnBufBits &b, int len)
{
	AsnLen l = 16;
	int templen = len;
	unsigned char cLen = ((unsigned char)len);
	unsigned char *c = &cLen;

	len >>= 8;

	cLen = ((unsigned char)len);
	cLen |= 0x80;
	c = &cLen;

	b.PutBits(c, 8);

	cLen = (unsigned char)templen;
	c = &cLen;

	b.PutBits(c, 8);

	return l;
}
 
  
_END_SNACC_NAMESPACE
