#ifndef TBL_DBG_H
#define TBL_DBG_H

#include "c-lib/inc/tbl-gen.h"

extern TdeExceptionCode DBGMinCode; /* Defaults to TDEINFO */
int DBGSimple PROTO ((AsnTag tag, AsnOcts* v, int begin));
int DBGType PROTO ((TBLType* type, AVal* val, int begin));
int DBGExc PROTO ((TdeExceptionCode code, void* p1, void* p2, void* p3));

#endif

