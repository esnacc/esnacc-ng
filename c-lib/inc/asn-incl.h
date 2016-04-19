/*
 * asn_incl.h
 *   includes hdr files nec for a user prg that calls the generated
 *   encoding/decoding routines.
 *
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
 * $Header: /baseline/SNACC/c-lib/inc/asn-incl.h,v 1.17 2004/03/09 16:38:43 gronej Exp $
 * $Log: asn-incl.h,v $
 * Revision 1.17  2004/03/09 16:38:43  gronej
 * Updated check for visible string, and created it's own .c and .h files,
 * commented out for now but did not remove old visible string code
 *
 * Revision 1.16  2004/01/16 15:23:31  gronej
 * Updated Compiler to accept and process relative-oid's
 *
 * Revision 1.15  2003/12/17 19:05:03  gronej
 * SNACC baseline merged with PER v1_7 tag
 *
 * Revision 1.14.2.1  2003/11/05 14:58:55  gronej
 * working PER code merged with esnacc_1_6
 *
 * Revision 1.14  2003/02/20 21:07:59  leonberp
 * added #ifdef __cplusplus extern "C" to headers
 *
 * Revision 1.13  2002/11/01 15:36:11  mcphersc
 * Fixed SetWriteError
 *
 * Revision 1.12  2002/10/23 13:33:47  mcphersc
 * Redo include references placement
 *
 * Revision 1.11  2002/10/22 15:49:36  mcphersc
 * Mods for gen-buf usage
 *
 * Revision 1.10  2002/10/22 14:40:11  mcphersc
 * added exp-buf include reference
 *
 * Revision 1.9  2002/10/18 13:10:32  mcphersc
 * took out long int to unsigned long
 *
 * Revision 1.8  2002/10/01 14:11:39  mcphersc
 * ASN "C" Buf modifications
 *
 * Revision 1.7  2002/05/10 16:39:37  leonberp
 * latest changes for release 2.2
 * includes integrating asn-useful into C & C++ runtime library, the compiler changes that go along with that, SnaccException changes for C++ runtime and compiler
 *
 * Revision 1.6  2002/02/13 18:04:11  sfl
 * Added EOL to avoid Linux compile warning.
 *
 * Revision 1.5  2002/01/16 17:26:24  vracarl
 * added new file - snaccCder.h and define for MAX_BUF
 *
 * Revision 1.4  2001/07/12 19:34:01  leonberp
 * Changed namespace to SNACC and added compiler options: -ns and -nons.  Also removed dead code.
 *
 * Revision 1.3  2001/03/05 19:36:21  nicholar
 * no message
 *
 * Revision 1.2  2000/10/24 14:54:41  rwc
 * Updated to remove high-level warnings (level 4 on MSVC++) for an easier build.
 * SOME warnings persist due to difficulty in modifying the SNACC compiler to
 * properly build clean source; also some files are built by Lex/Yacc.
 *
 * Revision 1.1.1.1  2000/08/21 20:35:54  leonberp
 * First CVS Version of SNACC.
 *
 * Revision 1.2  1995/07/24 21:01:13  rj
 * changed `_' to `-' in file names.
 *
 * Revision 1.1  1994/08/28  09:21:27  rj
 * first check-in. for a list of changes to the snacc-1.1 distribution please refer to the ChangeLog.
 *
 */

#include "asn-config.h"
#include "gen-buf.h"
/*
#include "exp-buf.h"
#include "sbuf.h"
*/
#include "asn-len.h"
#include "asn-tag.h"
#include "asn-bool.h"
#include "asn-int.h"
#include "asn-enum.h"
#include "asn-real.h"
#include "asn-octs.h"
#include "asn-bits.h"
#include "asn-oid.h"
#include "asn-relative-oid.h"
#include "asn-null.h"
#include "asn-any.h"
#include "asn-list.h"
#include "asn-der.h"
#include "asn-PrintableStr.h"
#include "asn-UniversalString.h"
#include "asn-BMPString.h"
#include "asn-UTF8String.h"
#include "asn-VisibleString.h"
#include "asn-IA5String.h"
#include "asn-NumericString.h"
#include "asn-TeletexString.h"
#include "snaccCder.h"
#include "print.h"
#define MAX_BUF 4096



