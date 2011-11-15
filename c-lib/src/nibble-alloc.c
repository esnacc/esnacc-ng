/*
 * .../c-lib/src/nibble-alloc.c - fast mem allocation for decoded values
 *
 * MS Dec 31/91
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
 * $Header: /baseline/SNACC/c-lib/src/nibble-alloc.c,v 1.6 2003/12/17 19:05:04 gronej Exp $
 * $Log: nibble-alloc.c,v $
 * Revision 1.6  2003/12/17 19:05:04  gronej
 * SNACC baseline merged with PER v1_7 tag
 *
 * Revision 1.5.2.1  2003/11/05 14:58:54  gronej
 * working PER code merged with esnacc_1_6
 *
 * Revision 1.5  2002/10/18 13:09:32  mcphersc
 * took out long int to unsigned long
 *
 * Revision 1.4  2002/05/10 16:39:37  leonberp
 * latest changes for release 2.2
 * includes integrating asn-useful into C & C++ runtime library, the compiler changes that go along with that, SnaccException changes for C++ runtime and compiler
 *
 * Revision 1.3  2001/03/12 14:52:12  mcphersc
 * Added check for nmG being null before goint in the for loop in ShutdownNibbleMem
 * function.
 *
 * Revision 1.2  2000/10/16 18:10:40  rwc
 * removed most warnings from C++-lib, some C-lib.
 *
 * Revision 1.1.1.1  2000/08/21 20:35:52  leonberp
 * First CVS Version of SNACC.
 *
 * Revision 1.4  1997/02/28 13:39:51  wan
 * Modifications collected for new version 1.3: Bug fixes, tk4.2.
 *
 * Revision 1.3  1995/07/27 09:06:37  rj
 * use memzero that is defined in .../snacc.h to use either memset or bzero.
 *
 * changed `_' to `-' in file names.
 *
 * Revision 1.2  1994/09/01  00:07:16  rj
 * more portable .h file inclusion.
 *
 * Revision 1.1  1994/08/28  09:46:07  rj
 * first check-in. for a list of changes to the snacc-1.1 distribution please refer to the ChangeLog.
 *
 *
 * Revision - 23 Jan 98 - DAD -
 * added a check to NibbleAlloc() for detecting requests for
 * "questionable" amounts of memory
 *  
 */


#include <malloc.h>
#include <string.h>
#include "asn-config.h"
#include "nibble-alloc.h"


NibbleMem *nmG = NULL;

void
InitNibbleMem PARAMS ((initialSize, incrementSize),
    unsigned long initialSize _AND_
    unsigned long incrementSize)
{
    NibbleMem *nm;

    nm = (NibbleMem*) malloc (sizeof (NibbleMem));
    nm->incrementSize = incrementSize;

    nm->currNibbleBuf = nm->firstNibbleBuf = (NibbleBuf*)malloc (sizeof (NibbleBuf));
    nm->firstNibbleBuf->curr = nm->firstNibbleBuf->start = (char*) malloc (initialSize);
    nm->firstNibbleBuf->end = nm->firstNibbleBuf->start + initialSize;
    nm->firstNibbleBuf->next = NULL;
    memzero (nm->currNibbleBuf->start, initialSize);

    nmG = nm;/* set global */

}  /* InitNibbleAlloc */


/*
 * alloc new nibble buf, link in, reset to curr nibble buf
 */
void
ServiceNibbleFault PARAMS ((size),
    unsigned long size)
{
    NibbleMem *nm;
    unsigned long newBufSize;

    nm = nmG;

    if (size > nm->incrementSize)
        newBufSize = size;
    else
        newBufSize = nm->incrementSize;

    nm->currNibbleBuf->next = (NibbleBuf*) malloc (sizeof (NibbleBuf));
    nm->currNibbleBuf = nm->currNibbleBuf->next;
    nm->currNibbleBuf->curr = nm->currNibbleBuf->start = (char*) malloc (newBufSize);
    nm->currNibbleBuf->end = nm->currNibbleBuf->start + newBufSize;
    nm->currNibbleBuf->next = NULL;
    memzero (nm->currNibbleBuf->start, newBufSize);
} /* serviceNibbleFault */



/*
 * returns requested space filled with zeros
 */
void*
NibbleAlloc PARAMS ((size),
    unsigned long size)
{
    NibbleMem *nm;
    char *retVal;
    unsigned long ndiff;

    nm = nmG;
    
    /* DAD - although error checking on the mallocs during
     * ServiceNibbleFault could be more robust, for now i'm
     * just going to avoid allocating really huge amounts
     * of memory (which can occur if the ASN.1 data has
     * become corrupted, and you were trying to decode).
     */
    if(size > 1024*1024)	/* say roughly a meg for now */
    	return(0);

    if ((nm->currNibbleBuf->end - nm->currNibbleBuf->curr) < (int)size)
         ServiceNibbleFault (size);

    retVal = nm->currNibbleBuf->curr;

    /*
     * maintain word alignment
     */
    ndiff = size % sizeof (long);
    if (ndiff != 0)
    {
        nm->currNibbleBuf->curr += size + sizeof (long) - ndiff;

        /*
         * this is a fix from Terry Sullivan <FCLTPS@nervm.nerdc.ufl.edu>
         *
         * makes sure curr does not go past the end ptr
         */
        if (nm->currNibbleBuf->curr > nm->currNibbleBuf->end)
            nm->currNibbleBuf->curr = nm->currNibbleBuf->end;
    }
    else
        nm->currNibbleBuf->curr += size;

    return retVal;
}  /* NibbleAlloc */



/*
 * frees all nibble buffers except the first,
 * resets the first to empty and zero's it
 */
void
ResetNibbleMem()
{
    NibbleMem *nm;
    NibbleBuf *tmp;
    NibbleBuf *nextTmp;

    nm = nmG;

    /*
     * reset first nibble buf
     */
    memzero (nm->firstNibbleBuf->start, nm->firstNibbleBuf->curr - nm->firstNibbleBuf->start);

    nm->firstNibbleBuf->curr = nm->firstNibbleBuf->start;

    /*
     * free incrementally added nibble bufs
     */
    for (tmp = nm->firstNibbleBuf->next; tmp != NULL; )
    {
        free (tmp->start);
        nextTmp = tmp->next;
        free (tmp);
        tmp = nextTmp;
    }

    /* From ftp://ftp.cs.ubc.ca/pub/local/src/snacc/bugs-in-1.1 */
    nm->firstNibbleBuf->next = NULL;
    nm->currNibbleBuf = nm->firstNibbleBuf;

} /* ResetNibbleMem */


/*
 * frees all nibble buffers, closing this
 * NibbleMem completely
 */
void
ShutdownNibbleMem()
{
    NibbleMem *nm;
    NibbleBuf *tmp;
    NibbleBuf *nextTmp;

    nm = nmG;
    nmG = NULL;
    /*
     * free nibble bufs
     */
	if (nm == NULL)
		return;
    for (tmp = nm->firstNibbleBuf; tmp != NULL; )
    {
        free (tmp->start);
        nextTmp = tmp->next;
        free (tmp);
        tmp = nextTmp;
    }

    free (nm);
} /* ShutdownNibbleMem */
