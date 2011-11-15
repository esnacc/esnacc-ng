/*
 * compiler/core/lib_types.c - tag form/code and any refs info
 *
 *
 * Copyright (C) 1991, 1992 Michael Sample
 *            and the University of British Columbia
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * $Header: /baseline/SNACC/compiler/core/lib-types.c,v 1.7 2004/01/14 19:07:52 gronej Exp $
 * $Log: lib-types.c,v $
 * Revision 1.7  2004/01/14 19:07:52  gronej
 * Updated Compiler to accept and process relative-oid's
 *
 * Revision 1.6  2003/07/07 14:50:13  nicholar
 * Eliminated headers and cleaned up include references
 *
 * Revision 1.5  2002/10/28 19:57:03  leonberp
 * Added BITSTRING CONTAINING support and fixed CONTAINED ANY DEFINED BY
 *
 * Revision 1.4  2002/10/24 21:07:22  leonberp
 * latest fixing for OCTET CONTAINING
 *
 * Revision 1.3  2002/05/15 17:00:59  leonberp
 * added support for new basicTypes to compiler
 *
 * Revision 1.2  2002/05/15 14:53:12  leonberp
 * added support for new basicTypes to compiler
 *
 * Revision 1.1.1.1  2000/08/21 20:36:00  leonberp
 * First CVS Version of SNACC.
 *
 * Revision 1.3  1995/07/25 19:41:33  rj
 * changed `_' to `-' in file names.
 *
 * Revision 1.2  1994/09/01  00:37:51  rj
 * snacc_config.h removed.
 *
 * Revision 1.1  1994/08/28  09:49:14  rj
 * first check-in. for a list of changes to the snacc-1.1 distribution please refer to the ChangeLog.
 *
 */
#define NO_EXTN_LIBTYPE
#include "asn-incl.h"
#include "asn1module.h"  /* for BASICTYPE_... choice ids */
#include "lib-types.h"

/*
 * Warning: this table must be in order of ascending
 * BASICTYPE ids such that
 *    libTypesG[BASICTYPE_X].typeId == BASICTYPE_X
 * is alwas true
 */

/* BER Types */



LibType libBERTypesG[ BASICTYPE_RELATIVE_OID + 1] =
{
  { BASICTYPE_UNKNOWN,       NO_TAG_CODE,          NULL_FORM, NULL },
  { BASICTYPE_BOOLEAN,       BOOLEAN_TAG_CODE,     PRIM,      NULL },
  { BASICTYPE_INTEGER,       INTEGER_TAG_CODE,     PRIM,      NULL },
  { BASICTYPE_BITSTRING,     BITSTRING_TAG_CODE,   ANY_FORM,  NULL },
  { BASICTYPE_OCTETSTRING,   OCTETSTRING_TAG_CODE, ANY_FORM,  NULL },
  { BASICTYPE_NULL,          NULLTYPE_TAG_CODE,    PRIM,      NULL },
  { BASICTYPE_OID,           OID_TAG_CODE,         PRIM,      NULL },
  { BASICTYPE_REAL,          REAL_TAG_CODE,        PRIM,      NULL },
  { BASICTYPE_ENUMERATED,    ENUM_TAG_CODE,        PRIM,      NULL },
  { BASICTYPE_SEQUENCE,      SEQ_TAG_CODE,         CONS,      NULL },
  { BASICTYPE_SEQUENCEOF,    SEQ_TAG_CODE,         CONS,      NULL },
  { BASICTYPE_SET,           SET_TAG_CODE,         CONS,      NULL },
  { BASICTYPE_SETOF,         SET_TAG_CODE,         CONS,      NULL },
  { BASICTYPE_CHOICE,        NO_TAG_CODE,          CONS,      NULL },
  { BASICTYPE_SELECTION,     NO_TAG_CODE,          NULL_FORM, NULL },
  { BASICTYPE_COMPONENTSOF,  NO_TAG_CODE,          CONS,      NULL },
  { BASICTYPE_ANY,           NO_TAG_CODE,          CONS,      NULL },
  { BASICTYPE_ANYDEFINEDBY,  NO_TAG_CODE,          CONS,      NULL },
  { BASICTYPE_LOCALTYPEREF,  NO_TAG_CODE,          NULL_FORM, NULL },
  { BASICTYPE_IMPORTTYPEREF, NO_TAG_CODE,          NULL_FORM, NULL },
  { BASICTYPE_MACROTYPE,     NO_TAG_CODE,          NULL_FORM, NULL },
  { BASICTYPE_MACRODEF,      NO_TAG_CODE,          NULL_FORM, NULL },
  { BASICTYPE_NUMERIC_STR,	  NUMERICSTRING_TAG_CODE,		ANY_FORM, NULL },
  { BASICTYPE_PRINTABLE_STR, PRINTABLESTRING_TAG_CODE,	ANY_FORM, NULL },
  { BASICTYPE_UNIVERSAL_STR, UNIVERSALSTRING_TAG_CODE,	ANY_FORM, NULL },
  { BASICTYPE_IA5_STR,		  IA5STRING_TAG_CODE,			ANY_FORM, NULL },
  { BASICTYPE_BMP_STR,		  BMPSTRING_TAG_CODE,			ANY_FORM, NULL },
  { BASICTYPE_UTF8_STR,		  UTF8STRING_TAG_CODE,		   ANY_FORM, NULL },
  { BASICTYPE_UTCTIME,       UTCTIME_TAG_CODE,           ANY_FORM, NULL },
  { BASICTYPE_GENERALIZEDTIME, GENERALIZEDTIME_TAG_CODE, ANY_FORM, NULL },
  { BASICTYPE_GRAPHIC_STR,   GRAPHICSTRING_TAG_CODE,     ANY_FORM, NULL },
  { BASICTYPE_VISIBLE_STR,   VISIBLESTRING_TAG_CODE,     ANY_FORM, NULL },
  { BASICTYPE_GENERAL_STR,   GENERALSTRING_TAG_CODE,     ANY_FORM, NULL },
  { BASICTYPE_OBJECTDESCRIPTOR, OD_TAG_CODE, ANY_FORM, NULL},
  { BASICTYPE_VIDEOTEX_STR, VIDEOTEXSTRING_TAG_CODE, ANY_FORM, NULL },
  { BASICTYPE_T61_STR,		  TELETEXSTRING_TAG_CODE,		ANY_FORM, NULL },
  { BASICTYPE_EXTERNAL, EXTERNAL_TAG_CODE, CONS, NULL },
  { BASICTYPE_OCTETCONTAINING, OCTETSTRING_TAG_CODE, PRIM, NULL },
  { BASICTYPE_BITCONTAINING, BITSTRING_TAG_CODE, PRIM, NULL },
  { BASICTYPE_RELATIVE_OID, RELATIVE_OID_TAG_CODE, PRIM, NULL }
};


/* DER Types */
LibType libDERTypesG[ BASICTYPE_RELATIVE_OID + 1] =
{
  { BASICTYPE_UNKNOWN,       NO_TAG_CODE,          NULL_FORM, NULL },
  { BASICTYPE_BOOLEAN,       BOOLEAN_TAG_CODE,     PRIM,      NULL },
  { BASICTYPE_INTEGER,       INTEGER_TAG_CODE,     PRIM,      NULL },
  { BASICTYPE_BITSTRING,     BITSTRING_TAG_CODE,   PRIM,      NULL },
  { BASICTYPE_OCTETSTRING,   OCTETSTRING_TAG_CODE, PRIM,      NULL },
  { BASICTYPE_NULL,          NULLTYPE_TAG_CODE,    PRIM,      NULL },
  { BASICTYPE_OID,           OID_TAG_CODE,         PRIM,      NULL },
  { BASICTYPE_REAL,          REAL_TAG_CODE,        PRIM,      NULL },
  { BASICTYPE_ENUMERATED,    ENUM_TAG_CODE,        PRIM,      NULL },
  { BASICTYPE_SEQUENCE,      SEQ_TAG_CODE,         CONS,      NULL },
  { BASICTYPE_SEQUENCEOF,    SEQ_TAG_CODE,         CONS,      NULL },
  { BASICTYPE_SET,           SET_TAG_CODE,         CONS,      NULL },
  { BASICTYPE_SETOF,         SET_TAG_CODE,         CONS,      NULL },
  { BASICTYPE_CHOICE,        NO_TAG_CODE,          CONS,      NULL },
  { BASICTYPE_SELECTION,     NO_TAG_CODE,          NULL_FORM, NULL },
  { BASICTYPE_COMPONENTSOF,  NO_TAG_CODE,          CONS,      NULL },
  { BASICTYPE_ANY,           NO_TAG_CODE,          CONS,      NULL },
  { BASICTYPE_ANYDEFINEDBY,  NO_TAG_CODE,          CONS,      NULL },
  { BASICTYPE_LOCALTYPEREF,  NO_TAG_CODE,          NULL_FORM, NULL },
  { BASICTYPE_IMPORTTYPEREF, NO_TAG_CODE,          NULL_FORM, NULL },
  { BASICTYPE_MACROTYPE,     NO_TAG_CODE,          NULL_FORM, NULL },
  { BASICTYPE_MACRODEF,      NO_TAG_CODE,          NULL_FORM, NULL },
  { BASICTYPE_NUMERIC_STR,	  NUMERICSTRING_TAG_CODE,		ANY_FORM, NULL },
  { BASICTYPE_PRINTABLE_STR, PRINTABLESTRING_TAG_CODE,	ANY_FORM, NULL },
  { BASICTYPE_UNIVERSAL_STR, UNIVERSALSTRING_TAG_CODE,   ANY_FORM, NULL },
  { BASICTYPE_IA5_STR,		  IA5STRING_TAG_CODE,			ANY_FORM, NULL },
  { BASICTYPE_BMP_STR,		  BMPSTRING_TAG_CODE,			ANY_FORM, NULL },
  { BASICTYPE_UTF8_STR,		  UTF8STRING_TAG_CODE,		   ANY_FORM, NULL },
  { BASICTYPE_UTCTIME,       UTCTIME_TAG_CODE,           ANY_FORM, NULL },
  { BASICTYPE_GENERALIZEDTIME, GENERALIZEDTIME_TAG_CODE, ANY_FORM, NULL },
  { BASICTYPE_GRAPHIC_STR,   GRAPHICSTRING_TAG_CODE,     ANY_FORM, NULL },
  { BASICTYPE_VISIBLE_STR,   VISIBLESTRING_TAG_CODE,     ANY_FORM, NULL },
  { BASICTYPE_GENERAL_STR,   GENERALSTRING_TAG_CODE,     ANY_FORM, NULL },
  { BASICTYPE_OBJECTDESCRIPTOR, OD_TAG_CODE, ANY_FORM, NULL},
  { BASICTYPE_VIDEOTEX_STR, VIDEOTEXSTRING_TAG_CODE, ANY_FORM, NULL },
  { BASICTYPE_T61_STR,		  TELETEXSTRING_TAG_CODE,		ANY_FORM, NULL },
  { BASICTYPE_EXTERNAL, EXTERNAL_TAG_CODE, CONS, NULL },
  { BASICTYPE_OCTETCONTAINING, OCTETSTRING_TAG_CODE, PRIM, NULL },
  { BASICTYPE_BITCONTAINING, OCTETSTRING_TAG_CODE, PRIM, NULL },
  { BASICTYPE_RELATIVE_OID, RELATIVE_OID_TAG_CODE, PRIM, NULL }
};

/* Default is BER */
LibType *libTypesG = libBERTypesG;

