/*
 * asn_len.c - BER encode, decode and utilities for ASN.1 lengths.
 *
 *   indefinite lens are representd by the highest AsnLen
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
 * $Header: /baseline/SNACC/c-lib/src/asn-len.c,v 1.6 2003/12/17 19:05:03 gronej Exp $
 * $Log: asn-len.c,v $
 * Revision 1.6  2003/12/17 19:05:03  gronej
 * SNACC baseline merged with PER v1_7 tag
 *
 * Revision 1.5.2.1  2003/11/05 14:58:53  gronej
 * working PER code merged with esnacc_1_6
 *
 * Revision 1.5  2002/10/23 10:23:37  mcphersc
 * Changed BUF_TYPE to AsnBuf
 *
 * Revision 1.4  2002/10/18 13:09:32  mcphersc
 * took out long int to unsigned long
 *
 * Revision 1.3  2002/10/01 14:13:29  mcphersc
 * ASN "C" Buf modifications
 *
 * Revision 1.2  2000/10/16 18:10:39  rwc
 * removed most warnings from C++-lib, some C-lib.
 *
 * Revision 1.1.1.1  2000/08/21 20:35:52  leonberp
 * First CVS Version of SNACC.
 *
 * Revision 1.4  1997/08/28 07:25:57  povey
 * Changes to support DER encoding/decoding
 *
 * Revision 1.3.1.1  1997/08/20 23:14:44  povey
 *
 * Revision 1.2  1995/07/27 08:58:36  rj
 * merged PeekEoc(), a function used only by the type table code.
 *
 * changed `_' to `-' in file names.
 *
 * Revision 1.1  1994/08/28  09:45:54  rj
 * first check-in. for a list of changes to the snacc-1.1 distribution please refer to the ChangeLog.
 *
 */
#include "config.h"
#include "asn-config.h"
#include "asn-len.h"


/* 
 * BER encode/decode routines
 */
AsnLen
BEncDefLen PARAMS ((b, len),
    GenBuf *b _AND_
    AsnLen len)
{
    /*
     * unrolled for efficiency
     * check each possibitlity of the 4 byte integer
     */
    if (len < 128)
    {
        BufPutByteRvs (b, (unsigned char)len);
        return 1;
    }
    else if (len < 256)
    {
        BufPutByteRvs (b, (unsigned char)len);
        BufPutByteRvs (b, 0x81);
        return 2;
    }
    else if (len < 65536)
    {
        BufPutByteRvs (b, (unsigned char)len);
        BufPutByteRvs (b, (unsigned char)(len >> 8));
        BufPutByteRvs (b, 0x82);
        return 3;
    }
    else if (len < 16777126)
    {
        BufPutByteRvs (b, (unsigned char)len);
        BufPutByteRvs (b, (unsigned char)(len >> 8));
        BufPutByteRvs (b, (unsigned char)(len >> 16));
        BufPutByteRvs (b, 0x83);
        return 4;
    }
    else
    {
        BufPutByteRvs (b, (unsigned char)len);
        BufPutByteRvs (b, (unsigned char)(len >> 8));
        BufPutByteRvs (b, (unsigned char)(len >> 16));
        BufPutByteRvs (b, (unsigned char)(len >> 24));
        BufPutByteRvs (b, 0x84);
        return 5;
    }
} /*  BEncDefLen */


/*
 * non unrolled version
 */
AsnLen
BEncDefLen2 PARAMS ((b, len),
    GenBuf *b _AND_
    long len)
{
    int i;
    unsigned long j;

    if (len < 128)
    {
        BufPutByteRvs (b, (unsigned char)len);
        return 1;
    }
    else
    {
        for (i = 0, j = len; j > 0; j >>= 8, i++)
            BufPutByteRvs (b, (unsigned char)j);

        BufPutByteRvs (b, (unsigned char)(0x80 | i));
        return i + 1;
    }

} /*  BEncDefLen2 */


/*
 * decodes and returns an ASN.1 length
 */
AsnLen
BDecLen PARAMS ((b, bytesDecoded, env),
    GenBuf *b _AND_
    unsigned long  *bytesDecoded _AND_
    jmp_buf env)
{
    AsnLen len;
    AsnLen byte;
    int lenBytes;

    byte = (unsigned long) BufGetByte (b);

    if (BufReadError (b))
    {
        Asn1Error ("BDecLen: ERROR - decoded past end of data\n");
        longjmp (env, -13);
    }

    (*bytesDecoded)++;
    if (byte < 128)   /* short length */
        return byte;

    else if (byte == (AsnLen) 0x080)  /* indef len indicator */
        return (unsigned long)INDEFINITE_LEN;

    else  /* long len form */
    {
        /*
         * strip high bit to get # bytes left in len
         */
        lenBytes = byte & (AsnLen) 0x7f;

        if (lenBytes > sizeof (AsnLen))
        {
            Asn1Error ("BDecLen: ERROR - length overflow\n");
            longjmp (env, -14);
        }

        (*bytesDecoded) += lenBytes;

        for (len = 0; lenBytes > 0; lenBytes--)
            len = (len << 8) | (AsnLen) BufGetByte (b);


        if (BufReadError (b))
        {
            Asn1Error ("BDecLen: ERROR - decoded past end of data\n");
            longjmp (env, -15);
        }

        return len;
    }
    /* not reached */
} /* BDecLen */


#ifdef _DEBUG
AsnLen
BEncEoc PARAMS ((b),
    GenBuf *b)
{
    BufPutByteRvs (b, 0);
    BufPutByteRvs (b, 0);
    return 2;
}   /* BEncEoc */
#endif
/*
 * Decodes an End of Contents (EOC) marker from the given buffer.
 * Flags and error if the octets are non-zero or if a read error
 * occurs.  Increments bytesDecoded by the length of the EOC marker.
 */

void
BDecEoc PARAMS ((b, bytesDecoded, env),
    GenBuf *b _AND_
    AsnLen *bytesDecoded _AND_
    jmp_buf env)
{
    if ((BufGetByte (b) != 0) || (BufGetByte (b) != 0) || BufReadError (b))
    {
        Asn1Error ("BDecEoc: ERROR - non zero byte in EOC or end of data reached\n");
        longjmp (env, -16);
    }
    (*bytesDecoded) += 2;

}  /* BDecEoc */

#if TTBL
/* returns true if the next tag is actually and EOC */
int PeekEoc PARAMS ((b),
    GenBuf *b)
{
    return BufPeekByte (b) == 0;
}  /* PeekEoc */
#endif


/*
 * DER Encoding/Routines
 */


/*
 * decodes and returns a DER encoded ASN.1 length
 */
AsnLen
DDecLen PARAMS ((b, bytesDecoded, env),
    GenBuf *b _AND_
    unsigned long  *bytesDecoded _AND_
    jmp_buf env)
{
    AsnLen len;
    AsnLen byte;
    int lenBytes;

    byte = (AsnLen) BufGetByte (b);

    if (BufReadError (b))
    {
        Asn1Error ("DDecLen: ERROR - decoded past end of data\n");
        longjmp (env, -13);
    }

    (*bytesDecoded)++;
    if (byte < 128)   /* short length */
        return byte;

    else if (byte == (AsnLen) 0x080)  {/* indef len indicator */
      Asn1Error("DDecLen: ERROR - Indefinite length decoded");
      longjmp(env, -666);
    }

    else  /* long len form */
    {
        /*
         * strip high bit to get # bytes left in len
         */
        lenBytes = byte & (AsnLen) 0x7f;

        if (lenBytes > sizeof (AsnLen))
        {
            Asn1Error ("DDecLen: ERROR - length overflow\n");
            longjmp (env, -14);
        }

        (*bytesDecoded) += lenBytes;

        for (len = 0; lenBytes > 0; lenBytes--)
            len = (len << 8) | (AsnLen) BufGetByte (b);


        if (BufReadError (b))
        {
            Asn1Error ("DDecLen: ERROR - decoded past end of data\n");
            longjmp (env, -15);
        }

        return len;
    }
    /* not reached */
} /* DDecLen */

