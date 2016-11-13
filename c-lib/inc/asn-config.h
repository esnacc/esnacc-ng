/*
 * asn_config.h - configures the ANSI/non ansi, defines
 *                decoder alloc routines and buffer routines
 *
 * MS 91
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
 * $Header: /baseline/SNACC/c-lib/inc/asn-config.h,v 1.15 2003/12/17 19:05:03 gronej Exp $
 * $Log: asn-config.h,v $
 * Revision 1.15  2003/12/17 19:05:03  gronej
 * SNACC baseline merged with PER v1_7 tag
 *
 * Revision 1.14.2.1  2003/11/05 14:58:55  gronej
 * working PER code merged with esnacc_1_6
 *
 * Revision 1.14  2003/02/20 21:07:59  leonberp
 * added #ifdef __cplusplus extern "C" to headers
 *
 * Revision 1.13  2002/11/01 15:35:37  mcphersc
 * Modified to fix SetWriteError
 *
 * Revision 1.12  2002/10/23 10:23:51  mcphersc
 * Changed BUF_TYPE to AsnBuf
 *
 * Revision 1.11  2002/10/22 15:49:42  mcphersc
 * Mods for gen-buf usage
 *
 * Revision 1.10  2002/10/15 17:32:41  mcphersc
 * ExpBuf and GenBuf chagnes
 *
 * Revision 1.9  2002/10/01 14:02:39  mcphersc
 * ASN "C" Buf modifications
 *
 * Revision 1.8  2002/08/30 19:11:34  vracarl
 * got rid of c++ comments
 *
 * Revision 1.7  2002/07/25 10:52:05  sfl
 * Added ifdef's around pragma statement to avoid warnings on unix boxes
 *
 * Revision 1.5  2002/01/16 17:25:40  vracarl
 * added more defs
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
 * Revision 1.6  1997/03/13 09:15:16  wan
 * Improved dependency generation for stupid makedepends.
 * Corrected PeekTag to peek into buffer only as far as necessary.
 * Added installable error handler.
 * Fixed small glitch in idl-code generator (Markku Savela <msa@msa.tte.vtt.fi>).
 *
 * Revision 1.5  1995/07/24 21:01:11  rj
 * changed `_' to `-' in file names.
 *
 * Revision 1.4  1995/02/13  14:47:33  rj
 * settings for IEEE_REAL_FMT/IEEE_REAL_LIB moved from {c_lib,c++_lib}/inc/asn_config.h to acconfig.h.
 *
 * Revision 1.3  1994/10/08  04:46:20  rj
 * config.h -> snacc.h, which now is the toplevel config file.
 *
 * Revision 1.2  1994/08/31  23:53:05  rj
 * redundant code moved into ../../config.h.bot
 *
 * Revision 1.1  1994/08/28  09:21:25  rj
 * first check-in. for a list of changes to the snacc-1.1 distribution please refer to the ChangeLog.
 *
 */

#ifndef _asn_config_h_
#define _asn_config_h_

#include <stdio.h>
#include <setjmp.h> /* for jmp_buf type, setjmp and longjmp */
#include <stdlib.h>
#include <stdio.h>
#include <memory.h>


#define ENV_TYPE jmp_buf

/* for pow() used in asn_real.c - must include to avoid casting err on pow */
/* #include <math.h> */

#include "snacc.h"


/* used to test if optionals are present */
#define NOT_NULL( ptr)			((ptr) != NULL)


#ifdef __cplusplus
extern "C" {
#endif

/*
 * Asn1Error (char *str) - configure error handler
 */
void Asn1Error PROTO ((char* str));


/*
 * Asn1Warning (char *str) - configure warning mechanism
 * (currently never called)
 */
void Asn1Warning PROTO ((char* str));

/*
 * Asn1ErrorHandler - procedure to call upon Asn1Warning (severity 0)
 * and Asn1Error (severity 1).
 */
typedef void (*Asn1ErrorHandler) PROTO ((char* str, int severity));

/*
 * Asn1InstallErrorHandler - installs new error handler, returns former one
 */
Asn1ErrorHandler Asn1InstallErrorHandler PROTO ((Asn1ErrorHandler handler));

/*
 * configure memory scheme used by decoder to allocate memory
 * for the decoded value.
 * The Asn1Free will be called in the optionally generated
 * hierachical free routines.
 *
 * nibble_alloc allocs from a single buffer and EVERYTHING
 * is freed by a single fcn call. Individual elmts cannot be freed
 */

#ifdef __cplusplus
} 
#endif /* __cplusplus */

#ifdef USE_NIBBLE_MEMORY
#include "nibble-alloc.h"

#define Asn1Alloc( size)		NibbleAlloc (size)
#define Asn1Free( ptr)			/* empty */
#define CheckAsn1Alloc( ptr, env)	\
	if ((ptr) == NULL)\
	  longjmp (env, -27)

#else /* !USE_NIBBLE_MEMORY */

#include "mem.h"

#define Asn1Alloc( size)		Malloc (size)
#define Asn1Free( ptr)			Free (ptr)
#define CheckAsn1Alloc( ptr, env)	\
	if ((ptr) == NULL)\
	  longjmp (env, -27)

#endif /* USE_NIBBLE_MEMORY */

/*
 * NOTE: for use with tables, I defined the (slower)
 *  GenBuf type that is more flexible (à la ISODE and XDR).
 *  This allows the encode/decode libs to support other
 *  buffer types dynamically instead of having different
 *  libs for each buffer type.
 *  The GenBufs are not provided for the compiled code
 *  (ie the c_lib directory) but could easily be added
 *  (I don't have time, tho).  Tables tools are
 *  around 4x slower than the compiled version so a
 *  the GenBufs aren't such a big performance hit for table stuff.
 *
 */
#include "gen-buf.h"

//#define BUF_TYPE			GenBuf *
#define BufGetByte( b)			GenBufGetByte (b)
#define BufGetSeg( b, lenPtr)		GenBufGetSeg (b, lenPtr)
#define BufCopy( dst, b, len)		GenBufCopy (dst, b, len)
#define BufSkip( b, len)		GenBufSkip (b, len)
#define BufPeekByte( b)			GenBufPeekByte (b)
#define BufPeekSeg( b, lenPtr)		GenBufPeekSeg (b, lenPtr)
#define BufPeekCopy( dst, b, len)	GenBufPeekCopy (dst, b, len)
#define BufPutByteRvs( b, byte)		GenBufPutByteRvs (b, byte)
#define BufPutSegRvs( b, data, len)	GenBufPutSegRvs (b, data, len)
#define BufReadError( b)		GenBufReadError (b)
#define BufWriteError( b)		GenBufWriteError (b)
#define BufFreeBufAndData( b)           GenBufFreeBufAndData(*(b))
#define BufResetInWriteRvsMode( b)      GenBufResetInWriteRvsMode(*(b))
#define BufResetInReadMode( b)          GenBufResetInReadMode((b))
#define BufSetWriteError(b, value)     GenBufSetWriteError(b, value)



#include "print.h"  /* for printing set up */

#ifdef WIN32
#pragma  warning( disable : 4127 )  /* IGNORE constant conditional expression. */
#endif

#endif /* conditional include */

