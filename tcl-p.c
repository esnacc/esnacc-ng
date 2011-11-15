/*
 * file: tcl-p.c
 * purpose: check and return via exit code whether the tcl interface needs to be made
 *
 * $Header: /baseline/SNACC/tcl-p.c,v 1.2 2003/12/17 19:05:02 gronej Exp $
 * $Log: tcl-p.c,v $
 * Revision 1.2  2003/12/17 19:05:02  gronej
 * SNACC baseline merged with PER v1_7 tag
 *
 * Revision 1.1.2.1  2003/11/05 14:58:53  gronej
 * working PER code merged with esnacc_1_6
 *
 * Revision 1.1.1.1  2000/08/21 20:35:46  leonberp
 * First CVS Version of SNACC.
 *
 * Revision 1.1  1995/07/25  22:24:48  rj
 * new file
 *
 */

#define COMPILER	1

#include "snacc.h"

main()
{
#if TCL
  return 0;
#else
  return 1;
#endif
}
