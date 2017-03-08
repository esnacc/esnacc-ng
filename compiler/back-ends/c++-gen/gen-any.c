/*
 * compiler/back_ends/c++_gen/gen_any.c
 *
 *     prints Routine to initialize the ANY Hash table.  The
 *     ANY Hash table maps the OBJECT IDENTIFIERS or INTEGERS
 *     to the correct decoding routines.
 *
 *     Also prints an enum to identify each ANY mapping.
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
 * INSERT_VDA_COMMENTS
 *
 * $Header: /baseline/SNACC/compiler/back-ends/c++-gen/gen-any.c,v 1.16 2003/12/19 20:55:49 leonberp Exp $
 * $Log: gen-any.c,v $
 * Revision 1.16  2003/12/19 20:55:49  leonberp
 * added parameter to PrintCxxOidValue() to allow printing of paren or quote to be specified.
 *
 * Revision 1.15  2003/07/07 14:52:35  nicholar
 * Eliminated headers and cleaned up include references
 *
 * Revision 1.14  2002/10/29 16:37:34  mcphersc
 * changed the any global int variable used for any defined by's
 *
 * Revision 1.13  2002/09/16 17:48:02  mcphersc
 *
 * iFixed warings
 *
 * Revision 1.12  2002/09/04 17:51:35  vracarl
 * got rid of c++ comments
 *
 * Revision 1.11  2002/05/15 14:53:10  leonberp
 * added support for new basicTypes to compiler
 *
 * Revision 1.10  2002/02/28 19:45:17  nicholar
 * Added calls to Dash2Underscore() to remove dashes in ANYs.
 *
 * Revision 1.9  2001/10/16 04:26:09  leonberp
 * removed destructor for any tables and moved it to the runtime library
 *
 * Revision 1.8  2001/10/12 15:05:45  leonberp
 * Fixed bug in InitAny destructor generator
 *
 * Revision 1.7  2001/10/12 14:53:47  leonberp
 * Fixed bug in InitAny destructor generator
 *
 * Revision 1.6  2001/10/11 18:34:31  leonberp
 * added code to generate destructor for InitAny classes so they oid and int hash tables will be cleaned up.  Also fixed the hash table clean up code to be recursive.
 *
 * Revision 1.5  2001/06/11 18:30:56  rwc
 * Re-added actual constructor of module specific ANY DEFINED BY table update.
 * vda test now works again.  NEED TO RE-TEST with multiple modules.
 *
 * Revision 1.4  2001/05/11 16:00:46  mcphersc
 * Fixed problems with Init Any's and class statement in gen-code.
 * Gen-code not allowing for "::" in some instances
 *
 * Revision 1.3  2001/04/18 16:28:56  rwc
 * Updated test for ANY DEFINED BY updates.  Now supports default CSM_Buffer (AsnAnyBuffer) result if
 * OID is not recognized.
 *
 * Revision 1.2  2000/10/24 14:54:43  rwc
 * Updated to remove high-level warnings (level 4 on MSVC++) for an easier build.
 * SOME warnings persist due to difficulty in modifying the SNACC compiler to
 * properly build clean source; also some files are built by Lex/Yacc.
 *
 * Revision 1.1.1.1  2000/08/21 20:36:05  leonberp
 * First CVS Version of SNACC.
 *
 * Revision 1.4  1995/07/25 18:19:11  rj
 * changed `_' to `-' in file names.
 *
 * Revision 1.3  1994/10/08  03:47:53  rj
 * since i was still irritated by cpp standing for c++ and not the C preprocessor, i renamed them to cxx (which is one known suffix for C++ source files). since the standard #define is __cplusplus, cplusplus would have been the more obvious choice, but it is a little too long.
 *
 * Revision 1.2  1994/09/01  01:06:31  rj
 * snacc_config.h removed.
 *
 * Revision 1.1  1994/08/28  09:47:58  rj
 * first check-in. for a list of changes to the snacc-1.1 distribution please refer to the ChangeLog.
 *
 */

#include "config.h"
#include <string.h>			/* REN -- 5/11/2001 -- added for string functions */
#include "asn-incl.h"
#include "asn1module.h"
#include "str-util.h"
#include "rules.h"
#include "snacc-util.h"		/* REN -- 5/11/2001 -- added for GetBuiltinType() */

#ifdef WIN32
#pragma  warning( disable : 4100 )  /* IGNORE unreferenced formal parameter */
#endif


void PrintCxxAnyEnum PROTO ((FILE *hdr, Module *m, CxxRules *r));
void PrintCxxAnyHashInitRoutine PROTO ((FILE *src, FILE *hdr, ModuleList *mods,
									   Module *m, CxxRules *r));
void PrintCxxOidValue PROTO ((FILE *f, CxxRules *r, AsnOid *oid, int parenOrQuote));

/* REN -- 5/11/2001 -- added following prototype */
static TypeDef* GetTypeDef PROTO ((Type *t));


extern int anyEnumValG;


void
PrintCxxAnyCode PARAMS ((src, hdr, r, mods, m),
    FILE *src _AND_
    FILE *hdr _AND_
    CxxRules *r _AND_
    ModuleList *mods _AND_
    Module *m)
{

    if (!m->hasAnys)
        return;

    PrintCxxAnyEnum (hdr, m, r);
    PrintCxxAnyHashInitRoutine (src, hdr, mods, m, r);

}  /* PrintAnyCode */



void
PrintCxxAnyEnum PARAMS ((hdr, m, r),
    FILE *hdr _AND_
    Module *m _AND_
    CxxRules *r)
{
    int firstPrinted = TRUE;
    char *modName;
/* REN -- 5/11/2001 */
/*  TypeDef *td;
     AnyRef *ar;
    AnyRefList *arl;
    int i; */
	ValueDef *vd;
	Type *t;
    char anyId[512];
/* REN -- end */

    modName = Asn1TypeName2CTypeName (m->modId->name);

    fprintf (hdr,"typedef enum %sAnyId\n", modName);
    fprintf (hdr,"{\n");

/* REN -- 5/11/2001 -- Changed method in which anyRefs get written.  Original 
code looped through the module's basic types and local type refs and wrote the 
anys that referred to them.  Corrected method loops through the valueRefs and 
writes all of the SNMP Object Types.
Removed the following loops:
     do any lib types 
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

REN -- 5/11/2001 -- added the following: */
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
        fprintf (hdr,"/* NO INTEGER or OBJECT IDENTIFIER to ANY type relationships were defined (via MACROs or other mechanism) */\n ??? \n");

    fprintf (hdr,"\n} %sAnyId;\n\n\n", modName);
    Free (modName);

}  /* PrintAnyEnum */


void
PrintCxxAnyHashInitRoutine PARAMS ((src, hdr, mods, m, r),
    FILE *src _AND_
    FILE *hdr _AND_
    ModuleList *mods _AND_
    Module *m _AND_
    CxxRules *r)
{

    TypeDef *td;
    int i;
    int installedSomeHashes=0;
    struct CxxTDI *cxxtdi;
    char *modName;
/* REN -- 5/11/2001 */
/*  AnyRef *ar;
    AnyRefList *arl;
    int j; */
    enum BasicTypeChoiceId typeId;
	ValueDef *vd;
	Type *t;
	BasicValue *bv;
    char anyId[512];
	char *typeName = NULL;
/* REN -- end */

    modName = Asn1TypeName2CTypeName (m->modId->name);

    /* print Any class src file */
    fprintf (src,"// this class will automatically intialize the any hash tbl\n");
    fprintf (src,"class InitAny%s\n", modName);
    fprintf (src,"{\n");
    fprintf (src,"  public:\n");
    fprintf (src,"    InitAny%s();\n", modName);
    fprintf (src,"    /* Do not add a destructor to this class!  It could\n");
    fprintf (src,"     * cause pre-mature destruction of the ANY tables.\n");
    fprintf (src,"     * The ANY tables will be destroyed by the runtime library.\n");
    fprintf (src,"     */\n");
#if 0
    fprintf (src,"    ~InitAny%s() { AsnAny::AsnAnyDestroyHashTbls(); }\n",modName);
#endif
    fprintf (src,"};\n\n");

    fprintf (src,"static InitAny%s anyInitalizer;\n", modName);

    /* print constructor method that build hash tbl to src file*/
    fprintf (src,"InitAny%s::InitAny%s()\n", modName, modName);
    fprintf (src,"{\n");

/* REN -- 5/11/2001 -- Changed method in which anyRefs get written.  Original
code looped through the module's basic types and local type refs and wrote the
anys that referred to them.  Corrected method loops through the valueRefs and
writes all of the SNMP Object Types.
Removed the following loops:
   first print value for OID's *
     do any lib types first *
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
                    fprintf (src,"    %s oid%d", r->typeConvTbl[BASICTYPE_OID].className, i++);
                    PrintCxxOidValue (src, r, ar->id->a.oid);
                    fprintf (src,";\n");
                }
                else if (ar->id->choiceId == OIDORINT_INTID)
                {
                    fprintf (src,"    %s int%d", r->typeConvTbl[BASICTYPE_INTEGER].className, i++);
                    PrintCxxIntValue (src, r, ar->id->a.intId);
                    fprintf (src,";\n");
                }
            }
        }
    }


    FOR_EACH_LIST_ELMT (td, m->typeDefs)
    {
        if (td->anyRefs != NULL)
        {
            cxxtdi = td->cxxTypeDefInfo;
            FOR_EACH_LIST_ELMT (ar, td->anyRefs)
            {
                installedSomeHashes = TRUE;
                if (ar->id->choiceId == OIDORINT_OID)
                {
                    fprintf (src,"    %s oid%d", r->typeConvTbl[BASICTYPE_OID].className, i++);
                    PrintCxxOidValue (src, r, ar->id->a.oid);
                    fprintf (src,";\n");
                }
                else if (ar->id->choiceId == OIDORINT_INTID)
                {
                    fprintf (src,"    %s int%d", r->typeConvTbl[BASICTYPE_INTEGER].className, i++);
                    PrintCxxIntValue (src, r, ar->id->a.intId);
                    fprintf (src,";\n");
                }
            }
        }
    }


    * now print hash init calls *
    i = 0;
    for (j = BASICTYPE_BOOLEAN; j < BASICTYPE_MACRODEF; j++)
    {
        arl = LIBTYPE_GET_ANY_REFS (j);
        if (arl != NULL)
        {
            FOR_EACH_LIST_ELMT (ar, arl)
            {
                if (ar->id->choiceId == OIDORINT_OID)
                    fprintf (src,"    AsnAny::InstallAnyByOid (oid%d, %s, new %s);\n", i++, ar->anyIdName, r->typeConvTbl[j].className);

                else
                    fprintf (src,"    AsnAny::InstallAnyByInt (int%d, %s, new %s);\n", i++, ar->anyIdName, r->typeConvTbl[j].className);

            }
        }
    }

    FOR_EACH_LIST_ELMT (td, m->typeDefs)
    {
        if (td->anyRefs != NULL)
        {
            FOR_EACH_LIST_ELMT (ar, td->anyRefs)
            {
                cxxtdi = td->cxxTypeDefInfo;

                if (ar->id->choiceId == OIDORINT_OID)
                    fprintf (src,"    AsnAny::InstallAnyByOid (oid%d, %s, new %s);\n", i++, ar->anyIdName, cxxtdi->className);

                else
                    fprintf (src,"    AsnAny::InstallAnyByInt (int%d, %s, new %s);\n", i++, ar->anyIdName, cxxtdi->className);

            }
        }
    }

REN -- 5/11/2001 -- added the following: */
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
	                    fprintf (src,"    %s oid%d", 
							r->typeConvTbl[BASICTYPE_OID].className, i++);
		                PrintCxxOidValue (src, r, bv->a.oid, 1);
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
					class name from the rules table. */
					typeId = t->basicType->choiceId;
					if (((typeId >= BASICTYPE_BOOLEAN) &&
						(typeId <= BASICTYPE_SETOF)) ||
						((typeId >= BASICTYPE_NUMERIC_STR) &&
						(typeId <= BASICTYPE_T61_STR)))
					{
						typeName = r->typeConvTbl[typeId].className;
					}
					/* Else if the syntax of this any is either a locally
					defined type or an imported type, get the class name from
					the this type's ref info. */
					else
					{
						td = GetTypeDef(t);
						if (td != NULL)
						{
							cxxtdi = td->cxxTypeDefInfo;
							typeName = cxxtdi->className;
						}
						else
					        typeName = NULL;
					}

					if (typeName == NULL)
						fprintf(src, "*** ERROR *** Unknown ANY\n\n");
					else
					{
						if (bv->choiceId == BASICVALUE_OID)
						{
							fprintf (src,
								"    AsnAny::InstallAnyByOid (oid%d, %s, new %s);\n",
								i++, anyId, typeName);
						}
						else if (bv->choiceId == BASICVALUE_INTEGER)
						{
							fprintf (src,
								"    AsnAny::InstallAnyByInt (%d, %s, new %s);\n",
								bv->a.integer, anyId, typeName);
						}
					}
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
        fprintf (src,"     * AsnAny::InstallAnyByInt (3, ??_ANY_ID, new <className>);\n");
        fprintf (src,"     * if the ids are OBJECT IDENTIFIERs use the following:\n");
        fprintf (src,"     * AsnAny::InstallAnyByOid (OidValue, ??_ANY_ID, new <className>);\n");
        fprintf (src,"     * put the ??_ANY_IDs in the AnyId enum.\n\n");
        fprintf (src,"     * For example if you have some thing like\n");
        fprintf (src,"     * T1 ::= SEQUENCE { id INTEGER, ANY DEFINED BY id }\n");
        fprintf (src,"     * and the id 1 maps to the type BOOLEAN use the following:\n");
        fprintf (src,"     * AsnAny::InstallAnyByInt (1, SOMEBOOL_ANY_ID, new AsnBool);\n");
        fprintf (src,"     */\n ???????\n");  /* generate compile error */
    }


    fprintf (src,"}  /* InitAny::InitAny */\n\n\n");
/* RWC;4/23/01;#endif */

/* RWC;4/23/01;    r;m;mods;hdr;src;  */ /*AVOIDS warning.*/
}  /* PrintAnyHashInitRoutine */


/* REN -- 5/11/2001 -- GetTypeDef() function added to return the type def info
for the given type. */
static TypeDef*
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
    /*fprintf (errFileG, "GetTypeDef: ERROR - cannot get type def for unlinked local/import type refs\n");
    return NULL;*/

}  /* GetTypeDef */
/* REN -- end */
