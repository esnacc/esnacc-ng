/*
 * asn_bool.c
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
 * $Header: /baseline/SNACC/c-lib/src/asn-bool.c,v 1.10 2004/04/21 13:28:42 gronej Exp $
 * $Log: asn-bool.c,v $
 * Revision 1.10  2004/04/21 13:28:42  gronej
 * v1_7 release fix
 *
 * Revision 1.9  2004/04/21 13:10:35  gronej
 * patch submitted by Oskar to fix free calls
 *
 * Revision 1.8  2004/01/22 20:03:12  nicholar
 * Changed indent variable to an unsigned int from an unsigned short.  Indent is  used in library print functions and generated print functions
 *
 * Revision 1.7  2003/12/17 19:05:03  gronej
 * SNACC baseline merged with PER v1_7 tag
 *
 * Revision 1.6.2.1  2003/11/05 14:58:53  gronej
 * working PER code merged with esnacc_1_6
 *
 * Revision 1.6  2002/10/23 10:23:37  mcphersc
 * Changed BUF_TYPE to AsnBuf
 *
 * Revision 1.5  2002/10/21 17:01:29  mcphersc
 * fixed unsigned short int
 *
 * Revision 1.4  2002/09/03 19:33:20  vracarl
 * got rid of c++ comments
 *
 * Revision 1.3  2002/07/25 10:57:17  sfl
 * modified line 117 to be tagId=tagId and line 134 to be indent=indent to get rid of warnings
 *
 * Revision 1.1.1.1  2000/08/21 20:35:52  leonberp
 * First CVS Version of SNACC.
 *
 * Revision 1.3  1995/07/24 21:04:49  rj
 * changed `_' to `-' in file names.
 *
 * Revision 1.2  1994/09/01  00:05:57  rj
 * reduce the risk of unwanted surprises with macro expansion by properly separating the C tokens.
 *
 * Revision 1.1  1994/08/28  09:45:51  rj
 * first check-in. for a list of changes to the snacc-1.1 distribution please refer to the ChangeLog.
 *
 */

#include "asn-config.h"
#include "asn-len.h"
#include "asn-tag.h"
#include "asn-bool.h"

/*
 * encodes universal TAG LENGTH and Contents of and ASN.1 BOOLEAN
 */
AsnLen
BEncAsnBool PARAMS ((b, data),
    GenBuf *b _AND_
    AsnBool *data)
{
    AsnLen len;

    len =  BEncAsnBoolContent (b, data);
    len += BEncDefLen (b, len);
    len += BEncTag1 (b, UNIV, PRIM, BOOLEAN_TAG_CODE);
    return len;
}  /* BEncAsnBool */

/*
 * decodes universal TAG LENGTH and Contents of and ASN.1 BOOLEAN
 */
void
BDecAsnBool PARAMS ((b, result, bytesDecoded, env),
    GenBuf *b _AND_
    AsnBool    *result _AND_
    AsnLen *bytesDecoded _AND_
    jmp_buf env)
{
    AsnTag tag;
    AsnLen elmtLen;

    if ((tag = BDecTag (b, bytesDecoded, env)) != MAKE_TAG_ID (UNIV, PRIM, BOOLEAN_TAG_CODE))
    {
         Asn1Error ("BDecAsnBool: ERROR - wrong tag on BOOLEAN.\n");
         longjmp (env, -40);
    }

    elmtLen = BDecLen (b, bytesDecoded, env);
    BDecAsnBoolContent (b, tag, elmtLen, result, bytesDecoded, env);

}  /* BDecAsnBool */

/*
 * Encodes just the content of the given BOOLEAN value to the given buffer.
 */
AsnLen
BEncAsnBoolContent PARAMS ((b, data),
    GenBuf *b _AND_
    AsnBool  *data)
{
    BufPutByteRvs (b, (unsigned char)(*data ? 0xFF : 0));
    return 1;
}  /* BEncAsnBoolContent */

/*
 * Decodes just the content of an ASN.1 BOOLEAN from the given buffer.
 * longjmps if there is a buffer reading problem
 */
void
BDecAsnBoolContent PARAMS ((b, tagId, len, result, bytesDecoded, env),
    GenBuf *b _AND_
    AsnTag tagId ESNACC_UNUSED _AND_
    AsnLen len _AND_
    AsnBool  *result _AND_
    AsnLen  *bytesDecoded _AND_
    jmp_buf env)
{
    if (len != 1) {
        Asn1Error("BDecAsnBoolContent: ERROR - BOOLEAN length must be 1\n");
        longjmp(env,-5);
    }

    (*bytesDecoded)++;
    *result = (unsigned char)(BufGetByte (b) != 0);

    if (BufReadError(b)) {
        Asn1Error("BDecAsnBoolContent: ERROR - decoded past end of data\n");
        longjmp(env, -6);
    }
}  /* BDecAsnBoolContent */

/*
 * Prints the given BOOLEAN to the given FILE * in ASN.1 Value notation.
 * Does not use the indent.
 */
void
PrintAsnBool PARAMS ((f, v, indent),
    FILE *f _AND_
    AsnBool *v _AND_
    unsigned int indent ESNACC_UNUSED)
{
    if (*v)
        fprintf(f, "TRUE");
    else
        fprintf(f, "FALSE");
}

void FreeAsnBool PARAMS ((b), AsnBool* b ESNACC_UNUSED)
{

}
