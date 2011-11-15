/*
 * compiler/back_ends/idl_gen/gen_any.c
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
 * $Header: /baseline/SNACC/compiler/back-ends/idl-gen/gen-any.c,v 1.6 2004/04/06 15:13:41 gronej Exp $
 * $Log: gen-any.c,v $
 * Revision 1.6  2004/04/06 15:13:41  gronej
 * *** empty log message ***
 *
 * Revision 1.5  2003/07/07 14:51:57  nicholar
 * Eliminated headers and cleaned up include references
 *
 * Revision 1.4  2002/10/30 13:41:51  mcphersc
 * modified anyid
 *
 * Revision 1.3  2002/09/04 18:02:28  vracarl
 * got rid of c++ comments
 *
 * Revision 1.2  2000/10/24 14:54:48  rwc
 * Updated to remove high-level warnings (level 4 on MSVC++) for an easier build.
 * SOME warnings persist due to difficulty in modifying the SNACC compiler to
 * properly build clean source; also some files are built by Lex/Yacc.
 *
 * Revision 1.1.1.1  2000/08/21 20:36:04  leonberp
 * First CVS Version of SNACC.
 *
 * Revision 1.1  1997/01/01 20:25:32  rj
 * first draft
 *
 */

#include "asn-incl.h"
#include "asn1module.h"
#include "str-util.h"
#include "rules.h"
#include "lib-types.h"


static int anyEnumValG = 0;

void PrintIDLAnyEnum PROTO ((FILE *idl, Module *m, IDLRules *r));
void PrintIDLAnyHashInitRoutine PROTO ((FILE *idl, ModuleList *mods,
									   Module *m, IDLRules *r));


void
PrintIDLAnyCode PARAMS ((idl, r, mods, m),
    FILE *idl _AND_
    IDLRules *r _AND_
    ModuleList *mods _AND_
    Module *m)
{

    if (!m->hasAnys)
        return;

    PrintIDLAnyEnum (idl, m, r);
#if 0
    PrintIDLAnyHashInitRoutine (idl, mods, m, r);
#endif
    mods=mods;   /*AVOIDS warning.*/

}  /* PrintAnyCode */



void
PrintIDLAnyEnum PARAMS ((idl, m, r),
    FILE *idl _AND_
    Module *m _AND_
    IDLRules *r)
{
    TypeDef *td;
    AnyRef *ar;
    AnyRefList *arl;
    int firstPrinted = TRUE;
    int i;
    char *modName;

    modName = Asn1TypeName2CTypeName (m->modId->name);

    fprintf (idl,"typedef enum %sAnyId\n", modName);
    fprintf (idl,"{\n");

    /* do any lib types */
    for (i = BASICTYPE_BOOLEAN; i < BASICTYPE_MACRODEF; i++)
    {
        arl = LIBTYPE_GET_ANY_REFS (i);
        if (arl != NULL)
        {
            FOR_EACH_LIST_ELMT (ar, arl)
            {
                if (!firstPrinted)
                    fprintf (idl,",\n");
                fprintf (idl,"    %s = %d", ar->anyIdName, anyEnumValG);
				anyEnumValG++;
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
                    fprintf (idl,",\n");
                fprintf (idl,"    %s = %d", ar->anyIdName, anyEnumValG);
				anyEnumValG++;
                firstPrinted = FALSE;
            }
        }
    }

    if (firstPrinted) /* none have been printed */
        fprintf (idl,"/* NO INTEGER or OBJECT IDENTIFIER to ANY type relationships were defined (via MACROs or other mechanism) */\n ??? \n");

    fprintf (idl,"\n} %sAnyId;\n\n\n", modName);
    Free (modName);

    r=r;   /*AVOIDS warning.*/
}  /* PrintAnyEnum */


#if 0
void
PrintIDLAnyHashInitRoutine PARAMS ((idl, mods, m, r),
    FILE *idl _AND_
    ModuleList *mods _AND_
    Module *m _AND_
    IDLRules *r)
{
    TypeDef *td;
    AnyRefList *arl;
    AnyRef *ar;
    IDLTDI *idltdi;
    int i;
    int j;
    enum BasicTypeChoiceId typeId;
    int installedSomeHashes = FALSE;


    /* print InitAny class src file */
    fprintf (src,"// this class will automatically intialize the any hash tbl\n");
    fprintf (src,"class InitAny\n");
    fprintf (src,"{\n");
    fprintf (src,"  public:\n");
    fprintf (src,"    InitAny();\n");
    fprintf (src,"};\n\n");

    fprintf (src,"static InitAny anyInitalizer;\n");

    /* print constructor method that build hash tbl to src file*/
    fprintf (src,"InitAny::InitAny()\n");
    fprintf (src,"{\n");

    /* first print value for OID's */

    /* do any lib types first */
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
                    PrintIDLOidValue (src, r, ar->id->a.oid);
                    fprintf (src,";\n");
                }
                else if (ar->id->choiceId == OIDORINT_INTID)
                {
                    fprintf (src,"    %s int%d", r->typeConvTbl[BASICTYPE_INTEGER].className, i++);
                    PrintIDLIntValue (src, r, ar->id->a.intId);
                    fprintf (src,";\n");
                }
            }
        }
    }


    FOR_EACH_LIST_ELMT (td, m->typeDefs)
    {
        if (td->anyRefs != NULL)
        {
            idltdi = td->idlTypeDefInfo;
            FOR_EACH_LIST_ELMT (ar, td->anyRefs)
            {
                installedSomeHashes = TRUE;
                if (ar->id->choiceId == OIDORINT_OID)
                {
                    fprintf (src,"    %s oid%d", r->typeConvTbl[BASICTYPE_OID].className, i++);
                    PrintIDLOidValue (src, r, ar->id->a.oid);
                    fprintf (src,";\n");
                }
                else if (ar->id->choiceId == OIDORINT_INTID)
                {
                    fprintf (src,"    %s int%d", r->typeConvTbl[BASICTYPE_INTEGER].className, i++);
                    PrintIDLIntValue (src, r, ar->id->a.intId);
                    fprintf (src,";\n");
                }
            }
        }
    }


    /* now print hash init calls */
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
                idltdi = td->idlTypeDefInfo;

                if (ar->id->choiceId == OIDORINT_OID)
                    fprintf (src,"    AsnAny::InstallAnyByOid (oid%d, %s, new %s);\n", i++, ar->anyIdName, idltdi->className);

                else
                    fprintf (src,"    AsnAny::InstallAnyByInt (int%d, %s, new %s);\n", i++, ar->anyIdName, idltdi->className);

            }
        }
    }

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

}  /* PrintAnyHashInitRoutine */
#endif
