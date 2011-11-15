/*
 * compiler/back_ends/c++_gen/rules.c - initialized c rule structure
 *           inits a table that contains info about
 *           converting each ASN.1 type to a C++ class
 *
 * Copyright (C) 1991, 1992 Michael Sample
 *           and the University of British Columbia
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * $Header: /baseline/SNACC/compiler/back-ends/c++-gen/rules.c,v 1.12 2004/02/20 19:36:05 gronej Exp $
 * $Log: rules.c,v $
 * Revision 1.12  2004/02/20 19:36:05  gronej
 * Updated compiler to support extensibility in set's sequences
 * Eliminated unreferenced warnings for extraLen and elmtLen in produced code
 *
 * Revision 1.11  2004/02/12 18:55:09  gronej
 * Stable SNACC
 * non optional choices, sets and sequences are now not pointers
 * merged with list code
 * all memory leaks within SNACC fixed
 *
 * Revision 1.10  2004/01/29 21:21:44  gronej
 * Took all Init() calls out of Clear() functions in generated code
 *
 * Revision 1.9  2004/01/14 19:07:53  gronej
 * Updated Compiler to accept and process relative-oid's
 *
 * Revision 1.8  2003/07/07 14:52:35  nicholar
 * Eliminated headers and cleaned up include references
 *
 * Revision 1.7  2002/10/28 19:57:02  leonberp
 * Added BITSTRING CONTAINING support and fixed CONTAINED ANY DEFINED BY
 *
 * Revision 1.6  2002/10/24 21:07:21  leonberp
 * latest fixing for OCTET CONTAINING
 *
 * Revision 1.5  2002/09/04 17:51:34  vracarl
 * got rid of c++ comments
 *
 * Revision 1.4  2002/05/15 17:00:57  leonberp
 * added support for new basicTypes to compiler
 *
 * Revision 1.3  2002/05/15 14:53:12  leonberp
 * added support for new basicTypes to compiler
 *
 * Revision 1.2  2002/05/10 16:39:39  leonberp
 * latest changes for release 2.2
 * includes integrating asn-useful into C & C++ runtime library, the compiler changes that go along with that, SnaccException changes for C++ runtime and compiler
 *
 * Revision 1.1.1.1  2000/08/21 20:36:06  leonberp
 * First CVS Version of SNACC.
 *
 * Revision 1.4  1995/07/25 18:24:13  rj
 * file name has been shortened for redundant part: c++-gen/c++-rules -> c++-gen/rules.
 *
 * changed `_' to `-' in file names.
 *
 * Revision 1.3  1994/10/08  03:47:49  rj
 * since i was still irritated by cpp standing for c++ and not the C preprocessor, i renamed them to cxx (which is one known suffix for C++ source files). since the standard #define is __cplusplus, cplusplus would have been the more obvious choice, but it is a little too long.
 *
 * Revision 1.2  1994/09/01  01:53:33  rj
 * snacc_config.h removed.
 *
 * Revision 1.1  1994/08/28  09:47:54  rj
 * first check-in. for a list of changes to the snacc-1.1 distribution please refer to the ChangeLog.
 *
 */

#include "asn-incl.h"
#include "asn1module.h"
#include "rules.h"


CxxRules cxxRulesG =
{
   4,
   "choiceId",
   "ChoiceIdEnum",
   "a",
   "ChoiceUnion",
   FALSE,
   "Enc",
   "Dec",
   "EncContent",
   "DecContent",
   "EncPdu",
   "DecPdu",
   {
      {  /* 0 */
         BASICTYPE_UNKNOWN, 
         "???",
         FALSE,
         FALSE,
         FALSE,
         TRUE,
         TRUE,
         TRUE,
         TRUE,
         "NOT_NULL",
         "unknown"
      },
      { /* 1 */
         BASICTYPE_BOOLEAN, /* 1 */
         "AsnBool",
         FALSE,
         TRUE,
         FALSE,
         TRUE,
         TRUE,
         FALSE,
         TRUE,
         "NOT_NULL",
         "bool"
      },
      {  /* 2 */
         BASICTYPE_INTEGER, /* 2 */
         "AsnInt",
         FALSE,
         TRUE,
         FALSE,
         TRUE,
         TRUE,
         FALSE,
         TRUE,
         "NOT_NULL",
         "integer"
      },
      { /* 3 */
         BASICTYPE_BITSTRING,
         "AsnBits",
         FALSE,
         TRUE,
         FALSE,
         TRUE,
         TRUE,
         FALSE,
         TRUE,
         "NOT_NULL",
         "bits"
      },
      {/* 4 */
         BASICTYPE_OCTETSTRING,
         "AsnOcts",
         FALSE,
         TRUE,
         FALSE,
         TRUE,
         TRUE,
         FALSE,
         TRUE,
         "NOT_NULL",
         "octs"
      },
      {/* 5 */
         BASICTYPE_NULL,
         "AsnNull",
         FALSE,
         TRUE,
         FALSE,
         TRUE,
         TRUE,
         FALSE,
         TRUE,
         "NOT_NULL",
         "null"
      },
      { /* 6 */
         BASICTYPE_OID,
         "AsnOid",
         FALSE,
         TRUE,
         FALSE,
         TRUE,
         TRUE,
         FALSE,
         TRUE,
         "NOT_NULL",
         "oid"
      },
      { /* 7 */
         BASICTYPE_REAL,
         "AsnReal",
         FALSE,
         TRUE,
         FALSE,
         TRUE,
         TRUE,
         FALSE,
         TRUE,
         "NOT_NULL",
         "real"
      },
      {/* 8 */
         BASICTYPE_ENUMERATED,
         "AsnEnum",
         FALSE,
         TRUE,
         FALSE,
         TRUE,
         TRUE,
         FALSE,
         TRUE,
         "NOT_NULL",
         "enumeration"
      },
      {/* 9 */
         BASICTYPE_SEQUENCE,
         NULL,
         FALSE,
         TRUE,
         FALSE,
         TRUE,
         TRUE,
         FALSE,
         TRUE,
         "NOT_NULL",
         "seq"
      },
      {/* 10 */
         BASICTYPE_SEQUENCEOF,
         "AsnList",
         FALSE,
         TRUE,
         FALSE,
         TRUE,
         TRUE,
         FALSE,
         TRUE,
         "NOT_NULL",
         "seqOf"
      },
      {/* 11 */
         BASICTYPE_SET,
         NULL,
         FALSE,
         TRUE,
         FALSE,
         TRUE,
         TRUE,
         FALSE,
         FALSE,
         "NOT_NULL",
         "set"
      },
      {/* 12 */
         BASICTYPE_SETOF,
         "AsnList",
         FALSE,
         TRUE,
         FALSE,
         TRUE,
         TRUE,
         FALSE,
         TRUE,
         "NOT_NULL",
         "setOf"
      },
      {/* 13 */
         BASICTYPE_CHOICE,
         NULL,
         FALSE,
         TRUE,
         FALSE,
         TRUE,
         TRUE,
         FALSE,
         FALSE,
         "NOT_NULL",
         "choice"
      },
      {/* 14 */
         BASICTYPE_SELECTION,
         NULL,
         FALSE,
         TRUE,
         FALSE,
         TRUE,
         TRUE,
         FALSE,
         TRUE,
         "NOT_NULL",
         "foo"
      },
      {/* 15 */
         BASICTYPE_COMPONENTSOF,
         NULL,
         FALSE,
         TRUE,
         FALSE,
         TRUE,
         TRUE,
         FALSE,
         TRUE,
         "NOT_NULL",
         "bar"
      },
      {/* 16 */
         BASICTYPE_ANY,
         "AsnAny",
         FALSE,
         TRUE,
         FALSE,
         TRUE,
         TRUE,
         FALSE,
         TRUE,
         "NOT_NULL",
         "any"
      },
      {/* 17 */
         BASICTYPE_ANYDEFINEDBY,
         "AsnAnyDefinedBy",
         FALSE,
         TRUE,
         FALSE,
         TRUE,
         TRUE,
         FALSE,
         TRUE,
         "NOT_NULL",
         "anyDefBy"
      },
      {/* 18 */
         BASICTYPE_LOCALTYPEREF,
         NULL,
         FALSE,
         TRUE,
         FALSE,
         TRUE,
         TRUE,
         FALSE,
         TRUE,
         "NOT_NULL",
         "foo"
      },
      {/* 19 */
         BASICTYPE_IMPORTTYPEREF,
         NULL,
         FALSE,
         TRUE,
         FALSE,
         TRUE,
         TRUE,
         FALSE,
         TRUE,
         "NOT_NULL",
         "bar"
      },
      {/* 20 */
         BASICTYPE_MACROTYPE,
         NULL,
         FALSE,
         FALSE,
         FALSE,
         TRUE,
         TRUE,
         FALSE,
         TRUE,
         "NOT_NULL",
         "foo"
      },
      {/* 21 */
         BASICTYPE_MACRODEF,
         NULL,
         FALSE,
         FALSE,
         FALSE,
         TRUE,
         TRUE,
         FALSE,
         TRUE,
         "NOT_NULL",
         "foo"
      },
      {/* 22 */
         BASICTYPE_NUMERIC_STR,
         "NumericString",
         FALSE,
         TRUE,
         FALSE,
         TRUE,
         TRUE,
         FALSE,
         TRUE,
         "NOT_NULL",
         "numericString"
      },
      {/* 23 */
         BASICTYPE_PRINTABLE_STR,
         "PrintableString",
         FALSE,
         TRUE,
         FALSE,
         TRUE,
         TRUE,
         FALSE,
         TRUE,
         "NOT_NULL",
         "printableString"
      },
      {/* 24 */
         BASICTYPE_UNIVERSAL_STR,
         "UniversalString",
         FALSE,
         TRUE,
         FALSE,
         TRUE,
         TRUE,
         FALSE,
         TRUE,
         "NOT_NULL",
         "universalString"
      },
      {/* 25 */
         BASICTYPE_IA5_STR,
         "IA5String",
         FALSE,
         TRUE,
         FALSE,
         TRUE,
         TRUE,
         FALSE,
         TRUE,
         "NOT_NULL",
         "ia5String"
      },            
      {/* 26 */
         BASICTYPE_BMP_STR,
         "BMPString",
         FALSE,
         TRUE,
         FALSE,
         TRUE,
         TRUE,
         FALSE,
         TRUE,
         "NOT_NULL",
         "bmpString"
      },            
      {/* 27 */
         BASICTYPE_UTF8_STR,
         "UTF8String",
         FALSE,
         TRUE,
         FALSE,
         TRUE,
         TRUE,
         FALSE,
         TRUE,
         "NOT_NULL",
         "utf8String"
      },
      {/* 28 */
         BASICTYPE_UTCTIME, /* 23 */
         "UTCTime",
         FALSE,
         TRUE,
         FALSE,
         TRUE,
         TRUE,
         FALSE,
         TRUE,
         "NOT_NULL",
         "utcTime"
      },
      {/* 29 */
         BASICTYPE_GENERALIZEDTIME, /* 24 */
         "GeneralizedTime",
         FALSE,
         TRUE,
         FALSE,
         TRUE,
         TRUE,
         FALSE,
         TRUE,
         "NOT_NULL",
         "generalizedTime"
      },
      {/* 30 */
         BASICTYPE_GRAPHIC_STR, /* 25 */
         "GraphicString",
         FALSE,
         TRUE,
         FALSE,
         TRUE,
         TRUE,
         FALSE,
         TRUE,
         "NOT_NULL",
         "graphicString"
      },
      {/* 31 */
         BASICTYPE_VISIBLE_STR, /* 26 */
         "VisibleString",
         FALSE,
         TRUE,
         FALSE,
         TRUE,
         TRUE,
         FALSE,
         TRUE,
         "NOT_NULL",
         "visibleString"
      },
      {/* 32 */
         BASICTYPE_GENERAL_STR, /* 27 */
         "GeneralString",
         FALSE,
         TRUE,
         FALSE,
         TRUE,
         TRUE,
         FALSE,
         TRUE,
         "NOT_NULL",
         "generalString"
      },
      {/* 33 */
         BASICTYPE_OBJECTDESCRIPTOR,
         "ObjectDescriptor",
         FALSE,
         TRUE,
         FALSE,
         TRUE,
         TRUE,
         FALSE,
         TRUE,
         "NOT_NULL",
         "objectDescriptor"
      } ,
      {/* 34 */
         BASICTYPE_VIDEOTEX_STR,
         "VideotexString",
         FALSE,
         TRUE,
         FALSE,
         TRUE,
         TRUE,
         FALSE,
         TRUE,
         "NOT_NULL",
         "videotexString"
      } ,           
      {/* 35 */
         BASICTYPE_T61_STR,
         "T61String",
         FALSE,
         TRUE,
         FALSE,
         TRUE,
         TRUE,
         FALSE,
         TRUE,
         "NOT_NULL",
         "t61String"
      },            
      {/* 36 */
         BASICTYPE_EXTERNAL,
         "EXTERNAL",
         FALSE,
         TRUE,
         FALSE,
         TRUE,
         TRUE,
         TRUE,
         TRUE,
         "NOT_NULL",
         "external"
      },            
      {/* 37 */
         BASICTYPE_OCTETCONTAINING,
         NULL,
         FALSE,
         TRUE,
         FALSE,
         TRUE,
         TRUE,
         FALSE,
         TRUE,
         "NOT_NULL",
         "octetContainer"
		},
      {/* 38 */
         BASICTYPE_BITCONTAINING,
         NULL,
         FALSE,
         TRUE,
         FALSE,
         TRUE,
         TRUE,
         FALSE,
         TRUE,
         "NOT_NULL",
         "bitContainer"
		},
        { /* 39 */
         BASICTYPE_RELATIVE_OID,
         "AsnRelativeOid",
         FALSE,
         TRUE,
         FALSE,
         TRUE,
         TRUE,
         FALSE,
         TRUE,
         "NOT_NULL",
         "relativeOid"
        },
        { /* 40 */
         BASICTYPE_EXTENSION,
         "AsnExtension",
         FALSE,
         TRUE,
         FALSE,
         TRUE,
         TRUE,
         FALSE,
         TRUE,
         "NULL",
         "extension"
        }

   }
};
