// file: .../c++-lib/inc/asn-config.h - decoder alloc routines and buffer routines and other configuration stuff.
//
// MS 92/06/18
// Copyright (C) 1992 Michael Sample and the University of British Columbia
//
// This library is free software; you can redistribute it and/or
// modify it provided that this copyright/license information is retained
// in original form.
//
// If you modify this file, you must clearly indicate your changes.
//
// This source code is distributed in the hope that it will be
// useful, but WITHOUT ANY WARRANTY; without even the implied warranty
// of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
//
// $Header: /baseline/SNACC/c++-lib/inc/asn-config.h,v 1.22 2002/11/07 16:28:50 leonberp Exp $
// $Log: asn-config.h,v $
// Revision 1.22  2002/11/07 16:28:50  leonberp
// latest runtime library changes
//
// Revision 1.21  2002/05/29 19:26:05  leonberp
// removed PDU_MEMBERS_MACRO and added BDecPDU and BEncPDU to AsnType
//
// Revision 1.20  2002/05/29 18:55:31  leonberp
// removed dependency on policy.h and snacc.h
//
// Revision 1.19  2002/05/22 15:40:40  leonberp
// Ported to Solaris with GCC 3.0.3
//
// Revision 1.18  2002/05/10 16:39:33  leonberp
// latest changes for release 2.2
// includes integrating asn-useful into C & C++ runtime library, the compiler changes that go along with that, SnaccException changes for C++ runtime and compiler
//
// Revision 1.17  2001/10/29 12:01:16  nicholar
// Removed #defines for Asn1New, Asn1Free, etc.
//
// Revision 1.16  2001/08/24 15:34:23  leonberp
// Moved list template logic to asn-list.h
//
// Revision 1.15  2001/08/16 21:23:27  rwc
// Updated to reflect customer's need to integrate __SGI_STL_PORT
// instead of the standard MS Windows STL library.  Minor
// change for includes.
//
// Revision 1.14  2001/07/25 15:36:32  sfl
// Yet another change for <string.h>; REMOVED reference to <string> which
// caused CML to not compile.
//
// Revision 1.13  2001/07/19 16:02:35  sfl
// Yet more string.h changes.
//
// Revision 1.12  2001/07/18 18:40:55  rwc
// Updates due to newly modified SNACC namespace additions.  The SNACC compilier
// was also modified (by Pierce) to create only .cpp, not .C which broke some of the
// make file rules.
//
// Revision 1.11  2001/07/12 19:33:32  leonberp
// Changed namespace to SNACC and added compiler options: -ns and -nons.  Also removed dead code.
//
// Revision 1.10  2001/06/28 21:38:01  rwc
// Updated to remove referneces to vdacerr, which originally replaced the cerr standard error output.
// Updated all references in macros and source that printed to vdacerr.  All code now performs an
// ASN_THROW(...) exception.
//
// Revision 1.9  2001/06/28 15:28:38  rwc
// ADDED "SNACCASN" namespace definition to all SNACC data structures.
// This should not affect most applications since we do not have any name
// conflicts.
// ALSO, combined all ASN primitive data type includes into asn-incl.h.
//
// Revision 1.8  2001/06/19 15:14:58  grafb
// Re-ordered includes and removed redundant includes for g++ 3.0 compile
//
// Revision 1.7  2001/06/18 17:47:22  rwc
// Updated to reflect newly added C++ Exception error handling, instead of "C" longjmp and setjmp calls.
// Changes made to both the compiler and the SNACC C++ run-time library.
//
// Revision 1.6  2001/01/23 20:51:44  rwc
// Updated to allow NOVDADER_RULES to disable VDA additions (except for new string
// class handling, e.g. PrintableString; had to allow).  Several sources are now disabled from
// the build if NOVDADER_RULES config item is chosen.  Builds, but untested.
//
// Revision 1.5  2001/01/11 18:19:38  rwc
// Updated to now define the VDADER_RULES, SNACC_DEEP_COPY in asn-config.h and not have these settings
// in the C++ pre-processor command line.
//
// Revision 1.4  2000/12/01 14:54:11  rwc
// Updated to place a ".h" on the #include <strstream> WIN32 include
// to support SCO port by customer.
//
// Revision 1.3  2000/11/02 14:56:12  rwc
// Updated to #ifdef WIN32 around #pragma for Unix warnings.
//
// Revision 1.2  2000/10/24 14:54:40  rwc
// Updated to remove high-level warnings (level 4 on MSVC++) for an easier build.
// SOME warnings persist due to difficulty in modifying the SNACC compiler to
// properly build clean source; also some files are built by Lex/Yacc.
//
// Revision 1.1.1.1  2000/08/21 20:36:11  leonberp
// First CVS Version of SNACC.
//
// Revision 1.7  1995/09/07 18:48:36  rj
// AsnIntType and AsnUIntType introduced to replace (unsigned) long int at a lot of places.
// they shall provide 32 bit integer types on all platforms.
//
// Revision 1.6  1995/07/25  20:19:00  rj
// changed `_' to `-' in file names.
//
// Revision 1.5  1995/02/13  14:47:46  rj
// settings for IEEE_REAL_FMT/IEEE_REAL_LIB moved from {c_lib,c++_lib}/inc/asn_config.h to acconfig.h.
//
// Revision 1.4  1994/10/08  04:17:59  rj
// code for meta structures added (provides information about the generated code itself).
//
// code for Tcl interface added (makes use of the above mentioned meta code).
//
// virtual inline functions (the destructor, the Clone() function, BEnc(), BDec() and Print()) moved from inc/*.h to src/*.C because g++ turns every one of them into a static non-inline function in every file where the .h file gets included.
//
// made Print() const (and some other, mainly comparison functions).
//
// several `unsigned long int' turned into `size_t'.
//
// Revision 1.3  1994/09/01  00:58:47  rj
// redundant code moved into ../../config.h.bot; semicolon removed from end of macro texts.
//
// Revision 1.2  1994/08/28  10:00:47  rj
// comment leader fixed.
//
// Revision 1.1  1994/08/28  09:20:29  rj
// first check-in. for a list of changes to the snacc-1.1 distribution please refer to the ChangeLog.

#ifndef _asn_config_h_
#define _asn_config_h_

#ifndef WIN32
#include <stdlib.h> /* for wchar_t on UNIX */
#endif

// RWC;VDA; added to support optional DLL build.
#ifndef SNACCDLL_API
#ifdef WIN32
#ifdef SNACCDLL_EXPORTS
#define SNACCDLL_API __declspec(dllexport)
#else
#ifdef SNACCDLL_NONE
#define SNACCDLL_API 
#else
#define SNACCDLL_API __declspec(dllimport)
#endif
#endif
#else
#define SNACCDLL_API 
#endif
#endif

#ifndef NO_NAMESPACE
#define _BEGIN_SNACC_NAMESPACE namespace SNACC {
#define _END_SNACC_NAMESPACE }
#else
#define _BEGIN_SNACC_NAMESPACE
#define _END_SNACC_NAMESPACE
#endif

// RWC;1/8/01; the following defines allow an application to default
//   the definitions for the SFL, ACL, and CML builds.
#define USE_EXP_BUF 1

// used not only by AsnInt (asn-int.h), but by AsnNameDesc (meta.h) as well:
#if SIZEOF_INT == 4
#  define I		int
#else
#  if SIZEOF_LONG == 4
#    define I		long
#  elif SIZEOF_SHORT == 4
#      define I		short
#  else
#    define I int
#  endif
#endif
#ifdef I
  typedef I		AsnIntType;
  typedef unsigned I	AsnUIntType;
#  undef I
#else
#  error "can't find integer type which is 4 bytes in size"
#endif

/* used to test if optionals are present */
#define NOT_NULL( ptr)		((ptr) != NULL)

#ifdef WIN32
#pragma  warning( disable : 4127 4611 4710 4800 )  // IGNORE constant conditional expression.
#endif

#endif /* conditional include */
