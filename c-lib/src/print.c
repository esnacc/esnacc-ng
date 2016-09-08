/*
 * print.c - library routines for printing ASN.1 values.
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
 * $Header: /baseline/SNACC/c-lib/src/print.c,v 1.8 2004/01/22 20:03:12 nicholar Exp $
 *
 */

#include "asn-config.h"
#include "print.h"


void
Indent PARAMS ((f, i),
    FILE *f _AND_
    unsigned int i)
{
    for (; i > 0; i--)
        fputs ("   ", f);
}

void Asn1DefaultErrorHandler PARAMS ((str, severity),
    char* str ESNACC_UNUSED _AND_
    int severity ESNACC_UNUSED)
{
  /*  fprintf(stderr,"%s",str); DAD - temp removing for now*/
}

static Asn1ErrorHandler asn1CurrentErrorHandler = Asn1DefaultErrorHandler;

void
Asn1Error PARAMS ((str),
    char* str)
{
    (*asn1CurrentErrorHandler)(str,1);
}

void
Asn1Warning PARAMS ((str),
    char* str)
{
    (*asn1CurrentErrorHandler)(str,0);
}

Asn1ErrorHandler
Asn1InstallErrorHandler PARAMS ((handler),
    Asn1ErrorHandler handler)
{
    Asn1ErrorHandler former = asn1CurrentErrorHandler;
    asn1CurrentErrorHandler = handler;
    return former;
}

