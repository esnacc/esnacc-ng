/*
 * compiler/back-ends/c-gen/gen-any.c
 *
 *     prints Routine to initialize the ANY Hash table.  The
 *     ANY Hash table maps the OBJECT IDENTIFIERS or INTEGERS
 *     to the correct encoding/decoding etc routines.
 *
 *     Also prints an enum to identify each ANY mapping.
 *
 *     if the given module has no ANY or ANY DEFINED BY  types
 *     nothing is printed.
 *
 * MS 92
 * Copyright (C) 1991, 1992 Michael Sample
 *            and the University of British Columbia
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * $Header: /baseline/SNACC/compiler/back-ends/c-gen/gen-any.c,v 1.8 2003/07/07 14:53:38 nicholar Exp $
 * $Log: gen-any.c,v $
 * Revision 1.8  2003/07/07 14:53:38  nicholar
 * Eliminated headers and cleaned up include references
 *
 * Revision 1.7  2003/04/29 21:05:37  leonberp
 * integerated Deepak's changes for IOB support
 *
 * Revision 1.6  2002/10/30 13:12:16  mcphersc
 * anyEnumValG is now set via the -a argument in snacc.c
 *
 * Revision 1.5  2002/09/16 17:34:47  mcphersc
 * iFixed warnings
 *
 * Revision 1.4  2002/09/04 17:54:19  vracarl
 * got rid of c++ comments
 *
 * Revision 1.3  2002/02/28 19:45:20  nicholar
 * Added calls to Dash2Underscore() to remove dashes in ANYs.
 *
 * Revision 1.2  2000/10/24 14:54:46  rwc
 * Updated to remove high-level warnings (level 4 on MSVC++) for an easier build.
 * SOME warnings persist due to difficulty in modifying the SNACC compiler to
 * properly build clean source; also some files are built by Lex/Yacc.
 *
 * Revision 1.1.1.1  2000/08/21 20:36:05  leonberp
 * First CVS Version of SNACC.
 *
 * Revision 1.3  1995/07/25 18:33:43  rj
 * file name has been shortened for redundant part: c-gen/gen-c-any -> c-gen/gen-any.
 *
 * changed `_' to `-' in file names.
 *
 * Revision 1.2  1994/09/01  00:21:15  rj
 * snacc_config.h removed.
 *
 * Revision 1.1  1994/08/28  09:48:15  rj
 * first check-in. for a list of changes to the snacc-1.1 distribution please refer to the ChangeLog.
 *
 */
#include "config.h"
#include <string.h>
#include "asn-incl.h"
#include "asn1module.h"
#include "rules.h"
#include "str-util.h"
#include "enc-rules.h"
/* REN -- 1/13/98 -- added next line for proto of GetBuiltinType() */
#include "snacc-util.h"

int anyEnumValG = 0;


void PrintCAnyEnum PROTO ((FILE *hdr, Module *m, CRules *r));

void PrintCAnyHashInitRoutine(FILE *src, FILE *hdr, ModuleList *mods, 
							  Module *m, CRules *r, int printEncoders, 
							  int printDecoders, int printPrinters, 
							  int printFree);
void PrintCOidValue PROTO ((FILE *f, CRules *r, AsnOid *oid));


/* REN -- 1/13/98 -- added following prototype */
TypeDef* GetTypeDef PROTO ((Type *t));


void PrintCAnyCode(FILE *src, FILE *hdr, CRules *r, ModuleList *mods, 
				   Module *m, int printEncoders, int printDecoders, 
				   int printPrinters, int printFree)
{
  EncRulesType *encoding;


    if (!m->hasAnys)
        return;

	/* XXX - Currently any types will only be able to be used with one of
	 * the encoding rules.  To fix, we need seperate hashes for each encoding
	 * rule 
	 */
	encoding = GetEncRules();
	if(!SetEncRules(*encoding)) {
        fprintf(stderr, "Failed to set encoding rules.\n");
        return;
    }
    
    PrintCAnyEnum (hdr, m, r);
    PrintCAnyHashInitRoutine (src, hdr, mods, m, r, printEncoders, 
		printDecoders, printPrinters, printFree);

}  /* PrintAnyCode */



void
PrintCAnyEnum PARAMS ((hdr, m, r),
    FILE *hdr _AND_
    Module *m _AND_
    CRules *r)
{
/*	TypeDef *td;
    AnyRef *ar;
    AnyRefList *arl;
	int i;*/
    int firstPrinted = TRUE;
    char *modName;
/* REN -- 1/13/98 */
	ValueDef *vd;
	Type *t;
    char anyId[256];
/* REN -- end */

    modName = Asn1TypeName2CTypeName (m->modId->name);

    fprintf (hdr,"typedef enum %sAnyId\n", modName);
    fprintf (hdr,"{\n");

/* REN -- 1/13/98 -- Changed method in which anyRefs get written.  Original 
code looped through the module's basic types and local type refs and wrote the 
anys that referred to them.  My method loops throught the valueRefs and 
writes the all the SNMP Object Types.
Removed the following loops:
    * do any lib types *
    for (i = BASICTYPE_BOOLEAN; i < BASICTYPE_MACRODEF; i++)
    {
        arl = LIBTYPE_GET_ANY_REFS (i);
        if (arl != NULL)
        {
            FOR_EACH_LIST_ELMT (ar, arl)
            {
                if (!firstPrinted)
                    fprintf (hdr,",\n");
                fprintf (hdr,"    %s = %d", ar->anyIdName, anyEnumValG++);
                firstPrinted = FALSE;
            }
        }
    }

    FOR_EACH_LIST_ELMT (td, m->typeDefs)
    {
        if (td->anyRefs != NULL)
        {
            FOR_EACH_LIST_ELMT (ar, td->anyRefs)
            {
                if (!firstPrinted)
                    fprintf (hdr,",\n");
                fprintf (hdr,"    %s = %d", ar->anyIdName, anyEnumValG++);
                firstPrinted = FALSE;
            }
        }
    }
	     Handle Imported type refs
      This code is needed to ensure that OBJECT-TYPE macros with 
      SYNTAX using an imported type are correctly generated
     
    if (m->imports) {
      FOR_EACH_LIST_ELMT (im, m->imports) {
	if (im->importElmts) {
	  ImportElmt *ie;
	  
	  FOR_EACH_LIST_ELMT(ie, im->importElmts) {
	    if (ie->resolvedRef && 
		ie->resolvedRef->choiceId == IMPORTELMTCHOICE_TYPE) {
	      FOR_EACH_LIST_ELMT(ar, ie->resolvedRef->a.type->anyRefs) {
                if (!firstPrinted)
		  fprintf (hdr,",\n");
                fprintf (hdr,"    %s = %d", ar->anyIdName, 
			 anyEnumValG++);
                firstPrinted = FALSE;
	      }
	      
	    }
	  }
	}
      }
    }


REN -- 1/13/98 -- added the following: */
    FOR_EACH_LIST_ELMT (vd, m->valueDefs)
    {
        if (vd->value != NULL)
		{
			t = vd->value->type;
			if ((GetBuiltinType(t) == BASICTYPE_MACROTYPE) &&
				(t->basicType->a.macroType->choiceId == 
				MACROTYPE_SNMPOBJECTTYPE))
			{
				strcpy (anyId, vd->definedName);
			    Dash2Underscore (anyId, strlen (anyId));
				strcat (anyId, "_ANY_ID");

				if (!firstPrinted)
					fprintf (hdr,",\n");
				fprintf (hdr,"    %s = %d", anyId, anyEnumValG);
				anyEnumValG++;
				firstPrinted = FALSE;
            }
        }
    }
/* REN -- end */

    if (firstPrinted) /* none have been printed */
        fprintf (hdr,"/* NO INTEGER or OBJECT IDENTIFIER to ANY type relationships were defined (via MACROs or other mechanism) */\n???\n");

    fprintf (hdr,"} %sAnyId;\n\n\n", modName);
    Free (modName);
   r = r;    /* AVOIDS warning. */
}  /* PrintAnyEnum */


void PrintCAnyHashInitRoutine(FILE *src, FILE *hdr, ModuleList *mods, 
							  Module *m, CRules *r, int printEncoders, 
							  int printDecoders, int printPrinters, 
							  int printFree)
{
    TypeDef *td;
/*	AnyRef *ar;
    AnyRefList *arl;*/
    char *modName;
    CTDI *ctdi;
    int i;
/*	int j;*/
    enum BasicTypeChoiceId typeId;
    char *encRoutineName=NULL;
    char *decRoutineName=NULL;
    char *freeRoutineName=NULL;
    char *printRoutineName=NULL;
    int installedSomeHashes = FALSE;
/* REN -- 1/13/98 */
	ValueDef *vd;
	Type *t;
	BasicValue *bv;
    char anyId[256];
	char *typeName=NULL;
/* REN -- end */

    /* print proto in hdr file */
    modName = Asn1TypeName2CTypeName (m->modId->name);
    fprintf (hdr,"void InitAny%s();\n\n", modName);

    /* print routine to src file */
    fprintf (src,"void InitAny%s()\n", modName);
    fprintf (src,"{\n");

/* REN -- 1/13/98 -- Changed method in which anyRefs get written.  Original 
code looped through the module's basic types and local type refs and wrote the 
anys that referred to them.  My method loops throught the valueRefs and 
writes the all the SNMP Object Types.
Removed the following loops:
    * first print value for OID's *
    * do any lib types first *
    i = 0;
    for (j = BASICTYPE_BOOLEAN; j < BASICTYPE_MACRODEF; j++)
    {
        arl = LIBTYPE_GET_ANY_REFS (j);
        if (arl != NULL)
        {
            FOR_EACH_LIST_ELMT (ar, arl)
            {
                installedSomeHashes = TRUE;
                if (ar->id->choiceId == OIDORINT_OID)
                {
                    fprintf (src,"    %s oid%d =", r->typeConvTbl[BASICTYPE_OID].cTypeName, i++);
                    PrintCOidValue (src, r, ar->id->a.oid);
                    fprintf (src,";\n");
                }
            }
        }
    }

    FOR_EACH_LIST_ELMT (td, m->typeDefs)
    {
        if (td->anyRefs != NULL)
        {
            ctdi = td->cTypeDefInfo;
            FOR_EACH_LIST_ELMT (ar, td->anyRefs)
            {
                installedSomeHashes = TRUE;
                if (ar->id->choiceId == OIDORINT_OID)
                {
                    fprintf (src,"    %s oid%d =", r->typeConvTbl[BASICTYPE_OID].cTypeName, i++);
                    PrintCOidValue (src, r, ar->id->a.oid);
                    fprintf (src,";\n");
                }
            }
        }
    }
    fprintf (src,"\n\n");

    * now print hash init calls *
    i = 0;

    * do lib types first *
    for (j = BASICTYPE_BOOLEAN; j < BASICTYPE_MACRODEF; j++)
    {
        arl = LIBTYPE_GET_ANY_REFS (j);
        if (arl != NULL)
        {
            FOR_EACH_LIST_ELMT (ar, arl)
            {

                encRoutineName = r->typeConvTbl[j].encodeRoutineName;
                decRoutineName = r->typeConvTbl[j].decodeRoutineName;
                printRoutineName = r->typeConvTbl[j].printRoutineName;

                 *
                 * use NULL free routine for types that
                 * have empyt macros for their free routines
                 * (since the any hash tbl needs the addr of the routine)
                 *
                switch (j)
                {
                    case BASICTYPE_BOOLEAN:
                    case BASICTYPE_INTEGER:
                    case BASICTYPE_NULL:
                    case BASICTYPE_REAL:
                    case BASICTYPE_ENUMERATED:
                        freeRoutineName = "NULL";
                        break;
                    default:
                        freeRoutineName =  r->typeConvTbl[j].freeRoutineName;
                }

                if (ar->id->choiceId == OIDORINT_OID)
                    fprintf (src,"    InstallAnyByOid (%s, &oid%d, sizeof (%s), (EncodeFcn) B%s, (DecodeFcn)B%s, (FreeFcn)%s, (PrintFcn)%s);\n\n", ar->anyIdName, i++,  r->typeConvTbl[j].cTypeName, encRoutineName, decRoutineName, freeRoutineName, printRoutineName);
                else
                    fprintf (src,"    InstallAnyByInt (%s, %d, sizeof (%s), (EncodeFcn) B%s, (DecodeFcn)B%s, (FreeFcn)%s, (PrintFcn)%s);\n\n", ar->anyIdName, ar->id->a.intId, r->typeConvTbl[j].cTypeName, encRoutineName, decRoutineName, freeRoutineName, printRoutineName);
            }
        }
    }

    FOR_EACH_LIST_ELMT (td, m->typeDefs)
    {
        if (td->anyRefs != NULL)
        {
            ctdi = td->cTypeDefInfo;
            FOR_EACH_LIST_ELMT (ar, td->anyRefs)
            {
                typeId = GetBuiltinType (td->type);

                encRoutineName = ctdi->encodeRoutineName;
                decRoutineName = ctdi->decodeRoutineName;
                printRoutineName = ctdi->printRoutineName;

                 *
                 * use NULL free routine for types that
                 * have empyt macros for their free routines
                 * (since the any hash tbl needs the addr of the routine)
                 *
                switch (typeId)
                {
                    case BASICTYPE_BOOLEAN:
                    case BASICTYPE_INTEGER:
                    case BASICTYPE_NULL:
                    case BASICTYPE_REAL:
                    case BASICTYPE_ENUMERATED:
                        freeRoutineName = "NULL";
                        break;
                    default:
                        freeRoutineName = ctdi->freeRoutineName;
                }

                if (ar->id->choiceId == OIDORINT_OID)
                    fprintf (src,"    InstallAnyByOid (%s, &oid%d, sizeof (%s), (EncodeFcn) B%s, (DecodeFcn)B%s, (FreeFcn)%s, (PrintFcn)%s);\n\n", ar->anyIdName, i++, ctdi->cTypeName, encRoutineName, decRoutineName, freeRoutineName, printRoutineName);
                else
                    fprintf (src,"    InstallAnyByInt (%s, %d, sizeof (%s), (EncodeFcn) B%s, (DecodeFcn)B%s, (FreeFcn)%s, (PrintFcn)%s);\n\n", ar->anyIdName, ar->id->a.intId, ctdi->cTypeName, encRoutineName, decRoutineName, freeRoutineName, printRoutineName);
            }
        }
    }
REN -- 1/13/98 -- added the following: */
	/* first print value for OID's */
    i = 0;

    FOR_EACH_LIST_ELMT (vd, m->valueDefs)
    {
        if (vd->value != NULL)
		{
			t = vd->value->type;
			if ((GetBuiltinType(t) == BASICTYPE_MACROTYPE) &&
				(t->basicType->a.macroType->choiceId == 
				MACROTYPE_SNMPOBJECTTYPE))
			{
				bv = vd->value->basicValue;
				if (bv != NULL)
				{
					installedSomeHashes = TRUE;
					if (bv->choiceId == BASICVALUE_OID)
					{
	                    fprintf (src,"    %s oid%d =", 
							r->typeConvTbl[BASICTYPE_OID].cTypeName, i++);
		                PrintCOidValue (src, r, bv->a.oid);
			            fprintf (src,";\n");
					}
				}
            }
        }
    }
    fprintf (src,"\n\n");

    /* now print hash init calls */
    i = 0;

    FOR_EACH_LIST_ELMT (vd, m->valueDefs)
    {
        if (vd->value != NULL)
		{
			t = vd->value->type;
			if ((GetBuiltinType(t) == BASICTYPE_MACROTYPE) &&
				(t->basicType->a.macroType->choiceId == 
				MACROTYPE_SNMPOBJECTTYPE))
			{
				bv = vd->value->basicValue;
				if (bv != NULL)
				{
					strcpy (anyId, vd->definedName);
				    Dash2Underscore (anyId, strlen (anyId));
					strcat (anyId, "_ANY_ID");

					installedSomeHashes = TRUE;
					t = t->basicType->a.macroType->a.snmpObjectType->syntax;

					/* If the syntax of this any is a basic type, get the
					function pointers from the rules table. */
					if (((t->basicType->choiceId >= BASICTYPE_BOOLEAN) && 
						(t->basicType->choiceId <= BASICTYPE_SETOF)) ||
						((t->basicType->choiceId >= BASICTYPE_NUMERIC_STR) &&
						(t->basicType->choiceId <= BASICTYPE_T61_STR)))
					{
						typeId = t->basicType->choiceId;
		                encRoutineName = 
							r->typeConvTbl[typeId].encodeRoutineName;
						decRoutineName = 
							r->typeConvTbl[typeId].decodeRoutineName;
						printRoutineName = 
							r->typeConvTbl[typeId].printRoutineName;
						switch (typeId)
						{
							case BASICTYPE_BOOLEAN:
							case BASICTYPE_INTEGER:
							case BASICTYPE_NULL:
							case BASICTYPE_REAL:
							case BASICTYPE_ENUMERATED:
								freeRoutineName = "NULL";
								break;
							default:
								freeRoutineName = 
									r->typeConvTbl[typeId].freeRoutineName;
						}

						typeName = r->typeConvTbl[typeId].cTypeName;
					}
					/* Else if the syntax of this any is either a locally
					defined type or an imported type, get the function names 
					from the this type's ref info. */
					else
					{
						td = GetTypeDef(t);
						if (td != NULL)
						{
							ctdi = td->cTypeDefInfo;
							
							encRoutineName = ctdi->encodeRoutineName;
							decRoutineName = ctdi->decodeRoutineName;
							printRoutineName = ctdi->printRoutineName;
							freeRoutineName = ctdi->freeRoutineName;
							typeName = ctdi->cTypeName;
						}
						else
					        encRoutineName = NULL;
					}

					if (bv->choiceId == BASICVALUE_OID)
						fprintf(src, "    InstallAnyByOid (%s, &oid%d, ",
							anyId, i++);
					else if (bv->choiceId == BASICVALUE_INTEGER)
						fprintf(src, "    InstallAnyByInt (%s, %d, ",
							anyId, bv->a.integer);

					if (encRoutineName != NULL)
					{
						fprintf(src, "sizeof (%s), (EncodeFcn)B%s, ", typeName, 
							encRoutineName);
						fprintf(src, "(DecodeFcn)B%s, ", decRoutineName);
						if (printFree)
							fprintf(src, "(FreeFcn)%s, ", freeRoutineName);
						else
							fprintf(src, "(FreeFcn)NULL, ");
						if (printPrinters)
							fprintf(src, "(PrintFcn)%s);\n\n", printRoutineName);
						else
							fprintf(src, "(PrintFcn)NULL);\n\n");
					}
					else
						fprintf(src, "*** ERROR *** Unknown ANY\n\n");
				}
            }
        }
    }
/* REN -- end */

    if (!installedSomeHashes)
    {
        fprintf (src,"    /* Since no INTEGER/OID to ANY type relations were defined\n");
        fprintf (src,"     * (usually done via MACROs) you must manually do the code\n");
        fprintf (src,"     * to fill the hash tbl.\n");
        fprintf (src,"     * if the ids are INTEGER use the following:\n");
        fprintf (src,"     * InstallAnyByInt (??_ANY_ID, intVal, sizeof (Foo), (EncodeFcn) %sEncFoo, (DecodeFcn)%sDecFoo, (FreeFcn)FreeFoo, (PrintFcn)PrintFoo);\n", GetEncRulePrefix(), GetEncRulePrefix());
        fprintf (src,"     * if the ids are OBJECT IDENTIFIERs use the following:\n");
        fprintf (src,"     *     InstallAnyByOid (??_ANY_ID, oidVal, sizeof (Foo), (EncodeFcn) %sEncFoo, (DecodeFcn)%sDecFoo, (FreeFcn)FreeFoo, (PrintFcn)PrintFoo);\n", GetEncRulePrefix(), GetEncRulePrefix());
        fprintf (src,"     * put the ??_ANY_IDs in the AnyId enum.\n\n");
        fprintf (src,"     * For example if you have some thing like\n");
        fprintf (src,"     * T1 ::= SEQUENCE { id INTEGER, ANY DEFINED BY id }\n");
        fprintf (src,"     * and the id 1 maps to the type BOOLEAN use the following:\n");
        fprintf (src,"     * InstallAnyByInt (SOMEBOOL_ANY_ID, 1, sizeof (AsnBool), (EncodeFcn) %sEncAsnBool, (DecodeFcn)%sDecAsnBool, (FreeFcn)NULL, (PrintFcn)PrintAsnBool);;\n", GetEncRulePrefix(), GetEncRulePrefix());
        fprintf (src,"     */\n ???????\n");  /* generate compile error */
    }

    fprintf (src,"}  /* InitAny%s */\n\n\n", modName);

    Free (modName);

    printDecoders = printDecoders;
    printEncoders = printEncoders;
    mods = mods; /* AVOIDS warnings. */
}  /* PrintAnyHashInitRoutine */

/* REN -- 1/13/98 -- GetTypeDef() function added to return the type def info
for the given type. */
TypeDef*
GetTypeDef PARAMS ((t),
    Type *t)
{
    if (t == NULL)
		return NULL;

	switch (t->basicType->choiceId)
    {
        case BASICTYPE_LOCALTYPEREF:
        case BASICTYPE_IMPORTTYPEREF:
			return t->basicType->a.localTypeRef->link;
            break;

        default:
            return NULL;
    }
    /*fprintf (fHandle, "GetTypeDef: ERROR - cannot get type def for unlinked local/import type refs\n");
    return NULL;*/

}  /* GetTypeDef */
/* REN -- end */
