/*
 * asn_null.c - BER encode, decode, print and free routines for the
 *              ASN.1 NULL type.
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
 * $Header: /baseline/SNACC/c-lib/src/asn-null.c,v 1.12 2004/04/21 13:28:21 gronej Exp $
 * $Log: asn-null.c,v $
 * Revision 1.12  2004/04/21 13:28:21  gronej
 * v1_7 release fix
 *
 * Revision 1.11  2004/04/21 13:10:35  gronej
 * patch submitted by Oskar to fix free calls
 *
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
 * Revision 1.6  2002/09/03 19:43:11  vracarl
 * fixed a typo
 *
 * Revision 1.5  2002/09/03 19:33:19  vracarl
 * got rid of c++ comments
 *
 * Revision 1.4  2002/07/25 11:05:04  sfl
 * get rid of warnings
 *
 * Revision 1.1.1.1  2000/08/21 20:35:52  leonberp
 * First CVS Version of SNACC.
 *
 * Revision 1.3  1995/07/24 21:04:52  rj
 * changed `_' to `-' in file names.
 *
 * Revision 1.2  1994/09/01  00:06:08  rj
 * reduce the risk of unwanted surprises with macro expansion by properly separating the C tokens.
 *
 * Revision 1.1  1994/08/28  09:45:57  rj
 * first check-in. for a list of changes to the snacc-1.1 distribution please refer to the ChangeLog.
 *
 */

#include "asn-config.h"
#include "asn-len.h"
#include "asn-tag.h"
#include "asn-null.h"


/*
 * encodes universal TAG LENGTH and Contents of and ASN.1 NULL
 */
AsnLen
BEncAsnNull PARAMS ((b, data),
    GenBuf *b _AND_
    AsnNull *data ESNACC_UNUSED)
{
    AsnLen len;

    len =  BEncAsnNullContent(b, data);
    len += BEncDefLen(b, len);
    len += BEncTag1(b, UNIV, PRIM, NULLTYPE_TAG_CODE);
    return len;
}  /* BEncAsnNull */


/*
 * decodes universal TAG LENGTH and Contents of and ASN.1 NULL
 */
void
BDecAsnNull PARAMS ((b, result, bytesDecoded, env),
    GenBuf *b _AND_
    AsnNull *result _AND_
    AsnLen *bytesDecoded _AND_
    jmp_buf env)
{
    AsnTag tag;
    AsnLen elmtLen;

    if ((tag = BDecTag (b, bytesDecoded, env)) != MAKE_TAG_ID (UNIV, PRIM, NULLTYPE_TAG_CODE))
    {
         Asn1Error ("BDecAsnNull: ERROR wrong tag on NULL.\n");
         longjmp (env, -40);
    }

    elmtLen = BDecLen (b, bytesDecoded, env);
    BDecAsnNullContent (b, tag, elmtLen, result, bytesDecoded, env);

}  /* BDecAsnNull */


void
BDecAsnNullContent PARAMS ((b, tagId, len, result, bytesDecoded, env),
    GenBuf *b ESNACC_UNUSED _AND_
    AsnTag tagId ESNACC_UNUSED _AND_
    AsnLen len _AND_
    AsnNull *result ESNACC_UNUSED _AND_
    AsnLen *bytesDecoded ESNACC_UNUSED _AND_
    jmp_buf env)
{
    if (len != 0)
    {
        Asn1Error ("BDecAsnNullContent: ERROR - NULL type's len must be 0\n");
        longjmp (env, -17);
    }
}  /* BDecAsnNullContent */

/*
 * Prints the NULL value to the given FILE * in Value Notation.
 * ignores the indent.
 */
void
PrintAsnNull PARAMS ((f,v, indent),
    FILE *f _AND_
    AsnNull *v ESNACC_UNUSED _AND_
    unsigned int indent ESNACC_UNUSED)
{
    fprintf(f, "NULL");
}


void FreeAsnNull PARAMS ((b), AsnNull *b ESNACC_UNUSED)
{

}
