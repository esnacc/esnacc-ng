/*
 * compiler/core/link.c
 *
 *  first links value refs in the import list then
 *  links value references in value defs and types' default values
 *
 *
 * Mike Sample
 * 91/09/04
 * Completely Rewritten for new ModuleList data structure (ASN.1 based)
 *
 * Copyright (C) 1991, 1992 Michael Sample
 *            and the University of British Columbia
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * $Header: /baseline/SNACC/compiler/core/link-values.c,v 1.14 2004/04/06 15:13:41 gronej Exp $
 * $Log: link-values.c,v $
 * Revision 1.14  2004/04/06 15:13:41  gronej
 * *** empty log message ***
 *
 * Revision 1.13  2004/01/29 21:21:45  gronej
 * Took all Init() calls out of Clear() functions in generated code
 *
 * Revision 1.12  2004/01/14 19:07:52  gronej
 * Updated Compiler to accept and process relative-oid's
 *
 * Revision 1.11  2003/07/14 21:07:44  nicholar
 * Changed how parser handles --snacc directives.  Added namespace option.
 *
 * Revision 1.10  2003/07/07 14:50:13  nicholar
 * Eliminated headers and cleaned up include references
 *
 * Revision 1.9  2003/04/29 21:08:25  leonberp
 * integerated Deepak's changes for IOB support
 *
 * Revision 1.8  2002/11/01 17:16:21  mcphersc
 * added import used
 *
 * Revision 1.7  2002/10/28 19:57:03  leonberp
 * Added BITSTRING CONTAINING support and fixed CONTAINED ANY DEFINED BY
 *
 * Revision 1.6  2002/10/24 21:07:22  leonberp
 * latest fixing for OCTET CONTAINING
 *
 * Revision 1.5  2002/09/16 16:50:08  mcphersc
 * Fixed warnings
 *
 * Revision 1.4  2002/09/04 18:23:06  vracarl
 * got rid of c++ comments
 *
 * Revision 1.3  2002/05/15 17:00:59  leonberp
 * added support for new basicTypes to compiler
 *
 * Revision 1.2  2000/10/24 14:54:53  rwc
 * Updated to remove high-level warnings (level 4 on MSVC++) for an easier build.
 * SOME warnings persist due to difficulty in modifying the SNACC compiler to
 * properly build clean source; also some files are built by Lex/Yacc.
 *
 * Revision 1.1.1.1  2000/08/21 20:36:00  leonberp
 * First CVS Version of SNACC.
 *
 * Revision 1.3  1995/07/25 19:41:38  rj
 * changed `_' to `-' in file names.
 *
 * Revision 1.2  1994/09/01  00:38:43  rj
 * snacc_config.h removed.
 *
 * Revision 1.1  1994/08/28  09:49:19  rj
 * first check-in. for a list of changes to the snacc-1.1 distribution please refer to the ChangeLog.
 *
 */

#include <ctype.h>
#include "asn-incl.h"
#include "asn1module.h"
#include "snacc-util.h"


/* non-exported fcn prototypes */
void ValueLinkImportLists PROTO ((ModuleList *m));
void ValueLinkTypeDef PROTO ((ModuleList *m, Module *currMod, TypeDef *head));
void ValueLinkElmtTypes PROTO ((ModuleList *m, Module *currMod, TypeDef *head, NamedTypeList *e));
void ValueLinkElmtType PROTO ((ModuleList *m, Module *currMod, TypeDef *head, NamedType *n));
void ValueLinkTag PROTO ((ModuleList *m, Module *currMod, Type *t, Tag *tag));
void ValueLinkType PROTO ((ModuleList *m, Module *currMod, TypeDef *head, Type *currType));
void ValueLinkBasicType PROTO ((ModuleList *m, Module *currMod, TypeDef *head, Type *type, BasicType *bt));
void ValueLinkSubtypes PROTO ((ModuleList *m, Module *currMod, TypeDef *head, Type *currType, Subtype *s));
void ValueLinkSubtypeValue PROTO ((ModuleList *m, Module *currMod, TypeDef *head, Type *currType, SubtypeValue *s));
void ValueLinkNamedElmts PROTO ((ModuleList *m, Module *currMod, TypeDef *head, Type *t, ValueDefList *v));
void ValueLinkValueDef PROTO ((ModuleList *m, Module *currMod, ValueDef *v));
void ValueLinkValue PROTO ((ModuleList *m, Module *currMod, ValueDef *head, Type *valuesType, Value *v));
void ValueLinkBasicValue PROTO ((ModuleList *m, Module *currMod, ValueDef *head, Type *valuesType, Value *v, BasicValue *bv));
void ValueLinkOid PROTO ((ModuleList *m, Module *currMod, ValueDef *head, Value *v, OID *oid));
void ValueLinkRosOperationMacroType PROTO ((ModuleList *m, Module *currMod, TypeDef *head, Type *t, BasicType *bt, RosOperationMacroType *op));
void ValueLinkRosErrorMacroType PROTO ((ModuleList *m, Module *currMod, TypeDef *head, Type *t, BasicType *bt, RosErrorMacroType *err));
void ValueLinkRosBindMacroType PROTO ((ModuleList *m, Module *currMod, TypeDef *head, Type *t, BasicType *bt, RosBindMacroType *bind));
void ValueLinkRosAseMacroType PROTO ((ModuleList *m, Module *currMod, TypeDef *head, Type *t, BasicType *bt, RosAseMacroType *ase));
void ValueLinkRosAcMacroType PROTO ((ModuleList *m, Module *currMod, TypeDef *head, Type *t, BasicType *bt, RosAcMacroType *ac));
void ValueLinkMtsasExtensionsMacroType PROTO ((ModuleList *m, Module *currMod, TypeDef *head, Type *t, BasicType *bt, MtsasExtensionsMacroType *exts));
void ValueLinkMtsasExtensionMacroType PROTO ((ModuleList *m, Module *currMod, TypeDef *head, Type *t, BasicType *bt, MtsasExtensionMacroType *ext));
void ValueLinkMtsasExtensionAttributeMacroType PROTO ((ModuleList *m, Module *currMod, TypeDef *head, Type *t, BasicType *bt, MtsasExtensionAttributeMacroType *ext));
void ValueLinkMtsasTokenMacroType PROTO ((ModuleList *m, Module *currMod, TypeDef *head, Type *t, BasicType *bt, MtsasTokenMacroType *tok));
void ValueLinkMtsasTokenDataMacroType PROTO ((ModuleList *m, Module *currMod, TypeDef *head, Type *t, BasicType *bt, MtsasTokenDataMacroType *tok));
void ValueLinkMtsasSecurityCategoryMacroType PROTO ((ModuleList *m, Module *currMod, TypeDef *head, Type *t, BasicType *bt, MtsasSecurityCategoryMacroType *sec));
void ValueLinkAsnObjectMacroType PROTO ((ModuleList *m, Module *currMod, TypeDef *head, Type *t, BasicType *bt, AsnObjectMacroType *obj));
void ValueLinkAsnPortMacroType PROTO ((ModuleList *m, Module *currMod, TypeDef *head, Type *t, BasicType *bt, AsnPortMacroType *p));
void ValueLinkAsnAbstractBindMacroType PROTO ((ModuleList *m, Module *currMod, TypeDef *head, Type *t, BasicType *bt, AsnAbstractBindMacroType *bind));
void ValueLinkSnmpObjectTypeMacroType PROTO ((ModuleList *m, Module *currMod, TypeDef *head, Type *t, BasicType *bt, SnmpObjectTypeMacroType *ot));
/* end of prototypes */




static char *asn1SrcFileNameG;
static int linkOidCallDepthG = 0; /* big hack!! */

/*
 * returns 0 if no link error occured
 * otherwise returns a value < 0.
 * processing should not continue is an error is returned
 */
int
LinkValueRefs PARAMS ((m),
    ModuleList *m)
{
    Module   *currMod;
    TypeDef  *td;
    ValueDef *vd;
    int linkErr = 0;

    /*
     * link imported types/values to there definition if
     * the defining module is in the modulelist
     */
    ValueLinkImportLists (m);


    /*
     * go through types, values & macros of each parsed module
     */

    FOR_EACH_LIST_ELMT (currMod, m)
    {

        asn1SrcFileNameG = currMod->asn1SrcFileName;

        /*
         * link this modules object identifier value
         */
        ValueLinkOid (m, currMod, NULL, NULL, currMod->modId->oid);


        /*
         * go through each type in typeList and link as nec
         */
        FOR_EACH_LIST_ELMT (td, currMod->typeDefs)
        {
            ValueLinkTypeDef (m, currMod, td);
        }

        /*
         *  go through each value in valueList and link as nec
         */
        FOR_EACH_LIST_ELMT (vd, currMod->valueDefs)
        {
            ValueLinkValueDef (m, currMod, vd);
        }

        if (currMod->status != MOD_ERROR)
		{
			currMod->ImportUsed = TRUE;
            currMod->status = MOD_OK;
		}
        else
            linkErr = -1;
    }

    return linkErr;

} /* ValueLinkRefs */


/*
 * go through each modules import lists and link
 * any values as nec. values'symbols start with a
 * lowercase letter
 */
void
ValueLinkImportLists PARAMS ((m),
    ModuleList *m)
{
    Module *currMod;
    ValueDef *v;
    ImportModule *currImpList;
    ImportElmt *currImpElmt;
    Module *impRefMod;


    /* link imports of each module in the list */
    FOR_EACH_LIST_ELMT (currMod, m)
    {
        /* for each import list in the current module */
        FOR_EACH_LIST_ELMT (currImpList, currMod->imports)
        {
            /* see if the referenced module is in the list */
            impRefMod = LookupModule (m, currImpList->modId->name, currImpList->modId->oid);

            if (impRefMod == NULL)
            {
                /* the type linker will have reported this error */
                continue;
            }

            /*
             * link each value referencing import elmt in
             * the current import list
             */
            FOR_EACH_LIST_ELMT (currImpElmt, currImpList->importElmts)
            {
                /*
                 * only link values (all vals have lowercase first letter)
                 */
                if (!islower (currImpElmt->name[0]))
                    continue;

                v = LookupValue (impRefMod->valueDefs, currImpElmt->name);
                if (v != NULL)
                {
                    if (!v->exported)
                    {
                        currMod->status = MOD_ERROR;
                        PrintErrLoc (currMod->asn1SrcFileName, (long)currImpElmt->lineNo);
                        fprintf (errFileG, "ERROR - \"%s\" module imports value \"%s\", which is not exported from module \"%s\".\n",
							currMod->modId->name, currImpElmt->name,
							impRefMod->modId->name);
                    }
                    /* resolve value */
                    currImpElmt->resolvedRef =
                        (ImportElmtChoice*)Malloc (sizeof (ImportElmtChoice));
                    currImpElmt->resolvedRef->choiceId = IMPORTELMTCHOICE_VALUE;
                    currImpElmt->resolvedRef->a.value = v;
                }
                else /* value not found in ref'd module */
                {
                    currMod->status = MOD_ERROR;
                    PrintErrLoc (currMod->asn1SrcFileName, (long)currImpElmt->lineNo);
                    fprintf (errFileG, "ERROR - \"%s\" is imported from module \"%s\" by module \"%s\", but is not defined in the referenced module\n",
						currImpElmt->name, impRefMod->modId->name,
						currMod->modId->name);
		}

            }
        }
    }
} /* ValueLinkImportLists */



void
ValueLinkTypeDef PARAMS ((m, currMod, head),
    ModuleList *m _AND_
    Module *currMod _AND_
    TypeDef *head)
{

    if (head == NULL)
        return;

    ValueLinkType (m, currMod, head, head->type);

}  /* ValueLinkTypeDef */



void
ValueLinkType PARAMS ((m, currMod, head, t),
    ModuleList *m _AND_
    Module *currMod _AND_
    TypeDef *head _AND_
    Type *t)
{
    Tag *tag;

    if (t == NULL)
        return;

	// Link any values that appear in the tags
    FOR_EACH_LIST_ELMT (tag, t->tags)
    {
        ValueLinkTag (m, currMod, t, tag);
    }

    ValueLinkBasicType (m, currMod, head, t, t->basicType);

    ValueLinkSubtypes (m, currMod, head, t, t->subtypes);

    if (t->defaultVal != NULL)
        ValueLinkValue (m, currMod, NULL, t,  t->defaultVal->value);

}  /* ValueLinkType */



void
ValueLinkTag PARAMS ((m, currMod, head, tag),
    ModuleList *m _AND_
    Module *currMod _AND_
    Type *t _AND_
    Tag *tag)
{
	if (tag->valueRef == NULL)
		return;

	ValueLinkValue (m, currMod, NULL, t,  tag->valueRef);

} /* ValueLinkTag */


void
ValueLinkElmtTypes PARAMS ((m, currMod, head, e),
    ModuleList *m _AND_
    Module *currMod _AND_
    TypeDef *head _AND_
    NamedTypeList *e)
{
    NamedType *n;
    FOR_EACH_LIST_ELMT (n, e)
    {
        ValueLinkElmtType (m, currMod, head, n);
    }
}  /* ValueLinkElmtTypes */


void
ValueLinkElmtType PARAMS ((m, currMod, head, n),
    ModuleList *m _AND_
    Module *currMod _AND_
    TypeDef *head _AND_
    NamedType *n)
{
    if (n != NULL)
        ValueLinkType (m, currMod, head, n->type);
}


void
ValueLinkBasicType PARAMS ((m, currMod, head, type, bt),
    ModuleList *m _AND_
    Module *currMod _AND_
    TypeDef *head _AND_
    Type *type _AND_
    BasicType *bt)
{

    if (bt == NULL)
        return;

    switch (bt->choiceId)
    {
		case BASICTYPE_SEQUENCET:		// Deepak: added on 29/Nov/2002
        case BASICTYPE_SEQUENCE:
        case BASICTYPE_SET:
        case BASICTYPE_CHOICE:			// Deepak: process the elements in sequence etc here.
            ValueLinkElmtTypes (m, currMod, head, bt->a.set);
            break;



        case BASICTYPE_SEQUENCEOF:
        case BASICTYPE_SETOF:
            ValueLinkType (m, currMod, head, bt->a.setOf);
            break;

		case BASICTYPE_OBJECTCLASS:			// Deepak: added on 31/03/2003
			ValueLinkElmtTypes (m, currMod, head, bt->a.objectclass->classdef);
			break;

        case BASICTYPE_SELECTION:
        case BASICTYPE_COMPONENTSOF:
        case BASICTYPE_ANYDEFINEDBY:
        case BASICTYPE_LOCALTYPEREF:
        case BASICTYPE_IMPORTTYPEREF:
		case BASICTYPE_OBJECTCLASSFIELDTYPE:		// Deepak: added on 05/Feb/2003
            break;

        /*
         * these types may optionally have named elmts
         */
        case BASICTYPE_INTEGER:
        case BASICTYPE_BITSTRING:
        case BASICTYPE_ENUMERATED:
            ValueLinkNamedElmts (m, currMod, head, type, bt->a.integer);
            break;



        /*
         * these types have no extra info and cause no linking action
         */
        case BASICTYPE_UNKNOWN:
        case BASICTYPE_BOOLEAN:
        case BASICTYPE_OCTETSTRING:
        case BASICTYPE_NULL:
        case BASICTYPE_OID:
        case BASICTYPE_RELATIVE_OID:
        case BASICTYPE_REAL:
        case BASICTYPE_EXTERNAL:
        case BASICTYPE_ANY:
        case BASICTYPE_MACRODEF:
        case BASICTYPE_NUMERIC_STR:
        case BASICTYPE_PRINTABLE_STR:
        case BASICTYPE_VISIBLE_STR:
        case BASICTYPE_GRAPHIC_STR:
        case BASICTYPE_VIDEOTEX_STR:
        case BASICTYPE_GENERAL_STR:
        case BASICTYPE_IA5_STR:
        case BASICTYPE_BMP_STR:
        case BASICTYPE_UNIVERSAL_STR:
        case BASICTYPE_UTF8_STR:
        case BASICTYPE_T61_STR:
        case BASICTYPE_GENERALIZEDTIME:
        case BASICTYPE_UTCTIME:
        case BASICTYPE_OBJECTDESCRIPTOR:
	    case BASICTYPE_OCTETCONTAINING:	
	    case BASICTYPE_BITCONTAINING:	
        case BASICTYPE_EXTENSION:
            break;

		/*
		 * these have no more info -  only the  choiceId is used
		 */
        case BASICTYPE_MACROTYPE:
            switch (bt->a.macroType->choiceId)
            {
        case MACROTYPE_ROSOPERATION:
        case MACROTYPE_ASNABSTRACTOPERATION:

            ValueLinkRosOperationMacroType (m, currMod, head, type, bt, bt->a.macroType->a.rosOperation);
            break;

        case MACROTYPE_ROSERROR:
        case MACROTYPE_ASNABSTRACTERROR:
            ValueLinkRosErrorMacroType (m, currMod, head, type, bt, bt->a.macroType->a.rosError);
             break;

        case MACROTYPE_ROSBIND:
        case MACROTYPE_ROSUNBIND:
            ValueLinkRosBindMacroType (m, currMod, head, type, bt, bt->a.macroType->a.rosBind);
             break;

        case MACROTYPE_ROSASE:
            ValueLinkRosAseMacroType (m, currMod, head, type, bt, bt->a.macroType->a.rosAse);
             break;

        case MACROTYPE_MTSASEXTENSIONS:
            ValueLinkMtsasExtensionsMacroType (m, currMod, head, type, bt, bt->a.macroType->a.mtsasExtensions);
            break;

        case MACROTYPE_MTSASEXTENSION:
            ValueLinkMtsasExtensionMacroType (m, currMod, head, type, bt, bt->a.macroType->a.mtsasExtension);
            break;

        case MACROTYPE_MTSASEXTENSIONATTRIBUTE:
            ValueLinkMtsasExtensionAttributeMacroType (m, currMod, head, type, bt, bt->a.macroType->a.mtsasExtensionAttribute);
            break;

        case MACROTYPE_MTSASTOKEN:
            ValueLinkMtsasTokenMacroType (m, currMod, head, type, bt, bt->a.macroType->a.mtsasToken);
            break;

        case MACROTYPE_MTSASTOKENDATA:
            ValueLinkMtsasTokenDataMacroType (m, currMod, head, type, bt, bt->a.macroType->a.mtsasTokenData);
            break;

        case MACROTYPE_MTSASSECURITYCATEGORY:
            ValueLinkMtsasSecurityCategoryMacroType (m, currMod, head, type, bt, bt->a.macroType->a.mtsasSecurityCategory);
            break;

        case MACROTYPE_ASNOBJECT:
            ValueLinkAsnObjectMacroType (m, currMod, head, type, bt, bt->a.macroType->a.asnObject);
            break;

        case MACROTYPE_ASNPORT:
            ValueLinkAsnPortMacroType (m, currMod, head, type, bt, bt->a.macroType->a.asnPort);
            break;

        case MACROTYPE_ASNABSTRACTBIND:
        case MACROTYPE_ASNABSTRACTUNBIND:
            ValueLinkAsnAbstractBindMacroType (m, currMod, head, type, bt, bt->a.macroType->a.asnAbstractBind);
            break;

        case MACROTYPE_AFALGORITHM:
        case MACROTYPE_AFENCRYPTED:
        case MACROTYPE_AFPROTECTED:
        case MACROTYPE_AFSIGNATURE:
        case MACROTYPE_AFSIGNED:
            ValueLinkType (m, currMod, head, bt->a.macroType->a.afAlgorithm);
            break;

        case MACROTYPE_SNMPOBJECTTYPE:
            ValueLinkSnmpObjectTypeMacroType (m, currMod, head, type, bt, bt->a.macroType->a.snmpObjectType);
            break;

        default:
            fprintf (errFileG, "ValueLinkBasicType: ERROR - unknown macro type id!\n");
        break;
    }
    break;

    default:
       fprintf (errFileG, "ValueLinkBasicType: ERROR - unknown basic type id!\n");

    }
}  /* ValueLinkBasicType */




/*
 * resolve any type/value refs in the subtypes (if any)
 */
void
ValueLinkSubtypes PARAMS ((m, currMod, head, currType, s),
    ModuleList *m _AND_
    Module *currMod _AND_
    TypeDef *head _AND_
    Type *currType _AND_
    Subtype *s)
{
    Subtype *sElmt;

    if (s == NULL)
        return;

    switch (s->choiceId)
    {
        case SUBTYPE_SINGLE:
            ValueLinkSubtypeValue (m, currMod, head, currType, s->a.single);
        break;

        case SUBTYPE_AND:
        case SUBTYPE_OR:
        case SUBTYPE_NOT:
            FOR_EACH_LIST_ELMT (sElmt, s->a.and)
            {
                ValueLinkSubtypes (m, currMod, head, currType, sElmt);
            }
        break;

        default:
            fprintf (errFileG, "ValueLinkSubtypes: ERROR - unknown Subtype id\n");
        break;
    }
}  /* ValueLinkSubtypes */





void
ValueLinkSubtypeValue PARAMS ((m, currMod, head, currType, s),
    ModuleList *m _AND_
    Module *currMod _AND_
    TypeDef *head _AND_
    Type *currType _AND_
    SubtypeValue *s)
{
    Constraint *constraint;

    if (s == NULL)
        return;

    switch (s->choiceId)
    {
        case SUBTYPEVALUE_SINGLEVALUE:
            ValueLinkValue (m, currMod, NULL, currType, s->a.singleValue);
            break;

        case SUBTYPEVALUE_CONTAINED:
            ValueLinkType (m, currMod, head, s->a.contained);
            break;

        case SUBTYPEVALUE_VALUERANGE:
            ValueLinkValue (m, currMod, NULL, currType,
				s->a.valueRange->lowerEndValue->endValue);
            ValueLinkValue (m, currMod, NULL, currType,
				s->a.valueRange->upperEndValue->endValue);
            break;

        case SUBTYPEVALUE_PERMITTEDALPHABET:
            ValueLinkSubtypes (m, currMod, head, currType, s->a.permittedAlphabet);
           break;

        case SUBTYPEVALUE_SIZECONSTRAINT:
            ValueLinkSubtypes (m, currMod, head, currType, s->a.sizeConstraint);
            break;

        case SUBTYPEVALUE_INNERSUBTYPE:
            FOR_EACH_LIST_ELMT (constraint, s->a.innerSubtype->constraints)
            {
                ValueLinkSubtypes (m, currMod, head, currType, constraint->valueConstraints);
            }
            break;

        default:
            fprintf (errFileG, "ValueLinkSubtype: ERROR - unknown subtype choiceId\n");
    }

} /* ValueLinkSubtype */




void
ValueLinkNamedElmts PARAMS ((m, currMod, head, t, v),
    ModuleList *m _AND_
    Module *currMod _AND_
    TypeDef *head _AND_
    Type *t _AND_
    ValueDefList *v)
{
    ValueDef *vd;
    FOR_EACH_LIST_ELMT (vd, v)
    {
        ValueLinkValue (m, currMod, vd, vd->value->type, vd->value);
    }

    t = t; /*AVOIDS Compiler warning.*/
    head = head;
}  /* ValueLinkNamedElmts */




void
ValueLinkValueDef PARAMS ((m, currMod, v),
    ModuleList *m _AND_
    Module *currMod _AND_
    ValueDef *v)
{
    if (v == NULL)
        return;

    ValueLinkType (m, currMod, NULL, v->value->type);
    ValueLinkValue (m, currMod, v, v->value->type, v->value);

}  /* ValueLinkValueDef */




void
ValueLinkValue PARAMS ((m, currMod, head, valuesType, v),
    ModuleList *m _AND_
    Module *currMod _AND_
    ValueDef *head _AND_
    Type *valuesType _AND_
    Value *v)
{

    if (v == NULL)
        return;

    ValueLinkBasicValue (m, currMod, head, valuesType, v, v->basicValue);

}  /* ValueLinkValue */



void
ValueLinkBasicValue PARAMS ((m, currMod, head, valuesType, v, bv),
    ModuleList *m _AND_
    Module *currMod _AND_
    ValueDef *head _AND_
    Type *valuesType _AND_
    Value *v _AND_
    BasicValue *bv)
{
    ValueDef *tmpValueDef;
    ImportElmt *impElmt;
    ImportModule *impMod;
    ValueDef *n;
    ValueDefList *namedElmtList;


    if (v == NULL)
        return;


    switch (bv->choiceId)
    {
        case BASICVALUE_UNKNOWN:
        case BASICVALUE_EMPTY:
        case BASICVALUE_INTEGER:
        case BASICVALUE_SPECIALINTEGER:
        case BASICVALUE_BOOLEAN:
        case BASICVALUE_REAL:
        case BASICVALUE_SPECIALREAL:
        case BASICVALUE_ASCIITEXT:
        case BASICVALUE_ASCIIHEX:
        case BASICVALUE_ASCIIBITSTRING:
        case BASICVALUE_BERVALUE:
        case BASICVALUE_PERVALUE:
        case BASICVALUE_NAMEDVALUE:
        case BASICVALUE_NULL:
        case BASICVALUE_VALUENOTATION:
        case BASICVALUE_OID:
            break;


        case BASICVALUE_LOCALVALUEREF:

            /*
             * parser sets all value refs to "Local" so must
             * check if local, then if import ....
             */

            /*
             * first check in named elmts of the given type
             */
            namedElmtList = GetAllNamedElmts (valuesType);
            if (namedElmtList != NULL)
            {
                n = LookupValue (namedElmtList, bv->a.localValueRef->valueName);

                if (n != NULL)
                {
                    bv->a.localValueRef->link = n;
                    bv->a.localValueRef->module = currMod;

                    /* now free list structure (not data elmts) */
                    AsnListFree (namedElmtList);

                    break; /* exit switch since done here. */
                }
            }

            /*
             * second, look for values defined in this module
             */
            tmpValueDef = LookupValue (currMod->valueDefs, bv->a.localValueRef->valueName);

            if (tmpValueDef != NULL)
            {
                bv->a.localValueRef->link = tmpValueDef;
                break; /* exit switch since done here. */
            }

            else
                bv->choiceId = BASICVALUE_IMPORTVALUEREF;
                /*!!!!!!!!!! fall through from else clause */


        case BASICVALUE_IMPORTVALUEREF:
            /* This handles "modname.value" value refs. */
            if (bv->a.importValueRef->moduleName != NULL)
            {
                /*
                 * Lookup the import list maintained in this module
                 * from the named module.  (the parser generates
                 * an import list from Foo module for "Foo.Bar" style
                 * import refs)
                 */
                impMod = LookupImportModule (currMod, bv->a.importValueRef->moduleName);

                if (impMod == NULL) /* whoa, compiler error */
                {
                    currMod->status = MOD_ERROR;
                    fprintf (errFileG, "Compiler Error: \"%s.%s\" valueref - no import list defined from module \"%s\".\n",
						bv->a.importValueRef->moduleName, bv->a.importValueRef->valueName,
						bv->a.importValueRef->moduleName);

                    return;
                }
                impElmt = LookupImportElmtInImportElmtList (impMod->importElmts, bv->a.importValueRef->valueName);

                if (impElmt == NULL) /* whoa, compiler error again */
                {
                    currMod->status = MOD_ERROR;
                    fprintf (errFileG, "Compiler Error: \"%s.%s\" valueref - no import element defined for value \"%s\".\n",
						bv->a.importValueRef->moduleName,
						bv->a.importValueRef->valueName,
						bv->a.importValueRef->valueName);


                }
                else if (impElmt->resolvedRef != NULL)
                {
                    if (impElmt->resolvedRef->choiceId !=
                        IMPORTELMTCHOICE_VALUE)
					{
                        fprintf (errFileG, "Linker Warning: import VALUE ref \"%s\" resolves with an imported TYPE\n",
							impElmt->name);
					}

                    bv->a.importValueRef->link = impElmt->resolvedRef->a.value;
                    bv->a.importValueRef->module = impMod->moduleRef;
                }
                else
                {
                    PrintErrLoc (currMod->asn1SrcFileName, (long)v->lineNo);
                    fprintf (errFileG, "reference to unresolved imported value \"%s\"\n",
						impElmt->name);
                }
            }
            else
            {
                impElmt = LookupImportElmtInModule (currMod, bv->a.importValueRef->valueName, &impMod);
                if ((impElmt != NULL) && (!impElmt->privateScope))
                {
                    /*
                     * if import elmt is resolved then
                     * set up link
                     */

                    if (impElmt->resolvedRef != NULL)
                    {
                        if (impElmt->resolvedRef->choiceId !=
                            IMPORTELMTCHOICE_VALUE)
						{
                            fprintf (errFileG, "Linker Warning: import VALUE ref \"%s\" resolves with an imported TYPE\n",
								impElmt->name);
						}

                        bv->a.importValueRef->link = impElmt->resolvedRef->a.value;
                        bv->a.importValueRef->module = impMod->moduleRef;
                    }
                    else
                    {
                        PrintErrLoc (currMod->asn1SrcFileName, (long)v->lineNo);
                        fprintf (errFileG, "reference to unresolved imported value \"%s\"\n",
							impElmt->name);
                    }
                }
                else
                {
                    /*
                     *  value not defined locally, nor imported nor
                     *  defined in useful types module
                     */
                    currMod->status = MOD_ERROR;
                    PrintErrLoc (currMod->asn1SrcFileName, (long)v->lineNo);
                    fprintf (errFileG, "ERROR - value \"%s\" is referenced but not defined or imported.\n",
						bv->a.importValueRef->valueName);
                }
            }
            break;

        case BASICVALUE_LINKEDOID:
            ValueLinkOid (m, currMod, head, v, bv->a.linkedOid);
            break;

        default:
           fprintf (errFileG, "ValueLinkBasicValue: ERROR - unknown value type\n");
    }

}  /* ValueLinkBasicValue */


/*
 * link the value refs from an object identifier
 *
 *
 * eg
 * for { ccitt foo (1) bar bell (bunt) 2 } the format is
 *
 * ccitt
 *    arcnum is set to number from oid table (oid.c)
 * foo (1)
 *   - arc num is set to 1
 *   - sets up a new value def foo defined as 1
 *   - makes oid valueref a value ref to foo (doesn't link it tho)
 * bar
 *   - makes oid valueref a value ref to bar (doesn't link it tho)
 * bell (bunt)
 *   - sets up a new value def bell defined as a val ref to bunt
 *   - makes oid valueref a value ref to bell (doesn't link it tho)
 * 2
 *  -arc num is set to 2
 *
 */
void
ValueLinkOid PARAMS ((m, currMod, head, v, oid),
    ModuleList *m _AND_
    Module *currMod _AND_
    ValueDef *head _AND_
    Value *v _AND_
    OID *oid)
{
    OID *firstElmt;

    /*
     * WARNING: for cyclic oid value definintions like.
     * foo OID ::= { bar 1 3 }
     * bar OID ::= { foo 1 3 }
     * infinite recursion is prevented by
     * a hack (linkOidCallDepth)
     */
    if (linkOidCallDepthG > 100)
    {
        currMod->status = MOD_ERROR;
        PrintErrLoc (currMod->asn1SrcFileName, (long)v->lineNo);
        fprintf (errFileG, "ERROR - OBJECT IDENTIFIER value \"%s\" appears to be defined recursively\n",
			head->definedName);
        linkOidCallDepthG = 0;
        return;
    }
    else
        linkOidCallDepthG++;

    for (firstElmt = oid; oid != NULL; oid = oid->next)
    {
        if (oid->valueRef != NULL)
        {
            ValueLinkValue (m, currMod, head, NULL, oid->valueRef);

            if ((oid->valueRef->basicValue->choiceId !=
                  BASICVALUE_LOCALVALUEREF) &&
                (oid->valueRef->basicValue->choiceId !=
                 BASICVALUE_IMPORTVALUEREF))
            {
                fprintf (errFileG, "Internal error: Oid valueref is not a ref\n");
                break; /* exit for */
            }

            /*
             * leave simplification (replacement of value refs with values)
             * of oid values to normalize.c
             */
        }
    }

    linkOidCallDepthG--;

}  /* ValueLinkOid */


void
ValueLinkRosOperationMacroType PARAMS ((m, currMod, head, t, bt, op),
    ModuleList *m _AND_
    Module *currMod _AND_
    TypeDef *head _AND_
    Type *t _AND_
    BasicType *bt _AND_
    RosOperationMacroType *op)
{
    TypeOrValue *tOrV;

    if (op->arguments != NULL)
        ValueLinkType (m, currMod, head, op->arguments->type);

    if (op->result != NULL)
        ValueLinkType (m, currMod, head, op->result->type);

    /*
     *  go through errors (if any) and link types/values
     */
    FOR_EACH_LIST_ELMT (tOrV,  op->errors)
    {
        if (tOrV->choiceId == TYPEORVALUE_TYPE)
            ValueLinkType (m, currMod, head, tOrV->a.type);
        else
            ValueLinkValue (m, currMod, NULL, t, tOrV->a.value);
    }

    /*
     *  go through linked operations (if any) and
     *  link types/values
     */
    FOR_EACH_LIST_ELMT (tOrV,  op->linkedOps)
    {
        if (tOrV->choiceId == TYPEORVALUE_TYPE)
            ValueLinkType (m, currMod, head, tOrV->a.type);
        else
            ValueLinkValue (m, currMod, NULL, t, tOrV->a.value);
    }
    bt = bt; /*AVOIDS Compiler warning.*/
    t = t;
}  /* ValueLinkRosOperationMacroType */


void
ValueLinkRosErrorMacroType PARAMS ((m, currMod, head, t, bt, err),
    ModuleList *m _AND_
    Module *currMod _AND_
    TypeDef *head _AND_
    Type *t _AND_
    BasicType *bt _AND_
    RosErrorMacroType *err)
{
    if ((err != NULL) && (err->parameter != NULL))
    {
        ValueLinkType (m, currMod, head, err->parameter->type);
    }
    bt = bt; /*AVOIDS Compiler warning.*/
    t = t;
}   /* ValueLinkRosErrorMacroType */


void
ValueLinkRosBindMacroType PARAMS ((m, currMod, head, t, bt, bind),
    ModuleList *m _AND_
    Module *currMod _AND_
    TypeDef *head _AND_
    Type *t _AND_
    BasicType *bt _AND_
    RosBindMacroType *bind)
{
    if (bind != NULL)
    {
        ValueLinkElmtType (m, currMod, head, bind->argument);
        ValueLinkElmtType (m, currMod, head, bind->result);
        ValueLinkElmtType (m, currMod, head, bind->error);
    }
    bt = bt; /*AVOIDS Compiler warning.*/
    t = t;
}   /* ValueLinkRosBindMacroType */


void
ValueLinkRosAseMacroType PARAMS ((m, currMod, head, t, bt, ase),
    ModuleList *m _AND_
    Module *currMod _AND_
    TypeDef *head _AND_
    Type *t _AND_
    BasicType *bt _AND_
    RosAseMacroType *ase)
{
    Value *v;

    FOR_EACH_LIST_ELMT (v, ase->operations)
        ValueLinkValue (m, currMod, NULL, t, v);


    FOR_EACH_LIST_ELMT (v, ase->consumerInvokes)
        ValueLinkValue (m, currMod, NULL, t, v);


    FOR_EACH_LIST_ELMT (v, ase->supplierInvokes)
        ValueLinkValue (m, currMod, NULL, t, v);

    bt = bt; /*AVOIDS Compiler warning.*/
    head = head;
}  /* ValueLinkRosAseMacroType */



void
ValueLinkRosAcMacroType PARAMS ((m, currMod, head, t, bt, ac),
    ModuleList *m _AND_
    Module *currMod _AND_
    TypeDef *head _AND_
    Type *t _AND_
    BasicType *bt _AND_
    RosAcMacroType *ac)
{
    Value *v;
    OID *oid;

    FOR_EACH_LIST_ELMT (v,  ac->nonRoElements)
        ValueLinkValue (m, currMod, NULL, t, v);


    ValueLinkType (m, currMod, head, ac->bindMacroType);
    ValueLinkType (m, currMod, head, ac->unbindMacroType);

    FOR_EACH_LIST_ELMT (v, ac->operationsOf)
        ValueLinkValue (m, currMod, NULL, t, v);


    FOR_EACH_LIST_ELMT (v, ac->initiatorConsumerOf)
        ValueLinkValue (m, currMod, NULL, t, v);


    FOR_EACH_LIST_ELMT (v, ac->responderConsumerOf)
        ValueLinkValue (m, currMod, NULL, t, v);


    FOR_EACH_LIST_ELMT (oid, ac->abstractSyntaxes)
        ValueLinkOid (m, currMod, NULL, NULL,  oid);

    bt = bt; /*AVOIDS Compiler warning.*/
}  /* ValueLinkRosAcMacroType */



void
ValueLinkMtsasExtensionsMacroType PARAMS ((m, currMod, head, t, bt, exts),
    ModuleList *m _AND_
    Module *currMod _AND_
    TypeDef *head _AND_
    Type *t _AND_
    BasicType *bt _AND_
    MtsasExtensionsMacroType *exts)
{
    Value *v;

    FOR_EACH_LIST_ELMT (v, exts->extensions)
        ValueLinkValue (m, currMod, NULL, t, v);

    bt = bt; /*AVOIDS Compiler warning.*/
    head = head;
}  /* ValueLinkMtsasExtensionsMacroType */


void
ValueLinkMtsasExtensionMacroType PARAMS ((m, currMod, head, t, bt, ext),
    ModuleList *m _AND_
    Module *currMod _AND_
    TypeDef *head _AND_
    Type *t _AND_
    BasicType *bt _AND_
    MtsasExtensionMacroType *ext)
{
    ValueLinkElmtType (m, currMod, head, ext->elmtType);
    ValueLinkValue (m, currMod, NULL, t, ext->defaultValue);

    bt = bt; /*AVOIDS Compiler warning.*/
}  /* ValueLinkMtsasExtensionMacroType */


void
ValueLinkMtsasExtensionAttributeMacroType PARAMS ((m, currMod, head, t,bt, ext),
    ModuleList *m _AND_
    Module *currMod _AND_
    TypeDef *head _AND_
    Type *t _AND_
    BasicType *bt _AND_
    MtsasExtensionAttributeMacroType *ext)
{

    if (ext != NULL)
        ValueLinkType (m, currMod, head, ext->type);

    bt = bt; /*AVOIDS Compiler warning.*/
    t = t;
}  /* ValueLinkMtsasExtensionAttributeMacroType */


void
ValueLinkMtsasTokenMacroType PARAMS ((m, currMod, head, t, bt, tok),
    ModuleList *m _AND_
    Module *currMod _AND_
    TypeDef *head _AND_
    Type *t _AND_
    BasicType *bt _AND_
    MtsasTokenMacroType *tok)
{
    if (tok != NULL)
        ValueLinkType (m, currMod, head, tok->type);

    bt = bt; /*AVOIDS Compiler warning.*/
    t = t;
}  /* ValueLinkMtsasTokenMacroType */


void
ValueLinkMtsasTokenDataMacroType PARAMS ((m, currMod, head, t, bt, tok),
    ModuleList *m _AND_
    Module *currMod _AND_
    TypeDef *head _AND_
    Type *t _AND_
    BasicType *bt _AND_
    MtsasTokenDataMacroType *tok)
{
    if (tok != NULL)
        ValueLinkType (m, currMod, head, tok->type);

    bt = bt; /*AVOIDS Compiler warning.*/
    t = t;
}  /* ValueLinkMtsasTokenDataMacroType */

void
ValueLinkMtsasSecurityCategoryMacroType PARAMS ((m, currMod, head, t, bt, sec),
    ModuleList *m _AND_
    Module *currMod _AND_
    TypeDef *head _AND_
    Type *t _AND_
    BasicType *bt _AND_
    MtsasSecurityCategoryMacroType *sec)
{

    if (sec != NULL)
        ValueLinkType (m, currMod, head, sec->type);

    bt =bt; /*AVOIDS Compiler warning.*/
    t = t;
}  /* ValueLinkMtsasSecurityCategoryMacroType */



void
ValueLinkAsnObjectMacroType PARAMS ((m, currMod, head, t, bt, obj),
    ModuleList *m _AND_
    Module *currMod _AND_
    TypeDef *head _AND_
    Type *t _AND_
    BasicType *bt _AND_
    AsnObjectMacroType *obj)
{
    AsnPort *ap;

    FOR_EACH_LIST_ELMT (ap, obj->ports)
        ValueLinkValue (m, currMod, NULL, t, ap->portValue);

    bt = bt; /*AVOIDS Compiler warning.*/
    head = head;
}  /* ValueLinkAsnObjectMacroType */


void
ValueLinkAsnPortMacroType PARAMS ((m, currMod, head, t, bt, p),
    ModuleList *m _AND_
    Module *currMod _AND_
    TypeDef *head _AND_
    Type *t _AND_
    BasicType *bt _AND_
    AsnPortMacroType *p)
{
    TypeOrValue *tOrV;

    FOR_EACH_LIST_ELMT (tOrV, p->abstractOps)
    {
        if (tOrV->choiceId == TYPEORVALUE_TYPE)
            ValueLinkType (m, currMod, head, tOrV->a.type);
        else
            ValueLinkValue (m, currMod, NULL, t, tOrV->a.value);
    }


    FOR_EACH_LIST_ELMT (tOrV, p->supplierInvokes)
    {
        if (tOrV->choiceId == TYPEORVALUE_TYPE)
            ValueLinkType (m, currMod, head, tOrV->a.type);
        else
            ValueLinkValue (m, currMod, NULL, t, tOrV->a.value);
    }


    FOR_EACH_LIST_ELMT (tOrV, p->consumerInvokes)
    {
        if (tOrV->choiceId == TYPEORVALUE_TYPE)
            ValueLinkType (m, currMod, head, tOrV->a.type);
        else
            ValueLinkValue (m, currMod, NULL, t, tOrV->a.value);
    }

    bt = bt; /*AVOIDS Compiler warning.*/
}  /* ValueLinkAsnPortMacroType */



void
ValueLinkAsnAbstractBindMacroType PARAMS ((m, currMod, head, t, bt, bind),
    ModuleList *m _AND_
    Module *currMod _AND_
    TypeDef *head _AND_
    Type *t _AND_
    BasicType *bt _AND_
    AsnAbstractBindMacroType *bind)
{
    AsnPort *ap;

    FOR_EACH_LIST_ELMT (ap, bind->ports)
        ValueLinkValue (m, currMod, NULL, t, ap->portValue);

    bt = bt; /*AVOIDS Compiler warning.*/
    head = head;
}  /* ValueLinkAsnBindMacroType */



void
ValueLinkSnmpObjectTypeMacroType PARAMS ((m, currMod, head, t, bt, ot),
    ModuleList *m _AND_
    Module *currMod _AND_
    TypeDef *head _AND_
    Type *t _AND_
    BasicType *bt _AND_
    SnmpObjectTypeMacroType *ot)
{
    TypeOrValue *tOrV;

    ValueLinkType (m, currMod, head, ot->syntax);
    ValueLinkValue (m, currMod, NULL, t, ot->description);
    ValueLinkValue (m, currMod, NULL, t, ot->reference);
    ValueLinkValue (m, currMod, NULL, t, ot->defVal);

    FOR_EACH_LIST_ELMT (tOrV, ot->index)
    {
        if (tOrV->choiceId == TYPEORVALUE_TYPE)
            ValueLinkType (m, currMod, head, tOrV->a.type);
        else
            ValueLinkValue (m, currMod, NULL, t, tOrV->a.value);
    }

    bt = bt; /*AVOIDS Compiler warning.*/
}  /* ValueLinkSnmpObjectTypeMacroType */
