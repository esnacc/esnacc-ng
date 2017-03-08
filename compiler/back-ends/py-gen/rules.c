/*
 * compiler/back-ends/py-gen/rules.c - initialized rule structure
 *           inits a table that contains info about
 *           converting each ASN.1 type to a python class
 *
 * Copyright (C) 2016 Aaron Conole
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#include "config.h"
#include "asn-incl.h"
#include "asn1module.h"
#include "rules.h"

PyRules pyRulesG =
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
         "None",
         "unknown"
      },
      { /* 1 */
         BASICTYPE_BOOLEAN,
         "asn_bool.AsnBool",
         FALSE,
         TRUE,
         FALSE,
         TRUE,
         TRUE,
         FALSE,
         TRUE,
         "None",
         "bool"
      },
      {  /* 2 */
         BASICTYPE_INTEGER, /* 2 */
         "asn_ints.AsnInt",
         FALSE,
         TRUE,
         FALSE,
         TRUE,
         TRUE,
         FALSE,
         TRUE,
         "None",
         "integer"
      },
      { /* 3 */
         BASICTYPE_BITSTRING,
         "asn_octs.AsnBits",
         FALSE,
         TRUE,
         FALSE,
         TRUE,
         TRUE,
         FALSE,
         TRUE,
         "None",
         "bits"
      },
      {/* 4 */
         BASICTYPE_OCTETSTRING,
         "asn_octs.AsnOcts",
         FALSE,
         TRUE,
         FALSE,
         TRUE,
         TRUE,
         FALSE,
         TRUE,
         "None",
         "octs"
      },
      {/* 5 */
         BASICTYPE_NULL,
         "asn_base.AsnNull",
         FALSE,
         TRUE,
         FALSE,
         TRUE,
         TRUE,
         FALSE,
         TRUE,
         "None",
         "null"
      },
      { /* 6 */
         BASICTYPE_OID,
         "asn_oid.AsnOid",
         FALSE,
         TRUE,
         FALSE,
         TRUE,
         TRUE,
         FALSE,
         TRUE,
         "None",
         "oid"
      },
      { /* 7 */
         BASICTYPE_REAL,
         "asn_ints.AsnReal",
         FALSE,
         TRUE,
         FALSE,
         TRUE,
         TRUE,
         FALSE,
         TRUE,
         "None",
         "real"
      },
      {/* 8 */
         BASICTYPE_ENUMERATED,
         "asn_ints.AsnEnum",
         FALSE,
         TRUE,
         FALSE,
         TRUE,
         TRUE,
         FALSE,
         TRUE,
         "None",
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
         "None",
         "seq"
      },
      {/* 10 */
         BASICTYPE_SEQUENCEOF,
         "asn_list.AsnSequenceOf",
         FALSE,
         TRUE,
         FALSE,
         TRUE,
         TRUE,
         FALSE,
         TRUE,
         "None",
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
         "None",
         "set"
      },
      {/* 12 */
         BASICTYPE_SETOF,
         "asn_list.AsnSetOf",
         FALSE,
         TRUE,
         FALSE,
         TRUE,
         TRUE,
         FALSE,
         TRUE,
         "None",
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
         "None",
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
         "None",
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
         "None",
         "bar"
      },
      {/* 16 */
         BASICTYPE_ANY,
         "asn_base.AsnAny",
         FALSE,
         TRUE,
         FALSE,
         TRUE,
         TRUE,
         FALSE,
         TRUE,
         "None",
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
         "None",
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
         "None",
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
         "None",
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
         "None",
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
         "None",
         "foo"
      },
      {/* 22 */
         BASICTYPE_NUMERIC_STR,
         "asn_useful.NumericString",
         FALSE,
         TRUE,
         FALSE,
         TRUE,
         TRUE,
         FALSE,
         TRUE,
         "None",
         "numericString"
      },
      {/* 23 */
         BASICTYPE_PRINTABLE_STR,
         "asn_useful.PrintableString",
         FALSE,
         TRUE,
         FALSE,
         TRUE,
         TRUE,
         FALSE,
         TRUE,
         "None",
         "printableString"
      },
      {/* 24 */
         BASICTYPE_UNIVERSAL_STR,
         "asn_useful.UniversalString",
         FALSE,
         TRUE,
         FALSE,
         TRUE,
         TRUE,
         FALSE,
         TRUE,
         "None",
         "universalString"
      },
      {/* 25 */
         BASICTYPE_IA5_STR,
         "asn_useful.IA5String",
         FALSE,
         TRUE,
         FALSE,
         TRUE,
         TRUE,
         FALSE,
         TRUE,
         "None",
         "ia5String"
      },
      {/* 26 */
         BASICTYPE_BMP_STR,
         "asn_useful.BMPString",
         FALSE,
         TRUE,
         FALSE,
         TRUE,
         TRUE,
         FALSE,
         TRUE,
         "None",
         "bmpString"
      },
      {/* 27 */
         BASICTYPE_UTF8_STR,
         "asn_useful.UTF8String",
         FALSE,
         TRUE,
         FALSE,
         TRUE,
         TRUE,
         FALSE,
         TRUE,
         "None",
         "utf8String"
      },
      {/* 28 */
         BASICTYPE_UTCTIME, /* 23 */
         "asn_useful.UTCTime",
         FALSE,
         TRUE,
         FALSE,
         TRUE,
         TRUE,
         FALSE,
         TRUE,
         "None",
         "utcTime"
      },
      {/* 29 */
         BASICTYPE_GENERALIZEDTIME, /* 24 */
         "asn_useful.GeneralizedTime",
         FALSE,
         TRUE,
         FALSE,
         TRUE,
         TRUE,
         FALSE,
         TRUE,
         "None",
         "generalizedTime"
      },
      {/* 30 */
         BASICTYPE_GRAPHIC_STR, /* 25 */
         "asn_useful.GraphicString",
         FALSE,
         TRUE,
         FALSE,
         TRUE,
         TRUE,
         FALSE,
         TRUE,
         "None",
         "graphicString"
      },
      {/* 31 */
         BASICTYPE_VISIBLE_STR, /* 26 */
         "asn_useful.VisibleString",
         FALSE,
         TRUE,
         FALSE,
         TRUE,
         TRUE,
         FALSE,
         TRUE,
         "None",
         "visibleString"
      },
      {/* 32 */
         BASICTYPE_GENERAL_STR, /* 27 */
         "asn_useful.GeneralString",
         FALSE,
         TRUE,
         FALSE,
         TRUE,
         TRUE,
         FALSE,
         TRUE,
         "None",
         "generalString"
      },
      {/* 33 */
         BASICTYPE_OBJECTDESCRIPTOR,
         "asn_useful.ObjectDescriptor",
         FALSE,
         TRUE,
         FALSE,
         TRUE,
         TRUE,
         FALSE,
         TRUE,
         "None",
         "objectDescriptor"
      } ,
      {/* 34 */
         BASICTYPE_VIDEOTEX_STR,
         "asn_useful.VideotexString",
         FALSE,
         TRUE,
         FALSE,
         TRUE,
         TRUE,
         FALSE,
         TRUE,
         "None",
         "videotexString"
      },
      {/* 35 */
         BASICTYPE_T61_STR,
         "asn_useful.T61String",
         FALSE,
         TRUE,
         FALSE,
         TRUE,
         TRUE,
         FALSE,
         TRUE,
         "None",
         "t61String"
      },
      {/* 36 */
         BASICTYPE_EXTERNAL,
         "asn_useful.EXTERNAL",
         FALSE,
         TRUE,
         FALSE,
         TRUE,
         TRUE,
         TRUE,
         TRUE,
         "None",
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
         "None",
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
         "None",
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
         "None",
         "relativeOid"
        },
        { /* 40 */
         BASICTYPE_EXTENSION,
         "asn_base.AsnExtension",
         FALSE,
         TRUE,
         FALSE,
         TRUE,
         TRUE,
         FALSE,
         TRUE,
         "None",
         "extension"
        }

   }
};
