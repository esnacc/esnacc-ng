/*
 * file: acconfig.h
 *
 * $Header: /baseline/SNACC/acconfig.h,v 1.2 2003/12/17 19:05:02 gronej Exp $
 * $Log: acconfig.h,v $
 * Revision 1.2  2003/12/17 19:05:02  gronej
 * SNACC baseline merged with PER v1_7 tag
 *
 * Revision 1.1.2.1  2003/11/05 14:58:53  gronej
 * working PER code merged with esnacc_1_6
 *
 * Revision 1.1.1.1  2000/08/21 20:35:44  leonberp
 * First CVS Version of SNACC.
 *
 * Revision 1.7  1997/03/03 11:58:26  wan
 * Final pre-delivery stuff (I hope).
 *
 * Revision 1.6  1997/02/28 13:39:34  wan
 * Modifications collected for new version 1.3: Bug fixes, tk4.2.
 *
 * Revision 1.5  1997/02/15 20:01:37  rj
 * check whether the compiler supports volatile functions (and whether abort() is volatile).
 *
 * Revision 1.4  1995/02/20  11:16:57  rj
 * cpp switch HAVE_VARIABLE_SIZED_AUTOMATIC_ARRAYS added.
 *
 * Revision 1.3  1995/02/13  14:46:49  rj
 * settings for IEEE_REAL_FMT/IEEE_REAL_LIB moved from {c_lib,c++_lib}/inc/asn_config.h to acconfig.h.
 *
 * Revision 1.2  1994/10/08  04:38:56  rj
 * slot for autoconf Tcl detection added.
 *
 * Revision 1.1  1994/09/01  00:51:19  rj
 * first check-in (new file).
 *
 */

/*
 * define IEEE_REAL_FMT if your system/compiler uses the native ieee double
 * this should improve the performance of encoding reals.
 * If your system has the IEEE library routines (iszero, isinf etc)
 * then define IEEE_REAL_LIB.  If neither are defined then
 * frexp is used.  Performance is probaby best for IEEE_REAL_FMT.
 *
 *  #define IEEE_REAL_FMT
 *  #define IEEE_REAL_LIB
 */
/* use ANSI or K&R style C? */
#undef __USE_ANSI_C__

/* does the C++ compiler have the bool type built-in? */
#undef BOOL_BUILTIN

/* does the C++ compiler allow variable sized automatic arryas? */
#undef HAVE_VARIABLE_SIZED_AUTOMATIC_ARRAYS

/* do we have all the libs we need for the Tcl interface? */
#undef HAVE_TCL

/* does the compiler support volatile functions (and is abort() volatile?) */
#undef COMPILER_WITHOUT_VOLATILE_FUNCTIONS
