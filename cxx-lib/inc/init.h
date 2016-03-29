/*
 * file: .../c++-lib/inc/init.h
 *
 * $Header: /baseline/SNACC/c++-lib/inc/init.h,v 1.1.1.1 2000/08/21 20:36:12 leonberp Exp $
 * $Log: init.h,v $
 * Revision 1.1.1.1  2000/08/21 20:36:12  leonberp
 * First CVS Version of SNACC.
 *
 * Revision 1.1  1995/07/27 09:22:35  rj
 * new file: .h file containing a declaration for a function defined in a C++ file, but with C linkage.
 *
 */

extern
#ifdef __cplusplus
	"C"
#endif
	     int Snacc_Init (Tcl_Interp *interp);
