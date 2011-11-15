/*
 * asn_enum.h
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
 * $Header: /baseline/SNACC/c-lib/inc/asn-enum.h,v 1.5 2003/12/17 19:05:03 gronej Exp $
 * $Log: asn-enum.h,v $
 * Revision 1.5  2003/12/17 19:05:03  gronej
 * SNACC baseline merged with PER v1_7 tag
 *
 * Revision 1.4.2.1  2003/11/05 14:58:55  gronej
 * working PER code merged with esnacc_1_6
 *
 * Revision 1.4  2003/02/20 21:07:59  leonberp
 * added #ifdef __cplusplus extern "C" to headers
 *
 * Revision 1.3  2002/10/23 10:23:51  mcphersc
 * Changed BUF_TYPE to AsnBuf
 *
 * Revision 1.2  2001/07/12 19:34:01  leonberp
 * Changed namespace to SNACC and added compiler options: -ns and -nons.  Also removed dead code.
 *
 * Revision 1.1.1.1  2000/08/21 20:35:54  leonberp
 * First CVS Version of SNACC.
 *
 * Revision 1.2  1995/07/24 21:01:12  rj
 * changed `_' to `-' in file names.
 *
 * Revision 1.1  1994/08/28  09:21:26  rj
 * first check-in. for a list of changes to the snacc-1.1 distribution please refer to the ChangeLog.
 *
 */

#ifndef _asn_enum_h_
#define _asn_enum_h_

#ifdef __cplusplus
extern "C" {
#endif


typedef AsnInt AsnEnum;

/*
 * ENUMERATED have a UNIVERSAL tag that is diff from INTEGERS
 * so need diff encoding routine tho content stuff is the same
 */
AsnLen BEncAsnEnum PROTO ((GenBuf *b, AsnEnum *data));

void BDecAsnEnum PROTO ((GenBuf *b, AsnEnum *result, AsnLen *bytesDecoded, ENV_TYPE env));

/* DAD - modified the two defines here so that enum Ptr's can
 * be resolved to the same size dest as what AsnInt gets
 * defined to be.
 */
#define BEncAsnEnumContent(a,b) BEncAsnIntContent((a),((AsnInt *) (b)))

#define BDecAsnEnumContent(a,b,c,d,e,f)  BDecAsnIntContent((a),(b),(c),((AsnInt*)(d)),(e),(f))

#define FreeAsnEnum FreeAsnInt

#define PrintAsnEnum PrintAsnInt

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* conditional include */

