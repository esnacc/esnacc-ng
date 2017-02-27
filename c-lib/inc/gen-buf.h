/*
 * gen_buf.h  - flexible (runtime configurable) buffer mgmt stuff.
 *
 *  These are somewhat slower than the direct approach used in
 *  the compiled stuff.  Since tables are around 4x slower,
 *  the flexibility of the GenBufs can be justified.  This
 *  also allows one enc/dec library to support all buffer types.
 *
 * MS 93
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
 */


#ifndef _gen_buf_h_
#define _gen_buf_h_

#include "c-lib/inc/snacc.h"

#ifdef __cplusplus
extern "C" {
#endif

/*
 *  These are the standard buffer routines that the lib
 *  routines need. Note that the Peek routines have be
 *  added to the standard list - they are necessary
 *  to nicely support  the table oriented decoder.
 *  The "void *b" param's real type will be the buffer
 *  type that is used inside the GenBuf
 *  (ie ExpBuf ** have been defined).
 *
 *  Note that macros can not be used for these standard functions
 *  because the GenBuf keeps a pointer to these routines.
 *  Thus the exp_buf.[ch] and  files are somewhat
 *  differnt than those in snacc/c_lib and snacc/c_include
 *
 */
typedef int (*BufCopyAnyFcn) PROTO ((void *b,  void *value, unsigned long *bytesDecoded, ENV_TYPE env));
typedef unsigned char	(*BufGetByteFcn) PROTO ((void *b));
typedef unsigned char	*(*BufGetSegFcn) PROTO ((void *b, unsigned long *lenPtr));
typedef long 	(*BufCopyFcn) PROTO ((char *dst, void *b, unsigned long len));
typedef void		(*BufSkipFcn) PROTO ((void *b, unsigned long len));
typedef unsigned char	(*BufPeekByteFcn) PROTO ((void *b));
typedef unsigned char	*(*BufPeekSegFcn) PROTO ((void *b, unsigned long *lenPtr));
typedef long 	(*BufPeekCopyFcn) PROTO ((char *dst, void *b, unsigned long len));
typedef void		(*BufPutByteRvsFcn) PROTO ((void *b, unsigned char byte));
typedef void		(*BufPutSegRvsFcn) PROTO ((void *b, char *data, unsigned long len));
typedef int		(*BufReadErrorFcn) PROTO ((void *b));
typedef int		(*BufWriteErrorFcn) PROTO ((void *b));
typedef int		(*BufSetWriteErrorFcn) PROTO ((void *b, unsigned short Value));
typedef void (*BufResetInReadModeFcn) PROTO ((void *b));

typedef struct GenBuf
{
  BufCopyAnyFcn		copyAny;
  BufGetByteFcn		getByte;
  BufGetSegFcn		getSeg;
  BufCopyFcn		copy;
  BufSkipFcn		skip;
  BufPeekByteFcn	peekByte;
  BufPeekSegFcn		peekSeg;
  BufPeekCopyFcn	peekCopy;
  BufPutByteRvsFcn	putByteRvs;
  BufPutSegRvsFcn	putSegRvs;
  BufReadErrorFcn	readError;
  BufSetWriteErrorFcn setWriteError;
  BufWriteErrorFcn	writeError;
  BufResetInReadModeFcn resetInReadMode;
  void			*bufInfo;
  void			*spare; /* hack to save space for ExpBuf ** type */
} GenBuf;


static inline
int
GenBufCopyAny(GenBuf *b, void *value, unsigned long *bytesDecoded,
              ENV_TYPE env)
{
    return b->copyAny(b->bufInfo, value, bytesDecoded, env);
}

static inline
unsigned char
GenBufGetByte(GenBuf *b)
{
    return b->getByte(b->bufInfo);
}

static inline
unsigned char *
GenBufGetSeg(GenBuf *b, unsigned long *lenPtr)
{
    return b->getSeg(b->bufInfo, lenPtr);
}

static inline
long
GenBufCopy(char *dst, GenBuf *b, unsigned long len)
{
    return b->copy(dst, b->bufInfo, len);
}

static inline
void
GenBufSkip(GenBuf *b, unsigned long len)
{
    b->skip(b->bufInfo, len);
}

static inline
unsigned char
GenBufPeekByte(GenBuf *b)
{
    return b->peekByte(b->bufInfo);
}

static inline
unsigned char *
GenBufPeekSeg(GenBuf *b, unsigned long *lenPtr)
{
    return b->peekSeg(b->bufInfo, lenPtr);
}

static inline
long
GenBufPeekCopy(char *dst, GenBuf *b, unsigned long len)
{
    return b->peekCopy(dst, b->bufInfo, len);
}

static inline
void
GenBufPutByteRvs(GenBuf *b, unsigned char byte)
{
    b->putByteRvs(b->bufInfo, byte);
}

static inline
void
GenBufPutSegRvs(GenBuf *b, char *data, unsigned long len)
{
    b->putSegRvs(b->bufInfo, data, len);
}

static inline
int
GenBufReadError(GenBuf *b)
{
    return b->readError(b->bufInfo);
}

static inline
int
GenBufWriteError(GenBuf *b)
{
    return b->writeError(b->bufInfo);
}

static inline
int
GenBufSetWriteError(GenBuf *b, unsigned short value)
{
    return b->setWriteError(b->bufInfo, value);
}

static inline
void
GenBufResetInReadMode(GenBuf *b)
{
    b->resetInReadMode(b->bufInfo);
}

#define GenBufFree(b) free(b)

#ifdef __cplusplus
}
#endif


#endif /* _gen_buf_h_ conditional include  */


