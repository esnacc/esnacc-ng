/*
 * tbl-gen-c-hdr.h -  prints C type defs for vals
 *                  tbl decoder will return for the given type table.
 *
 *
 * Mike Sample
 *
 * Copyright (C) 1993 Michael Sample
 *            and the University of British Columbia
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
 * $Header: /baseline/SNACC/c-lib/inc/tbl-gen-c-hdr.h,v 1.3 2003/12/17 19:05:03 gronej Exp $
 * $Log: tbl-gen-c-hdr.h,v $
 * Revision 1.3  2003/12/17 19:05:03  gronej
 * SNACC baseline merged with PER v1_7 tag
 *
 * Revision 1.2.2.1  2003/11/05 14:58:55  gronej
 * working PER code merged with esnacc_1_6
 *
 * Revision 1.2  2001/07/12 19:34:08  leonberp
 * Changed namespace to SNACC and added compiler options: -ns and -nons.  Also removed dead code.
 *
 * Revision 1.1.1.1  2000/08/21 20:35:55  leonberp
 * First CVS Version of SNACC.
 *
 * Revision 1.1  1995/07/27 08:55:56  rj
 * first check-in after being merged into .../c-lib/.
 *
 */

void TblPrintCHdr PROTO ((TBL *tbl, FILE *f));

