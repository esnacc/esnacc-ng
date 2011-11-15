/*
 * asn-der.c - These are used by DER encoders to sort the order of the
 *             components in a SET OF
 *
 * Dean Povey 97/08
 * Copyright (C) 1997 Dean Povey and the Distributed Systems Technology Centre
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
 * $Header: /baseline/SNACC/c-lib/src/asn-der.c,v 1.7 2003/12/17 19:05:03 gronej Exp $
 * $Log: asn-der.c,v $
 * Revision 1.7  2003/12/17 19:05:03  gronej
 * SNACC baseline merged with PER v1_7 tag
 *
 * Revision 1.6.2.1  2003/11/05 14:58:53  gronej
 * working PER code merged with esnacc_1_6
 *
 * Revision 1.6  2002/10/15 17:46:39  mcphersc
 * Compare mods
 *
 * Revision 1.4  2002/03/20 22:36:06  rwc
 * Fixed DER SET OF encoding rule failures; COMPILER code for these rules worked fine,
 * but the actual qsort handling routine did not return the
 * proper return code for sorting.  Tested using the ./c-examples/vdatestC/vdatestC.c program.
 *
 * Revision 1.3  2002/03/20 22:09:28  vracarl
 * added 2 BufResetInReadMode calls
 *
 * Revision 1.2  2000/10/24 14:54:42  rwc
 * Updated to remove high-level warnings (level 4 on MSVC++) for an easier build.
 * SOME warnings persist due to difficulty in modifying the SNACC compiler to
 * properly build clean source; also some files are built by Lex/Yacc.
 *
 * Revision 1.1.1.1  2000/08/21 20:35:52  leonberp
 * First CVS Version of SNACC.
 *
 * Revision 1.1  1997/08/28 07:25:57  povey
 * Changes to support DER encoding/decoding
 *
 *
 */


#include "asn-config.h"
#include "asn-der.h"
#include <memory.h>

// Simple Compare
int EncodedElmtCmp(const void* a1, const void* b1)
{
  int len = 0;
  int cmp = -1;
  unsigned char data1;
  unsigned char data2;
  EncodedElmt *a = (EncodedElmt *) a1;
  EncodedElmt *b = (EncodedElmt *) b1;

  /* Get minimum length */
  len = (a->len > b->len)?b->len:a->len;
  while (len-- > 0)
  {
	  data1 = BufGetByte (a->b);
	  data2 = BufGetByte (b->b);
      if (data1++ != data2++)
		cmp = data1 < data2 ? -1 : 1;
  }

  BufResetInReadMode(a->b);
  BufResetInReadMode(b->b);

  return cmp;

}
