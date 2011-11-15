/*
 * snacced -	Snacc_Init added to the default tkXAppInit.
 *
 * $Header: /baseline/SNACC/c++-lib/src/tkAppInit.c,v 1.1.1.1 2000/08/21 20:36:09 leonberp Exp $
 * $Log: tkAppInit.c,v $
 * Revision 1.1.1.1  2000/08/21 20:36:09  leonberp
 * First CVS Version of SNACC.
 *
 * Revision 1.2  1997/02/28 13:39:48  wan
 * Modifications collected for new version 1.3: Bug fixes, tk4.2.
 *
 * Revision 1.1  1997/01/02 09:07:59  rj
 * first check-in
 *
 */

#include "snacc.h"

#if TCL

/* 
 * tkXAppInit.c --
 *
 *      Provides a default version of the TclX_AppInit procedure for use with
 *      applications built with Extended Tcl and Tk.  This is based on the
 *      the UCB Tk file tkAppInit.c
 *
 *-----------------------------------------------------------------------------
 * Copyright 1991-1993 Karl Lehenbauer and Mark Diekhans.
 *
 * Permission to use, copy, modify, and distribute this software and its
 * documentation for any purpose and without fee is hereby granted, provided
 * that the above copyright notice appear in all copies.  Karl Lehenbauer and
 * Mark Diekhans make no representations about the suitability of this
 * software for any purpose.  It is provided "as is" without express or
 * implied warranty.
 *-----------------------------------------------------------------------------
 * $Id: tkAppInit.c,v 1.1.1.1 2000/08/21 20:36:09 leonberp Exp $
 *-----------------------------------------------------------------------------
 * Copyright (c) 1993 The Regents of the University of California.
 * All rights reserved.
 *
 * Permission is hereby granted, without written agreement and without
 * license or royalty fees, to use, copy, modify, and distribute this
 * software and its documentation for any purpose, provided that the
 * above copyright notice and the following two paragraphs appear in
 * all copies of this software.
 * 
 * IN NO EVENT SHALL THE UNIVERSITY OF CALIFORNIA BE LIABLE TO ANY PARTY FOR
 * DIRECT, INDIRECT, SPECIAL, INCIDENTAL, OR CONSEQUENTIAL DAMAGES ARISING OUT
 * OF THE USE OF THIS SOFTWARE AND ITS DOCUMENTATION, EVEN IF THE UNIVERSITY OF
 * CALIFORNIA HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * THE UNIVERSITY OF CALIFORNIA SPECIFICALLY DISCLAIMS ANY WARRANTIES,
 * INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY
 * AND FITNESS FOR A PARTICULAR PURPOSE.  THE SOFTWARE PROVIDED HEREUNDER IS
 * ON AN "AS IS" BASIS, AND THE UNIVERSITY OF CALIFORNIA HAS NO OBLIGATION TO
 * PROVIDE MAINTENANCE, SUPPORT, UPDATES, ENHANCEMENTS, OR MODIFICATIONS.
 */

#ifndef lint
static char rcsid[] = "$Header: /baseline/SNACC/c++-lib/src/tkAppInit.c,v 1.1.1.1 2000/08/21 20:36:09 leonberp Exp $ SPRITE (Berkeley)";
#endif /* not lint */

#include <tk.h>

#include "init.h"

int
main(argc, argv)
    int argc;                   /* Number of command-line arguments. */
    char **argv;                /* Values of command-line arguments. */
{
    Tk_Main(argc, argv, Tcl_AppInit);
    return 0;                   /* Needed only to prevent compiler warning. */
}

int
Tcl_AppInit (interp)
    Tcl_Interp *interp;		/* Interpreter for application. */
{
    if (Tcl_Init(interp) == TCL_ERROR) {
        return TCL_ERROR;
    }
    if (Tk_Init(interp) == TCL_ERROR) {
        return TCL_ERROR;
    }
    Tcl_StaticPackage(interp, "Tk", Tk_Init, (Tcl_PackageInitProc *) NULL);

    if (Snacc_Init (interp) == TCL_ERROR)
	return TCL_ERROR;

    if (Tree_Init (interp) == TCL_ERROR)
	return TCL_ERROR;

    Tcl_SetVar (interp, "tcl_rcFileName", "~/.snaccedrc", TCL_GLOBAL_ONLY);

    return TCL_OK;
}

#endif
