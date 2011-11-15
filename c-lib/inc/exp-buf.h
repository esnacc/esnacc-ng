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
 * $Header: /baseline/SNACC/c-lib/inc/exp-buf.h,v 1.10 2003/12/17 19:05:03 gronej Exp $
 * $Log: exp-buf.h,v $
 * Revision 1.10  2003/12/17 19:05:03  gronej
 * SNACC baseline merged with PER v1_7 tag
 *
 * Revision 1.9.2.1  2003/11/05 14:58:55  gronej
 * working PER code merged with esnacc_1_6
 *
 * Revision 1.9  2003/08/04 10:34:17  colestor
 * Updated several improperly referenced "b" buffer parameters when dealing with
 * ANY load/unloads (encode/decodes).  This code has never been tested in the
 * "C" library.
 * Also, added DEnc[Dec]AsnAny references to be BEnc[Dec]AsnAny.
 *
 * Revision 1.8  2003/02/20 21:07:59  leonberp
 * added #ifdef __cplusplus extern "C" to headers
 *
 * Revision 1.7  2002/10/23 13:33:23  mcphersc
 * Took GenBuf defines out
 *
 * Revision 1.6  2002/10/22 15:49:25  mcphersc
 * Mods for gen-buf usage
 *
 * Revision 1.5  2002/10/18 13:10:32  mcphersc
 * took out long int to unsigned long
 *
 * Revision 1.4  2002/10/15 17:33:34  mcphersc
 * Changed der structure to accept GenBufs
 *
 * Revision 1.3  2002/10/01 14:18:08  mcphersc
 * ASN "C" Buf modifications
 *
 * Revision 1.2  2001/07/12 19:34:04  leonberp
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


#ifdef __cplusplus
extern "C" {
#endif

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

void		PutExpBufInGenBuf PROTO ((ExpBuf *eb,GenBuf *gb));
void		ExpBuftoGenBuf PROTO ((ExpBuf *eb,GenBuf **gb));

void		ExpBufInit PROTO ((unsigned long dataBlkSize));
ExpBuf		*ExpBufAllocBuf();
void		ExpBufFreeBuf PROTO ((ExpBuf *ptr));
char		*ExpBufAllocData();
void		ExpBufFreeData PROTO ((char *ptr));
void		ExpBufFreeBufAndData PROTO (( ExpBuf *b));

ExpBuf		*ExpBufNext PROTO ((ExpBuf *b));
ExpBuf		*ExpBufPrev PROTO ((ExpBuf *b));
void		ExpBufResetInReadMode PROTO ((ExpBuf **b));
void		ExpBufResetInWriteRvsMode PROTO ((ExpBuf *b));

int		ExpBufAtEod PROTO ((ExpBuf *b));
int		ExpBufFull PROTO ((ExpBuf *b));
int		ExpBufHasNoData PROTO ((ExpBuf *b));
unsigned long	ExpBufDataSize PROTO ((ExpBuf *b));
unsigned long	ExpBufDataBlkSize PROTO ((ExpBuf *b));
char		*ExpBufDataPtr PROTO ((ExpBuf *b));


extern unsigned long expBufDataBlkSizeG;



int           ExpBufReadError PROTO ((ExpBuf **b));
int           ExpBufWriteError PROTO ((ExpBuf **b));
int			  ExpBufSetWriteError PROTO ((ExpBuf *b, unsigned short Value));

ExpBuf		*ExpBufAllocBufAndData();
void		ExpBufInstallDataInBuf PROTO ((ExpBuf *b, char *data, unsigned long len));
void		ExpBufFreeBufAndDataList PROTO (( ExpBuf *b));
ExpBuf		*ExpBufListLastBuf PROTO ((ExpBuf *b));
ExpBuf		*ExpBufListFirstBuf PROTO ((ExpBuf *b));

void ExpBufCopyToFile PROTO ((ExpBuf *b, FILE *f));

/* reading and writing routines */

int			ExpBufCopyAny PROTO ((ExpBuf **b,void *value, unsigned long *bytesDecoded, ENV_TYPE env));
void		ExpBufSkip PROTO (( ExpBuf**, unsigned long len));
int			ExpBufCopy PROTO (( char *dst, ExpBuf **b, unsigned long len));
unsigned char	ExpBufPeekByte PROTO (( ExpBuf **b));
int			ExpBufPeekCopy PROTO ((char *dst, ExpBuf **b, unsigned long len));
char		*ExpBufPeekSeg PROTO ((ExpBuf **b, unsigned long *len));
char		*ExpBufGetSeg PROTO ((ExpBuf **b, unsigned long *len));
void		ExpBufPutSegRvs PROTO ((ExpBuf **b, char *data, unsigned long len));
unsigned char	ExpBufGetByte PROTO ((ExpBuf **b));
void		ExpBufPutByteRvs PROTO ((ExpBuf **b, unsigned char byte));


#ifdef __cplusplus
}
#endif

#endif /* conditional include */

