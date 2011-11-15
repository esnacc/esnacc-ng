/* config.h.  Generated automatically by configure.  */
/* config.h.in.  Generated automatically from configure.in by autoheader.  */

/* Define to empty if the keyword does not work.  */
/* #undef const */

/* Define to `int' if <sys/types.h> doesn't define.  */
/* #undef gid_t */

/* Define if you don't have vprintf but do have _doprnt.  */
/* #undef HAVE_DOPRNT */

/* Define if you have the vprintf function.  */
#define HAVE_VPRINTF 1

/* Define if your C compiler doesn't accept -c and -o together.  */
/* #undef NO_MINUS_C_MINUS_O */

/* Define if you need to in order for stat and other things to work.  */
/* #undef _POSIX_SOURCE */

/* Define to `unsigned' if <sys/types.h> doesn't define.  */
/* #undef size_t */

/* Define if you have the ANSI C header files.  */
#define STDC_HEADERS 1

/* Define if you can safely include both <sys/time.h> and <time.h>.  */
#define TIME_WITH_SYS_TIME 1

/* Define to `int' if <sys/types.h> doesn't define.  */
/* #undef uid_t */

/* Define if your processor stores words with the most significant
   byte first (like Motorola and SPARC, unlike Intel and VAX).  */
#define WORDS_BIGENDIAN 1

/* Define if the X Window System is missing or not being used.  */
/* #undef X_DISPLAY_MISSING */

/* Define if lex declares yytext as a char * by default, not a char[].  */
#define YYTEXT_POINTER 1

/*
 * file: acconfig.h
 *
 * $Header: /baseline/SNACC/config_Used.h,v 1.3 2003/12/17 19:05:02 gronej Exp $
 * $Log: config_Used.h,v $
 * Revision 1.3  2003/12/17 19:05:02  gronej
 * SNACC baseline merged with PER v1_7 tag
 *
 * Revision 1.2.2.1  2003/11/05 14:58:53  gronej
 * working PER code merged with esnacc_1_6
 *
 * Revision 1.2  2002/06/11 15:08:47  leonberp
 * removed bogus files from CVS
 *
 * Revision 1.1.1.1  2000/08/21 20:35:45  leonberp
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
#define __USE_ANSI_C__ 1

/* does the C++ compiler have the bool type built-in? */
#define BOOL_BUILTIN 1

/* does the C++ compiler allow variable sized automatic arryas? */
#define HAVE_VARIABLE_SIZED_AUTOMATIC_ARRAYS 1

/* do we have all the libs we need for the Tcl interface? */
/* #undef HAVE_TCL */

/* does the compiler support volatile functions (and is abort() volatile?) */
#define COMPILER_WITHOUT_VOLATILE_FUNCTIONS 1

/* The number of bytes in a double.  */
#define SIZEOF_DOUBLE 8

/* The number of bytes in a int.  */
#define SIZEOF_INT 4

/* The number of bytes in a long.  */
#define SIZEOF_LONG 4

/* The number of bytes in a short.  */
#define SIZEOF_SHORT 2

/* Define if you have the finite function.  */
#define HAVE_FINITE 1

/* Define if you have the isinf function.  */
/* #undef HAVE_ISINF */

/* Define if you have the memcmp function.  */
#define HAVE_MEMCMP 1

/* Define if you have the memcpy function.  */
#define HAVE_MEMCPY 1

/* Define if you have the memset function.  */
#define HAVE_MEMSET 1

/* Define if you have the <dirent.h> header file.  */
#define HAVE_DIRENT_H 1

/* Define if you have the <fcntl.h> header file.  */
#define HAVE_FCNTL_H 1

/* Define if you have the <malloc.h> header file.  */
#define HAVE_MALLOC_H 1

/* Define if you have the <memory.h> header file.  */
#define HAVE_MEMORY_H 1

/* Define if you have the <ndir.h> header file.  */
/* #undef HAVE_NDIR_H */

/* Define if you have the <string.h> header file.  */
#define HAVE_STRING_H 1

/* Define if you have the <sys/dir.h> header file.  */
/* #undef HAVE_SYS_DIR_H */

/* Define if you have the <sys/ndir.h> header file.  */
/* #undef HAVE_SYS_NDIR_H */

/* Define if you have the <unistd.h> header file.  */
#define HAVE_UNISTD_H 1

/* Define if you have the m library (-lm).  */
#define HAVE_LIBM 1
