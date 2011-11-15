/*
 * print.h
 *
 * MS 92
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
 * $Header: /baseline/SNACC/c-lib/inc/print.h,v 1.7 2004/01/22 20:02:58 nicholar Exp $
 */

#ifndef _snaccc_print_h_
#define _snaccc_print_h_

#ifdef __cplusplus
extern "C" {
#endif


void Indent PROTO ((FILE *f, unsigned int i));

void Asn1DefaultErrorHandler PROTO ((char* str,int severity));


#ifdef __cplusplus
}
#endif

#endif /* conditional include */

