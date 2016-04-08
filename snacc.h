/*
 * file: snacc.h
 *
 * INSERT_VDA_COMMENTS
 *
 * $Header: /baseline/SNACC/snacc.h,v 1.13 2004/03/22 20:04:00 gronej Exp $
 * $Log: snacc.h,v $
 * Revision 1.13  2004/03/22 20:04:00  gronej
 * took IBM references out of the code (to the best of our knowledge, we don't use any of it anymore)
 *
 * Revision 1.12  2003/12/17 19:05:02  gronej
 * SNACC baseline merged with PER v1_7 tag
 *
 * Revision 1.11.2.1  2003/11/05 14:58:53  gronej
 * working PER code merged with esnacc_1_6
 *
 * Revision 1.11  2003/02/21 12:13:16  leonberp
 * cleaned up project settings for 1.5 release
 *
 * Revision 1.10  2002/12/16 17:23:11  mcphersc
 * DISABLED TCL
 *
 * Revision 1.9  2002/12/13 17:43:43  mcphersc
 * Modified the defines for META and TCL for use with the configure script
 *
 * Revision 1.8  2002/12/13 17:31:56  mcphersc
 * Modified TCL/META defines
 *
 * Revision 1.7  2002/12/10 14:21:19  mcphersc
 * *** empty log message ***
 *
 * Revision 1.6  2002/12/10 13:41:10  mcphersc
 * Added undefine of TCL for compiler code only.
 *
 * Revision 1.5  2002/09/04 18:33:43  vracarl
 * got rid of c++ comments
 *
 * Revision 1.4  2002/05/10 16:25:43  leonberp
 * latest changes for release 2.2
 *
 * Revision 1.3  2002/01/10 20:04:41  sfl
 * Updates to Unix ./configure script so that config.h is named
 * config_Used.h (similar to config_win32.h for windows).  This allows the
 * run-time libs and includes to use a unique include, but still be
 * dynamically built for the individual platform.
 *
 * Revision 1.2  2000/10/24 14:54:37  rwc
 * Updated to remove high-level warnings (level 4 on MSVC++) for an easier build.
 * SOME warnings persist due to difficulty in modifying the SNACC compiler to
 * properly build clean source; also some files are built by Lex/Yacc.
 *
 * Revision 1.1.1.1  2000/08/21 20:35:45  leonberp
 * First CVS Version of SNACC.
 *
 * Revision 1.7  1997/04/07 13:13:18  wan
 * Made more C++ readable (credits to Steve Walker)
 *
 * Revision 1.6  1997/02/28 13:39:35  wan
 * Modifications collected for new version 1.3: Bug fixes, tk4.2.
 *
 * Revision 1.5  1997/02/15 20:38:48  rj
 * In member functions, return *this after calling abort() for stupid compilers that don't seem to know about volatile abort() (they would otherwise abort with an error).
 *
 * Revision 1.4  1995/07/24  15:06:52  rj
 * configure checks for mem* functions. define replacements using b* functions, if necessary.
 *
 */

#ifndef _SNACC_H_
#define _SNACC_H_

#define GLASS	1
#define KHO	1

#include "config.h"

#ifndef NULL
#define NULL	0
#endif

#if HAVE_MEMCMP /* memcmp(3) returns <0, 0 and 0, bcmp(3) returns only 0 and !0 */
#define memcmpeq( a, b, len)	memcmp (a, b, len)
#else
#define memcmpeq( a, b, len)	bcmp (a, b, len)
#endif
#if HAVE_MEMSET
#define memzero( p, len)	memset (p, 0, len)
#else
#define memzero( p, len)	bzero (p, len)
#endif
#if !HAVE_MEMCPY
#define memcpy( dst, src, len)	bcopy (src, dst, len)
#endif

#ifdef __cplusplus

#ifdef VOLATILE_RETRUN
#  define RETURN_THIS_FOR_COMPILERS_WITHOUT_VOLATILE_FUNCTIONS	return *this;
#else
#  define RETURN_THIS_FOR_COMPILERS_WITHOUT_VOLATILE_FUNCTIONS
#endif

#if !BOOL_BUILTIN
#ifndef true
/* enum bool { false, true }; */
/* the above looks elegant, but leads to anachronisms (<, ==, !=, ... return value of type int, not enum bool), therefore: */
typedef int bool;
enum { false, true };
#endif
#endif

#else /* !__cplusplus */

#ifndef FALSE
#define FALSE	0
#endif
#ifndef TRUE
#define TRUE	1
#endif

#endif /* __cplusplus */

/*
 *  Inspired by gdb 4.0, for better or worse...
 *  (grabbed from Barry Brachman - MS)
 *
 *  These macros munge C routine declarations such
 *  that they work for ANSI or non-ANSI C compilers
 */
#ifndef __USE_NON_ANSI_C__

#define PROTO( X)			X
#define PARAMS( arglist, args)  	(args)
#define NOPARAMS()	        	(void)
#define _AND_				,
#define DOTS				, ...

#else /* __USE_ANSI_C__ */

#define PROTO( X)			()
#define PARAMS( arglist, args)	 	arglist args;
#define NOPARAMS()	        	()
#define _AND_				;
#define DOTS
#define void                            char

#endif /* __USE_ANSI_C__ */

#include "policy.h"

#if COMPILER
	// If we have TCL on this system then add TCL to the compiler
	#if defined (HAVE_TCLNOT)
		#define TCL	HAVE_TCL
		#define META	1	
	#endif
#elif defined (ENABLE_TCL) && defined (HAVE_TCL)
	#define TCL 1;
	#define META 1;
#endif

#ifdef ENABLE_META
#ifndef META
	#define META 1
#endif
#endif

#define COMMA			,

#define if_IBM_ENC( code)


#ifdef  META
#define if_META( code)		code
#else
#define if_META( code)
#endif

#if defined(TCL) && defined (META)
#define if_TCL( code)		code
#else
#define if_TCL( code)
#endif

#endif /* _SNACC_H_ */
