/*
 * compiler/core/link_types.c
 *
 *  Links type references. Also increments 'refCount' in a TypeDef
 *
 *  Does type checking when linking SELECTION and COMPONENTS OF types
 *
 * MS
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
 * $Header: /baseline/SNACC/compiler/core/link-types.c,v 1.21 2004/04/06 15:13:41 gronej Exp $
 * $Log: link-types.c,v $
 * Revision 1.21  2004/04/06 15:13:41  gronej
 * *** empty log message ***
 *
 * Revision 1.20  2004/03/25 19:20:17  gronej
 * fixed some linux warnings
 *
 * Revision 1.19  2004/02/06 21:21:32  nicholar
 * Replaced List<T> with std::list<T>
 * Numerous changes/fixes to constraints code
 *
 * Revision 1.18  2004/01/29 21:21:45  gronej
 * Took all Init() calls out of Clear() functions in generated code
 *
 * Revision 1.17  2004/01/14 19:07:52  gronej
 * Updated Compiler to accept and process relative-oid's
 *
 * Revision 1.16  2003/07/28 11:11:23  colestor
 * Changes to complete handing of the "--snacc namespace" compiler directive.
 * Also, updates to handle ASN.1 constant integer tag designations for C++/C.
 *
 * Revision 1.15  2003/07/14 21:07:44  nicholar
 * Changed how parser handles --snacc directives.  Added namespace option.
 *
 * Revision 1.14  2003/07/07 14:50:13  nicholar
 * Eliminated headers and cleaned up include references
 *
 * Revision 1.13  2003/04/29 21:08:36  leonberp
 * integerated Deepak's changes for IOB support
 *
 * Revision 1.12  2003/01/06 16:16:31  leonberp
 * added GeneralString
 *
 * Revision 1.11  2002/11/01 17:16:39  mcphersc
 * added import used
 *
 * Revision 1.10  2002/10/28 19:57:03  leonberp
 * Added BITSTRING CONTAINING support and fixed CONTAINED ANY DEFINED BY
 *
 * Revision 1.9  2002/10/24 21:07:22  leonberp
 * latest fixing for OCTET CONTAINING
 *
 * Revision 1.8  2002/09/16 16:50:04  mcphersc
 * iFixed warnings
 * CV: -rnings
 * ---------------------------------------------------------------------
 *
 * Revision 1.7  2002/09/04 18:23:07  vracarl
 * got rid of c++ comments
 *
 * Revision 1.6  2002/05/15 17:00:59  leonberp
 * added support for new basicTypes to compiler
 *
 * Revision 1.5  2002/05/15 14:53:12  leonberp
 * added support for new basicTypes to compiler
 *
 * Revision 1.4  2002/05/10 16:39:40  leonberp
 * latest changes for release 2.2
 * includes integrating asn-useful into C & C++ runtime library, the compiler changes that go along with that, SnaccException changes for C++ runtime and compiler
 *
 * Revision 1.3  2001/05/11 16:32:34  rwc
 * Update to remove need for "-u"; code core dumped if "-u" was not present
 * on the command line due to newly added logic handling the new String
 * handling ASN.1 classes (e.g. PrintableString).
 *
 * Revision 1.2  2000/10/24 14:54:52  rwc
 * Updated to remove high-level warnings (level 4 on MSVC++) for an easier build.
 * SOME warnings persist due to difficulty in modifying the SNACC compiler to
 * properly build clean source; also some files are built by Lex/Yacc.
 *
 * Revision 1.1.1.1  2000/08/21 20:36:00  leonberp
 * First CVS Version of SNACC.
 *
 * Revision 1.3  1995/07/25 19:41:36  rj
 * changed `_' to `-' in file names.
 *
 * Revision 1.2  1994/09/01  00:38:30  rj
 * snacc_config.h removed.
 *
 * Revision 1.1  1994/08/28  09:49:17  rj
 * first check-in. for a list of changes to the snacc-1.1 distribution please refer to the ChangeLog.
 *
 */


#include <string.h>
#include <ctype.h>
#include "asn-incl.h"
#include "asn1module.h"
#include "snacc-util.h"


/* non-exported prototypes */
void TypeLinkImportLists PROTO ((ModuleList *m));
void TypeLinkTypeDef PROTO ((ModuleList *m, Module *currMod, TypeDef *head));
void TypeLinkElmtTypes PROTO ((ModuleList *m, Module *currMod, TypeDef *head, NamedTypeList *e));
void TypeLinkElmtType PROTO ((ModuleList *m, Module *currMod, TypeDef *head, NamedType *n));
void TypeLinkType PROTO ((ModuleList *m, Module *currMod, TypeDef *head, Type *currType));
void TypeLinkBasicType PROTO ((ModuleList *m, Module *currMod, TypeDef *head, Type *type, BasicType *bt));
void TypeLinkSubtypes PROTO ((ModuleList *m, Module *currMod, TypeDef *head, Type *currType, Subtype *s));
void TypeLinkSubtypeValue PROTO ((ModuleList *m, Module *currMod, TypeDef *head, Type *currType, SubtypeValue *s));
void TypeLinkNamedElmts PROTO ((ModuleList *m, Module *currMod, TypeDef *head, Type *t, ValueDefList *v));

// Deepak: 04/Mar/2003
void TypeLinkObjectAssignmentField PROTO ((ModuleList *m, Module *currMod, ObjectAssignment *head, ObjectAssignmentField *field));
void TypeLinkObjectAssignments PROTO ((ModuleList *m, Module *currMod, ObjectAssignment *head));
void TypeLinkObjectSetAssignments PROTO ((ModuleList *m, Module *currMod, ObjectSetAssignment *head));
void TypeLinkWithSyntaxes PROTO ((ModuleList *m, Module *currMod, TypeDef *head, ObjectClassDef *ocd));
////////////////////

void TypeLinkValueDef PROTO ((ModuleList *m, Module *currMod, ValueDef *v));
void TypeLinkValue PROTO ((ModuleList *m, Module *currMod, ValueDef *head, Type *valuesType, Value *v));
void TypeLinkRosOperationMacroType PROTO ((ModuleList *m, Module *currMod, TypeDef *head, Type *t, BasicType *bt, RosOperationMacroType *op));
void TypeLinkRosErrorMacroType PROTO ((ModuleList *m, Module *currMod, TypeDef *head, Type *t, BasicType *bt, RosErrorMacroType *err));
void TypeLinkRosBindMacroType PROTO ((ModuleList *m, Module *currMod, TypeDef *head, Type *t, BasicType *bt, RosBindMacroType *bind));
void TypeLinkRosAseMacroType PROTO ((ModuleList *m, Module *currMod, TypeDef *head, Type *t, BasicType *bt, RosAseMacroType *ase));
void TypeLinkRosAcMacroType PROTO ((ModuleList *m, Module *currMod, TypeDef *head, Type *t, BasicType *bt, RosAcMacroType *ac));
void TypeLinkMtsasExtensionsMacroType PROTO ((ModuleList *m, Module *currMod, TypeDef *head, Type *t, BasicType *bt, MtsasExtensionsMacroType *exts));
void TypeLinkMtsasExtensionMacroType PROTO ((ModuleList *m, Module *currMod, TypeDef *head, Type *t, BasicType *bt, MtsasExtensionMacroType *ext));
void TypeLinkMtsasExtensionAttributeMacroType PROTO ((ModuleList *m, Module *currMod, TypeDef *head, Type *t, BasicType *bt, MtsasExtensionAttributeMacroType *ext));
void TypeLinkMtsasTokenMacroType PROTO ((ModuleList *m, Module *currMod, TypeDef *head, Type *t, BasicType *bt, MtsasTokenMacroType *tok));
void TypeLinkMtsasTokenDataMacroType PROTO ((ModuleList *m, Module *currMod, TypeDef *head, Type *t, BasicType *bt, MtsasTokenDataMacroType *tok));
void TypeLinkMtsasSecurityCategoryMacroType PROTO ((ModuleList *m, Module *currMod, TypeDef *head, Type *t, BasicType *bt, MtsasSecurityCategoryMacroType *sec));
void TypeLinkAsnObjectMacroType PROTO ((ModuleList *m, Module *currMod, TypeDef *head, Type *t, BasicType *bt, AsnObjectMacroType *obj));
void TypeLinkAsnPortMacroType PROTO ((ModuleList *m, Module *currMod, TypeDef *head, Type *t, BasicType *bt, AsnPortMacroType *p));
void TypeLinkAsnAbstractBindMacroType PROTO ((ModuleList *m, Module *currMod, TypeDef *head, Type *t, BasicType *bt, AsnAbstractBindMacroType *bind));
void TypeLinkSnmpObjectTypeMacroType PROTO ((ModuleList *m, Module *currMod, TypeDef *head, Type *t, BasicType *bt, SnmpObjectTypeMacroType *ot));
/* end of prototypes */


static char *asn1SrcFileNameG;


/*
 * returns 0 if no link error occured,
 * otherwise returns a value < 0.
 * Processing should not continue if an error is returned
 */
int
LinkTypeRefs PARAMS ((m),
    ModuleList *m)
{
    Module   *currMod;
    TypeDef  *td;
    ValueDef *vd;
	ObjectAssignment *oa;
	ObjectSetAssignment *osa;
    int linkErr = 0;

    /*
     * link imported types/values to their definition if
     * the defining module is in the modulelist
     */
    TypeLinkImportLists (m);


    /*
     * go through types, values & macros of  each module
     */
    FOR_EACH_LIST_ELMT (currMod, m)
    {
        asn1SrcFileNameG = currMod->asn1SrcFileName;

        /*
         * go through each type in typeList and link as nec
         */
        FOR_EACH_LIST_ELMT (td, currMod->typeDefs)	// Deepak: all typedefs are processed here
        {
            TypeLinkTypeDef (m, currMod, td);		// Deepak: all the major types e.g. sequence, objectclass
        }											// are linked here, subtypes in further functions.


        /*
         *  go through each value in valueList and link as nec
         */
        FOR_EACH_LIST_ELMT (vd, currMod->valueDefs)	// Deepak: all valuedefs are processed here
        {
            TypeLinkValueDef (m, currMod, vd);
        }

        /*	Deepak: 04/Mar/2003
         *  go through each field in objectAssignments and link as nec
         */
        FOR_EACH_LIST_ELMT (oa, currMod->objAssignments)
        {
            TypeLinkObjectAssignments (m, currMod, oa);		// Deepak: 04/Mar/2003
        }

        /*	Deepak: 04/Mar/2003
         *  go through each object in objectSetAssignments and link as nec
         */
        FOR_EACH_LIST_ELMT (osa, currMod->objSetAssignments)
        {
            TypeLinkObjectSetAssignments (m, currMod, osa);
        }

        if (currMod->status != MOD_ERROR)
            currMod->status = MOD_OK;
        else
            linkErr = -1;
    }

    return linkErr;

} /* LinkRefs */


/*
 * goes through import lists of each module making sure each
 * imported type is in the referenced module.  Will flag
 * errors if the imported type cannot be found or is not
 * exported by the referenced module.
 */
void
TypeLinkImportLists PARAMS ((m),
    ModuleList *m)
{
    Module *currMod;
    TypeDef *t;
    /* ValueDef *v; */
    ImportModule *currImpList;
    ImportElmt *currImpElmt;
    Module *impRefMod;


    /* Link each modules imports */
    FOR_EACH_LIST_ELMT (currMod, m)
    {
        /*
         * Link each import list in the currMod.
         * (there is an import list for every module
         * imported from by this module
         */
        FOR_EACH_LIST_ELMT (currImpList, currMod->imports)
        {
            /* lookup ref'd module by it's name and oid (if any) */
            impRefMod = LookupModule (m, currImpList->modId->name, currImpList->modId->oid);

            if (impRefMod == NULL)
            {
                /*
                 * The needed module is not available.
                 * Let user know and set fatal error
                 */
                currMod->status = MOD_ERROR;
                PrintErrLoc (currMod->asn1SrcFileName, (long)currImpList->lineNo);
                fprintf (errFileG, "ERROR - cannot locate IMPORT module \"%s\", ",
					currImpList->modId->name);

                fprintf (errFileG, "so the following types/values are missing:\n");
                FOR_EACH_LIST_ELMT (currImpElmt, currImpList->importElmts)
                {
                    fprintf (errFileG, "        "); /* indent */
                    if (currImpElmt->privateScope)
                        fprintf (errFileG, "%s.", currImpList->modId->name);
                    fprintf (errFileG, "%s\n", currImpElmt->name);
                }
                fprintf (errFileG, "\n");
                /*
                 * go onto next import list in this module
                 * to report more errors if any
                 */
                continue;
            }

            /*
             * go through each import elements and look for the
             * the referenced type in the ref'd module
             */
            FOR_EACH_LIST_ELMT (currImpElmt, currImpList->importElmts)
            {
                /*
                 * only do types (types have uppercase first letter)
                 */
                if  (!isupper (currImpElmt->name[0]))
                    continue;

                /* look for the type in the ref'd module */
                t = LookupType (impRefMod->typeDefs, currImpElmt->name);

                if (t != NULL)
                {
                    if (!t->exported)
                    {
                        currMod->status = MOD_ERROR;
                        PrintErrLoc (currMod->asn1SrcFileName, (long)currImpElmt->lineNo);
                        fprintf (errFileG, "ERROR - \"%s\" module imports \"%s\", which is not exported from module \"%s\".\n",
							currMod->modId->name, currImpElmt->name,
							impRefMod->modId->name);
                    }

                    /* set as ref'd if imported by someone */
                    t->importRefCount++;
                    currImpElmt->resolvedRef =
                        (ImportElmtChoice*)Malloc (sizeof (ImportElmtChoice));
                    currImpElmt->resolvedRef->choiceId = IMPORTELMTCHOICE_TYPE;
                    currImpElmt->resolvedRef->a.type = t;
					currMod->ImportUsed = TRUE;

                }
                else /* type not found in ref'd module */
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
} /* TypeLinkImportLists */


void	// Deepak: 04/Mar/2003
TypeLinkObjectAssignments PARAMS ((m, currMod, oa),
    ModuleList *m _AND_
    Module *currMod _AND_
    ObjectAssignment *oa)
{
	ObjectAssignmentField *oaf;
    TypeDef *tmpTypeDef;
	NamedType* nt;

    if (oa == NULL)
        return;

	// chk here objClassName CLASS exists or not.
	tmpTypeDef = LookupType (currMod->typeDefs, oa->objectClassName);
	if(tmpTypeDef)		// CLASS is defined
	{
		tmpTypeDef->localRefCount++;		// CLASS is referenced here
	}
	else		// CLASS is not defined
	{
		currMod->status = MOD_ERROR;
		PrintErrLoc (currMod->asn1SrcFileName, (long)oa->lineNo);
		fprintf (errFileG, "ERROR - CLASS \"%s\" is referenced but not defined.\n",
			oa->objectClassName);
	}

	FOR_EACH_LIST_ELMT (oaf, oa->objectAssignmentField)
	{
		if(tmpTypeDef->type->basicType->a.objectclass->withsyntax)
		{	// if WITH SYNTAX is used
			WithSyntax *ws;
			char bError;
			bError = FALSE;
			oa->bWithSyntaxPresent = 1;	// Deepak: ????? Remove if not used further.
			ws = (WithSyntax*)LookupObjectClassFieldTypeWithSyntax(tmpTypeDef->type->basicType->a.objectclass->withsyntax, oaf->objectFieldName, &bError);
			if(ws)
			{
				if(bError)	// typeName present, but With Syntax construct is not used
				{
					currMod->status = MOD_ERROR;
					PrintErrLoc (currMod->asn1SrcFileName, (long)oaf->lineNo);
					fprintf (errFileG, "ERROR - type \"%s\" must be used in place of \"%s\" in CLASS %s.\n",
						ws->definedName, oaf->objectFieldName, oa->objectClassName);
				}
				else	// typeName is present, process it
				{
					oaf->objectFieldNameWS = oaf->objectFieldName;	// WITH SYNTAX NAME
					oaf->objectFieldName = ws->typeName;			// ORIGINAL NAME
					oaf->bOptional = ws->bOptional;
				}

				// Check whether it is Unknown Type or not
				nt = LookupObjectClassFieldType(tmpTypeDef->type->basicType->a.objectclass->classdef, oaf->objectFieldName);
				if(nt && nt->type->basicType->choiceId == 0)
					oaf->bUnknownType = 1;
			}
			else	// typeName is not present et al
			{
				currMod->status = MOD_ERROR;
				PrintErrLoc (currMod->asn1SrcFileName, (long)oaf->lineNo);
				fprintf (errFileG, "ERROR - type \"%s\" is referenced but not defined in CLASS %s.\n",
					oaf->objectFieldName, oa->objectClassName);
			}
		}
		else	// if WITH SYNTAX is not used
		{
			oa->bWithSyntaxPresent = 0;	// Deepak: ?????
			nt = LookupObjectClassFieldType(tmpTypeDef->type->basicType->a.objectclass->classdef, oaf->objectFieldName);
			if(nt)
			{
				if(nt->type->basicType->choiceId == 0)
					oaf->bUnknownType = 1;
			}
			else			// CLASS.&identifier is not defined
			{
				currMod->status = MOD_ERROR;
				PrintErrLoc (currMod->asn1SrcFileName, (long)oaf->lineNo);
				fprintf (errFileG, "ERROR - type \"%s\" is referenced but not defined in CLASS %s.\n",
					oaf->objectFieldName, oa->objectClassName);
			}
		}
		TypeLinkObjectAssignmentField(m, currMod, oa, oaf);
	}

	/*	For all oaf's that are in classdef but not in objectAssignment, mark their bPresent = 0
		and also add those in the oaf's list.
		For all others, which are already present, mark bPresent = 1
		This whole gamut is done just to write "<oa>.m.<oaf>Present = 0;" in init_Module_nameObjects(...).
		Deepak: 22/Mar/2003
	*/
	FOR_EACH_LIST_ELMT (nt, tmpTypeDef->type->basicType->a.objectclass->classdef)
	{
		int bFound = 0;
		FOR_EACH_LIST_ELMT (oaf, oa->objectAssignmentField)
		{
			if(strcmp(nt->fieldName, oaf->objectFieldName) == 0)
			{
				oaf->bPresent = 1;
				bFound = 1;
				break;
			}
		}
		if(!bFound)
		{
			ObjectAssignmentField *t;
			t = MT (ObjectAssignmentField);
			t->objectFieldName = nt->fieldName;
			t->bOptional = nt->type->optional;
			t->typeOrValue = NULL;
			APPEND (t, oa->objectAssignmentField);
		}
	}


}  /* LinkTypeDef */

void	// Deepak: 04/Mar/2003
TypeLinkObjectAssignmentField PARAMS ((m, currMod, head, field),
    ModuleList *m _AND_
    Module *currMod _AND_
	ObjectAssignment *head _AND_
    ObjectAssignmentField *field)
{
	TypeOrValue *tOrV;

    if (field == NULL)
        return;

    tOrV = field->typeOrValue;
    if (tOrV->choiceId == TYPEORVALUE_TYPE)
        TypeLinkType (m, currMod, NULL, tOrV->a.type);
    else
        TypeLinkValue (m, currMod, NULL, NULL, tOrV->a.value);
	head = head;	// avoids warning
}

void	// Deepak: 04/Mar/2003
TypeLinkObjectSetAssignments PARAMS ((m, currMod, head),
    ModuleList *m _AND_
    Module *currMod _AND_
    ObjectSetAssignment *head)
{
	TypeOrValue *tOrV;
    TypeDef *tmpTypeDef;
	ObjectAssignment *oa;

	if (head == NULL)
        return;

	// chk here objClassName CLASS exists or not.
	tmpTypeDef = LookupType (currMod->typeDefs, head->objectClassName);
	if(tmpTypeDef)		// CLASS is defined
	{
		tmpTypeDef->localRefCount++;		// CLASS is referenced here
		head->objectClassDefLink = tmpTypeDef->type->basicType->a.objectclass;	// Deepak: 26/Mar/2003
	}
	else		// CLASS is not defined
	{
		currMod->status = MOD_ERROR;
		PrintErrLoc (currMod->asn1SrcFileName, (long)head->lineNo);
		fprintf (errFileG, "ERROR - CLASS \"%s\" is referenced but not defined.\n",
			head->objectClassName);
	}

	// then chk each object exists or not.
	FOR_EACH_LIST_ELMT (tOrV, head->objectNameList)
	{
		oa = LookupObjectClassObjectAssignment(currMod->objAssignments, tOrV->a.value->basicValue->a.namedValue->fieldName);
		tOrV->a.value->basicValue->a.objAssignment = oa;
		tOrV->a.value->basicValue->a.localValueRef->module = currMod;
	}

	//TypeLinkObjectAssignmentField (m, currMod, head, head->type);
	m = m; // avoids warning
}  /* LinkTypeDef */



/*
 * given a type def, it goes through the entire typedef
 * (aggregate parts if any) and links refs
 */
void
TypeLinkTypeDef PARAMS ((m, currMod, head),
    ModuleList *m _AND_
    Module *currMod _AND_
    TypeDef *head)
{
    if (head == NULL)
        return;

    TypeLinkType (m, currMod, head, head->type);
}  /* LinkTypeDef */


/*
 * given a type t, this routine goes through the components of
 * the type and links any type references
 */
void
TypeLinkType PARAMS ((m, currMod, head, t),
    ModuleList *m _AND_
    Module *currMod _AND_
    TypeDef *head _AND_
    Type *t)
{
    if (t == NULL)
        return;

    /* like main type information */
    TypeLinkBasicType (m, currMod, head, t, t->basicType);	// Deepak: main job is done here...

    /* link any type references in the subtypes (if any)*/
    TypeLinkSubtypes (m, currMod, head, t, t->subtypes);	// Deepak: e.g. NumericString(SIZE(15)) has a subtype SIZE

    /* like type refs in the default value (if any) */
    if (t->defaultVal != NULL)
        TypeLinkValue (m, currMod, NULL, t,  t->defaultVal->value);

}  /* TypeLinkType */


/*
 * given a sequence of NamedTypes (components of a SET, SEQ or
 * CHOICE etc), this links any type refs in each one.
 */
void
TypeLinkElmtTypes PARAMS ((m, currMod, head, e),
    ModuleList *m _AND_
    Module *currMod _AND_
    TypeDef *head _AND_
    NamedTypeList *e)
{
    NamedType *n;
    FOR_EACH_LIST_ELMT (n, e)	// Process All the elements in the sequence etc.
    {
        TypeLinkElmtType (m, currMod, head, n);
    }
}  /* TypeLinkElmtTypes */


void
TypeLinkElmtType PARAMS ((m, currMod, head, n),
    ModuleList *m _AND_
    Module *currMod _AND_
    TypeDef *head _AND_
    NamedType *n)
{
    if (n != NULL)
        TypeLinkType (m, currMod, head, n->type);
}

/*
 * given a BasicType, this links any type refs that are
 * part of it.
 */
void
TypeLinkBasicType PARAMS ((m, currMod, head, type, bt),
    ModuleList *m _AND_
    Module *currMod _AND_
    TypeDef *head _AND_
    Type *type _AND_
    BasicType *bt)
{
    TypeDef *tmpTypeDef;
    /*TypeDefList *tmpTypeDefs; */
    Type *tmpType;
    /*Module *tmpMod; */
    NamedType *tmpElmtType;
    ImportElmt *impElmt;
    ImportModule *impMod;
    /*int implicitRef; */

    if (bt == NULL)
        return;

    switch (bt->choiceId)
    {
		case BASICTYPE_SEQUENCET:		// Deepak: added on 29/Nov/2002
        case BASICTYPE_SEQUENCE:
        case BASICTYPE_SET:
        case BASICTYPE_CHOICE:			// Deepak: process the elements in sequence etc here.
            TypeLinkElmtTypes (m, currMod, head, bt->a.set);	// Deepak: bt->a.set is of NamedTypeList*
            break;

		  case BASICTYPE_BITCONTAINING:
        case BASICTYPE_OCTETCONTAINING:
				TypeLinkType (m, currMod, head, bt->a.stringContaining);
				break;

        case BASICTYPE_SEQUENCEOF:
        case BASICTYPE_SETOF:
            TypeLinkType (m, currMod, head, bt->a.setOf);
            break;

        case BASICTYPE_SELECTION:
            TypeLinkType (m, currMod, head, bt->a.selection->typeRef);

            /*
             * check that elmt type is CHOICE
             * and set up link  (if resolved)
             */
            tmpType = bt->a.selection->typeRef;
            if ((tmpType->basicType->choiceId == BASICTYPE_IMPORTTYPEREF) ||
                (tmpType->basicType->choiceId == BASICTYPE_LOCALTYPEREF))
            {
                tmpTypeDef = tmpType->basicType->a.importTypeRef->link;
                if (tmpTypeDef == NULL) /* unlinked import or local type */
                {
                    currMod->status = MOD_ERROR;
                    return;
                }
            }
            else
            {
                PrintErrLoc (currMod->asn1SrcFileName, (long)type->lineNo);
                fprintf (errFileG, "ERROR - selection type defines type instead of referencing CHOICE field.\n");
                currMod->status = MOD_ERROR;
                return;
            }

            /*
             * selections types must reference choice types
             */
            tmpType = ParanoidGetType (tmpTypeDef->type);
            if (tmpType->basicType->choiceId != BASICTYPE_CHOICE)
            {
                PrintErrLoc (currMod->asn1SrcFileName, (long)type->lineNo);
                fprintf (errFileG, "ERROR - SELECTION types must reference a CHOICE type\n");
                currMod->status = MOD_ERROR;
                return;
            }

            /*
             * find field ref'd by selection
             */
            tmpElmtType = LookupFieldInType (tmpTypeDef->type, bt->a.selection->fieldName);
            if (tmpElmtType == NULL)
            {
                PrintErrLoc (currMod->asn1SrcFileName, (long)type->lineNo);
                fprintf (errFileG, "ERROR - selection type's reference field name \"%s\" is not in CHOICE \"%s\".\n", bt->a.selection->fieldName, tmpTypeDef->definedName);
                currMod->status = MOD_ERROR;
                return;
            }

            bt->a.selection->link = tmpElmtType;
            break;


        case BASICTYPE_COMPONENTSOF:
            TypeLinkType (m, currMod, head, bt->a.componentsOf);
            /* error checks done in normalize.c */
            break;


        case BASICTYPE_ANYDEFINEDBY:
            /*
             *  set the link to the defining field if not already linked
             */
            if (bt->a.anyDefinedBy->link == NULL)
            {
                /*
                 * get set or seq that holds this any def'd by
                 */
                tmpType = GetParentS (head->type, type);

                if (tmpType == NULL)
                {
                    PrintErrLoc (currMod->asn1SrcFileName, (long)type->lineNo);
                    fprintf (errFileG, " ERROR - could not find parent type for linking ANY DEFINED BY\n");
                }

                /*
                 * find "defining" field
                 */
                tmpElmtType = LookupFieldInType (tmpType, bt->a.anyDefinedBy->fieldName);

                if (tmpElmtType == NULL)
                {
                    currMod->status = MOD_ERROR;
                    PrintErrLoc (currMod->asn1SrcFileName, (long)type->lineNo);
                    fprintf (errFileG, " ERROR - could not find identifier field \"%s\" in type \"%s\" for linking ANY DEFINED BY\n",
						bt->a.anyDefinedBy->fieldName, head->definedName);
                }

                bt->a.anyDefinedBy->link = tmpElmtType;
            }
            break;

		case BASICTYPE_OBJECTCLASSFIELDTYPE:		// Deepak: added on 04/Feb/2003
			{
				char* name;
				name = strchr(bt->a.localTypeRef->typeName, '.');
				if(name)
				{
					NamedType *namedType;
					int pos = name - bt->a.localTypeRef->typeName;
					// Deepak: changed from malloc to Malloc
					//char* name2 = Malloc(sizeof(bt->a.localTypeRef->typeName + 1));
					char* name2 = Malloc(strlen(bt->a.localTypeRef->typeName) + 1);
					strcpy(name2, bt->a.localTypeRef->typeName);
					name2[pos] = '\0';			// Name of CLASS
					tmpTypeDef = LookupType (currMod->typeDefs, name2);
					if(tmpTypeDef)		// CLASS is defined
					{
						tmpTypeDef->localRefCount++;		// CLASS is referenced here
						namedType = LookupObjectClassFieldType(tmpTypeDef->type->basicType->a.objectclass->classdef, &name[1]);
						if(namedType)	// CLASS.&identifier is defined
						{
							TypeLinkBasicType (m, currMod, head, type, namedType->type->basicType);
							
							// Link the basic type & forget about the ObjectClassFieldType (choiceId:37)
							// only except when choice id == 0	// Deepak: 31/Mar/2003
							if(namedType->type->basicType->choiceId)
							{
								type->basicType = namedType->type->basicType;
								type->tags = namedType->type->tags;	// Deepak: 31/Mar/2003
							}
							type->typeName = &name[1];	// Deepak: 26/Mar/2003
							

							// Check for the TableConstraints are valid or not?		// Deepak: 11/Mar/2003
							if(type->tableConstraint)
							{
								ObjectSetAssignment *osa;
								osa = LookupObjectClassObjectSetAssignment (currMod->objSetAssignments, type->tableConstraint->objSetAssignment->objectSetName);
								if(osa)
								{
									head->bHasTableConstraint = TRUE;
									type->tableConstraint->objSetAssignment = osa;	// Deepak: 26/Mar/2003
								}
								else
								{
									currMod->status = MOD_ERROR;
									PrintErrLoc (currMod->asn1SrcFileName, (long)type->lineNo);
									fprintf (errFileG, "ERROR - TableConstraint \"%s\" is referenced but not defined.\n",
										type->tableConstraint->objSetAssignment->objectSetName);
								}
								/////////////////////////////////////////////////////////////////////////////
								// Also to be checked the validity of AtNotations here	?????
								// LookupAtNotationsType (type->basicType->a.set, type->tableConstraint->atNotations);
								// A new func has to be written for this.
								/////////////////////////////////////////////////////////////////////////////
							}
						}
						else			// CLASS.&identifier is not defined
						{
							currMod->status = MOD_ERROR;
							PrintErrLoc (currMod->asn1SrcFileName, (long)type->lineNo);
							fprintf (errFileG, "ERROR - type \"%s\" is referenced but not defined in CLASS %s.\n",
								&name[1], name2);
						}
					}
					else		// CLASS is not defined
					{
						currMod->status = MOD_ERROR;
						PrintErrLoc (currMod->asn1SrcFileName, (long)type->lineNo);
						fprintf (errFileG, "ERROR - CLASS \"%s\" is referenced but not defined.\n",
							name2);
					}
				}
			}
			break;
        case BASICTYPE_LOCALTYPEREF:
            /*
             * Remember: the parser sets any typeref it encounters
             * to LOCALTYPE_REF, so some Localtyperefs may be import
             * type refs.
             */
            /*
             * First, look in this module's type defs and create a
             * resolvedLocalTypeRef if it's there.
             */
			if ((tmpTypeDef = LookupType (currMod->typeDefs, bt->a.localTypeRef->typeName)) != NULL)
            {
                /*
                 * locally defined type
                 */
                tmpTypeDef->localRefCount++;

                bt->a.localTypeRef->link = tmpTypeDef;
                bt->a.localTypeRef->module = currMod;
                break; /* finished here */
            }
			// ~~~~~~~~~~~~ This else is for variableTypeValueFieldSpec ~~~~~~~~~~~~~~
			else if(head && ((head->type->basicType->choiceId == BASICTYPE_OBJECTCLASS)		// Deepak: 10/Feb/2003
				/*|| (type->basicType->choiceId == BASICTYPE_OBJECTCLASSFIELDTYPE)*/   ))		// Deepak: 24/Apr/2003
			{
				//if(bt->a.localTypeRef->typeName[0] == '&')		// i.e. variable type field
				{
					NamedType* namedType = NULL;
					namedType = LookupObjectClassFieldType(head->type->basicType->a.objectclass->classdef, bt->a.localTypeRef->typeName);
					if(namedType)
					{
						if(namedType->type->basicType->choiceId == BASICTYPE_UNKNOWN)
						{
							TypeLinkBasicType (m, currMod, head, type, namedType->type->basicType);
							type->basicType = namedType->type->basicType;
						}
						else
						{
							currMod->status = MOD_ERROR;
							PrintErrLoc (currMod->asn1SrcFileName, (long)type->lineNo);
							fprintf (errFileG, "ERROR - \"%s\" must be a type field. \n",
								bt->a.localTypeRef->typeName);
						} 

					}
					else
					{
							currMod->status = MOD_ERROR;
							PrintErrLoc (currMod->asn1SrcFileName, (long)type->lineNo);
							fprintf (errFileG, "ERROR - \"%s\" is not a field of the CLASS \"%s\" \n",
								bt->a.localTypeRef->typeName, head->definedName);
					}
					break;
				}
			}
			else if(type->basicType->choiceId == BASICTYPE_OBJECTCLASSFIELDTYPE)
			{
			}
            else /* not locally defined type */
                bt->choiceId = BASICTYPE_IMPORTTYPEREF;
               /*  !!!!!! fall through !!!!!!!! */
        case BASICTYPE_IMPORTTYPEREF:

            /* This handles "modname.type" type refs. */
            if (bt->a.importTypeRef->moduleName != NULL)
            {
                /*
                 * Lookup the import list maintained in this module
                 * from the named module.  (the parser generates
                 * an import list from Foo module for "Foo.Bar" style
                 * import refs)
                 */
                impMod = LookupImportModule (currMod, bt->a.importTypeRef->moduleName);

                if (impMod == NULL) /* whoa, compiler error */
                {
                    currMod->status = MOD_ERROR;
                    fprintf (errFileG, "Compiler Error: \"%s.%s\" typeref - no import list defined from module \"%s\"\n",
						bt->a.importTypeRef->moduleName,
						bt->a.importTypeRef->typeName,
						bt->a.importTypeRef->moduleName);

                    return;
                }
                impElmt = LookupImportElmtInImportElmtList (impMod->importElmts, bt->a.importTypeRef->typeName);

                if (impElmt == NULL) /* whoa, compiler error again */
                {
                    currMod->status = MOD_ERROR;
                    fprintf (errFileG, "Compiler Error: \"%s.%s\" typeref - no import element defined for type \"%s\"\n",
						bt->a.importTypeRef->moduleName,
						bt->a.importTypeRef->typeName,
						bt->a.importTypeRef->typeName);

                    return;
                }
                /*
                 * should already be resolved unless could not find
                 * the import for some reason
                 */
                if (impElmt->resolvedRef != NULL)
                {
                   if (impElmt->resolvedRef->choiceId != IMPORTELMTCHOICE_TYPE)
                        fprintf (errFileG, "Linker Warning: import TYPE ref \"%s\" resolves with an imported VALUE\n",
						impElmt->name);

                   bt->a.importTypeRef->link = impElmt->resolvedRef->a.type;
                   bt->a.importTypeRef->link->importRefCount++;
                   bt->a.importTypeRef->module = impMod->moduleRef;
                }
                else
                {
                    /* print loc of refs to unresolved imports */
                    PrintErrLoc (currMod->asn1SrcFileName, (long)type->lineNo);
                    fprintf (errFileG, "reference to unresolved imported type \"%s\"\n",
						impElmt->name);
                }

            }
            else    /* not a "modname.type" type ref */
            {
                impElmt = LookupImportElmtInModule (currMod, bt->a.importTypeRef->typeName, &impMod);

                /*
                 * privateScope one's should only resolve with one's
                 * non-null module names (see last if) (mod.type form)
                 */
                if ((impElmt != NULL) && (!impElmt->privateScope))
                {
                    /*
                     * should already be resolved unless could not find
                     * the import for some reason
                     */
                    if (impElmt->resolvedRef != NULL)
                    {
                        if (impElmt->resolvedRef->choiceId != IMPORTELMTCHOICE_TYPE)
                            fprintf (errFileG, "Linker Warning: import TYPE ref \"%s\" resolves with an imported VALUE\n",
							impElmt->name);

                        bt->a.importTypeRef->link = impElmt->resolvedRef->a.type;
                        bt->a.importTypeRef->link->importRefCount++;
                        bt->a.importTypeRef->module = impMod->moduleRef;
                    }
                    else
                    {
                        /* print loc of refs to unresolved imports */
                        PrintErrLoc (currMod->asn1SrcFileName, (long)type->lineNo);
                        fprintf (errFileG, "reference to unresolved imported type \"%s\"\n",
							impElmt->name);
                    }
                }
                else  /* impElmt == NULL */
                {
                    /*
                     *  Type not defined locally, imported or
                     *  in useful types module.
                     */
                    currMod->status = MOD_ERROR;
                    PrintErrLoc (currMod->asn1SrcFileName, (long)type->lineNo);
                    fprintf (errFileG, "ERROR - type \"%s\" is referenced but not defined or imported.\n",
						bt->a.importTypeRef->typeName);
                }
            }
            break;


        /*
         * these types may optionally have named elmts
         */
        case BASICTYPE_INTEGER:
        case BASICTYPE_BITSTRING:
        case BASICTYPE_ENUMERATED:
            TypeLinkNamedElmts (m, currMod, head, type, bt->a.integer);
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
        case BASICTYPE_ANY:
        case BASICTYPE_EXTERNAL:
        case BASICTYPE_MACRODEF:
        case BASICTYPE_EXTENSION:
            break;

        /* Pierce 4-29-2002
           Added as native types.  no linking necessary */
         
        case BASICTYPE_NUMERIC_STR:
        case BASICTYPE_PRINTABLE_STR:
        case BASICTYPE_GENERAL_STR:
        case BASICTYPE_IA5_STR:
        case BASICTYPE_BMP_STR:
        case BASICTYPE_UNIVERSAL_STR:
        case BASICTYPE_UTF8_STR:
        case BASICTYPE_T61_STR:
        case BASICTYPE_VISIBLE_STR:
        case BASICTYPE_GRAPHIC_STR:
        case BASICTYPE_GENERALIZEDTIME:
        case BASICTYPE_UTCTIME:
        case BASICTYPE_VIDEOTEX_STR:
            break;

        case BASICTYPE_MACROTYPE:
            switch (bt->a.macroType->choiceId)
            {
                case MACROTYPE_ROSOPERATION:
                case MACROTYPE_ASNABSTRACTOPERATION:
                    TypeLinkRosOperationMacroType (m, currMod, head, type, bt, bt->a.macroType->a.rosOperation);
                    break;

                case MACROTYPE_ROSERROR:
                case MACROTYPE_ASNABSTRACTERROR:
                    TypeLinkRosErrorMacroType (m, currMod, head, type, bt, bt->a.macroType->a.rosError);
                    break;

                case MACROTYPE_ROSBIND:
                case MACROTYPE_ROSUNBIND:
                    TypeLinkRosBindMacroType (m, currMod, head, type, bt, bt->a.macroType->a.rosBind);
                    break;

                case MACROTYPE_ROSASE:
                    TypeLinkRosAseMacroType (m, currMod, head, type, bt, bt->a.macroType->a.rosAse);
                    break;

                case MACROTYPE_MTSASEXTENSIONS:
                    TypeLinkMtsasExtensionsMacroType (m, currMod, head, type, bt, bt->a.macroType->a.mtsasExtensions);
                    break;

                case MACROTYPE_MTSASEXTENSION:
                    TypeLinkMtsasExtensionMacroType (m, currMod, head, type, bt, bt->a.macroType->a.mtsasExtension);
                    break;

                case MACROTYPE_MTSASEXTENSIONATTRIBUTE:
                    TypeLinkMtsasExtensionAttributeMacroType (m, currMod, head, type, bt, bt->a.macroType->a.mtsasExtensionAttribute);
                    break;

                case MACROTYPE_MTSASTOKEN:
                    TypeLinkMtsasTokenMacroType (m, currMod, head, type, bt, bt->a.macroType->a.mtsasToken);
                    break;

                case MACROTYPE_MTSASTOKENDATA:
                    TypeLinkMtsasTokenDataMacroType (m, currMod, head, type, bt, bt->a.macroType->a.mtsasTokenData);
                    break;

                case MACROTYPE_MTSASSECURITYCATEGORY:
                    TypeLinkMtsasSecurityCategoryMacroType (m, currMod, head, type, bt, bt->a.macroType->a.mtsasSecurityCategory);
                    break;

                case MACROTYPE_ASNOBJECT:
                    TypeLinkAsnObjectMacroType (m, currMod, head, type, bt, bt->a.macroType->a.asnObject);
                    break;

                case MACROTYPE_ASNPORT:
                    TypeLinkAsnPortMacroType (m, currMod, head, type, bt, bt->a.macroType->a.asnPort);
                    break;

                case MACROTYPE_ASNABSTRACTBIND:
                case MACROTYPE_ASNABSTRACTUNBIND:
                    TypeLinkAsnAbstractBindMacroType (m, currMod, head, type, bt, bt->a.macroType->a.asnAbstractBind);
                    break;

                case MACROTYPE_AFALGORITHM:
                case MACROTYPE_AFENCRYPTED:
                case MACROTYPE_AFPROTECTED:
                case MACROTYPE_AFSIGNATURE:
                case MACROTYPE_AFSIGNED:
                    TypeLinkType (m, currMod, head, bt->a.macroType->a.afAlgorithm);
                    break;

                case MACROTYPE_SNMPOBJECTTYPE:
                    TypeLinkSnmpObjectTypeMacroType (m, currMod, head, type, bt, bt->a.macroType->a.snmpObjectType);
                    break;

                default:
                    fprintf (errFileG, "TypeLinkBasicType: ERROR - unknown macro type id!\n");
            }
            break;
		case BASICTYPE_OBJECTCLASS:		// Deepak: 28/Jan/2003
			{						// Deepak: process the elements in objectclass here.
				TypeLinkElmtTypes (m, currMod, head, bt->a.objectclass->classdef);
				if(bt->a.objectclass->withsyntax)
					TypeLinkWithSyntaxes (m, currMod, head, bt->a.objectclass);
			}
			break;

        default:
            fprintf (errFileG, "TypeLinkBasicType: ERROR - unknown basic type id!\n");
    }

}  /* LinkBasicType */


/* Deepak: 05/Mar/2003
 * given a ObjectClassDef, this Checks any with syntax refs that are
 * part of it.
 */
void
TypeLinkWithSyntaxes PARAMS ((m, currMod, head, ocd),
    ModuleList *m _AND_
    Module *currMod _AND_
    TypeDef *head _AND_
	ObjectClassDef *ocd)
{
	WithSyntax* ws;
	NamedType *nt;

    if (ocd == NULL)
        return;

	FOR_EACH_LIST_ELMT (ws, ocd->withsyntax)
	{
		nt = LookupObjectClassFieldType(ocd->classdef, ws->typeName);
		if(nt == NULL)
		{
			currMod->status = MOD_ERROR;
			PrintErrLoc (currMod->asn1SrcFileName, (long)ws->lineNo);
			fprintf (errFileG, "ERROR - type \"%s\" is referenced but not defined in CLASS %s.\n",
				ws->typeName, head->definedName);
		}
		else
		{	// WS is Present
			// Now, Perform Optionality Test
			// Here check if the type is having default value or not?
			int bTempTypeOptional = nt->type->optional;
			if(nt->type->defaultVal)
				bTempTypeOptional = 1;	// Assume that the parameter is optional if the default value is not NULL
			if (bTempTypeOptional != ws->bOptional)
			{
				currMod->status = MOD_ERROR;
				PrintErrLoc (currMod->asn1SrcFileName, (long)ws->lineNo);
				if(ws->bOptional)
				{
					fprintf (errFileG, "ERROR - type \"[%s]\" in CLASS \"%s\" should be defined as \"%s\"\n",
						ws->definedName, head->definedName, ws->definedName);
				}
				else
				{
					fprintf (errFileG, "ERROR - type \"%s\" in CLASS \"%s\" should be defined as \"[%s]\"\n",
						ws->definedName, head->definedName, ws->definedName);
				}
			}
		}
	}
	m=m; // avoids warning
}	/* TypeLinkWithSyntaxes */


/* Deepak: 05/Mar/2003 */
/*void
TypeLinkWithSyntax PARAMS ((m, currMod, head, classdef, ws),
    ModuleList *m _AND_
    Module *currMod _AND_
    TypeDef *head _AND_
	NamedTypeList* classdef _AND_
	WithSyntax* ws)
{
	NamedType *nt;

    if (ws == NULL)
        return;

	FOR_EACH_LIST_ELMT (nt, classdef)
	{
		////////
	}

}*/	/* TypeLinkWithSyntax */

///////////////////////////////////////////////////////////////////////////////

/*
 * resolve any type/value refs in the subtypes (if any)
 */
void
TypeLinkSubtypes PARAMS ((m, currMod, head, currType, s),
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
		TypeLinkSubtypeValue(m, currMod, head, currType, s->a.single);
		break;

	case SUBTYPE_AND:
	case SUBTYPE_OR:
		FOR_EACH_LIST_ELMT (sElmt, s->a.and)
			TypeLinkSubtypes(m, currMod, head, currType, sElmt);
		break;

	case SUBTYPE_NOT:
		TypeLinkSubtypes(m, currMod, head, currType, s->a.not);

	default:
		fprintf (errFileG, "TypeLinkSubtypes: ERROR - unknown Subtype id\n");
		break;
	}
}  /* TypeLinkSubtypes */




/*
 * link any type referenced in the value parts of subtypes
 */
void
TypeLinkSubtypeValue PARAMS ((m, currMod, head, currType, s),
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
            TypeLinkValue (m, currMod, NULL, currType, s->a.singleValue);
            break;

        case SUBTYPEVALUE_CONTAINED:
            TypeLinkType (m, currMod, head, s->a.contained);
            break;

        case SUBTYPEVALUE_VALUERANGE:
            TypeLinkValue (m, currMod, NULL, currType,
				s->a.valueRange->lowerEndValue->endValue);
            TypeLinkValue (m, currMod, NULL, currType,
				s->a.valueRange->upperEndValue->endValue);
            break;

        case SUBTYPEVALUE_PERMITTEDALPHABET:
            TypeLinkSubtypes (m, currMod, head, currType, s->a.permittedAlphabet);
           break;

        case SUBTYPEVALUE_SIZECONSTRAINT:
            TypeLinkSubtypes (m, currMod, head, currType, s->a.sizeConstraint);
            break;

        case SUBTYPEVALUE_INNERSUBTYPE:
            FOR_EACH_LIST_ELMT (constraint, s->a.innerSubtype->constraints)
            {
                TypeLinkSubtypes (m, currMod, head, currType, constraint->valueConstraints);
            }
            break;

        default:
            fprintf (errFileG, "TypeLinkSubtype: ERROR - unknown subtype choiceId\n");
    }

} /* TypeLinkSubtype */



/*
 * go through named elements of INTEGER/ENUMERATED/BOOLEAN
 * and link any type refs in the values
 */
void
TypeLinkNamedElmts PARAMS ((m, currMod, head, t, v),
    ModuleList *m _AND_
    Module *currMod _AND_
    TypeDef *head _AND_
    Type *t _AND_
    ValueDefList *v)
{
    ValueDef *vd;
    FOR_EACH_LIST_ELMT (vd, v)
    {
        TypeLinkValue (m, currMod, vd, vd->value->type, vd->value);
    }
    t = t;  /* AVOIDS compiler warning.*/
    head = head;
}  /* TypeLinkNamedElmts */



/*
 * only use this for 'real' value defs
 * ie those in the value def list - not ones for namedElmts
 * since infinitite recursion can result from the
 * attempt to link the values type which will try to link
 * this value again.
*/
void
TypeLinkValueDef PARAMS ((m, currMod, v),
    ModuleList *m _AND_
    Module *currMod _AND_
    ValueDef *v)
{
    if (v == NULL)
        return;

    TypeLinkType (m, currMod, NULL, v->value->type);	// Deepak: chk the type of the value defined here.

    if ((v->value->valueType == BASICTYPE_UNKNOWN) &&
        (v->value->type != NULL))
        v->value->valueType = v->value->type->basicType->choiceId;

}  /* TypeLinkValueDef */



/*
 * link any type refs associated with the given value.
 * also sets the values type field with the given
 * 'valuesType' Type.
 */
void
TypeLinkValue PARAMS ((m, currMod, head, valuesType, v),
    ModuleList *m _AND_
    Module *currMod _AND_
    ValueDef *head _AND_
    Type *valuesType _AND_
    Value *v)
{

    if (v == NULL)
        return;

    v->type = valuesType;
/*    TypeLinkType (m, currMod, NULL, v->typeRef); */

    if ((v->valueType == BASICTYPE_UNKNOWN) && (valuesType != NULL))
        v->valueType = valuesType->basicType->choiceId;
    head = head;   /* AVOIDS compiler warning.*/
    currMod = currMod;
    m = m;
}  /* TypeLinkValue */


/*
 * link any type refs in this macro's parsed data struct
 */
void
TypeLinkRosOperationMacroType PARAMS ((m, currMod, head, t, bt, op),
    ModuleList *m _AND_
    Module *currMod _AND_
    TypeDef *head _AND_
    Type *t _AND_
    BasicType *bt _AND_
    RosOperationMacroType *op)
{
    TypeOrValue *tOrV;

    if (op->arguments != NULL)
        TypeLinkType (m, currMod, head, op->arguments->type);

    if (op->result != NULL)
        TypeLinkType (m, currMod, head, op->result->type);

    /*
     *  go through errors (if any) and link types/values
     */
    FOR_EACH_LIST_ELMT (tOrV,  op->errors)
    {
        if (tOrV->choiceId == TYPEORVALUE_TYPE)
            TypeLinkType (m, currMod, head, tOrV->a.type);
        else
            TypeLinkValue (m, currMod, NULL, t, tOrV->a.value);
    }

    /*
     *  go through linked operations (if any) and
     *  link types/values
     */
    FOR_EACH_LIST_ELMT (tOrV,  op->linkedOps)
    {
        if (tOrV->choiceId == TYPEORVALUE_TYPE)
            TypeLinkType (m, currMod, head, tOrV->a.type);
        else
            TypeLinkValue (m, currMod, NULL, t, tOrV->a.value);
    }
    bt = bt; /*AVOIDS Compiler warning.*/
}  /* TypeLinkRosOperationMacroType */


/*
 * link any type refs in this macro's parsed data struct
 */
void
TypeLinkRosErrorMacroType PARAMS ((m, currMod, head, t, bt, err),
    ModuleList *m _AND_
    Module *currMod _AND_
    TypeDef *head _AND_
    Type *t _AND_
    BasicType *bt _AND_
    RosErrorMacroType *err)
{
    if ((err != NULL) && (err->parameter != NULL))
    {
        TypeLinkType (m, currMod, head, err->parameter->type);
    }
    bt = bt; /*AVOIDS Compiler warning.*/
    t = t;
}   /* TypeLinkRosErrorMacroType */

/*
 * link any type refs in this macro's parsed data struct
 */
void
TypeLinkRosBindMacroType PARAMS ((m, currMod, head, t, bt, bind),
    ModuleList *m _AND_
    Module *currMod _AND_
    TypeDef *head _AND_
    Type *t _AND_
    BasicType *bt _AND_
    RosBindMacroType *bind)
{
    if (bind != NULL)
    {
        TypeLinkElmtType (m, currMod, head, bind->argument);
        TypeLinkElmtType (m, currMod, head, bind->result);
        TypeLinkElmtType (m, currMod, head, bind->error);
    }
    bt = bt; /*AVOIDS Compiler warning.*/
    t = t;
}   /* TypeLinkRosBindMacroType */


/*
 * link any type refs in this macro's parsed data struct
 */
void
TypeLinkRosAseMacroType PARAMS ((m, currMod, head, t, bt, ase),
    ModuleList *m _AND_
    Module *currMod _AND_
    TypeDef *head _AND_
    Type *t _AND_
    BasicType *bt _AND_
    RosAseMacroType *ase)
{
    Value *v;

    FOR_EACH_LIST_ELMT (v, ase->operations)
        TypeLinkValue (m, currMod, NULL, t, v);


    FOR_EACH_LIST_ELMT (v, ase->consumerInvokes)
        TypeLinkValue (m, currMod, NULL, t, v);


    FOR_EACH_LIST_ELMT (v, ase->supplierInvokes)
        TypeLinkValue (m, currMod, NULL, t, v);

    bt = bt; /*AVOIDS Compiler warning.*/
    head = head;

}  /* TypeLinkRosAseMacroType */


/*
 * link any type refs in this macro's parsed data struct
 */
void
TypeLinkRosAcMacroType PARAMS ((m, currMod, head, t, bt, ac),
    ModuleList *m _AND_
    Module *currMod _AND_
    TypeDef *head _AND_
    Type *t _AND_
    BasicType *bt _AND_
    RosAcMacroType *ac)
{
    Value *v;
    /*OID *oid; */

    FOR_EACH_LIST_ELMT (v,  ac->nonRoElements)
        TypeLinkValue (m, currMod, NULL, t, v);


    TypeLinkType (m, currMod, head, ac->bindMacroType);
    TypeLinkType (m, currMod, head, ac->unbindMacroType);

    FOR_EACH_LIST_ELMT (v, ac->operationsOf)
        TypeLinkValue (m, currMod, NULL, t, v);


    FOR_EACH_LIST_ELMT (v, ac->initiatorConsumerOf)
        TypeLinkValue (m, currMod, NULL, t, v);


    FOR_EACH_LIST_ELMT (v, ac->responderConsumerOf)
        TypeLinkValue (m, currMod, NULL, t, v);

    bt = bt; /*AVOIDS Compiler warning.*/
    head = head;
}  /* TypeLinkRosAcMacroType */


/*
 * link any type refs in this macro's parsed data struct
 */
void
TypeLinkMtsasExtensionsMacroType PARAMS ((m, currMod, head, t, bt, exts),
    ModuleList *m _AND_
    Module *currMod _AND_
    TypeDef *head _AND_
    Type *t _AND_
    BasicType *bt _AND_
    MtsasExtensionsMacroType *exts)
{
    Value *v;

    FOR_EACH_LIST_ELMT (v, exts->extensions)
        TypeLinkValue (m, currMod, NULL, t, v);

    bt= bt; /*AVOIDS Compiler warning.*/
    head = head;
}  /* TypeLinkMtsasExtensionsMacroType */


/*
 * link any type refs in this macro's parsed data struct
 */
void
TypeLinkMtsasExtensionMacroType PARAMS ((m, currMod, head, t, bt, ext),
    ModuleList *m _AND_
    Module *currMod _AND_
    TypeDef *head _AND_
    Type *t _AND_
    BasicType *bt _AND_
    MtsasExtensionMacroType *ext)
{
    TypeLinkElmtType (m, currMod, head, ext->elmtType);
    TypeLinkValue (m, currMod, NULL, t, ext->defaultValue);

    bt = bt; /*AVOIDS Compiler warning.*/
}  /* TypeLinkMtsasExtensionMacroType */


/*
 * link any type refs in this macro's parsed data struct
 */
void
TypeLinkMtsasExtensionAttributeMacroType PARAMS ((m, currMod, head, t, bt, ext),
    ModuleList *m _AND_
    Module *currMod _AND_
    TypeDef *head _AND_
    Type *t _AND_
    BasicType *bt _AND_
    MtsasExtensionAttributeMacroType *ext)
{

    if (ext != NULL)
        TypeLinkType (m, currMod, head, ext->type);

    bt = bt; /*AVOIDS Compiler warning.*/
    t = t;
}  /* TypeLinkMtsasExtensionAttributeMacroType */

/*
 * link any type refs in this macro's parsed data struct
 */
void
TypeLinkMtsasTokenMacroType PARAMS ((m, currMod, head, t, bt, tok),
    ModuleList *m _AND_
    Module *currMod _AND_
    TypeDef *head _AND_
    Type *t _AND_
    BasicType *bt _AND_
    MtsasTokenMacroType *tok)
{
    if (tok != NULL)
        TypeLinkType (m, currMod, head, tok->type);

    bt = bt
	    ; /*AVOIDS Compiler warning.*/
    t = t;
}  /* TypeLinkMtsasTokenMacroType */

/*
 * link any type refs in this macro's parsed data struct
 */
void
TypeLinkMtsasTokenDataMacroType PARAMS ((m, currMod, head, t, bt, tok),
    ModuleList *m _AND_
    Module *currMod _AND_
    TypeDef *head _AND_
    Type *t _AND_
    BasicType *bt _AND_
    MtsasTokenDataMacroType *tok)
{
    if (tok != NULL)
        TypeLinkType (m, currMod, head, tok->type);

    bt = bt; /*AVOIDS Compiler warning.*/
    t = t;
}  /* TypeLinkMtsasTokenDataMacroType */

/*
 * link any type refs in this macro's parsed data struct
 */
void
TypeLinkMtsasSecurityCategoryMacroType PARAMS ((m, currMod, head, t, bt, sec),
    ModuleList *m _AND_
    Module *currMod _AND_
    TypeDef *head _AND_
    Type *t _AND_
    BasicType *bt _AND_
    MtsasSecurityCategoryMacroType *sec)
{

    if (sec != NULL)
        TypeLinkType (m, currMod, head, sec->type);

    bt = bt; /*AVOIDS Compiler warning.*/
    t = t;
}  /* TypeLinkMtsasSecurityCategoryMacroType */


/*
 * link any type refs in this macro's parsed data struct
 */
void
TypeLinkAsnObjectMacroType PARAMS ((m, currMod, head, t, bt, obj),
    ModuleList *m _AND_
    Module *currMod _AND_
    TypeDef *head _AND_
    Type *t _AND_
    BasicType *bt _AND_
    AsnObjectMacroType *obj)
{
    AsnPort *ap;

    FOR_EACH_LIST_ELMT (ap, obj->ports)
        TypeLinkValue (m, currMod, NULL, t, ap->portValue);

    bt = bt; /*AVOIDS Compiler warning.*/
    head = head;
}  /* TypeLinkAsnObjectMacroType */

/*
 * link any type refs in this macro's parsed data struct
 */
void
TypeLinkAsnPortMacroType PARAMS ((m, currMod, head, t, bt, p),
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
            TypeLinkType (m, currMod, head, tOrV->a.type);
        else
            TypeLinkValue (m, currMod, NULL, t, tOrV->a.value);
    }


    FOR_EACH_LIST_ELMT (tOrV, p->supplierInvokes)
    {
        if (tOrV->choiceId == TYPEORVALUE_TYPE)
            TypeLinkType (m, currMod, head, tOrV->a.type);
        else
            TypeLinkValue (m, currMod, NULL, t, tOrV->a.value);
    }


    FOR_EACH_LIST_ELMT (tOrV, p->consumerInvokes)
    {
        if (tOrV->choiceId == TYPEORVALUE_TYPE)
            TypeLinkType (m, currMod, head, tOrV->a.type);
        else
            TypeLinkValue (m, currMod, NULL, t, tOrV->a.value);
    }

    bt = bt; /*AVOIDS Compiler warning.*/
}  /* TypeLinkAsnPortMacroType */


/*
 * link any type refs in this macro's parsed data struct
 */
void
TypeLinkAsnAbstractBindMacroType PARAMS ((m, currMod, head, t, bt, bind),
    ModuleList *m _AND_
    Module *currMod _AND_
    TypeDef *head _AND_
    Type *t _AND_
    BasicType *bt _AND_
    AsnAbstractBindMacroType *bind)
{
    AsnPort *ap;

    FOR_EACH_LIST_ELMT (ap, bind->ports)
        TypeLinkValue (m, currMod, NULL, t, ap->portValue);

    bt = bt; /*AVOIDS Compiler warning.*/
    head = head;
}  /* TypeLinkAsnBindMacroType */


/*
 * link any type refs in this macro's parsed data struct
 */
void
TypeLinkSnmpObjectTypeMacroType PARAMS ((m, currMod, head, t, bt, ot),
    ModuleList *m _AND_
    Module *currMod _AND_
    TypeDef *head _AND_
    Type *t _AND_
    BasicType *bt _AND_
    SnmpObjectTypeMacroType *ot)
{
    TypeOrValue *tOrV;

    TypeLinkType (m, currMod, head, ot->syntax);
    TypeLinkValue (m, currMod, NULL, t, ot->description);
    TypeLinkValue (m, currMod, NULL, t, ot->reference);
    TypeLinkValue (m, currMod, NULL, t, ot->defVal);

    FOR_EACH_LIST_ELMT (tOrV, ot->index)
    {
        if (tOrV->choiceId == TYPEORVALUE_TYPE)
            TypeLinkType (m, currMod, head, tOrV->a.type);
        else
            TypeLinkValue (m, currMod, NULL, t, tOrV->a.value);
    }

    bt = bt; /*AVOIDS Compiler warning.*/
}  /* TypeLinkSnmpObjectTypeMacroType */
