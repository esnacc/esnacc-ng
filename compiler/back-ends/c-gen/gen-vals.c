/*
 * compiler/back-ends/c-gen/gen-vals.c - prints ASN.1 values in C format
 *
 *
 * MS Feb 92
 * Copyright (C) 1991, 1992 Michael Sample
 *            and the University of British Columbia
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * $Header: /baseline/SNACC/compiler/back-ends/c-gen/gen-vals.c,v 1.7 2004/01/14 19:07:53 gronej Exp $
 * $Log: gen-vals.c,v $
 * Revision 1.7  2004/01/14 19:07:53  gronej
 * Updated Compiler to accept and process relative-oid's
 *
 * Revision 1.6  2003/07/28 14:28:56  colestor
 * Updated for "C" builds to properly load integer constant references in tag designations.
 * Initialization was leaving the entire value blank, causing an error.
 *
 * Revision 1.5  2003/07/07 14:53:38  nicholar
 * Eliminated headers and cleaned up include references
 *
 * Revision 1.4  2003/04/29 21:01:38  leonberp
 * integerated Deepak's changes for IOB support
 *
 * Revision 1.3  2002/09/16 17:35:04  mcphersc
 * Fixed warnings
 *
 * Revision 1.2  2000/10/24 14:54:48  rwc
 * Updated to remove high-level warnings (level 4 on MSVC++) for an easier build.
 * SOME warnings persist due to difficulty in modifying the SNACC compiler to
 * properly build clean source; also some files are built by Lex/Yacc.
 *
 * Revision 1.1.1.1  2000/08/21 20:36:05  leonberp
 * First CVS Version of SNACC.
 *
 * Revision 1.4  1997/08/28 07:26:09  povey
 * Changes to support DER encoding/decoding
 *
 * Revision 1.3.1.1  1997/08/20 23:14:41  povey
 *
 * Revision 1.3  1995/07/25 18:44:12  rj
 * file name has been shortened for redundant part: c-gen/gen-c-vals -> c-gen/gen-vals.
 *
 * changed `_' to `-' in file names.
 *
 * Revision 1.2  1994/09/01  00:24:18  rj
 * snacc_config.h removed.
 *
 * Revision 1.1  1994/08/28  09:48:33  rj
 * first check-in. for a list of changes to the snacc-1.1 distribution please refer to the ChangeLog.
 *
 */
#include "config.h"
#include "asn-incl.h"
#include "asn1module.h"
#include "rules.h"
#include "str-util.h"


void PrintCValueInstantiation PROTO ((FILE *hdr, CRules *r, Value *v));
void PrintCOidValue PROTO ((FILE *f, CRules *r, AsnOid *oid));
static void PrintValueDefsName PROTO ((FILE *f, CRules *r, ValueDef *v));
static void PrintValueDefsType PROTO ((FILE *f, CRules *r, ValueDef *v));


void
PrintCValueDef PARAMS ((src, r, v),
    FILE *src _AND_
    CRules *r _AND_
    ValueDef *v)
{
    /* just do oid's, ints and bools for now */
	/* Commented the below condition to allow Some More ValueDefs */
/*    if ((v->value->basicValue->choiceId != BASICVALUE_OID) &&
        (v->value->basicValue->choiceId != BASICVALUE_INTEGER) &&
        (v->value->basicValue->choiceId != BASICVALUE_BOOLEAN))
        return;
*/
    /*
     * put instantiation in src file
     */
    PrintValueDefsType (src, r, v);
    fprintf (src," ");
    PrintValueDefsName (src, r, v);
    fprintf (src," = ");
    PrintCValueInstantiation (src, r, v->value);
    fprintf (src,";\n");

}  /* PrintCValueDef */

void
PrintCValueExtern PARAMS ((hdr, r, v),
    FILE *hdr _AND_
    CRules *r _AND_
    ValueDef *v)
{
    /* just do oid's, ints and bools for now */

	/* modified for more types Deepak: 17/Mar/2003 */
/*    if ((v->value->basicValue->choiceId != BASICVALUE_OID) &&
        (v->value->basicValue->choiceId != BASICVALUE_INTEGER) &&
        (v->value->basicValue->choiceId != BASICVALUE_BOOLEAN))
        return;
*/
    /*
     * put extern declaration in hdr file
     */
    fprintf (hdr,"extern ");
    PrintValueDefsType (hdr, r, v);
    fprintf (hdr," ");
    PrintValueDefsName (hdr, r, v);
    fprintf (hdr,";\n");

}  /* PrintCValueExtern */

void
PrintCValueInstantiation PARAMS ((f, r, v),
    FILE *f _AND_
    CRules *r _AND_
    Value *v)
{

  /* needs work - just do ints, bools and oids for now */
  switch (v->basicValue->choiceId) {
  case BASICVALUE_OID:
    PrintCOidValue (f, r, v->basicValue->a.oid);
    break;
    
  case BASICVALUE_INTEGER:
    fprintf (f, "%d", v->basicValue->a.integer);
    break;
      
  case BASICVALUE_BOOLEAN:
    if (v->basicValue->a.boolean)
      fprintf (f, "TRUE");
    else
      fprintf (f, "FALSE");
    break;

	/* modified for more types Deepak: 17/Mar/2003 */
	/* Also add in func PrintValueDefsType(...) */
  case BASICVALUE_ASCIITEXT:
	fprintf (f, "%s", r->typeConvTbl[BASICTYPE_PRINTABLE_STR].cTypeName);
    break;

  case BASICVALUE_LOCALVALUEREF:
      if (v->basicValue->a.localValueRef &&
          v->basicValue->a.localValueRef->link &&
          v->basicValue->a.localValueRef->link->value &&
          v->basicValue->a.localValueRef->link->value->basicValue)
      {
            int iValue=500;     // WILL indicate a problem on source creation...
            if (v->basicValue->a.localValueRef->link->value->basicValue->choiceId == 
                        BASICVALUE_INTEGER)
            {
                iValue = v->basicValue->a.localValueRef->link->
                                                value->basicValue->a.integer;
            }       // IF Integer
            else if (v->basicValue->a.localValueRef->link->value->basicValue->choiceId == 
                        BASICVALUE_LOCALVALUEREF)
            {
                ValueRef *pvalueRef=NULL;
                if (v->basicValue->a.localValueRef->link->value->basicValue->choiceId == BASICVALUE_LOCALVALUEREF)
                {
                    pvalueRef = v->basicValue->a.localValueRef->link->value->basicValue->a.localValueRef;
                    if (pvalueRef->link->value && pvalueRef->link->value->basicValue &&
                        pvalueRef->link->value->basicValue->choiceId == BASICVALUE_INTEGER)
                        iValue = pvalueRef->link->value->basicValue->a.integer;
                }       // END IF LOCALVALUEREF (recursed)
            }           // END IF LOCALVALUEREF under LCOALVALUEREF
       	    fprintf (f, "%d", iValue);
      }     // END if LocalValueRef type.
    break;

  default:
    break;
  }
   
}

static void
PrintValueDefsName PARAMS ((f, r, v),
    FILE *f _AND_
    CRules *r _AND_
    ValueDef *v)
{
    char *cName;
    cName = Asn1ValueName2CValueName (v->definedName);
    fprintf (f, "%s", cName);
    Free (cName);
    r = r;  /*AVOIDS warning.*/
}

static void
PrintValueDefsType PARAMS ((f, r, v),
    FILE *f _AND_
    CRules *r _AND_
    ValueDef *v)
{
    /* needs work - just do ints bools and oid's for now */
	switch (v->value->basicValue->choiceId)
    {
        case BASICVALUE_OID:
            fprintf (f, "%s", r->typeConvTbl[BASICTYPE_OID].cTypeName);
            break;

        case BASICVALUE_RELATIVE_OID:
            fprintf (f, "%s", r->typeConvTbl[BASICTYPE_RELATIVE_OID].cTypeName);
            break;

        case BASICVALUE_INTEGER:
            fprintf (f, "%s", r->typeConvTbl[BASICTYPE_INTEGER].cTypeName);
            break;

        case BASICVALUE_BOOLEAN:
            fprintf (f, "%s", r->typeConvTbl[BASICTYPE_BOOLEAN].cTypeName);
            break;

			/* modified for more types Deepak: 17/Mar/2003 */
			/* Also add in func PrintCValueInstantiation(...) */
		case BASICVALUE_ASCIITEXT:
			fprintf (f, "%s", r->typeConvTbl[BASICTYPE_PRINTABLE_STR].cTypeName);
            break;
	//	case BASICVALUE_VALUENOTATION:
	//		fprintf (f, "%s", r->typeConvTbl[BASICTYPE_PRINTABLE_STR].cTypeName);
	//		break;
        default:
           break;
    }
}






/*
 * given an AOID, a c value is produced.
 * This is used for turning ASN.1 OBJECT ID values
 * into usable c values.
 *
 * eg for the oid { 0 1 2 } (in AOID format)
 *
 * {
 *     2,
 *     "\1\2"
 * }
 * is produced.
 */
void
PrintCOidValue PARAMS ((f, r, oid),
    FILE *f _AND_
    CRules *r _AND_
    AsnOid *oid)
{
    int i;

    fprintf (f, "{ ");
    fprintf (f, "%d, ",(int)oid->octetLen);
    fprintf (f, "\"");

    /* print encoded oid string in C's 'octal' escape format */
    for (i = 0; i < (int)(oid->octetLen); i++)
        fprintf (f, "\\%o", (unsigned char)oid->octs[i]);
    fprintf (f, "\"");
    fprintf (f, " }");

    r = r;  /*AVOIDS warning.*/
} /* PrintCOidValue */
