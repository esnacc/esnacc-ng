/*
 * compiler/back_ends/idl_gen/gen_vals.c - prints ASN.1 values in IDL format
 *
 * MS 92
 * Copyright (C) 1991, 1992 Michael Sample
 *           and the University of British Columbia
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * $Header: /baseline/SNACC/compiler/back-ends/idl-gen/gen-vals.c,v 1.5 2004/01/14 19:07:53 gronej Exp $
 * $Log: gen-vals.c,v $
 * Revision 1.5  2004/01/14 19:07:53  gronej
 * Updated Compiler to accept and process relative-oid's
 *
 * Revision 1.4  2003/07/17 15:16:28  nicholar
 * Fixed bugs related to removing headers.
 *
 * Revision 1.3  2002/09/16 17:53:50  mcphersc
 * Fixed warnings
 *
 * Revision 1.2  2000/10/24 14:54:48  rwc
 * Updated to remove high-level warnings (level 4 on MSVC++) for an easier build.
 * SOME warnings persist due to difficulty in modifying the SNACC compiler to
 * properly build clean source; also some files are built by Lex/Yacc.
 *
 * Revision 1.1.1.1  2000/08/21 20:36:04  leonberp
 * First CVS Version of SNACC.
 *
 * Revision 1.1  1997/01/01 20:25:36  rj
 * first draft
 *
 */

#include "asn-incl.h"
#include "asn1module.h"
#include "str-util.h"
#include "rules.h"


/* non-exported routines' prototypes */
static void PrintIDLIntValue PROTO ((FILE *f, IDLRules *r, AsnInt v));
static void PrintIDLOidValue PROTO ((FILE *f, IDLRules *r, AsnOid *v));
static void PrintIDLValuesClass PROTO ((FILE *f, IDLRules *r, Value *v));
static void PrintIDLValueDefsName PROTO ((FILE *f, IDLRules *r, ValueDef *v));
static void PrintIDLValueInstatiation PROTO ((FILE *f, IDLRules *r, Value *v));




void
PrintIDLValueDef PARAMS ((idl, r, v),
    FILE	*idl _AND_
    IDLRules	*r _AND_
    ValueDef	*v)
{
    /* just do oid's, ints and bools for now */
    if ((v->value->basicValue->choiceId != BASICVALUE_OID) &&
        (v->value->basicValue->choiceId != BASICVALUE_INTEGER) &&
        (v->value->basicValue->choiceId != BASICVALUE_BOOLEAN))
        return;

    /*
     * put instantiation in idl file
     */
    fprintf (idl, "  const ");
    PrintIDLValuesClass (idl, r, v->value);
    fprintf (idl, " ");
    PrintIDLValueDefsName (idl, r, v);
    fprintf (idl, " = ");
    PrintIDLValueInstatiation (idl, r, v->value);
    fprintf (idl, ";\n\n");

}  /* PrintIDLValueDef */


static void
PrintIDLValueDefsName PARAMS ((f, r, v),
    FILE *f _AND_
    IDLRules *r _AND_
    ValueDef *v)
{
    char *cName;
    cName = Asn1ValueName2CValueName (v->definedName);
    fprintf (f, "%s", cName);
    Free (cName);
    r=r;  /*AVOIDS warning.*/
}

void
PrintIDLValuesClass PARAMS ((f, r, v),
    FILE *f _AND_
    IDLRules *r _AND_
    Value *v)
{
    /* needs work - just do ints bools and oid's for now */
    switch (v->basicValue->choiceId)
    {
        case BASICVALUE_OID:
            fprintf (f, "%s", r->typeConvTbl[BASICTYPE_OID].typeName);
            break;
        
        case BASICVALUE_RELATIVE_OID:
            fprintf (f, "%s", r->typeConvTbl[BASICTYPE_RELATIVE_OID].typeName);
            break;

        case BASICVALUE_INTEGER:
            fprintf (f, "%s", r->typeConvTbl[BASICTYPE_INTEGER].typeName);
            break;

        case BASICVALUE_BOOLEAN:
            fprintf (f, "%s", r->typeConvTbl[BASICTYPE_BOOLEAN].typeName);
            break;

        default:
           break;
    }
}


void
PrintIDLValueInstatiation PARAMS ((f, r, v),
    FILE *f _AND_
    IDLRules *r _AND_
    Value *v)
{
    /* needs work - just do oids, ints and bools for now */
    switch (v->basicValue->choiceId)
    {
        case BASICVALUE_OID:
            PrintIDLOidValue (f, r, v->basicValue->a.oid);
            break;

        case BASICVALUE_INTEGER:
            PrintIDLIntValue (f, r, v->basicValue->a.integer);
            break;

        case BASICVALUE_BOOLEAN:
            fprintf (f, v->basicValue->a.boolean ? "TRUE" : "FALSE");
            break;

        default:
           break;
    }
}



/*
 * given an AOID, c++ AOID constructors params are produced.
 * This is used for turning ASN.1 OBJECT ID values
 * into usable c++ values.
 *
 * eg for the oid { 0 1 2 } (in AOID format)
 *   (0,1,2)
 * is produced.
 */
void
PrintIDLOidValue PARAMS ((f, r, v),
    FILE *f _AND_
    IDLRules *r _AND_
    AsnOid *v)
{
    unsigned short int firstArcNum;
    unsigned long int arcNum;
    int i;

    fprintf (f, "(");

    /* un-munge first two arc numbers */
    for (arcNum = 0, i=0; (i < (int)v->octetLen) && (v->octs[i] & 0x80);i++)
        arcNum = (arcNum << 7) + (v->octs[i] & 0x7f);

    arcNum = (arcNum << 7) + (v->octs[i] & 0x7f);
    i++;
    firstArcNum = (unsigned short)(arcNum/40);
    if (firstArcNum > 2)
        firstArcNum = 2;

    fprintf (f, "%u, %u", firstArcNum, (int)(arcNum - (firstArcNum * 40)));

    for (; i < (int)v->octetLen; )
    {
        for (arcNum = 0; (i < (int)v->octetLen) && (v->octs[i] & 0x80);i++)
            arcNum = (arcNum << 7) + (v->octs[i] & 0x7f);

        arcNum = (arcNum << 7) + (v->octs[i] & 0x7f);
        i++;

        fprintf (f, ", %u", (unsigned int)arcNum);
    }
    fprintf (f, ")");

    r=r;  /*AVOIDS warning.*/
} /* PrintIDLOidValue */



void
PrintIDLIntValue PARAMS ((f, r, v),
    FILE *f _AND_
    IDLRules *r _AND_
    AsnInt v)
{
    fprintf (f, "%d", v);

    r=r;  /*AVOIDS warning.*/
} /* PrintIDLIntValue */
