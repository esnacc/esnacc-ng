/*
 * asn_len.h
 *
 * Warning: many of these routines are MACROs for performance reasons
 *          - be carful where you use them.  Don't use more than one per
 *          assignment statement -
 *          (eg itemLen += BEncEoc (b) + BEncFoo (b) ..; this
 *           will break the code)
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
 * $Header: /baseline/SNACC/c-lib/inc/asn-len.h,v 1.7 2003/12/17 19:05:03 gronej Exp $
 * $Log: asn-len.h,v $
 * Revision 1.7  2003/12/17 19:05:03  gronej
 * SNACC baseline merged with PER v1_7 tag
 *
 * Revision 1.6.2.1  2003/11/05 14:58:55  gronej
 * working PER code merged with esnacc_1_6
 *
 * Revision 1.6  2003/02/20 21:07:59  leonberp
 * added #ifdef __cplusplus extern "C" to headers
 *
 * Revision 1.5  2002/10/23 10:23:51  mcphersc
 * Changed BUF_TYPE to AsnBuf
 *
 * Revision 1.4  2002/10/21 17:13:24  mcphersc
 * fixed long int
 *
 * Revision 1.3  2002/10/01 14:13:34  mcphersc
 * ASN "C" Buf modifications
 *
 * Revision 1.2  2001/07/12 19:34:02  leonberp
 * Changed namespace to SNACC and added compiler options: -ns and -nons.  Also removed dead code.
 *
 * Revision 1.1.1.1  2000/08/21 20:35:54  leonberp
 * First CVS Version of SNACC.
 *
 * Revision 1.4  1997/08/28 07:25:52  povey
 * Changes to support DER encoding/decoding
 *
 * Revision 1.3.1.1  1997/08/20 23:14:43  povey
 *
 * Revision 1.2  1995/07/27 08:42:40  rj
 * cpp macro TBL changed to TTBL since some type table code uses TBL as a type name.
 *
 * changed `_' to `-' in file names.
 *
 * Revision 1.1  1994/08/28  09:21:29  rj
 * first check-in. for a list of changes to the snacc-1.1 distribution please refer to the ChangeLog.
 *
 */

#ifndef _asn_len_h_
#define _asn_len_h_

#ifdef __cplusplus
extern "C" {
#endif


typedef unsigned long AsnLen;

/*
 * BER Encoding/Decoding routines
 */

/* max unsigned value  - used for internal rep of indef len */
#define INDEFINITE_LEN		~0UL


#ifdef USE_INDEF_LEN


#define BEncEocIfNec( b)	BEncEoc (b)

/*
 * include len for EOC  (2 must be first due to BEncIndefLen
 * - ack! ugly macros!)
 */
#define BEncConsLen( b, len)	2 + BEncIndefLen(b)


#else  /* use definite length - faster?/smaller encodings */


/* do nothing since only using definite lens */
#define BEncEocIfNec( b)

#define BEncConsLen( b, len)	BEncDefLen(b, len)


#endif



/*
 * writes indefinite length byte to buffer. 'returns' encoded len (1)
 */
#define BEncIndefLen( b)\
    1;\
    BufPutByteRvs (b, 0x80);

#ifndef _DEBUG
#define BEncEoc( b)\
    2;\
    BufPutByteRvs (b, 0);\
    BufPutByteRvs (b, 0);
#endif

/*
 * use if you know the encoded length will be 0 >= len <= 127
 * Eg for booleans, nulls, any resonable integers and reals
 *
 * NOTE: this particular Encode Routine does NOT return the length
 * encoded (1).
 */
#define BEncDefLenTo127( b, len)\
    BufPutByteRvs (b, (unsigned char) len)

#define BDEC_2ND_EOC_OCTET( b, bytesDecoded, env)\
{\
    if ((BufGetByte (b) != 0) || BufReadError (b)) {\
        Asn1Error ("ERROR - second octet of EOC not zero\n");\
        longjmp (env, -28);}\
     (*bytesDecoded)++;\
}


AsnLen BEncDefLen PROTO ((GenBuf *b, AsnLen len));
AsnLen BEncDefLen2 PROTO ((GenBuf *b, long  len));
AsnLen BDecLen PROTO ((GenBuf *b, AsnLen  *bytesDecoded, ENV_TYPE env));

#ifdef _DEBUG
AsnLen BEncEoc PROTO ((GenBuf *b));
#endif
void BDecEoc PROTO ((GenBuf *b, AsnLen *bytesDecoded, ENV_TYPE env));

#if TTBL
int PeekEoc PROTO ((GenBuf *b));
#endif

/*
 * DER Encoding/Decoding routines 
 */

/* We always use Definite length encoders */

/* do nothing since only using definite lens */
#define DEncEocIfNec( b)

#define DEncConsLen DEncDefLen

/*
 * use if you know the encoded length will be 0 >= len <= 127
 * Eg for booleans, nulls, any resonable integers and reals
 *
 * NOTE: this particular Encode Routine does NOT return the length
 * encoded (1).
 */
#define DEncDefLenTo127( b, len)\
    BufPutByteRvs (b, (unsigned char) len)

#define DEncDefLen BEncDefLen

AsnLen DDecLen PROTO ((GenBuf *b, AsnLen  *bytesDecoded, ENV_TYPE env));

/* Error conditions */
#define DDecEoc(a, b, env) longjmp(env, -666)

/* Should never happen */
#define DDEC_2ND_EOC_OCTET(a, b, env)

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* conditional include */

