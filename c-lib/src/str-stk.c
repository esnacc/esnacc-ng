/*
 * str_stk.c  - maintains a stack of the components of a bit string
 *        or octet string so they can be copied into a single chunk
 *
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
 * $Header: /baseline/SNACC/c-lib/src/str-stk.c,v 1.3 2004/03/23 18:49:21 gronej Exp $
 * $Log: str-stk.c,v $
 * Revision 1.3  2004/03/23 18:49:21  gronej
 * fixed linux warnings
 *
 * Revision 1.2  2003/12/17 19:05:04  gronej
 * SNACC baseline merged with PER v1_7 tag
 *
 * Revision 1.1.2.1  2003/11/05 14:58:54  gronej
 * working PER code merged with esnacc_1_6
 *
 * Revision 1.1.1.1  2000/08/21 20:35:53  leonberp
 * First CVS Version of SNACC.
 *
 * Revision 1.2  1995/07/24 21:04:57  rj
 * changed `_' to `-' in file names.
 *
 * Revision 1.1  1994/08/28  09:46:09  rj
 * first check-in. for a list of changes to the snacc-1.1 distribution please refer to the ChangeLog.
 *
 */
#include "config.h"
#include "asn-config.h"
#include "str-stk.h"

/* global for use by AsnBits and AsnOcts */

StrStk strStkG = { NULL, 128, 0, 64, 0, 0 };





