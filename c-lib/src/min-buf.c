#include "config.h"
#include "snacc.h"
#include "asn-config.h"
#include "min-buf.h"
#include "gen-buf.h"
#include <string.h>

static unsigned char *
MinBufGetSeg__(void *b, unsigned long *len)
{
    unsigned char *i = (unsigned char *)(*(unsigned char **)b);
    *(unsigned char **)b = i + *len;
    return i;
}

static unsigned char
MinBufGetByte__(void *b)
{
    unsigned long l = 1;
    return *MinBufGetSeg__(b, &l);
}

static long
MinBufCopy__(char *dst, void *b, unsigned long len)
{
    char *src = MinBufGetSeg__(b, &len);
    memcpy(dst, src, len);
    return len;
}

static void
MinBufSkip__(void *b, unsigned long len)
{
    MinBufGetSeg__(b, &len);
}

static unsigned char *
MinBufPeekSeg__(void *b, unsigned long *len)
{
    return (unsigned char *)(*(unsigned char **)b);
}

static unsigned char
MinBufPeekByte__(void *b)
{
    unsigned long i = 1;
    return *MinBufPeekSeg__(b, &i);
}

static long
MinBufPeekCopy__(char *dst, void *b, unsigned long len)
{
    unsigned char *src = MinBufPeekSeg__(b, &len);
    memcpy(dst, src, len);
    return len;
}

static void
MinBufPutSegRvs__(void *b, char *src, unsigned long len)
{
    unsigned char *dst = (unsigned char *)(*(unsigned char **)b);
    dst -= len;
    memcpy(dst, src, len);
}

static void
MinBufPutByteRvs__(void *b, unsigned char byte)
{
    MinBufPutSegRvs__(b, &byte, 1);
}

static int
MinBufIgnore__(void *b)
{
    return 0;
}

static int
MinBufSetWriteError__(void *b, unsigned short i)
{
    return 0;
}

static void
MinBufReset__(void *b)
{
}

static int
MinBufCopyAny__(void *b,  void *value, unsigned long *bytesDecoded,
                ENV_TYPE env)
{
    AsnLen elmtsLen, totalElmtsLen;
    AsnOcts *data;
    GenBuf gb;
    char *loc = 0;

    GenBufFromMinBuf(&gb, b);
    loc = (char *)GenBufPeekSeg(&gb, 0);

    elmtsLen = BDecLen (&gb, &totalElmtsLen, env);	/* len of item */
    if (elmtsLen == INDEFINITE_LEN) {
        Asn1Error("BDecUnknownAsnAny: ERROR - indef length object found\n");
        longjmp(env, -910);
    }

    data = (AsnOcts *) value; /* allocated by the any routine */
    data->octetLen = elmtsLen + totalElmtsLen; /* tag+len+data lengths */
    data->octs = Asn1Alloc(data->octetLen +1);

    gb.spare = loc;
    MinBufCopy__(data->octs, gb.bufInfo, totalElmtsLen+elmtsLen);

    /* add null terminator - this is not included in the str's len */
    data->octs[data->octetLen] = '\0';
    (*bytesDecoded) += data->octetLen;
    return 0;
}

static struct GenBuf minBufOpsG =
{
    MinBufCopyAny__,
    MinBufGetByte__,
    MinBufGetSeg__,
    MinBufCopy__,
    MinBufSkip__,
    MinBufPeekByte__,
    MinBufPeekSeg__,
    MinBufPeekCopy__,
    MinBufPutByteRvs__,
    MinBufPutSegRvs__,
    MinBufIgnore__,
    MinBufSetWriteError__,
    MinBufIgnore__,
    MinBufReset__,
    NULL,
    NULL,
};

void
GenBufFromMinBuf PARAMS ((gb, data),
                         GenBuf *gb _AND_
                         void *data)
{
    *gb = minBufOpsG;
    gb->spare = data;
    gb->bufInfo = (void *) &gb->spare;
}
