/*
 * exp_buf.h - read/write/alloc/free routines for a simple buffer structure
 *
 * MACROS are gross but execution speed is important
 *
 * NOTE: replacing the malloc and free with a allocs/frees
 *       from/to buffer pools or similar tuned/fixed size
 *       mem mgmt will improve performance.
 *
 *  You should tune the buffer management to your environment
 *  for best results
 *
 * MS 91
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
 * $Header: /baseline/SNACC/c-lib/inc/junk.h,v 1.4 2003/12/17 19:05:03 gronej Exp $
 * $Log: junk.h,v $
 * Revision 1.4  2003/12/17 19:05:03  gronej
 * SNACC baseline merged with PER v1_7 tag
 *
 * Revision 1.3.2.1  2003/11/05 14:58:55  gronej
 * working PER code merged with esnacc_1_6
 *
 * Revision 1.3  2002/10/21 17:13:24  mcphersc
 * fixed long int
 *
 * Revision 1.2  2001/07/12 19:34:05  leonberp
 * Changed namespace to SNACC and added compiler options: -ns and -nons.  Also removed dead code.
 *
 * Revision 1.1.1.1  2000/08/21 20:35:54  leonberp
 * First CVS Version of SNACC.
 *
 * Revision 1.2  1995/07/27 08:54:45  rj
 * functions used by gen-bufs or type tables merged.
 *
 * changed `_' to `-' in file names.
 *
 * Revision 1.1  1994/08/28  09:21:40  rj
 * first check-in. for a list of changes to the snacc-1.1 distribution please refer to the ChangeLog.
 *
 */

#ifndef _exp_buf_h_
#define _exp_buf_h_

typedef struct ExpBuf
{
    char          *dataStart; /* points to first valid data byte */
                              /* when empty, 1 byte past blk end (rvs write)*/
    char          *dataEnd;   /* pts to first byte AFTER last valid data byte*/
    char          *curr;      /* current location to read form */
                              /* points to next byte to read */
    struct ExpBuf *next;      /* next buf (NULL if no next buffer)*/
    struct ExpBuf *prev;      /* prev buf (NULL if no prev buffer)*/
    char          *blkStart;  /* points to first byte of the blk */
    char          *blkEnd;    /* points the first byte AFTER blks last byte */
    int            readError; /* non-zero is attempt to read past end of data*/
    int            writeError;/* non-zero is attempt write fails (no mor bufs)*/
} ExpBuf;



/* init, alloc and free routines */
#if defined (DEBUG) /* use fcns when debugging/macros later */ || defined (USE_GEN_BUF)

#ifdef USE_GEN_BUF
void		PutExpBufInGenBuf PROTO ((ExpBuf *eb,GenBuf *gb));
#endif

void		ExpBufInit PROTO ((unsigned long dataBlkSize));
ExpBuf		*ExpBufAllocBuf();
void		ExpBufFreeBuf PROTO ((ExpBuf *ptr));
char		*ExpBufAllocData();
void		ExpBufFreeData PROTO ((char *ptr));
void		ExpBufFreeBufAndData PROTO (( ExpBuf *b));

ExpBuf		*ExpBufNext PROTO ((ExpBuf *b));
ExpBuf		*ExpBufPrev PROTO ((ExpBuf *b));
void		ExpBufResetInReadMode PROTO ((ExpBuf *b));
void		ExpBufResetInWriteRvsMode PROTO ((ExpBuf *b));

int		ExpBufAtEod PROTO ((ExpBuf *b));
int		ExpBufFull PROTO ((ExpBuf *b));
int		ExpBufHasNoData PROTO ((ExpBuf *b));
unsigned long	ExpBufDataSize PROTO ((ExpBuf *b));
unsigned long	ExpBufDataBlkSize PROTO ((ExpBuf *b));
char		*ExpBufDataPtr PROTO ((ExpBuf *b));

#else

extern unsigned long expBufDataBlkSizeG;

#define ExpBufInit( size)		expBufDataBlkSizeG = size;
#define ExpBufAllocBuf()		((ExpBuf *)malloc (sizeof (ExpBuf)))
#define ExpBufFreeBuf( ptr)		free (ptr)
#define ExpBufAllocData()		((void *)malloc (expBufDataBlkSizeG))
#define ExpBufFreeData( ptr)		free (ptr)
#define ExpBufFreeBufAndData( b)	{ ExpBufFreeData ((b)->blkStart); ExpBufFreeBuf (b); }
#define ExpBufNext( b)			((b)->next)
#define ExpBufPrev( b)			((b)->prev)
#define ExpBufResetInReadMode( b)	{ (b)->curr = (b)->dataStart; (b)->readError = 0; (b)->writeError = 1; }
#define ExpBufResetInWriteRvsMode( b)	{ (b)->dataStart = (b)->dataEnd = (b)->blkEnd; (b)->writeError = 0; (b)->readError = 1; }

/* ExpBufAtEod only valid during reads (fwd) */
#define ExpBufAtEod( b)			((b)->curr == (b)->dataEnd)

/* ExpBufFull only valid during write (reverse) */
#define ExpBufFull( b)			((b)->dataStart == (b)->blkStart)
#define ExpBufHasNoData( b)		((b)->dataStart == (b)->dataEnd)
#define ExpBufDataSize( b)		((b)->dataEnd - (b)->dataStart)
#define ExpBufDataBlkSize( b)		((b)->blkEnd - (b)->blkStart)
#define ExpBufDataPtr( b)		(ExpBufHasNoData (b)? NULL: (b)->dataStart)

#endif  /* DEBUG || USE_GEN_BUF */

#ifdef USE_GEN_BUF
int           ExpBufReadError PROTO ((ExpBuf **b));
int           ExpBufWriteError PROTO ((ExpBuf **b));
#else
#define ExpBufReadError( b)		((*b)->readError)
#define ExpBufWriteError( b)		((*b)->writeError)
#endif

ExpBuf		*ExpBufAllocBufAndData();
void		ExpBufInstallDataInBuf PROTO ((ExpBuf *b, char *data, unsigned long len));
void		ExpBufFreeBufAndDataList PROTO (( ExpBuf *b));
ExpBuf		*ExpBufListLastBuf PROTO ((ExpBuf *b));
ExpBuf		*ExpBufListFirstBuf PROTO ((ExpBuf *b));

void ExpBufCopyToFile PROTO ((ExpBuf *b, FILE *f));

/* reading and writing routines */

void		ExpBufSkip PROTO (( ExpBuf**, unsigned long len));
int		ExpBufCopy PROTO (( char *dst, ExpBuf **b, unsigned long len));
unsigned char	ExpBufPeekByte PROTO (( ExpBuf **b));
#if TTBL
int		ExpBufPeekCopy PROTO ((char *dst, ExpBuf **b, unsigned long len));
char		*ExpBufPeekSeg PROTO ((ExpBuf **b, unsigned long *len));
#endif
char		*ExpBufGetSeg PROTO ((ExpBuf **b, unsigned long *len));
void		ExpBufPutSegRvs PROTO ((ExpBuf **b, char *data, unsigned long len));
unsigned char	ExpBufGetByte PROTO ((ExpBuf **b));
void		ExpBufPutByteRvs PROTO ((ExpBuf **b, unsigned char byte));

#endif /* conditional include */

