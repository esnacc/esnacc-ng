/*
 * file: policy.h
 *
 */

/*
 * enable the snacc compiler's Tcl interface generating code?
 * set it to 0 or 1.
 */
#ifndef NO_TCL
#define NO_TCL		0
#endif

/*
 * enable code for meta code generation?
 * the Tcl code needs it.
 */
#ifndef NO_META
#define NO_META		NO_TCL
#endif

/*
 * enable code for CORBA IDL generation?
 */
#ifndef IDL
#define IDL		1
#endif
