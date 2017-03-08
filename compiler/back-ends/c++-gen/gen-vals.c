/*
 * compiler/back_ends/c++_gen/gen_vals.c - prints ASN.1 values in c++ format
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
 * $Header: /baseline/SNACC/compiler/back-ends/c++-gen/gen-vals.c,v 1.8 2004/04/06 15:13:41 gronej Exp $
 * $Log: gen-vals.c,v $
 * Revision 1.8  2004/04/06 15:13:41  gronej
 * *** empty log message ***
 *
 * Revision 1.7  2003/12/19 20:55:49  leonberp
 * added parameter to PrintCxxOidValue() to allow printing of paren or quote to be specified.
 *
 * Revision 1.6  2003/07/07 14:52:35  nicholar
 * Eliminated headers and cleaned up include references
 *
 * Revision 1.5  2002/10/21 17:18:52  mcphersc
 * fixed long int
 *
 * Revision 1.4  2002/05/15 14:53:11  leonberp
 * added support for new basicTypes to compiler
 *
 * Revision 1.3  2001/08/30 17:25:38  rwc
 * Updated AsnOid constructor, automatic build of constants to now load strings, not
 * the 10 parameter constructor choice (which was removed by Pierce eariler).
 *
 * Revision 1.2  2000/10/24 14:54:46  rwc
 * Updated to remove high-level warnings (level 4 on MSVC++) for an easier build.
 * SOME warnings persist due to difficulty in modifying the SNACC compiler to
 * properly build clean source; also some files are built by Lex/Yacc.
 *
 * Revision 1.1.1.1  2000/08/21 20:36:06  leonberp
 * First CVS Version of SNACC.
 *
 * Revision 1.4  1995/07/25 18:20:05  rj
 * use true/false instead of AsnBool::true/false.
 *
 * changed `_' to `-' in file names.
 *
 * Revision 1.3  1994/10/08  03:47:57  rj
 * since i was still irritated by cpp standing for c++ and not the C preprocessor, i renamed them to cxx (which is one known suffix for C++ source files). since the standard #define is __cplusplus, cplusplus would have been the more obvious choice, but it is a little too long.
 *
 * Revision 1.2  1994/09/01  00:17:22  rj
 * snacc_config.h removed.
 *
 * Revision 1.1  1994/08/28  09:48:05  rj
 * first check-in. for a list of changes to the snacc-1.1 distribution please refer to the ChangeLog.
 *
 */

#include "config.h"
#include "asn-incl.h"
#include "asn1module.h"
#include "str-util.h"
#include "rules.h"


extern char *bVDAGlobalDLLExport;


void PrintCxxValuesClass PROTO ((FILE *f, CxxRules *r, Value *v));
void PrintCxxValueInstantiation PROTO ((FILE *f, CxxRules *r, Value *v));
void PrintCxxOidValue PROTO ((FILE *f, CxxRules *r, AsnOid *oid, int parenOrQuote));
void PrintCxxIntValue PROTO ((FILE *f, CxxRules *r, AsnInt oid));
static void PrintCxxValueDefsName PROTO ((FILE *f, CxxRules *r, ValueDef *v));




void
PrintCxxValueDef PARAMS ((src, r, v),
    FILE *src _AND_
    CxxRules *r _AND_
    ValueDef *v)
{
    /* just do oid's, ints and bools for now */
    if ((v->value->basicValue->choiceId != BASICVALUE_OID) &&
        (v->value->basicValue->choiceId != BASICVALUE_INTEGER) &&
        (v->value->basicValue->choiceId != BASICVALUE_BOOLEAN))
        return;

	if (v->value->basicValue->choiceId == BASICVALUE_OID)
	{
		fprintf(src, "const char *");
		PrintCxxValueDefsName (src, r, v);
		fprintf(src, " =""");
		PrintCxxOidValue (src, r, v->value->basicValue->a.oid, 2);
		fprintf(src, """;\n\n");
	}
	else
	{
		/*
		* put instantiation in src file
		*/
		fprintf (src, "const ");
		PrintCxxValuesClass (src, r, v->value);
		fprintf (src, " ");
		PrintCxxValueDefsName (src, r, v);
		fprintf (src, " ");
		PrintCxxValueInstantiation (src, r, v->value);
		fprintf (src, ";\n\n");
	}


}  /* PrintCxxValueDef */

void
PrintCxxValueExtern PARAMS ((hdr, r, v),
    FILE *hdr _AND_
    CxxRules *r _AND_
    ValueDef *v)
{
    /* just do oid's, ints and bools for now */
    if ((v->value->basicValue->choiceId != BASICVALUE_OID) &&
        (v->value->basicValue->choiceId != BASICVALUE_INTEGER) &&
        (v->value->basicValue->choiceId != BASICVALUE_BOOLEAN))
        return;

    /*
     * put extern declaration in hdr file
     */
    if (bVDAGlobalDLLExport != NULL) 
        fprintf (hdr, "extern const %s ", bVDAGlobalDLLExport);
    else
        fprintf (hdr, "extern const ");

	if (v->value->basicValue->choiceId == BASICVALUE_OID)
	{
		fprintf(hdr, "char *");
		PrintCxxValueDefsName (hdr, r, v);
		fprintf(hdr, ";\n");
	}
	else
	{
		PrintCxxValuesClass (hdr, r, v->value);
		fprintf (hdr, " ");
		PrintCxxValueDefsName (hdr, r, v);
		fprintf (hdr, ";\n");
	}

}  /* PrintCxxValueExtern */


static void
PrintCxxValueDefsName PARAMS ((f, r, v),
    FILE *f _AND_
    CxxRules *r _AND_
    ValueDef *v)
{
    char *cName;
    cName = Asn1ValueName2CValueName (v->definedName);
    fprintf (f, "%s", cName);
    Free (cName);
    r=r;   /*AVOIDS warning.*/
}

void
PrintCxxValuesClass PARAMS ((f, r, v),
    FILE *f _AND_
    CxxRules *r _AND_
    Value *v)
{
    /* needs work - just do ints bools and oid's for now */
    switch (v->basicValue->choiceId)
    {
        case BASICVALUE_OID:
            fprintf (f, "%s", r->typeConvTbl[BASICTYPE_OID].className);
            break;

        case BASICVALUE_INTEGER:
            fprintf (f, "%s", r->typeConvTbl[BASICTYPE_INTEGER].className);
            break;

        case BASICVALUE_BOOLEAN:
            fprintf (f, "%s", r->typeConvTbl[BASICTYPE_BOOLEAN].className);
            break;

        default:
           break;
    }
}


void
PrintCxxValueInstantiation PARAMS ((f, r, v),
    FILE *f _AND_
    CxxRules *r _AND_
    Value *v)
{
    /* needs work - just do oids, ints and bools for now */
    switch (v->basicValue->choiceId)
    {
        case BASICVALUE_OID:
            PrintCxxOidValue (f, r, v->basicValue->a.oid, 1);
            break;

        case BASICVALUE_INTEGER:
            PrintCxxIntValue (f, r, v->basicValue->a.integer);
            break;

        case BASICVALUE_BOOLEAN:
	    fprintf (f, "(%s)", v->basicValue->a.boolean ? "true" : "false");
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
 *
 * To wrap the dotted notation in parens pass in a parenOrQuote value of 1
 *
 * To wrap the dotted notation in quotes pass in a parenOrQuote value of 2
 */
void
PrintCxxOidValue (FILE *f, CxxRules *r, AsnOid *v, int parenOrQuote)
{
    unsigned short int firstArcNum;
    unsigned long arcNum;
    int i;

	if (parenOrQuote == 1)
		fprintf (f, "(\"");
	else
		fprintf (f, "\"");

    /* un-munge first two arc numbers */
    for (arcNum = 0, i=0; (i < (int)v->octetLen) && (v->octs[i] & 0x80);i++)
        arcNum = (arcNum << 7) + (v->octs[i] & 0x7f);

    arcNum = (arcNum << 7) + (v->octs[i] & 0x7f);
    i++;
    firstArcNum = (unsigned short)(arcNum/40);
    if (firstArcNum > 2)
        firstArcNum = 2;

#ifdef NOW_CONST_OIDS_ARE_STRINGS
    fprintf (f, "%u, %u", firstArcNum, arcNum - (firstArcNum * 40));
#else
    fprintf (f, "%u.%lu", firstArcNum, arcNum - (firstArcNum * 40));
#endif
    for (; i < (int)v->octetLen; )
    {
        for (arcNum = 0; (i < (int)v->octetLen) && (v->octs[i] & 0x80);i++)
            arcNum = (arcNum << 7) + (v->octs[i] & 0x7f);

        arcNum = (arcNum << 7) + (v->octs[i] & 0x7f);
        i++;

#ifdef NOW_CONST_OIDS_ARE_STRINGS
        fprintf (f, ", %u", arcNum);
#else
        fprintf (f, ".%lu", arcNum);
#endif
    }

	if (parenOrQuote == 1)
		fprintf (f, "\")");
	else
		fprintf (f, "\"");

    r=r;   /*AVOIDS warning.*/
} /* PrintCxxOidValue */



void
PrintCxxIntValue PARAMS ((f, r, v),
    FILE *f _AND_
    CxxRules *r _AND_
    AsnInt v)
{
    fprintf (f, "(%d)", v);
    r=r;   /*AVOIDS warning.*/
} /* PrintCxxIntValue */
