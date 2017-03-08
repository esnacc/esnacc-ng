/*
 * compiler/back-ends/py-gen/gen-any.c - routines for printing python
 *                                       anytype code
 *
 * assumes that the type tree has already been run through the
 * python type generator (py-gen/types.c).
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
#include <string.h>
#include "asn-incl.h"
#include "asn1module.h"
#include "str-util.h"
#include "rules.h"
#include "snacc-util.h"

#ifdef WIN32
#pragma  warning( disable : 4100 )  /* IGNORE unreferenced formal parameter */
#endif


void PrintPyAnyEnum PROTO ((FILE *hdr, Module *m));
void PrintPyAnyHashInitRoutine PROTO ((FILE *src, FILE *hdr,
									   Module *m, PyRules *r));
void PrintPyOidValue PROTO ((FILE *f, PyRules *r, AsnOid *oid, int parenOrQuote));

static TypeDef* GetTypeDef PROTO ((Type *t));


extern int anyEnumValG;


void
PrintPyAnyCode PARAMS ((src, hdr, r, mods, m),
    FILE *src _AND_
    FILE *hdr _AND_
    PyRules *r _AND_
    ModuleList *mods _AND_
    Module *m)
{

    if (!m->hasAnys)
        return;

    PrintPyAnyEnum(hdr, m);
    PrintPyAnyHashInitRoutine(src, hdr, m, r);

}  /* PrintAnyCode */



void
PrintPyAnyEnum PARAMS ((hdr, m),
                       FILE *hdr _AND_
                       Module *m)
{
    int firstPrinted = TRUE;
    char *modName;
	ValueDef *vd;
	Type *t;
    char anyId[512];

    modName = Asn1TypeName2CTypeName (m->modId->name);

    fprintf (hdr,"typedef enum %sAnyId\n", modName);
    fprintf (hdr,"{\n");

    FOR_EACH_LIST_ELMT (vd, m->valueDefs) {
        if (vd->value != NULL) {
            t = vd->value->type;
            if ((GetBuiltinType(t) == BASICTYPE_MACROTYPE) &&
                (t->basicType->a.macroType->choiceId ==
                 MACROTYPE_SNMPOBJECTTYPE)) {
                strcpy(anyId, vd->definedName);
                Dash2Underscore(anyId, strlen (anyId));
                strcat(anyId, "_ANY_ID");

                if (!firstPrinted)
                    fprintf (hdr,",\n");
                fprintf (hdr,"    %s = %d", anyId, anyEnumValG);
                anyEnumValG++;
                firstPrinted = FALSE;
            }
        }
    }

    if (firstPrinted)
        fprintf (hdr,"/* NO INTEGER or OBJECT IDENTIFIER to ANY type relationships were defined (via MACROs or other mechanism) */\n ??? \n");

    fprintf (hdr,"\n} %sAnyId;\n\n\n", modName);
    Free (modName);

}  /* PrintAnyEnum */


void
PrintPyAnyHashInitRoutine PARAMS ((src, hdr, m, r),
    FILE *src _AND_
    FILE *hdr _AND_
    Module *m _AND_
    PyRules *r)
{

    TypeDef *td;
    int i;
    int installedSomeHashes=0;
    struct CxxTDI *cxxtdi;
    char *modName;
    enum BasicTypeChoiceId typeId;
	ValueDef *vd;
	Type *t;
	BasicValue *bv;
    char anyId[512];
	char *typeName = NULL;

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
    fprintf (src,"};\n\n");

    fprintf (src,"static InitAny%s anyInitalizer;\n", modName);

    /* print constructor method that build hash tbl to src file*/
    fprintf (src,"InitAny%s::InitAny%s()\n", modName, modName);
    fprintf (src,"{\n");

	/* first print value for OID's */
    i = 0;

    FOR_EACH_LIST_ELMT (vd, m->valueDefs) {
        if (vd->value != NULL) {
            t = vd->value->type;
            if ((GetBuiltinType(t) == BASICTYPE_MACROTYPE) &&
                (t->basicType->a.macroType->choiceId ==
                 MACROTYPE_SNMPOBJECTTYPE)) {
                bv = vd->value->basicValue;
                if (bv != NULL)	{
                    installedSomeHashes = TRUE;
                    if (bv->choiceId == BASICVALUE_OID)	{
                        fprintf(src,"    %s oid%d",
                                r->typeConvTbl[BASICTYPE_OID].className, i++);
#if 0
                        PrintPyOidValue(src, r, bv->a.oid, 1);
#endif
                        fprintf(src,";\n");
					}
				}
            }
        }
    }
    fprintf (src,"\n\n");

    /* now print hash init calls */
    i = 0;

    FOR_EACH_LIST_ELMT (vd, m->valueDefs) {
        if (vd->value != NULL) {
            t = vd->value->type;
            if ((GetBuiltinType(t) == BASICTYPE_MACROTYPE) &&
                (t->basicType->a.macroType->choiceId ==
                 MACROTYPE_SNMPOBJECTTYPE))	{
                bv = vd->value->basicValue;
                if (bv != NULL) {
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
                         (typeId <= BASICTYPE_T61_STR))) {
                        typeName = r->typeConvTbl[typeId].className;
                    } else {
                        /* Else if the syntax of this any is either a locally
                           defined type or an imported type, get the name from
                           the this type's ref info. */
                        td = GetTypeDef(t);
                        if (td != NULL)	{
                            cxxtdi = td->cxxTypeDefInfo;
                            typeName = cxxtdi->className;
						} else
					        typeName = NULL;
					}

                    if (typeName == NULL) {
                        fprintf(src, "*** ERROR *** Unknown ANY\n\n");
                    } else {
                        if (bv->choiceId == BASICVALUE_OID)	{
                            fprintf(src,
                                    "    AsnAny::InstallAnyByOid (oid%d, %s, new %s);\n",
                                    i++, anyId, typeName);
                        } else if (bv->choiceId == BASICVALUE_INTEGER) {
                            fprintf(src,
                                    "    AsnAny::InstallAnyByInt (%d, %s, new %s);\n",
                                    bv->a.integer, anyId, typeName);
                        }
                    }
                }
            }
        }
    }

    if (!installedSomeHashes) {
        fprintf(src,"    /* Since no INTEGER/OID to ANY type relations were defined\n");
        fprintf(src,"     * (usually done via MACROs) you must manually do the code\n");
        fprintf(src,"     * to fill the hash tbl.\n");
        fprintf(src,"     * if the ids are INTEGER use the following:\n");
        fprintf(src,"     * AsnAny::InstallAnyByInt (3, ??_ANY_ID, new <className>);\n");
        fprintf(src,"     * if the ids are OBJECT IDENTIFIERs use the following:\n");
        fprintf(src,"     * AsnAny::InstallAnyByOid (OidValue, ??_ANY_ID, new <className>);\n");
        fprintf(src,"     * put the ??_ANY_IDs in the AnyId enum.\n\n");
        fprintf(src,"     * For example if you have some thing like\n");
        fprintf(src,"     * T1 ::= SEQUENCE { id INTEGER, ANY DEFINED BY id }\n");
        fprintf(src,"     * and the id 1 maps to the type BOOLEAN use the following:\n");
        fprintf(src,"     * AsnAny::InstallAnyByInt (1, SOMEBOOL_ANY_ID, new AsnBool);\n");
        fprintf(src,"     */\n ???????\n");  /* generate compile error */
    }


    fprintf (src,"}  /* InitAny::InitAny */\n\n\n");
}  /* PrintAnyHashInitRoutine */


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
    return NULL;

}  /* GetTypeDef */
