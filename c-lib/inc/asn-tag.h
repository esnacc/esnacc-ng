/*
 * asn_tag.h
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
 *
 * ------------------------------------------------------------------------
 * -  J.G. Van Dyke & Associates, Inc. Modification History of SNACC 1.3  -
 * ------------------------------------------------------------------------
 * 
 * All modification are relative to the v1.3 of SNACC.  We used SunOS 4.1.3's 
 * SCCS.  The revision #'s start at 1.1,  which is the original version from 
 * SNACC 1.3.
 * 
 * 
 * ../SCCS/s.asn-tag.h:
 * 
 * D 1.2	98/04/24 22:38:58 pleonber	2 1	00012/00000/00221
 * added INSERT_VDA_COMMENTS for script that adds SCCS history.  Also added support for UniversalString and BMPString.
 * 
 * D 1.1	98/04/23 12:11:09 pleonber	1 0	00221/00000/00000
 * date and time created 98/04/23 12:11:09 by pleonber
 * 
 * ----------------------- End of VDA Modifications ---------------------------
 * 
 * 
 * 
 * $Header: /baseline/SNACC/c-lib/inc/asn-tag.h,v 1.9 2004/03/22 20:04:18 gronej Exp $
 * $Log: asn-tag.h,v $
 * Revision 1.9  2004/03/22 20:04:18  gronej
 * took IBM references out of the code (to the best of our knowledge, we don't use any of it anymore)
 *
 * Revision 1.8  2004/01/14 19:07:52  gronej
 * Updated Compiler to accept and process relative-oid's
 *
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
 * Revision 1.4  2002/05/15 17:00:57  leonberp
 * added support for new basicTypes to compiler
 *
 * Revision 1.3  2001/07/12 19:34:04  leonberp
 * Changed namespace to SNACC and added compiler options: -ns and -nons.  Also removed dead code.
 *
 * Revision 1.2  2000/09/19 14:16:15  rwc
 * Updated UTF8String for proper tag (12, instead of 11th position in enum).
 *
 * Revision 1.1.1.1  2000/08/21 20:35:54  leonberp
 * First CVS Version of SNACC.
 *
 * Revision 1.3  1995/07/27 08:44:15  rj
 * cpp macro TBL changed to TTBL since some type table code uses TBL as a type name.
 *
 * changed `_' to `-' in file names.
 *
 * Revision 1.2  1995/02/18  16:22:23  rj
 * let cpp choose a 32 bit integer type.
 *
 * Revision 1.1  1994/08/28  09:21:37  rj
 * first check-in. for a list of changes to the snacc-1.1 distribution please refer to the ChangeLog.
 *
 */

#ifndef _asn_tag_h_
#define _asn_tag_h_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
typedef uint32_t UL;
typedef UL	AsnTag;

/* Tag Id's byte length */
#define TB	sizeof (AsnTag)

/*
 * The MAKE_TAG_ID macro generates the TAG_ID rep for the
 * the given class/form/code (rep'd in long integer form)
 * if the class/form/code are constants the compiler (should)
 * calculate the tag completely --> zero runtime overhead.
 * This is good for efficiently comparing tags in switch statements
 * (decoding) etc.  because run-time bit fiddling (eliminated) minimized
 */

#define MAKE_TAG_ID( cl, fm, cd)\
	((((UL)(cl)) << ((TB -1) * 8)) | (((UL)(fm)) << ((TB -1) * 8)) | (MAKE_TAG_ID_CODE (((UL)(cd)))))


#define MAKE_TAG_ID_CODE(cd)\
( (cd < 31) ?  (MAKE_TAG_ID_CODE1 (cd)):\
      ((cd < 128)?  (MAKE_TAG_ID_CODE2 (cd)):\
         ((cd < 16384)?  (MAKE_TAG_ID_CODE3 (cd)):\
           (MAKE_TAG_ID_CODE4 (cd)))))

#define MAKE_TAG_ID_CODE1(cd)  (cd << ((TB -1) * 8))
#define MAKE_TAG_ID_CODE2(cd)  ((31 << ((TB -1) * 8)) | (cd << ((TB-2) * 8)))
#define MAKE_TAG_ID_CODE3(cd)  ((31 << ((TB -1) * 8))\
                                | ((cd & 0x3f80) << 9)\
                                | ( 0x0080 << ((TB-2) * 8))\
                                | ((cd & 0x007F) << ((TB-3)* 8)))

#define MAKE_TAG_ID_CODE4(cd)  ((31 << ((TB -1) * 8))\
                                | ((cd & 0x1fc000) << 2)\
                                | ( 0x0080 << ((TB-2) * 8))\
                                | ((cd & 0x3f80) << 1)\
                                | ( 0x0080 << ((TB-3) * 8))\
                                | ((cd & 0x007F) << ((TB-4)*8)))



typedef enum
{
    ANY_CLASS = -2,
    NULL_CLASS = -1,
    UNIV = 0,
    APPL = (1 << 6),
    CNTX = (2 << 6),
    PRIV = (3 << 6)
} BER_CLASS;

typedef enum
{
    ANY_FORM = -2,
    NULL_FORM = -1,
    PRIM = 0,
    CONS = (1 << 5)
} BER_FORM;


typedef enum
{
    NO_TAG_CODE = 0,
    BOOLEAN_TAG_CODE = 1,
    INTEGER_TAG_CODE,
    BITSTRING_TAG_CODE,
    OCTETSTRING_TAG_CODE,
    NULLTYPE_TAG_CODE,
    OID_TAG_CODE,
    OD_TAG_CODE,
    EXTERNAL_TAG_CODE,
    REAL_TAG_CODE,
    ENUM_TAG_CODE,
	 UTF8STRING_TAG_CODE=12,
     RELATIVE_OID_TAG_CODE=13,
    SEQ_TAG_CODE =  16,
    SET_TAG_CODE,
    NUMERICSTRING_TAG_CODE,
    PRINTABLESTRING_TAG_CODE,
    TELETEXSTRING_TAG_CODE,
    VIDEOTEXSTRING_TAG_CODE,
    IA5STRING_TAG_CODE,
    UTCTIME_TAG_CODE,
    GENERALIZEDTIME_TAG_CODE,
    GRAPHICSTRING_TAG_CODE,
    VISIBLESTRING_TAG_CODE,
    GENERALSTRING_TAG_CODE,
    UNIVERSALSTRING_TAG_CODE = 28,
    BMPSTRING_TAG_CODE = 30
} BER_UNIV_CODE;

#define TT61STRING_TAG_CODE	TELETEXSTRING_TAG_CODE
#define ISO646STRING_TAG_CODE	VISIBLESTRING_TAG_CODE


/*
 * the TAG_ID_[CLASS/FORM/CODE] macros are not
 * super fast - try not to use during encoding/decoding
 */
#define TAG_ID_CLASS( tid)	((tid & (0xC0 << ((TB-1) *8))) >> ((TB -1) * 8))
#define TAG_ID_FORM( tid)	((tid & (0x20 << ((TB-1) *8))) >> ((TB -1) * 8))

/*
 * TAG_IS_CONS evaluates to true if the given AsnTag type
 * tag has the constructed bit set.
 */
#define TAG_IS_CONS( tag)	((tag) & (CONS << ((TB-1) *8)))
#define CONSIFY( tag)		(tag | (CONS << ((TB-1) *8)))
#define DECONSIFY( tag)		(tag &  ~(CONS << ((TB-1) *8)))


/* not a valid tag - usually the first EOC octet */
#define EOC_TAG_ID		0



/*
 * tag encoders.  given constant values for class form &
 * code in the  source, these can be optimized by the compiler
 * (e.g.  do the shifts and bitwise ands & ors etc)
 *
 * This is the prototype that the following BEncTag routines
 * would use if they were routines.  They return the number of
 * octets written to the buffer.
 *
 *
 *AsnLen BEncTag PROTO ((GenBuf *b, BER_CLASS class, BER_FORM form, int code));
 *
 * WARNING: these are FRAGILE macros (What people will do for performance!)
 *          Be careful of situations like:
 *            if (foo)
 *                  encLen += BEncTag1 (...);
 *          Use {}'s to enclose any ASN.1 related routine that you are
 *          treating as a single statement in your code.
 */
#define BEncTag1( b, class, form, code)\
    1;\
    BufPutByteRvs (b, (class) | (form) | (code));

#define BEncTag2( b, class, form, code)\
    2;\
    BufPutByteRvs (b, code);\
    BufPutByteRvs (b, (class) | (form) | 31);

#define BEncTag3( b, class, form, code)\
    3;\
    BufPutByteRvs (b, (code) & 0x7F);\
    BufPutByteRvs (b, 0x80 | ((code) >> 7));\
    BufPutByteRvs (b, (class) | (form) | 31);

#define BEncTag4( b, class, form, code)\
    4;\
    BufPutByteRvs (b, (code) & 0x7F);\
    BufPutByteRvs (b, 0x80 | ((code) >> 7));\
    BufPutByteRvs (b, 0x80 | ((code) >> 14));\
    BufPutByteRvs (b, (class) | (form) | 31);

#define BEncTag5( b, class, form, code)\
    5;\
    BufPutByteRvs (b, (code) & 0x7F);\
    BufPutByteRvs (b, 0x80 | ((code) >> 7));\
    BufPutByteRvs (b, 0x80 | ((code) >> 14));\
    BufPutByteRvs (b, 0x80 | ((code) >> 21));\
    BufPutByteRvs (b, (class) | (form) | 31);


/* the following are protos for routines ins asn_tag.c */


AsnTag BDecTag PROTO ((GenBuf *b, AsnLen *bytesDecoded, ENV_TYPE env));
#if TTBL
AsnTag PeekTag PROTO ((GenBuf *b, ENV_TYPE env));
#endif

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* conditional include */

