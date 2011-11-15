/*
 * compiler/back-ends/c-gen/type-info.c  - fills in c type information
 *
 * MS 91/92
 * Copyright (C) 1991, 1992 Michael Sample
 *            and the University of British Columbia
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * $Header: /baseline/SNACC/compiler/back-ends/c-gen/type-info.c,v 1.9 2004/03/25 19:20:16 gronej Exp $
 * $Log: type-info.c,v $
 * Revision 1.9  2004/03/25 19:20:16  gronej
 * fixed some linux warnings
 *
 * Revision 1.8  2003/07/31 18:33:23  colestor
 * Updated to reflect newly added ANY processing as AsnOcts.  All built code will
 * now compile directly.
 *
 * Revision 1.7  2003/07/14 21:07:34  nicholar
 * Changed how parser handles --snacc directives.  Added namespace option.
 *
 * Revision 1.6  2003/07/07 14:53:38  nicholar
 * Eliminated headers and cleaned up include references
 *
 * Revision 1.5  2003/04/29 21:00:53  leonberp
 * integerated Deepak's changes for IOB support
 *
 * Revision 1.4  2002/09/16 17:35:06  mcphersc
 * Fixed warnings
 *
 * Revision 1.3  2002/09/04 18:02:17  vracarl
 * got rid of c++ comments
 *
 * Revision 1.2  2000/10/24 14:54:48  rwc
 * Updated to remove high-level warnings (level 4 on MSVC++) for an easier build.
 * SOME warnings persist due to difficulty in modifying the SNACC compiler to
 * properly build clean source; also some files are built by Lex/Yacc.
 *
 * Revision 1.1.1.1  2000/08/21 20:36:05  leonberp
 * First CVS Version of SNACC.
 *
 * Revision 1.3  1995/07/25 18:47:45  rj
 * changed `_' to `-' in file names.
 *
 * Revision 1.2  1994/09/01  00:26:44  rj
 * snacc_config.h removed.
 *
 * Revision 1.1  1994/08/28  09:48:42  rj
 * first check-in. for a list of changes to the snacc-1.1 distribution please refer to the ChangeLog.
 *
 */

#include <ctype.h>
#include <string.h>
#include "asn-incl.h"
#include "asn1module.h"
#include "snacc-util.h"
#include "str-util.h"
#include "rules.h"


static DefinedObj *definedNamesG;
/*
 *  All Typedefs, union,struct & enum Tags, and definedvalues (enum consts)
 *  are assumed to share the same name space - this list is used to
 *  assure uniqueness. (actually 4 name spaces in C - see pg 227 KR 2nd Ed)
 */


/* unexported prototypes */

static void FillCTypeDefInfo PROTO ((CRules *r,  Module *m,  TypeDef *td));
static void FillCFieldNames PROTO ((CRules *r, NamedTypeList *firstSibling));
static void FillCTypeRefInfo PROTO ((CRules *r,  Module *m,  TypeDef *head,
									Type *t, CTypeId parentTypeId));
static void FillCStructElmts PROTO ((CRules *r,  Module *m,  TypeDef *head,
									NamedTypeList *t));
static void FillCChoiceElmts PROTO ((CRules *r, Module *m, TypeDef *head,
									NamedTypeList *first));
static int IsCPtr PROTO ((CRules *r, TypeDef *td, Type *t,
						 CTypeId parentTypeId));
const char* GetDirectiveName(SnaccDirectiveEnum dirType);

static void ParseTypeDefAttribs PROTO ((CTDI *ctdi,
									   SnaccDirectiveList *attrList));
static void ParseTypeRefAttribs PROTO ((CTRI *ctri,
									   SnaccDirectiveList *attrList));
static void FillCTDIDefaults PROTO ((CRules *r, CTDI *ctdi, TypeDef *td));


/*
 *  allocates and fills all the "cTypeDefInfo" for each type def
 *  and "cTypeRefInfo" for each type in the given modules.
 *  Also does the useful types module if it is not null.
 */
void
FillCTypeInfo PARAMS ((r, modList),
    CRules *r _AND_
    ModuleList *modList)
{
    TypeDef *td;
    Module *m;

    /*
     * go through each module's type defs and fill
     * in the C type and enc/dec routines etc
     */
    definedNamesG = NewObjList();

    FOR_EACH_LIST_ELMT (m, modList)
    {
        FOR_EACH_LIST_ELMT (td, m->typeDefs)
            FillCTypeDefInfo (r, m, td);
    }


    /*
     * now that type def info is filled in
     * set up set/seq/list/choice elements that ref
     * those definitions
     */
    FOR_EACH_LIST_ELMT (m, modList)
    {
        FOR_EACH_LIST_ELMT (td, m->typeDefs)
            FillCTypeRefInfo (r, m, td, td->type, C_TYPEDEF);
    }

    /*
     * modules compiled together (ie one call to snacc with
     * multiple args) likely to be C compiled together so
     * need a unique routines/types/defines/enum values
     * since assuming they share same name space.
     *  All Typedefs, union, struct & enum Tags, and defined values
     * (enum consts), #define names
     *  are assumed to share the same name space
     */

    FreeDefinedObjs (&definedNamesG);

}  /* FillCTypeInfo */


/*
 *  allocates and fills structure holding C type definition information
 *  fo the given ASN.1 type definition.  Does not fill CTRI for contained
 *  types etc.
 */
void
FillCTypeDefInfo PARAMS ((r, m, td),
    CRules *r _AND_
    Module *m _AND_
    TypeDef *td)
{
    int len;
    char *tmpName;
    CTDI *ctdi;

    /*
     * if CTDI is present this type def has already been 'filled'
     */
    if (td->cTypeDefInfo != NULL)
        return;

    ctdi = td->cTypeDefInfo = MT (CTDI);
    ctdi->cTypeId = C_TYPEDEF;

    /* get default type def attributes from table for type on rhs of ::= */

    FillCTDIDefaults (r, ctdi, td);


    /*
     * if defined by a ref to another type definition fill in that type
     * def's CTDI so can inherit (actully completly replace default
     * attributes) from it
     */
    if ((td->type->basicType->choiceId == BASICTYPE_LOCALTYPEREF) ||
        (td->type->basicType->choiceId == BASICTYPE_IMPORTTYPEREF))
    {
        /*
         * Fill in CTDI for defining type if nec.
         * this works for importTypeRef as well since both a.localTypeRef
         * and a.importTypeRef are of type TypeRef
         */
        FillCTypeDefInfo (r, td->type->basicType->a.localTypeRef->module, td->type->basicType->a.localTypeRef->link);

        memcpy (ctdi, td->type->basicType->a.localTypeRef->link->cTypeDefInfo, sizeof (CTDI));
    }


    /*
     * Zap default names for routines/type with NULL so
     * can determine if the --snacc attributes specified any
     */
    ctdi->cTypeName = NULL;
    ctdi->printRoutineName = NULL;
    ctdi->encodeRoutineName = NULL;
    ctdi->decodeRoutineName = NULL;
    ctdi->freeRoutineName = NULL;


    /*
     * check for any "--snacc" attributes that overide the current
     * ctdi fields
     */
    ParseTypeDefAttribs (ctdi, td->attrList);

    /* If BigInt's used, then reset the default type def attributes */
/*    if (ctdi->asn1TypeId == BASICTYPE_BIGINT) {
      td->type->basicType->choiceId = BASICTYPE_BIGINT; 
    } */



    /*
     * generate c typename for this  type def if not given by
     * --snacc attributes
     */
    if (ctdi->cTypeName == NULL)
    {
        tmpName = Asn1TypeName2CTypeName (td->definedName);
        len = strlen (tmpName);
        ctdi->cTypeName = Malloc (len + r->maxDigitsToAppend + 1);
        strcpy (ctdi->cTypeName, tmpName);
        Free (tmpName);

       /*
        * make sure c type def name is unique
        * (no need to check if cTypeName was specified by --snacc attribs)
        */
       MakeCStrUnique (definedNamesG, ctdi->cTypeName,r->maxDigitsToAppend, 1);
       DefineObj (&definedNamesG, ctdi->cTypeName);
    }


    /*
     * make names for encoder,decoder, print and free routines
     * (if not already set by --snacc attributes
     */
    if (ctdi->encodeRoutineName == NULL)
    {
        ctdi->encodeRoutineName =
            Malloc (strlen (ctdi->cTypeName) + strlen (r->encodeRoutineBaseName)
                   + 1);
        strcpy (ctdi->encodeRoutineName, r->encodeRoutineBaseName);
        strcat (ctdi->encodeRoutineName, ctdi->cTypeName);
    }

    if (ctdi->decodeRoutineName == NULL)
    {
        ctdi->decodeRoutineName =
            Malloc (strlen (ctdi->cTypeName) + strlen (r->decodeRoutineBaseName) + 1);
        strcpy (ctdi->decodeRoutineName, r->decodeRoutineBaseName);
        strcat (ctdi->decodeRoutineName, ctdi->cTypeName);
    }

    if (ctdi->printRoutineName == NULL)
    {
        ctdi->printRoutineName =
            Malloc (strlen (ctdi->cTypeName) + strlen (r->printRoutineBaseName) + 1);
        strcpy (ctdi->printRoutineName, r->printRoutineBaseName);
        strcat (ctdi->printRoutineName, ctdi->cTypeName);
    }

    if (ctdi->freeRoutineName == NULL)
    {
        ctdi->freeRoutineName =
            Malloc (strlen (ctdi->cTypeName) + strlen (r->freeRoutineBaseName) + 1);
        strcpy (ctdi->freeRoutineName, r->freeRoutineBaseName);
        strcat (ctdi->freeRoutineName, ctdi->cTypeName);
    }

    m = m;  /*AVOIDS warning.*/
}  /* FillCTypeDefInfo */


static void
FillCTypeRefInfo PARAMS ((r, m, head, t, parentTypeId),
    CRules *r _AND_
    Module *m _AND_
    TypeDef *head _AND_
    Type *t _AND_
    CTypeId parentTypeId)
{
    CTRI	*ctri;
    CTDI	*tmpCtdi;
    ValueDef	*namedElmt;
    CNamedElmt	*cne;
    CNamedElmt	**cneHndl;
    char	*elmtName;
    char	*unionName;
    enum BasicTypeChoiceId basicTypeId;
    int len;

    /*
     * you must check for cycles yourself before calling this
     */
    if (t->cTypeRefInfo == NULL)
    {
        ctri = MT (CTRI);
        t->cTypeRefInfo = ctri;
    }
    else
        ctri =  t->cTypeRefInfo;

    basicTypeId = t->basicType->choiceId;

    tmpCtdi = &r->typeConvTbl[basicTypeId];

	// Deepak: 31/Mar/2003
	// change typeId here so that C_OBJECTCLASSFIELDTYPE could automatically behave as C_LIB
	if(tmpCtdi->cTypeId == C_OBJECTCLASSFIELDTYPE)
	   tmpCtdi->cTypeId = C_LIB;

    /* get base type def info from the conversion table in the rules */
    /* if the cTypeId is C_LIB, nothing more needs to be done */
    ctri->cTypeId = tmpCtdi->cTypeId;
    ctri->cTypeName = tmpCtdi->cTypeName;
    ctri->optTestRoutineName = tmpCtdi->optTestRoutineName;
    ctri->printRoutineName = tmpCtdi->printRoutineName;
    ctri->encodeRoutineName = tmpCtdi->encodeRoutineName;
    ctri->decodeRoutineName = tmpCtdi->decodeRoutineName;
    ctri->freeRoutineName = tmpCtdi->freeRoutineName;
    ctri->isEncDec = tmpCtdi->isEncDec;

    if (ctri->cTypeId == C_ANY)
    {
        /*RWC;fprintf (errFileG, "Warning - generated code for the \"ANY\" type in type \"%s\" will need modification by YOU.",
			head->definedName);
        fprintf (errFileG, "  The source files will have a \"/\ * ANY - Fix Me! * /\" comment before related code.\n\n");*RWC;*/
     }

    /*
     * convert named elmts to C.
     * check for name conflict with other defined Types/Names/Values
     */
    if ((basicTypeId == BASICTYPE_INTEGER || basicTypeId == BASICTYPE_ENUMERATED || basicTypeId == BASICTYPE_BITSTRING) && !(LIST_EMPTY (t->basicType->a.integer)))
/*    if ((basicTypeId == BASICTYPE_INTEGER || basicTypeId == BASICTYPE_ENUMERATED || basicTypeId == BASICTYPE_BITSTRING) && !(LIST_EMPTY (t->basicType->a.integer))) */
    {
        ctri->cNamedElmts = AsnListNew (sizeof (void *));
        FOR_EACH_LIST_ELMT (namedElmt, t->basicType->a.integer)
        {
            cneHndl = (CNamedElmt **)AsnListAppend (ctri->cNamedElmts);
            cne = *cneHndl = MT (CNamedElmt);
            elmtName = Asn1ValueName2CValueName (namedElmt->definedName);
            len = strlen (elmtName);
            cne->name = Malloc (len + 1 + r->maxDigitsToAppend);
            strcpy (cne->name, elmtName);
            Free (elmtName); /* not very efficient */

            if (namedElmt->value->basicValue->choiceId == BASICVALUE_INTEGER)
                cne->value = namedElmt->value->basicValue->a.integer;
            else
            {
                fprintf (errFileG, "Warning: unlinked defined value using -9999999\n");
                cne->value = -9999999;
            }

            if (r->capitalizeNamedElmts)
                Str2UCase (cne->name, len);

            /*
             * append digits until there is not name conflict
             * if nec
             */
            MakeCStrUnique (definedNamesG, cne->name, r->maxDigitsToAppend, 1);
            DefineObj (&definedNamesG, cne->name);
        }
    }

    /*
     *  Fill in c type name, routines, ptr attibutes etc
     */
    if (r->typeConvTbl[basicTypeId].cTypeId == C_TYPEREF)
    {
        /*
         * don't do this anymore - it cause problems since FillTypeDef
         * changes name ie ORName -> ORName1 and other type use new name
         *
         * don't define type or print/enc/dec/free routines
         * if typedef name is the same as the defining type ref name
         * in P2: ORName ::= P1.ORName
        if ((parentTypeId == C_TYPEDEF) &&
            (strcmp (head->definedName, t->basicType->a.localTypeRef->typeName)
             == 0))
        {
            tmpCtdi = head->cTypeDefInfo;
            tmpCtdi->genPrintRoutine = FALSE;
            tmpCtdi->genEncodeRoutine = FALSE;
            tmpCtdi->genDecodeRoutine = FALSE;
            tmpCtdi->genFreeRoutine = FALSE;
            tmpCtdi->genTypeDef = FALSE;
        }
         */

        /*
         * grab type name from link (link is the def of the
         * the ref'd type)
         */
        if (t->basicType->a.localTypeRef->link != NULL)
        {
            /* inherit attributes from referenced type */
            tmpCtdi=  t->basicType->a.localTypeRef->link->cTypeDefInfo;
            ctri->cTypeName = tmpCtdi->cTypeName;
            ctri->printRoutineName  = tmpCtdi->printRoutineName;
            ctri->encodeRoutineName = tmpCtdi->encodeRoutineName;
            ctri->decodeRoutineName = tmpCtdi->decodeRoutineName;
            ctri->freeRoutineName = tmpCtdi->freeRoutineName;
            ctri->isEncDec = tmpCtdi->isEncDec;
            ctri->optTestRoutineName = tmpCtdi->optTestRoutineName;

        }
        else
        {
            /*
             * guess type and routine names
             */
            fprintf (errFileG, "Assuming C Type and Routine names for unresolved type ref \"%s\"\n",
				t->basicType->a.localTypeRef->typeName);

            ctri->cTypeName = Asn1TypeName2CTypeName (t->basicType->a.localTypeRef->typeName);

            ctri->printRoutineName = Malloc (strlen (r->printRoutineBaseName) + strlen (ctri->cTypeName) + 1);
            strcpy (ctri->printRoutineName, r->printRoutineBaseName);
            strcat (ctri->printRoutineName, ctri->cTypeName);

            ctri->encodeRoutineName = Malloc (strlen (r->encodeRoutineBaseName)+ strlen (ctri->cTypeName) +  1);
            strcpy (ctri->encodeRoutineName, r->encodeRoutineBaseName);
            strcat (ctri->encodeRoutineName, ctri->cTypeName);

            ctri->decodeRoutineName = Malloc (strlen (r->decodeRoutineBaseName)+ strlen (ctri->cTypeName) + 1);
            strcpy (ctri->decodeRoutineName, r->decodeRoutineBaseName);
            strcat (ctri->decodeRoutineName, ctri->cTypeName);

            ctri->freeRoutineName = Malloc (strlen (ctri->cTypeName) + strlen (r->freeRoutineBaseName) + 1);
            strcpy (ctri->freeRoutineName, r->freeRoutineBaseName);
            strcat (ctri->freeRoutineName, ctri->cTypeName);
        }

    }
	// Deepak: 05/Feb/2003
#ifdef DEEPAK
    else if (r->typeConvTbl[basicTypeId].cTypeId == C_OBJECTCLASSFIELDTYPE)
    {
        /*
         * grab type name from link (link is the def of the
         * the ref'd type)
         */
        if (t->basicType->a.localTypeRef->namedTypeLink != NULL)
        {
            /* inherit attributes from referenced type */
          /*  tmpCtdi=  t->basicType->a.localTypeRef->namedTypeLink->type->cTypeRefInfo;
            ctri->cTypeName = tmpCtdi->cTypeName;
            ctri->printRoutineName  = tmpCtdi->printRoutineName;
            ctri->encodeRoutineName = tmpCtdi->encodeRoutineName;
            ctri->decodeRoutineName = tmpCtdi->decodeRoutineName;
            ctri->freeRoutineName = tmpCtdi->freeRoutineName;
            ctri->isEncDec = tmpCtdi->isEncDec;
            ctri->optTestRoutineName = tmpCtdi->optTestRoutineName;
		  */
        }
        else
        {
            /*
             * guess type and routine names
             */
            fprintf (errFileG, "Assuming C Type and Routine names for unresolved type ref \"%s\"\n",
				t->basicType->a.localTypeRef->typeName);

            ctri->cTypeName = Asn1TypeName2CTypeName (t->basicType->a.localTypeRef->typeName);

            ctri->printRoutineName = Malloc (strlen (r->printRoutineBaseName) + strlen (ctri->cTypeName) + 1);
            strcpy (ctri->printRoutineName, r->printRoutineBaseName);
            strcat (ctri->printRoutineName, ctri->cTypeName);

            ctri->encodeRoutineName = Malloc (strlen (r->encodeRoutineBaseName)+ strlen (ctri->cTypeName) +  1);
            strcpy (ctri->encodeRoutineName, r->encodeRoutineBaseName);
            strcat (ctri->encodeRoutineName, ctri->cTypeName);

            ctri->decodeRoutineName = Malloc (strlen (r->decodeRoutineBaseName)+ strlen (ctri->cTypeName) + 1);
            strcpy (ctri->decodeRoutineName, r->decodeRoutineBaseName);
            strcat (ctri->decodeRoutineName, ctri->cTypeName);

            ctri->freeRoutineName = Malloc (strlen (ctri->cTypeName) + strlen (r->freeRoutineBaseName) + 1);
            strcpy (ctri->freeRoutineName, r->freeRoutineBaseName);
            strcat (ctri->freeRoutineName, ctri->cTypeName);
        }

    }
#endif	// DEEPAK
	else if (r->typeConvTbl[basicTypeId].cTypeId == C_OBJECTCLASS)
	{	// Deepak: 11/Mar/2003
		//FillCTypeRefInfo (r, m, head, t->basicType->a.objectclass->classdef, C_LIST);
        unionName = Malloc (strlen (head->cTypeDefInfo->cTypeName) +1);
        strcpy (unionName, head->cTypeDefInfo->cTypeName);
        ctri->cTypeName = unionName;

        FillCStructElmts (r, m, head, t->basicType->a.objectclass->classdef);		// Deepak: All the elements in this structure are filled here.
        FillCFieldNames (r, t->basicType->a.objectclass->classdef);	// Deepak: All the identifiers are stored over here.
	}
    else if (r->typeConvTbl[basicTypeId].cTypeId == C_LIST)
    {
        /*
         * List  types (SET OF/ SEQ OF)
         * fill in component type
         */

        FillCTypeRefInfo (r, m, head, t->basicType->a.setOf, C_LIST);
    }

    else if (r->typeConvTbl[basicTypeId].cTypeId == C_CHOICE)
    {
        /*
         * Choice - set up choice Id elmt names, choiceid enum name
         * choiceid enum fieldName, choice union name.
         * this will only be the first type in the typedef
         * ie will not be embedded (those are turned into type
         * refs in nomalize.c)
         */

        /*
         * make union name (tag) from enclosing typedefs name plus "Choice"
         * put in the cTypeName part. (the typeDef name is already unique
         * but make sure union tag/name does not conflict with other types)
         */
        len = strlen (head->cTypeDefInfo->cTypeName);
        unionName = (char*) Malloc (len + strlen (r->choiceUnionDefSuffix) + r->maxDigitsToAppend + 1);
        strcpy (unionName, head->cTypeDefInfo->cTypeName);
        strcat (unionName, r->choiceUnionDefSuffix);
        MakeCStrUnique (definedNamesG, unionName,  r->maxDigitsToAppend, 1);
        DefineObj (&definedNamesG, unionName);
        ctri->cTypeName = unionName;

        ctri->choiceIdEnumName = Malloc (len + strlen (r->choiceIdEnumSuffix) + r->maxDigitsToAppend + 1);
        strcpy (ctri->choiceIdEnumName, head->cTypeDefInfo->cTypeName);
        strcat (ctri->choiceIdEnumName, r->choiceIdEnumSuffix);
        MakeCStrUnique (definedNamesG, ctri->choiceIdEnumName,  r->maxDigitsToAppend, 1);
        DefineObj (&definedNamesG, ctri->choiceIdEnumName);

        ctri->choiceIdEnumFieldName = r->choiceIdFieldName; /* "choiceId" */
        ctri->cFieldName = r->choiceUnionFieldName;         /* "a" */

        /*
         * must fill field names BEFORE filling choice elmts
         * (allows better naming for choice ids
         */
        FillCFieldNames (r, t->basicType->a.choice);
        FillCChoiceElmts (r, m, head, t->basicType->a.choice);

    }

    else if (r->typeConvTbl[basicTypeId].cTypeId == C_STRUCT)
    {
        /*
         * SETs and SEQUENCEs
         */

        /*
         * make struct name (tag) (the typeDef name is already unique)
         * the same as the enclosing typeDef
         */
        unionName = Malloc (strlen (head->cTypeDefInfo->cTypeName) +1);
        strcpy (unionName, head->cTypeDefInfo->cTypeName);
        ctri->cTypeName = unionName;

        FillCStructElmts (r, m, head, t->basicType->a.set);		// Deepak: All the elements in this structure are filled here.
        FillCFieldNames (r, t->basicType->a.set);	// Deepak: All the identifiers are stored over here.
    }
    else if (r->typeConvTbl[basicTypeId].cTypeId == C_MACROTYPE)
    {		// Deepak: 17/Apr/2003
        /*
         * Macro Types
         */

        /*
         * make struct name (tag) (the typeDef name is already unique)
         * the same as the enclosing typeDef
         */
        unionName = Malloc (strlen (head->cTypeDefInfo->cTypeName) +1);
        strcpy (unionName, head->cTypeDefInfo->cTypeName);
        ctri->cTypeName = unionName;

		switch(t->basicType->a.macroType->choiceId)
		{
        case MACROTYPE_ASNABSTRACTOPERATION:
        case MACROTYPE_ROSOPERATION:

			if(t->basicType->a.macroType->a.rosOperation->arguments != NULL)
				FillCTypeRefInfo (r, m, head, t->basicType->a.macroType->a.rosOperation->arguments->type, C_MACROTYPE);

			if(t->basicType->a.macroType->a.rosOperation->result != NULL)
				FillCTypeRefInfo (r, m, head, t->basicType->a.macroType->a.rosOperation->result->type, C_MACROTYPE);
			break;
		default:
		  break;
		}
    }

    /*
     * figure out whether this is a ptr based on the enclosing
     * type (if any) and optionality/default
     */
    ctri->isPtr = (unsigned char)IsCPtr (r, head, t, parentTypeId);		// ????? check for C_MACROTYPE

    /* let user overide any defaults with the --snacc attributes */
    ParseTypeRefAttribs (ctri, t->attrList);

    /* Check for bigint conversion */
  /*  if (strcmp(ctri->cTypeName, "AsnBigInt") == 0) {
      t->basicType->choiceId = BASICTYPE_BIGINT;
    } */


}  /* FillCTypeRefInfo */



static void
FillCStructElmts PARAMS ((r, m, head, elmts),
    CRules *r _AND_
    Module *m _AND_
    TypeDef *head _AND_
    NamedTypeList *elmts)
{
    NamedType *et;

    FOR_EACH_LIST_ELMT (et, elmts)		// Take struct elements one by one.
    {
        FillCTypeRefInfo (r, m, head, et->type, C_STRUCT);
    }

}  /* FillCStructElmts */



/*
 *  Figures out non-conflicting enum names for the
 *  choice id's
 */
static void
FillCChoiceElmts PARAMS ((r, m, head, elmts),
    CRules *r _AND_
    Module *m _AND_
    TypeDef *head _AND_
    NamedTypeList *elmts)
{
    NamedType *et;
    int idCount = 0;
    CTRI *ctri;
    char *firstName;
    char *secondName;
    int   len;

    /*
     * fill in type info for elmt types first
     */
    FOR_EACH_LIST_ELMT (et, elmts)
        FillCTypeRefInfo (r, m, head, et->type, C_CHOICE);

    /*
     * set choiceId Symbol & value
     * eg
     *  Car ::= CHOICE {          typedef struct Car {
     *     chev ChevCar,   ->         enum CarChoiceId {
     *     ford FordCar                  CAR_CHEV,  <- typename_fieldName
     *     toyota ToyotaCar              CAR_FORD,
     *     }                             CAR_TOYOTA } choiceId;
     *                                union CarChoiceUnion {
     *                                      ChevCar *chev;
     *                                      FordCar *ford;
     *                                      ToyotaCar *toyota; } a;
     *                               }
     */
    FOR_EACH_LIST_ELMT (et, elmts)
    {
        ctri =  et->type->cTypeRefInfo;

        if (ctri == NULL)
            continue; /* wierd type */

        ctri->choiceIdValue = idCount++;

        firstName = Asn1TypeName2CTypeName (head->cTypeDefInfo->cTypeName);
        secondName = ctri->cFieldName;
        ctri->choiceIdSymbol = Malloc (strlen (firstName) + strlen (secondName) + 2 + r->maxDigitsToAppend);
        strcpy (ctri->choiceIdSymbol, firstName);
        strcat (ctri->choiceIdSymbol, "_");
        strcat (ctri->choiceIdSymbol, secondName);
        Free (firstName);
        len = strlen (ctri->choiceIdSymbol);

        if (r->capitalizeNamedElmts)
            Str2UCase (ctri->choiceIdSymbol, len);

        MakeCStrUnique (definedNamesG, ctri->choiceIdSymbol, r->maxDigitsToAppend, 0);
        DefineObj (&definedNamesG, ctri->choiceIdSymbol);
    }

}  /* FillCChoiceElmts */


/*
 * takes a list of "sibling" (eg same level in a structure)
 * ElmtTypes and fills sets up the c field names in
 * the CTypeRefInfo struct
 */
static void
FillCFieldNames PARAMS ((r, elmts),
    CRules *r _AND_
    NamedTypeList *elmts)
{
    NamedType  *et;
    CTRI *ctri;
    DefinedObj *fieldNames;
    char      *tmpName;
    char      *asn1FieldName;
    char      *cFieldName;

    fieldNames = NewObjList();

    /*
     * Initialize fieldname data
     * allocate (if nec) and fill in CTRI fieldname if poss
     * from asn1 field name.  leave blank otherwise
     */
    FOR_EACH_LIST_ELMT (et, elmts)
    {
        ctri =  et->type->cTypeRefInfo;
        if (ctri == NULL)
        {
            ctri = MT (CTRI);
            et->type->cTypeRefInfo = ctri;
        }
        if (et->fieldName != NULL)
        {
            asn1FieldName = et->fieldName;
            ctri->cFieldName = Asn1FieldName2CFieldName (asn1FieldName);
            DefineObj (&fieldNames, ctri->cFieldName);
        }
    }


    FOR_EACH_LIST_ELMT (et, elmts)
    {
        ctri =  et->type->cTypeRefInfo;

        /*
         * generate field names for those without them
         */
        if (ctri->cFieldName == NULL)
        {
            if ((et->type->basicType->choiceId == BASICTYPE_LOCALTYPEREF) ||
                 (et->type->basicType->choiceId == BASICTYPE_IMPORTTYPEREF))
            {
                /*
                 * take ref'd type name as field name
                 * convert first let to lower case
                 */
                tmpName = et->type->basicType->a.localTypeRef->link->cTypeDefInfo->cTypeName;
                tmpName =  Asn1TypeName2CTypeName (tmpName);
                cFieldName = Malloc (strlen (tmpName) + r->maxDigitsToAppend +1);
                strcpy (cFieldName, tmpName);
                Free (tmpName);
                if (isupper (cFieldName[0]))
                    cFieldName[0] = (char)tolower (cFieldName[0]);
            }
            else
            {
                /*
                 * get default field name for this type
                 */
                tmpName = r->typeConvTbl[et->type->basicType->choiceId].defaultFieldName;
                cFieldName = Malloc (strlen (tmpName) + r->maxDigitsToAppend +1);
                strcpy (cFieldName, tmpName);

                if (isupper (cFieldName[0]))
                    cFieldName[0] = (char)tolower (cFieldName[0]);
            }


            MakeCStrUnique (fieldNames, cFieldName, r->maxDigitsToAppend, 1);

            DefineObj (&fieldNames, cFieldName);
            ctri->cFieldName = cFieldName;
        }
    }
    FreeDefinedObjs (&fieldNames);
}  /* FillCFieldNames */



/*
 * returns true if this c type for this type should be
 * be ref'd as a ptr
 */
static int
IsCPtr PARAMS ((r, td, t, parentCTypeId),
    CRules *r _AND_
    TypeDef *td _AND_
    Type *t _AND_
    CTypeId parentCTypeId)
{
    CTDI *ctdi;
    int retVal = FALSE;

    /*
     * inherit ptr attriubutes from ref'd type if any
     * otherwise grab lib c type def from the CRules
     */
    if ((t->basicType->choiceId == BASICTYPE_LOCALTYPEREF) ||
        (t->basicType->choiceId == BASICTYPE_IMPORTTYPEREF))
    {
        ctdi = t->basicType->a.localTypeRef->link->cTypeDefInfo;
    }
    else
        ctdi = &r->typeConvTbl[GetBuiltinType (t)];

    if ((parentCTypeId == C_TYPEDEF) && (ctdi->isPtrForTypeDef))
        retVal = TRUE;

    else if ((parentCTypeId == C_STRUCT) && (ctdi->isPtrForTypeRef))
        retVal = TRUE;

	// Deepak: 18/Apr/2003
    else if ((parentCTypeId == C_MACROTYPE) && (ctdi->isPtrForTypeRef))
        retVal = TRUE;

    else if ((parentCTypeId == C_CHOICE) && (ctdi->isPtrInChoice))
        retVal = TRUE;

    else if (((t->optional) || (t->defaultVal != NULL)) && (ctdi->isPtrForOpt))
        retVal = TRUE;

    td = td;  /*AVOIDS warning.*/
    return retVal;
}  /* IsCPtr */



#define BAD_VALUE(attrValue, attrType)\
    fprintf (errFileG, "Warning: ignoring attribute with improper value (%s/%s)\n",\
		attrType, attrValue)

/*
 * attrList is a list of strings that hold attribute value
 * pairs.  A list is used in case the attr/value pairs are
 * given in multiple ASN.1 comments around the type.
 */
void ParseTypeDefAttribs PARAMS ((ctdi, attrList),
    CTDI *ctdi _AND_
    SnaccDirectiveList *attrList)
{
    SnaccDirective* pDirective;

    if (attrList == NULL)
        return;

    FOR_EACH_LIST_ELMT (pDirective, attrList)
    {
		switch (pDirective->type)
		{
		case ASN1_TypeID:
			ctdi->asn1TypeId = pDirective->value.asnTypeVal;
			break;

		case C_TypeID:
			ctdi->cTypeId = pDirective->value.cTypeVal;
			break;

		case C_TypeName:
			ctdi->cTypeName = pDirective->value.stringVal;
			break;

		case IsPDU:
			ctdi->isPdu = pDirective->value.boolVal;
			break;

		case IsPtrForTypeDef:
			ctdi->isPtrForTypeDef = pDirective->value.boolVal;
			break;

		case IsPtrForTypeRef:
			ctdi->isPtrForTypeRef = pDirective->value.boolVal;
			break;

		case IsPtrInChoice:
			ctdi->isPtrInChoice = pDirective->value.boolVal;
			break;

		case IsPtrForOpt:
			ctdi->isPtrForOpt = pDirective->value.boolVal;
			break;

		case OptionalTestRoutineName:
			ctdi->optTestRoutineName = pDirective->value.stringVal;
			break;

		case DefaultFieldName:
			ctdi->defaultFieldName = pDirective->value.stringVal;
			break;

		case PrintRoutineName:
			ctdi->printRoutineName = pDirective->value.stringVal;
			break;

		case EncodeRoutineName:
			ctdi->encodeRoutineName = pDirective->value.stringVal;
			break;

		case DecodeRoutineName:
			ctdi->decodeRoutineName = pDirective->value.stringVal;
			break;

		case FreeRoutineName:
			ctdi->freeRoutineName = pDirective->value.stringVal;
			break;

		case IsEncDec:
			ctdi->isEncDec = pDirective->value.boolVal;
			break;

		case GenTypeDef:
			ctdi->genTypeDef = pDirective->value.boolVal;
			break;

		case GenPrintRoutine:
			ctdi->genPrintRoutine = pDirective->value.boolVal;
			break;

		case GenEncodeRoutine:
			ctdi->genEncodeRoutine = pDirective->value.boolVal;
			break;

		case GenDecodeRoutine:
			ctdi->genDecodeRoutine = pDirective->value.boolVal;
			break;

		case GenFreeRoutine:
			ctdi->genFreeRoutine = pDirective->value.boolVal;
			break;

		default:
			fprintf (errFileG, "Warning: ignoring unrecognized type def attribute '%s'\n",
				GetDirectiveName(pDirective->type));
		}

    } /* end for */

} /* ParseTypeDefAttribs */


void ParseTypeRefAttribs PARAMS ((ctri, attrList),
    CTRI *ctri _AND_
    SnaccDirectiveList *attrList)
{
	SnaccDirective* pDirective;

    if (attrList == NULL)
        return;

    FOR_EACH_LIST_ELMT (pDirective, attrList)
    {
		switch (pDirective->type)
		{
		case C_TypeID:
			ctri->cTypeId = pDirective->value.cTypeVal;
			break;

		case C_TypeName:
			ctri->cTypeName = pDirective->value.stringVal;
			break;

		case C_FieldName:
			ctri->cFieldName = pDirective->value.stringVal;
			break;

		case IsPtr:
			ctri->isPtr = pDirective->value.boolVal;
			break;

		case ChoiceIdValue:
			ctri->choiceIdValue = pDirective->value.integerVal;
			break;

		case ChoiceIdSymbol:
			ctri->choiceIdSymbol = pDirective->value.stringVal;
			break;

		case ChoiceIdEnumName:
			ctri->choiceIdEnumName = pDirective->value.stringVal;
			break;

		case ChoiceIdEnumFieldName:
			ctri->choiceIdEnumFieldName = pDirective->value.stringVal;
			break;

		case OptionalTestRoutineName:
			ctri->optTestRoutineName = pDirective->value.stringVal;
			break;

		case PrintRoutineName:
			ctri->printRoutineName = pDirective->value.stringVal;
			break;

		case EncodeRoutineName:
			ctri->encodeRoutineName = pDirective->value.stringVal;
			break;

		case DecodeRoutineName:
			ctri->decodeRoutineName = pDirective->value.stringVal;
			break;

		case FreeRoutineName:
			ctri->freeRoutineName = pDirective->value.stringVal;
			break;

		case IsEncDec:
			ctri->isEncDec = pDirective->value.boolVal;
			break;

		case IsBigInt:
			if (pDirective->value.boolVal)
			{
				ctri->cTypeName = "AsnBigInt";
				ctri->printRoutineName = "PrintAsnBigInt";
				ctri->encodeRoutineName = "EncAsnBigInt";
				ctri->decodeRoutineName = "DecAsnBigInt";
				ctri->freeRoutineName = "FreeAsnBigInt";
			}
			break;

		default:
			fprintf (errFileG, "Warning: ignoring unrecognized type def attribute '%s'\n",
				GetDirectiveName(pDirective->type));
		}

	} /* end of for loop */

} /* ParseTypeRefAttribs */


/* fill given ctdi with defaults from table for given typedef */
void
FillCTDIDefaults PARAMS ((r, ctdi, td),
    CRules *r _AND_
    CTDI *ctdi _AND_
    TypeDef *td)
{
    CTDI *tblCtdi;
    int typeIndex;

    typeIndex = GetBuiltinType (td->type);

    if (typeIndex < 0)
        return;

    tblCtdi = &r->typeConvTbl[typeIndex];

    memcpy (ctdi, tblCtdi, sizeof (CTDI));

}

const char* GetDirectiveName(SnaccDirectiveEnum dirType)
{
	switch (dirType)
	{
	case ASN1_TypeID:
		return "asn1TypeId";
	case C_TypeID:
		return "cTypeId";
	case C_TypeName:
		return "cTypeName";
	case C_FieldName:
		return "cFieldName";
	case IsPDU:
		return "isPdu";
	case IsPtr:
		return "isPtr";
	case IsPtrForTypeDef:
		return "isPtrForTypeDef";
	case IsPtrForTypeRef:
		return "isPtrForTypeRef";
	case IsPtrInChoice:
		return "isPtrInChoice";
	case IsPtrForOpt:
		return "isPtrForOpt";
	case OptionalTestRoutineName:
		return "optTestRoutineName";
	case DefaultFieldName:
		return "defaultFieldName";
	case PrintRoutineName:
		return "printRoutineName";
	case EncodeRoutineName:
		return "encodeRoutineName";
	case DecodeRoutineName:
		return "decodeRoutineName";
	case FreeRoutineName:
		return "freeRoutineName";
	case IsEncDec:
		return "isEncDec";
	case GenTypeDef:
		return "genTypeDef";
	case GenPrintRoutine:
		return "genPrintRoutine";
	case GenEncodeRoutine:
		return "genEncodeRoutine";
	case GenDecodeRoutine:
		return "genDecodeRoutine";
	case GenFreeRoutine:
		return "genFreeRoutine";
	case ChoiceIdSymbol:
		return "choiceIdSymbol";
	case ChoiceIdValue:
		return "choiceIdValue";
	case ChoiceIdEnumName:
		return "choiceIdEnumName";
	case ChoiceIdEnumFieldName:
		return "choiceIdEnumFieldName";
	case IsBigInt:
		return "isBigInt";
	default:
		return "<unknown>";
	}
}
