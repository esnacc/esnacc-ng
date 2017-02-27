#ifndef TBL_GEN_H
#define TBL_GEN_H

#define USE_GEN_BUF 1
#include "c-lib/inc/tbl-incl.h"

typedef enum {TDEINFO, TDEEOC=TDEINFO, TDEPEEKTAG, TDEPUSHTAG,
	TDEWARNING, TDEUNEXPECTED=TDEWARNING, TDENONOPTIONAL, TDEMANDATORY,
	TDECONSTRAINT, TDENOMATCH,
	TDEERROR} TdeExceptionCode;

typedef int (*TdeTypeProc) PROTO ((TBLType* type, AVal* val, int begin));
typedef int (*TdeSimpleProc) PROTO ((AsnTag tag, AsnOcts* val, int begin));
typedef int (*TdeExcProc) PROTO ((TdeExceptionCode code, void* p1, void* p2, void* p3));

int
TdeDecode PROTO ((TBL* tbl, GenBuf *b, unsigned long* bytesDecoded,
    	TdeTypeProc typeproc, TdeSimpleProc simpleproc, TdeExcProc excproc));

int
TdeDecodeSpecific PROTO ((TBL* tbl, GenBuf *b, TBLType* type,
	unsigned long* bytesDecoded,
    	TdeTypeProc typeproc, TdeSimpleProc simpleproc, TdeExcProc excproc));
#endif

