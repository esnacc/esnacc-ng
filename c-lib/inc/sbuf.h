/*
 * sbuf.h  - a buffer consisting of one contiguous block
 *               that checks for read and write range errors.
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
 * $Header: /baseline/SNACC/c-lib/inc/sbuf.h,v 1.10 2003/12/17 19:05:03 gronej Exp $
 * $Log: sbuf.h,v $
 * Revision 1.10  2003/12/17 19:05:03  gronej
 * SNACC baseline merged with PER v1_7 tag
 *
 * Revision 1.9.2.1  2003/11/05 14:58:55  gronej
 * working PER code merged with esnacc_1_6
 *
 * Revision 1.9  2003/02/20 21:07:59  leonberp
 * added #ifdef __cplusplus extern "C" to headers
 *
 * Revision 1.8  2002/10/24 14:51:56  mcphersc
 * fixed SBufCopy prototype
 *
 * Revision 1.7  2002/10/24 12:02:37  mcphersc
 * Modified prototypes
 *
 * Revision 1.6  2002/10/23 17:55:33  mcphersc
 * Bug fixes
 *
 * Revision 1.5  2002/10/22 17:46:59  mcphersc
 * fixed prototypes
 *
 * Revision 1.4  2002/10/22 15:49:08  mcphersc
 * Mods for gen-buf usage
 *
 * Revision 1.3  2002/10/21 17:13:24  mcphersc
 * fixed long int
 *
 * Revision 1.2  2001/07/12 19:34:06  leonberp
 * Changed namespace to SNACC and added compiler options: -ns and -nons.  Also removed dead code.
 *
 * Revision 1.1.1.1  2000/08/21 20:35:54  leonberp
 * First CVS Version of SNACC.
 *
 * Revision 1.2  1995/07/27 08:54:46  rj
 * functions used by gen-bufs or type tables merged.
 *
 * changed `_' to `-' in file names.
 *
 * Revision 1.1  1994/08/28  09:45:39  rj
 * first check-in. for a list of changes to the snacc-1.1 distribution please refer to the ChangeLog.
 *
 */

#ifndef _sbuf_h_
#define _sbuf_h_


#ifdef __cplusplus
extern "C" {
#endif

typedef struct SBuf
{
    char *dataStart;  /* byte last written (or end) */
    char *dataEnd;    /* ptr to first byte after last valid data byte */
    char *blkStart;   /* ptr to first byte of the buffer */
    char *blkEnd;     /* ptr to first byte past end of the buffer */
    char *readLoc;    /* next byte to read (or end) */
    int writeError;   /* whether write error occurred */
    int readError;    /* whether read error occurred */
} SBuf;

void		SBuftoGenBuf PROTO ((SBuf *eb,GenBuf **gb));
int			SBufPeekCopy PROTO ((char *dst, SBuf **b, unsigned long copyLen));
void		PutSBufInGenBuf PROTO ((SBuf *sb, GenBuf *gb));
void		SBufInit PROTO ((SBuf *b, char *data, long dataLen));
void		SBufResetInReadMode PROTO ((SBuf **b));
void		SBufResetInWriteRvsMode PROTO ((SBuf *b));
void		SBufInstallData PROTO ((SBuf *b, char *data, long dataLen));
long		SBufDataLen PROTO ((SBuf *b));
char		*SBufDataPtr PROTO ((SBuf *b));
long		SBufBlkLen PROTO ((SBuf *b));
char		*SBufBlkPtr PROTO ((SBuf *b));
int			SBufEod PROTO ((SBuf *b));
int			SBufReadError PROTO ((SBuf **b));
int			SBufWriteError PROTO ((SBuf **b));
void		SBufSkip PROTO ((SBuf **b, long skipLen));
void		SBufCopy PROTO ((char *dst, SBuf **b, long copyLen));
unsigned char	SBufPeekByte PROTO ((SBuf **b));
char		*SBufGetSeg PROTO ((SBuf **b,long *lenPtr));
void		SBufPutSegRvs PROTO ((SBuf **b, char *seg, long segLen));
unsigned char	SBufGetByte PROTO ((SBuf **b));
char		*SBufPeekSeg PROTO ((SBuf **b, long *lenPtr));
void		SBufPutByteRvs PROTO ((SBuf **b, unsigned char byte));
int			SBufSetWriteError PROTO ((SBuf *b, unsigned short Value));
int			SBufCopyAny PROTO ((SBuf *b, void *value, unsigned long *bytesDecoded, ENV_TYPE env));

#ifdef __cplusplus
}
#endif

#endif /* conditional include */

