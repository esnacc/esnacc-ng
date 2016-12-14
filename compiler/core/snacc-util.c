/*
 * compiler/core/snacc_util.c
 *
 * utilities for dealing with the Module data structure
 *
 * AUTHOR: Mike Sample
 * DATE:   91/09/02
 *
 * Copyright (C) 1991, 1992 Michael Sample
 *            and the University of British Columbia
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * $Header: /baseline/SNACC/compiler/core/snacc-util.c,v 1.15 2004/04/06 15:13:41 gronej Exp $
 * $Log: snacc-util.c,v $
 * Revision 1.15  2004/04/06 15:13:41  gronej
 * *** empty log message ***
 *
 * Revision 1.14  2004/03/25 19:20:17  gronej
 * fixed some linux warnings
 *
 * Revision 1.13  2004/01/14 19:07:53  gronej
 * Updated Compiler to accept and process relative-oid's
 *
 * Revision 1.12  2003/07/28 11:11:24  colestor
 * Changes to complete handing of the "--snacc namespace" compiler directive.
 * Also, updates to handle ASN.1 constant integer tag designations for C++/C.
 *
 * Revision 1.11  2003/07/07 14:50:14  nicholar
 * Eliminated headers and cleaned up include references
 *
 * Revision 1.10  2003/04/29 21:07:14  leonberp
 * integerated Deepak's changes for IOB support
 *
 * Revision 1.9  2003/01/27 16:50:45  leonberp
 * added GetLastNamedNumberValue()
 *
 * Revision 1.8  2002/10/28 20:50:58  leonberp
 * Ok.. I think this is the final fix for OCTET/BIT STRING CONTAINING
 *
 * Revision 1.7  2002/10/24 21:07:22  leonberp
 * latest fixing for OCTET CONTAINING
 *
 * Revision 1.6  2002/10/21 17:15:19  mcphersc
 * fixed long int
 *
 * Revision 1.5  2002/09/16 16:50:27  mcphersc
 * Fixed warnings
 *
 * Revision 1.4  2002/09/04 18:31:39  vracarl
 * got rid of c++ comments
 *
 * Revision 1.3  2002/05/15 17:01:01  leonberp
 * added support for new basicTypes to compiler
 *
 * Revision 1.2  2000/10/24 14:54:55  rwc
 * Updated to remove high-level warnings (level 4 on MSVC++) for an easier build.
 * SOME warnings persist due to difficulty in modifying the SNACC compiler to
 * properly build clean source; also some files are built by Lex/Yacc.
 *
 * Revision 1.1.1.1  2000/08/21 20:36:01  leonberp
 * First CVS Version of SNACC.
 *
 * Revision 1.3  1995/07/25 19:41:44  rj
 * changed `_' to `-' in file names.
 *
 * Revision 1.2  1994/09/01  00:45:09  rj
 * snacc_config.h removed.
 *
 * Revision 1.1  1994/08/28  09:49:39  rj
 * first check-in. for a list of changes to the snacc-1.1 distribution please refer to the ChangeLog.
 *
 */

#include <string.h>
#include "asn-incl.h"
#include "asn1module.h"
#include "lib-types.h"
#include "define.h"
#include "snacc-util.h"


extern FILE* errFileG;		// Defined in snacc.c


/*
 * Allocates and initializes a type and it's basicType info
 * used extensively by asn1.yacc
 * (was a macro)
 */
void
SetupType PARAMS ((t, typeId, lineNum),
    Type **t _AND_
    enum BasicTypeChoiceId typeId _AND_
    unsigned long lineNum)
{
    Tag **tmpPtr;

    (*t) = (Type*)Malloc (sizeof (Type));
    (*t)->lineNo = lineNum;
    (*t)->basicType = (BasicType*)Malloc (sizeof (BasicType));
    (*t)->basicType->choiceId = typeId;
    (*t)->tags = (TagList*)AsnListNew (sizeof (void*));
    if (LIBTYPE_GET_UNIV_TAG_CODE ((typeId)) != NO_TAG_CODE)
    {
         tmpPtr = (Tag**)AsnListAppend ((AsnList*)(*t)->tags);
         *tmpPtr = (Tag*)Malloc (sizeof (Tag));
         (*tmpPtr)->tclass = UNIV;
         (*tmpPtr)->code = LIBTYPE_GET_UNIV_TAG_CODE ((typeId));
    }
} /* SetupType */


/*
 * Allocates and initializes a type and it's basicType to MACROTYPE
 * and sets the MACROTYPE type to the given macrotype
 */
void
SetupMacroType PARAMS ((t, macroTypeId, lineNum),
    Type **t _AND_
    enum MacroTypeChoiceId macroTypeId _AND_
    unsigned long lineNum)
{

    (*t) = MT (Type);
    (*t)->lineNo = lineNum;
    (*t)->basicType = MT (BasicType);
    (*t)->basicType->choiceId = BASICTYPE_MACROTYPE;
    (*t)->tags = (TagList*)AsnListNew (sizeof (void*));
    (*t)->basicType->a.macroType = MT (MacroType);
    (*t)->basicType->a.macroType->choiceId = macroTypeId;
} /* SetupMacroType */


/*
 * similar to SetupType but for values instead
 */
void
SetupValue PARAMS ((v, valId, lineNum),
    Value **v _AND_
    enum BasicValueChoiceId valId _AND_
    unsigned long lineNum)
{
    *v = (Value*)Malloc (sizeof (Value));
    (*v)->basicValue = (BasicValue*)Malloc (sizeof (BasicValue));
    (*v)->basicValue->choiceId = valId;
    (*v)->lineNo = lineNum;
} /* SetupValue */


/*
 * adds elmt with given name to module m's import list from
 * the module with name refdModuleName.  If module m does not
 * have an import list from that module one is created.
 * The import element is given the private scope implied
 * by the ASN.1 modname.typ-or-val-name reference format
 * The passed in strings (name, refdModuleName) are copied.
 */
void
AddPrivateImportElmt PARAMS ((m, name, refdModuleName, lineNo),
    Module *m _AND_
    char *name _AND_
    char *refdModuleName _AND_
    long  lineNo)
{
    ImportElmt   *newElmt;
    ImportElmt   *ie;
    ImportModule *impMod;


    /* see if module m already imports something from "refdModule" */
    if ((impMod = LookupImportModule (m, refdModuleName)) == NULL)
    {
        impMod = MT (ImportModule);
        impMod->modId = MT (ModuleId);
        impMod->modId->name = Malloc (strlen (refdModuleName)+1);
        strcpy (impMod->modId->name, refdModuleName);

        newElmt = MT (ImportElmt);
        newElmt->name = Malloc (strlen (name)+1);
        strcpy (newElmt->name, name);
        newElmt->privateScope = TRUE;

        APPEND (newElmt, impMod->importElmts);
        APPEND (impMod, m->imports);
    }
    else  /* module "refdModule is already imported from */
    {
        ie = LookupImportElmtInImportElmtList (impMod->importElmts, name);

        if (ie == NULL)
        {
            newElmt  = MT (ImportElmt);
            newElmt->name = Malloc (strlen (name)+1);
            strcpy (newElmt->name, name);
            APPEND (newElmt, impMod->importElmts);
        }
        else if (!ie->privateScope)
        {
            PrintErrLoc (m->asn1SrcFileName, (long)lineNo);
            fprintf (errFileG, "WARNING - \"%s.%s\" type/value reference refers to a type/value already in the import list that does not have private scope.\n",
				refdModuleName, name);
        }
    }
} /* AddPrivateImportElmt */

/*
 *   looks for the named import type/value in all of the IMPORT lists of the
 *   given module.
 *   RETURNS a ptr to the import elmt if found, NULL if it was not found.
 *   If the item was found (ptr returned) the foundImportModule addr
 *   parameter will be set to the module's importModule that holds
 *   the found elmt.
 *
 *   returns NULL if the named import name was not found
 *
 */
ImportElmt*
LookupImportElmtInModule PARAMS ((m, name, foundImportModule),
    Module *m _AND_
    char *name _AND_
    ImportModule **foundImportModule)
{
     ImportModule *importMod;
     ImportElmt *importElmt;
     ImportElmt *retVal;
     void *tmp;

     if (m->imports == NULL)
         return NULL;

     tmp = (void*)CURR_LIST_NODE (m->imports);
     retVal = NULL;
     FOR_EACH_LIST_ELMT (importMod, m->imports)
     {
         importElmt = LookupImportElmtInImportElmtList (importMod->importElmts, name);
         if (importElmt != NULL)
         {
             *foundImportModule = importMod;
             retVal = importElmt;
             break;
         }
     }

     SET_CURR_LIST_NODE (m->imports, tmp); /* restore orig loc */
     return retVal;

}  /* LookupImportElmtInModule */


/*
 * given a list of import elmts, returns ptr to the elmt with
 * the matching name. NULL if not found
 */
ImportElmt*
LookupImportElmtInImportElmtList PARAMS ((impElmtList, name),
    ImportElmtList *impElmtList _AND_
    char *name)
    
{
     ImportElmt *impElmt;
     ImportElmt *retVal;
     void *tmp;

     if (impElmtList == NULL)
         return NULL;

     tmp = (void*) CURR_LIST_NODE (impElmtList);
     retVal = NULL;
     FOR_EACH_LIST_ELMT (impElmt, impElmtList)
     {
         if (strcmp (impElmt->name, name) == 0)
         {
             retVal = impElmt;
             break;
         }
     }

     SET_CURR_LIST_NODE (impElmtList, tmp);
     return retVal;

}  /* LookupImportElmtInImportElmtList */




/*
 *   looks for an import list that imports from "importModuleName"
 *   module in the given module.
 *
 *   returns a ptr to the ImportList if found
 *   returns NULL if not found
 */
ImportModule*
LookupImportModule PARAMS ((m, importModuleName),
    Module *m _AND_
    char *importModuleName)
{
    ImportModule *importModule;
    ImportModule *retVal;
    void *tmp;

    if (m->imports == NULL)
        return NULL;

    tmp = (void*)CURR_LIST_NODE (m->imports);
    retVal = NULL;
    FOR_EACH_LIST_ELMT (importModule, m->imports)
    {
        if (strcmp (importModule->modId->name, importModuleName) == 0)
        {
            retVal= importModule;
            break;
        }
    }

    SET_CURR_LIST_NODE (m->imports, tmp);
    return retVal;

}  /* LookupImportModule */




/*
 *  Looks for the type with name matching typeName (null terminated char*)
 *  in the given the TypeDef list
 *
 *  RETURNS: ptr to the TypeDef with the matching typeName (if any)
 *           NULL if no match was made
 */
TypeDef*
LookupType PARAMS ((typeDefList, typeName),
    TypeDefList *typeDefList _AND_
    char *typeName)
{
    TypeDef *td;
    TypeDef *retVal;
    void *tmp;

    if (typeDefList == NULL)
        return NULL;

    if (typeName == NULL)
    {
#ifdef DEBUG
        fprintf (errFileG, "LookupType: warning - failure due to NULL key\n");
#endif
        return NULL;
    }

    tmp = (void*)CURR_LIST_NODE (typeDefList); /* remember curr list spot */
    retVal = NULL;
    FOR_EACH_LIST_ELMT (td, typeDefList)
    {
        if (strcmp (typeName, td->definedName) == 0)
        {
            retVal = td;
            break;
        }
    }

    SET_CURR_LIST_NODE (typeDefList,tmp); /* restore curr location */

    return retVal;

}  /* LookupType */




/*	Deepak: 04/Feb/2003
 *  Looks for the type with name matching typeName (null terminated char*)
 *  in the given NamedTypeList
 *
 *  RETURNS: ptr to the NamedType with the matching typeName (if any)
 *           NULL if no match was made
 */
NamedType*
LookupObjectClassFieldType PARAMS ((namedTypeList, typeName),
    NamedTypeList *namedTypeList _AND_
    char *typeName)
{
    NamedType *nt;
    NamedType *retVal;
    void *tmp;

    if (namedTypeList == NULL)
        return NULL;

    if (typeName == NULL)
    {
#ifdef DEBUG
        fprintf (errFileG, "LookupObjectClassFieldType: warning - failure due to NULL key\n");
#endif
        return NULL;
    }

    tmp = (void*)CURR_LIST_NODE (namedTypeList); /* remember curr list spot */
    retVal = NULL;
    FOR_EACH_LIST_ELMT (nt, namedTypeList)
    {
        if (strcmp (typeName, nt->fieldName) == 0)
        {
            retVal = nt;
            break;
        }
    }

    SET_CURR_LIST_NODE (namedTypeList,tmp); /* restore curr location */

    return retVal;

}  /* LookupObjectClassFieldType */




/*	Deepak: 05/Mar/2003
 *  Looks for the type with name matching typeName according to With Syntax (null terminated char*)
 *  in the given NamedTypeList
 *
 *  RETURNS: ptr to the WithSyntax with the matching typeName (if any)
 *           NULL if no match was made
 */
WithSyntax*
LookupObjectClassFieldTypeWithSyntax PARAMS ((withSyntaxList, typeName, bError),
    WithSyntaxList *withSyntaxList _AND_
    char *typeName _AND_
	char* bError)
{
    WithSyntax *ws;
    WithSyntax *retVal;
    void *tmp;

    if (withSyntaxList == NULL)
        return NULL;

    if (typeName == NULL)
    {
#ifdef DEBUG
        fprintf (errFileG, "LookupObjectClassFieldType: warning - failure due to NULL key\n");
#endif
        return NULL;
    }

    tmp = (void*)CURR_LIST_NODE (withSyntaxList); /* remember curr list spot */
    retVal = NULL;
    FOR_EACH_LIST_ELMT (ws, withSyntaxList)
    {
        if (strcmp (typeName, ws->definedName) == 0)
        {
            retVal = ws;
			*bError = FALSE;
            break;
        }
        else if (strcmp (typeName, ws->typeName) == 0)
        {
            retVal = ws;
			*bError = TRUE;
            break;
        }
    }

    SET_CURR_LIST_NODE (withSyntaxList,tmp); /* restore curr location */

    return retVal;

}  /* LookupObjectClassFieldTypeWithSyntax */


/*	Deepak: 05/Mar/2003
 *  Looks for the object with name matching objectName (null terminated char*)
 *  in the given objAssignmentList
 *
 *  RETURNS: ptr to the ObjectAssignment with the matching objectName (if any)
 *           NULL if no match was made
 */
ObjectAssignment*
LookupObjectClassObjectAssignment PARAMS ((objAssignmentList, objName),
    ObjectAssignmentList *objAssignmentList _AND_
    char *objName)
{
    ObjectAssignment *oa;
    ObjectAssignment *retVal;
    void *tmp;

    if (objAssignmentList == NULL)
        return NULL;

    if (objName == NULL)
    {
#ifdef DEBUG
        fprintf (errFileG, "LookupObjectClassFieldType: warning - failure due to NULL key\n");
#endif
        return NULL;
    }

    tmp = (void*)CURR_LIST_NODE (objAssignmentList); /* remember curr list spot */
    retVal = NULL;
    FOR_EACH_LIST_ELMT (oa, objAssignmentList)
    {
        if (strcmp (objName, oa->objectName) == 0)
        {
            retVal = oa;
            break;
        }
    }

    SET_CURR_LIST_NODE (objAssignmentList,tmp); /* restore curr location */

    return retVal;

}  /* LookupObjectClassObjectAssignment */


/*	Deepak: 11/Mar/2003
 *  Looks for the ObjectSet with name matching objectName (null terminated char*)
 *  in the given objSetAssignmentList
 *
 *  RETURNS: ptr to the ObjectSetAssignment with the matching objectName (if any)
 *           NULL if no match was made
 */
ObjectSetAssignment*
LookupObjectClassObjectSetAssignment PARAMS ((objSetAssignmentList, objSetName),
    ObjectSetAssignmentList *objSetAssignmentList _AND_
    char *objSetName)
{
    ObjectSetAssignment *osa;
    ObjectSetAssignment *retVal;
    void *tmp;

    if (objSetAssignmentList == NULL)
        return NULL;

    if (objSetName == NULL)
    {
#ifdef DEBUG
        fprintf (errFileG, "LookupObjectClassFieldType: warning - failure due to NULL key\n");
#endif
        return NULL;
    }

    tmp = (void*)CURR_LIST_NODE (objSetAssignmentList); /* remember curr list spot */
    retVal = NULL;
    FOR_EACH_LIST_ELMT (osa, objSetAssignmentList)
    {
        if (strcmp (objSetName, osa->objectSetName) == 0)
        {
            retVal = osa;
            break;
        }
    }

    SET_CURR_LIST_NODE (objSetAssignmentList,tmp); /* restore curr location */

    return retVal;

}  /* LookupObjectClassObjectSetAssignment */



/*
 * Returns ptr to module that has matching name or OID
 * if oid is not null, lookup done only by oid
 *
 * returns NULL if no match was found
 */
Module*
LookupModule PARAMS ((moduleList, modName, oid),
    ModuleList *moduleList _AND_
    char *modName _AND_
    OID *oid)
{
    Module *currMod;
    Module *retVal;
    void *tmp;

    if ((moduleList == NULL) || ((modName == NULL) && (oid == NULL)))
        return NULL;

    tmp = (void*)CURR_LIST_NODE (moduleList); /* remember orig loc */
    retVal = NULL;
    FOR_EACH_LIST_ELMT (currMod, moduleList)
    {

        /*
         *  may fail due to unresolved int or oid value ref
         *  so try name match anyway.
         *  This is not standard (CCITT) if the oids were resolved
         *  but different, in which case the match should
         *  fail regardless of the name match. oh well, ts.
         */
        if (CompareOids (oid, currMod->modId->oid))
        {
            retVal = currMod;
            break; /* exit for loop */
        }

        else if ((modName != NULL) &&
            (strcmp (modName, currMod->modId->name) == 0))
        {
            retVal = currMod;
            break; /* exit for loop */
        }
    }

    SET_CURR_LIST_NODE (moduleList, tmp);
    return retVal;

}  /* LookupModule */




/*
 * Given a constructed type, it returns the component of that
 * type with the matching field name.  Returns NULL if teh
 * given type does not have the named field or is not
 * a type that has fields.
 */
NamedType*
LookupFieldInType PARAMS ((tRef, fieldName),
    Type *tRef _AND_
    char *fieldName)
{
    NamedType *e;
    NamedType *retVal;
    Type *t;
    void *tmp;

    t = ParanoidGetType (tRef); /* skip any references etc */

    if ((t->basicType->choiceId != BASICTYPE_SET) &&
        (t->basicType->choiceId != BASICTYPE_SEQUENCE) &&
        (t->basicType->choiceId != BASICTYPE_CHOICE))
    {
#ifdef DEBUG
        fprintf (errFileG, "LookupFieldInType: ERROR - attempt to look for field in a non SET/SEQ/CHOICE type\n");
#endif
        return NULL;
    }

    /* return if null list */
    if (t->basicType->a.set == NULL)
        return NULL;

    /* remember set's original curr elmt */
    tmp = (void*)CURR_LIST_NODE (t->basicType->a.set);
    retVal = NULL;
    FOR_EACH_LIST_ELMT (e, t->basicType->a.set)
    {
        /* remember fieldname is optional so it can be null */
        if ((e->fieldName != NULL) && (strcmp (e->fieldName, fieldName) == 0))
        {
            retVal = e;
            break; /* exit  for loop */
        }
    }
    SET_CURR_LIST_NODE (t->basicType->a.set, tmp);
    return retVal;

} /* LookupFieldInType */



/*
 *  Goes through typerefs (if any) to get to actual
 *  ASN1 type. Returns the found "defining" type.
 *  May return the given type t, if it's not a typeref
 *  or if it is an unlinked type ref
 */
Type*
GetType PARAMS ((type),
    Type *type)
{
    TypeDef *td;
    Type *t;

    t = type;
    if (t == NULL)
        return NULL;

    while (1)
    {
        switch (t->basicType->choiceId)
        {
            case BASICTYPE_LOCALTYPEREF:
            case BASICTYPE_IMPORTTYPEREF:
                td = t->basicType->a.localTypeRef->link;
                if (td == NULL)
                    return type;
                else
                    t = td->type;
                break;

            default:
                return t;
        }
    }
}  /* GetType */


/*
 * like GetType ie, skips type references to return the defining type.
 * This is a paranoid version - it checks for circular type errors.
 *  eg: A ::= B
 *      B ::= A
 * would make the normal GetType recurse forever (until no stk mem)
 */
Type*
ParanoidGetType PARAMS ((type),
    Type *type)
{
    TypeDef *td;
    Type *t;
    DefinedObj *l;

    t = type;
    if (t == NULL)
        return NULL;

    l = NewObjList();
    while (1)
    {
        switch (t->basicType->choiceId)
        {
            case BASICTYPE_LOCALTYPEREF:
            case BASICTYPE_IMPORTTYPEREF:
                td = t->basicType->a.localTypeRef->link;
                if ((td == NULL) || (ObjIsDefined (l, td->type, ObjPtrCmp)))
                {
                    return type;
                }
                else
                {
                    t = td->type;
                    DefineObj (&l, t);
                }
                break;

            default:
                FreeDefinedObjs (&l);
                return t;
        }
    }

}  /* ParnoidGetType */


/*
 *  Goes through typerefs (if any) to get to actual
 *  ASN1 basic type (eg int, bool, seq, seq of, set,
 *  set of, choice, any, etc.
 *  Returns the typeId of that type, otherwise -1.
 */
enum BasicTypeChoiceId
GetBuiltinType PARAMS ((t),
    Type *t)
{
    Type *definingType;

    definingType = GetType (t);
    if (definingType != NULL)
        return definingType->basicType->choiceId;
    else
        return -1;

}  /* GetBuiltinType */


/*  Paranoid version of GetBuiltinType
 *  goes through typerefs (if any) to get to actual
 *  ASN1 basic type (eg int, bool, seq, seq of, set,
 *  set of, choice, any, etc.
 *  Returns the typeId of that type, otherwise -1.
 */
enum BasicTypeChoiceId
ParanoidGetBuiltinType PARAMS ((t),
    Type *t)
{
    Type *definingType;

    definingType = ParanoidGetType (t);
    if (definingType != NULL)
        return definingType->basicType->choiceId;
    else
        return -1;

}  /* GetBuiltinType */



/*
 * Goes through typerefs (if any) to get to
 * the namedElmts (if any) associated with the
 * given type (INTEGER, ENUMERATED, BITSTRING or
 * LOCAL/IMPORT REFS to these types).
 * Returns NULL if there are no associated Named Elmts
 */
NamedNumberList*
GetNamedElmts PARAMS ((t),
    Type *t)
{
    Type *definingType;

    if (t == NULL)
        return NULL;

    definingType = ParanoidGetType (t);

    if (definingType == NULL)
        return NULL;

    switch (definingType->basicType->choiceId)
    {
        case BASICTYPE_INTEGER:
        case BASICTYPE_ENUMERATED:
        case BASICTYPE_BITSTRING:
            return definingType->basicType->a.integer;

        /*
         * for non-named elmt types
         * just return NULL
         */
        default:
            return NULL;
    }

    /* not reached */
}  /* GetNamedElmts */


/*
 * [Same as GetNamedElmts except goes through CHOICEs as well &
 *  REQUIRES you to deallocate the list (but not its members).]
 *  This is nec. for CHOICEs that contain INTs etc. with named #'s]
 *  This is used for value linking.
 *
 * Goes through typerefs (if any) to get to
 * the namedElmts (if any) associated with the
 * given type (INTEGER, ENUMERATED, BITSTRING or
 * LOCAL/IMPORT REFS to these types).  Also returns
 * a named element list for CHOICE types that contain
 * named elemnts
 * Returns an empty list if there are no associated Named Elmts.
 * you are responsible  for freeing this list.  Do not free the list
 * elmts - they are part of the types.
 */
NamedNumberList*
GetAllNamedElmts PARAMS ((t),
    Type *t)
{
    Type *definingType;
    NamedType *nt;
    NamedNumberList *retVal;
    NamedNumberList *ntElmtList;
    ValueDef *nn;  /* named number is a valuedef */
    ValueDef **nnHndl;

    retVal = AsnListNew (sizeof (void*));

    if (t == NULL)
        return retVal;

    definingType = ParanoidGetType (t);

    if (definingType == NULL)
        return retVal;


    switch (definingType->basicType->choiceId)
    {
        case BASICTYPE_INTEGER:
        case BASICTYPE_ENUMERATED:
        case BASICTYPE_BITSTRING:
            /*
             * add the named elmts (if any) to the new list
             */
            FOR_EACH_LIST_ELMT (nn, definingType->basicType->a.integer)
            {
                nnHndl = (ValueDef**)AsnListAppend (retVal);
                *nnHndl = nn;
            }
            break;

        /*
         * for choices must group all named elmts from choice components
         * and return in a list.
         */
        case BASICTYPE_CHOICE:
            FOR_EACH_LIST_ELMT (nt, definingType->basicType->a.choice)
            {
                ntElmtList = GetAllNamedElmts (nt->type);
                retVal = AsnListConcat (retVal, ntElmtList);
                Free (ntElmtList); /* zap now unused list head */
            }
            break;
    default:
      break;
    }
    return retVal;
}  /* GetAllNamedElmts */


/*
 * Recursively does pseudo breadth first search from the given ancestor
 * looking for the given child node.  Returns the direct parent Type
 * of the child if found, NULL otherwise.  This routine does not follow
 * type references.
 */
Type*
GetParentS PARAMS ((ancestor, child),
    Type *ancestor _AND_
    Type *child)
{
    NamedType *e;
    Type *parent;
    void *tmp;

    if ((ancestor->basicType->choiceId != BASICTYPE_SET) &&
        (ancestor->basicType->choiceId != BASICTYPE_SEQUENCE) &&
        (ancestor->basicType->choiceId != BASICTYPE_CHOICE) &&
        (ancestor->basicType->choiceId != BASICTYPE_SETOF) &&
        (ancestor->basicType->choiceId != BASICTYPE_SEQUENCEOF) &&
        (ancestor->basicType->choiceId != BASICTYPE_OCTETCONTAINING) &&
        (ancestor->basicType->choiceId != BASICTYPE_BITCONTAINING))
    {
        return NULL;
    }

    if (ancestor->basicType->a.set == NULL)
        return NULL;

    if ((ancestor->basicType->choiceId == BASICTYPE_SETOF) ||
        (ancestor->basicType->choiceId == BASICTYPE_SEQUENCEOF))
    {
        if (child == ancestor->basicType->a.setOf)
            return ancestor;
        else
            return GetParentS (ancestor->basicType->a.setOf, child);
    }

    tmp = (void*)CURR_LIST_NODE (ancestor->basicType->a.set);
    /*
     * look through direct children of ancestor first
     */
    FOR_EACH_LIST_ELMT (e,  ancestor->basicType->a.set)
    {
        if ( ((e->type->basicType->choiceId == BASICTYPE_OCTETCONTAINING) ||
              (e->type->basicType->choiceId == BASICTYPE_BITCONTAINING)) &&
             (child == e->type->basicType->a.stringContaining) )
		  {
            SET_CURR_LIST_NODE (ancestor->basicType->a.set, tmp);
            return ancestor;
		  }
		  else if (child == e->type)
        {
            SET_CURR_LIST_NODE (ancestor->basicType->a.set, tmp);
            return ancestor;
        }
    }


    /*
     * look through grandchildren if not in children
     */
    FOR_EACH_LIST_ELMT (e,  ancestor->basicType->a.set)
    {
        if ((parent = GetParentS (e->type, child)) != NULL)
        {
            SET_CURR_LIST_NODE (ancestor->basicType->a.set, tmp);
            return parent;
        }
    }

    SET_CURR_LIST_NODE (ancestor->basicType->a.set, tmp);
    return NULL;
}  /* GetParent */



/*
 *  Looks for the value with the given valueName (null term char*) in the
 *  given list of ValueDefs
 *  RETURNS: ptr to ValueDef with matching key (if any)
 *           NULL if no match was made
 */

ValueDef*
LookupValue PARAMS ((valueList, valueName),
    ValueDefList *valueList _AND_
    char *valueName)
{
    ValueDef *v;
    ValueDef *retVal;
    void *tmp;

    if (valueName == NULL)
    {
#ifdef DEBUG
        fprintf (errFileG, "LookupType: warning - failure due to NULL key\n");
#endif
        return NULL;
    }

    if (valueList == NULL)
        return NULL;

    tmp = (void*)CURR_LIST_NODE (valueList);
    retVal = NULL;
    FOR_EACH_LIST_ELMT (v, valueList)
    {
        if (strcmp (valueName, v->definedName) == 0)
        {
            retVal = v;
            break; /* exit for loop */
        }
    }

    SET_CURR_LIST_NODE (valueList, tmp);
    return retVal;

}  /* LookupValue */

BasicValue *
GetLastNamedNumberValue PARAMS ((valueList),
    NamedNumberList *valueList)
{
    ValueDef *v;
    BasicValue *retVal;
    void *tmp;

    if (valueList == NULL)
        return NULL;

    tmp = (void*)CURR_LIST_NODE (valueList);
    
    retVal = NULL;

    FOR_EACH_LIST_ELMT_RVS (v, valueList)
    {
        if (v->value->basicValue->choiceId == BASICVALUE_INTEGER)
        {
            retVal = v->value->basicValue;
            break; /* exit for loop */
        }
    }

    SET_CURR_LIST_NODE (valueList, tmp);
    return retVal;
}  /* LookupValue */



/*
 *  Goes through valuerefs (if any) to get to actual
 *  ASN1 value.  Analogous to GetType.
 */
Value*
GetValue PARAMS ((v),
    Value *v)
{
    ValueDef *vd;

    while (v != NULL)
    {
        switch (v->basicValue->choiceId)
        {
            case BASICVALUE_LOCALVALUEREF:
            case BASICVALUE_IMPORTVALUEREF:
                vd = v->basicValue->a.localValueRef->link;
                if (vd == NULL)
                    v = NULL;
                else
                    v = vd->value;
                break;

            default:
                return v;
        }
    }
    fprintf (errFileG, "GetValue: ERROR - cannot get value for unlinked local/import value refs\n");
    return NULL;

}  /* GetValue */


/*
 * Returns TRUE if oid1 and oid2 are identical otherwise FALSE
 */
int
CompareOids PARAMS ((oid1, oid2),
    OID *oid1 _AND_
    OID *oid2)
{
    if ((oid1 == NULL) && (oid2 == NULL))
        return FALSE;

    for (; (oid1 != NULL) && (oid2 != NULL); oid1 = oid1->next, oid2 = oid2->next)
    {
        /*
         *  fail if value refs have not been resolved or
         *   no match between arcnums
         */
        if ((oid1->arcNum == NULL_OID_ARCNUM) ||
             (oid2->arcNum == NULL_OID_ARCNUM) ||
             (oid1->arcNum != oid2->arcNum))
            return FALSE;

        /*
         * could check ref'd values for same name
         * incase value ref has not been resolved
         * and put in arcNum
         */
    }

    if ((oid1 == NULL) && (oid2 == NULL))
        return TRUE;
    else
        return FALSE;

}  /*  CompareOids */


/*
 *  Returns TRUE if the given type is INTEGER, ENUMERATED or
 *  BIT STRING and it has named elements
 *  ie Foo ::= INTEGER { one (1), two (2) } would return TRUE
 */
int
HasNamedElmts PARAMS ((t),
    Type *t)
{
    return ((t->basicType->choiceId == BASICTYPE_INTEGER) ||
             (t->basicType->choiceId == BASICTYPE_ENUMERATED) ||
             (t->basicType->choiceId == BASICTYPE_BITSTRING)) &&
           (t->basicType->a.integer != NULL) &&
           !LIST_EMPTY (t->basicType->a.integer);
} /* HasNamedElmts */


/*
 * Returns true if the given tag lists are the same
 * (assumes value refs have be resolved)
 */
int
TagsAreIdentical PARAMS ((t1, t2),
    TagList *t1 _AND_
    TagList *t2)
{
    Tag *tag1;
    Tag *tag2;

    /* both lists are empty */
    if (((t1 == NULL) || LIST_EMPTY (t1)) &&
        ((t2 == NULL) || LIST_EMPTY (t2)))
        return TRUE;

    else if ((t1 == NULL) || (t2 == NULL))
        return FALSE;

    else if (LIST_COUNT (t1) == LIST_COUNT (t2))
    {
        SET_CURR_LIST_NODE (t2, FIRST_LIST_NODE (t2));
        FOR_EACH_LIST_ELMT (tag1, t1)
        {
            tag2 = (Tag*) CURR_LIST_ELMT (t2);
            if ((tag1->tclass != tag2->tclass) || (tag1->code == tag2->code))
                return FALSE;
            SET_CURR_LIST_NODE (t2, NEXT_LIST_NODE (t2));
        }
        return TRUE;
    }
    else
        return FALSE;

} /* TagsAreIdentical */



/*
 * Returns TRUE if the tag currently on the given type has the default
 * tag specified in the type tbl. otherwise returns FALSE.
 */
int
HasDefaultTag PARAMS ((t),
    Type *t)
{
    Tag *firstTag = NULL;
    int dfltCode;
    int dfltClass;

    dfltClass = UNIV;
    dfltCode = LIBTYPE_GET_UNIV_TAG_CODE (t->basicType->choiceId);
    if ((t->tags != NULL) && !LIST_EMPTY (t->tags))
        firstTag = (Tag*)FIRST_LIST_ELMT (t->tags);

    return ((firstTag != NULL) && (LIST_COUNT (t->tags) == 1) &&
             (firstTag->tclass == dfltClass) && (firstTag->code == dfltCode)) ||
            ((firstTag == NULL) && (dfltCode == NO_TAG_CODE));

} /* HasDefaultTag */


/*
 * Returns TRUE if t is a primitive type or if it is
 * defined by a reference to a primitive type
 */
int
IsPrimitiveByDefOrRef PARAMS ((t),
    Type *t)
{
    Type *definingType;

    definingType = GetType (t);

    if (definingType == NULL)
        return FALSE; /* bad error handling */

    return IsPrimitiveByDef (definingType);
}  /* IsPrimitiveByDefOrRef */


/*
 * Returns TRUE if the given type is a primitive type. Does NOT
 * follow type references - type refs are not considered primitive.
 * The following types are considered primitive:
 *  BOOLEAN
 *  INTEGER/BIG_INT
 *  BITSTRING
 *  OCTETSTRING
 *  NULL
 *  OID
 *  REAL
 *  ENUMERATED
 *	NumericString, PrintableString, IA5String, BMPString, 
 *	UniversalString, UTF8String, and TeletexString (T61String)
 */
int
IsPrimitiveByDef PARAMS ((t),
    Type *t)
{
    switch (t->basicType->choiceId)
    {
        case BASICTYPE_LOCALTYPEREF:
        case BASICTYPE_IMPORTTYPEREF:
		case BASICTYPE_SEQUENCET:	// Deepak: 29/Nov/2002 
		case BASICTYPE_OBJECTCLASS:	// Deepak: 05/Feb/2003
        case BASICTYPE_OBJECTCLASSFIELDTYPE:	// Deepak: 05/Feb/2003
        case BASICTYPE_SEQUENCE:
        case BASICTYPE_SET:
        case BASICTYPE_CHOICE:
        case BASICTYPE_SEQUENCEOF:
        case BASICTYPE_SETOF:
        case BASICTYPE_COMPONENTSOF:
        case BASICTYPE_ANYDEFINEDBY:
        case BASICTYPE_ANY:
            return FALSE;
            break;


        case BASICTYPE_SELECTION:
            if (t->basicType->a.selection->link != NULL)
            return IsPrimitiveByDef (t->basicType->a.selection->link->type);
                   break;

        case BASICTYPE_BOOLEAN:
        case BASICTYPE_INTEGER:
        case BASICTYPE_BITSTRING:
        case BASICTYPE_OCTETSTRING:
        case BASICTYPE_NULL:
        case BASICTYPE_OID:
        case BASICTYPE_RELATIVE_OID:
        case BASICTYPE_REAL:
        case BASICTYPE_ENUMERATED:
		  case BASICTYPE_NUMERIC_STR:
		  case BASICTYPE_PRINTABLE_STR:
		  case BASICTYPE_IA5_STR:
		  case BASICTYPE_BMP_STR:
		  case BASICTYPE_UNIVERSAL_STR:
		  case BASICTYPE_UTF8_STR:
		  case BASICTYPE_T61_STR:
        case BASICTYPE_GENERALIZEDTIME:
        case BASICTYPE_UTCTIME:
        case BASICTYPE_OBJECTDESCRIPTOR:
        case BASICTYPE_VISIBLE_STR:
        case BASICTYPE_GRAPHIC_STR:
        case BASICTYPE_VIDEOTEX_STR:
        case BASICTYPE_GENERAL_STR:

            return TRUE;
            break;

        case BASICTYPE_UNKNOWN:
        case BASICTYPE_MACROTYPE:
        case BASICTYPE_MACRODEF:
        case BASICTYPE_EXTERNAL:
		    return FALSE;
            break;

        default:
            fprintf (errFileG, "IsPrimitiveByDef: ERROR - unknown type id ?!");
    }
    return FALSE;
}  /* IsPrimitiveByDef */


/*
 * Returns TRUE if the given type is a local type reference or an
 * import type ref.
 * e.g.
 *
 * Gumby ::= P1.ORName   --> isTypeRef returns TRUE P1.ORName
 * Bar ::= INTEGER       --> isTypeRef returns FALSE for INTEGER
 * Foo ::= Bar           --> isTypeRef returns TRUE for Bar
 */
int
IsTypeRef PARAMS ((t),
    Type *t)
{
    if ((t->basicType->choiceId == BASICTYPE_LOCALTYPEREF) ||
        (t->basicType->choiceId == BASICTYPE_IMPORTTYPEREF))
        return TRUE;
    else
        return FALSE;
}  /* IsTypeRef */



/*
 * Returns TRUE if the given type is defined
 * by a library type such as OCTET STRING.
 * Does NOT follow type refs - type refs return FALSE.
 *
 * NOTE - some possibly non-primitive types are defined by
 *        library types (ANY, ANY DEFINED BY)
 *
 * types defined by type refs or structured defs
 * cause FALSE to be returned. i.e.
 *  Foo ::= Bar  -> FALSE for Bar
 *  Bell ::= SEQUENCE { .. } -> False for SEQ...
 *
 * useful types are considered as type references and hence
 * return FALSE.
 */
int
IsDefinedByLibraryType PARAMS ((t),
    Type *t)
{
    int retVal;

    if (t == NULL)
        retVal = FALSE;

    else if (IsPrimitiveByDef (t))
        retVal = TRUE;

    /*
     * check for non-primitive types that
     * are defined by a library type
     */
    else
        switch (t->basicType->choiceId)
        {
            case BASICTYPE_ANYDEFINEDBY:
            case BASICTYPE_ANY:
                retVal = TRUE;
            break;

           default:
                retVal = FALSE;
    }
    return retVal;

} /* IsDefinedByLibraryType*/


/*
 * Returns FALSE if type t is
 *    a. a library type with default universal tags and no named elements
 *       OR
 *    b. a reference to a type with no extra tagging
 *
 * otherwise returns true, indicating that is is a new type derived
 * by tagging or adding named elmts to another type.
 *
 * eg  INTEGER  --> FALSE   (same as lib type)
 *     [APPLICATION 2] INTEGER --> TRUE (re-tagged lib type)
 *     INTEGER { one (1), two (2) } --> TRUE (lib type with named elmts)
 *     Bar2 --> FALSE (simple type ref)
 */
int
IsNewType PARAMS ((t),
    Type *t)
{
    /*
     * Type = [re-tagging] DefiningType [namedelmts]
     * identical: no retagging and no named elements
     */
    if (IsDefinedByLibraryType (t) && HasDefaultTag (t) && ! HasNamedElmts (t))
        return FALSE;

    else if (IsTypeRef (t) && ((t->tags == NULL) || (LIST_EMPTY (t->tags))))
        return FALSE;

    else
        return TRUE;

} /* IsNewType */


/*
 * Returns TRUE if elmts including curr list elmt
 * onward are all optional otherwise returns FALSE.
 * (note: this relies on the 'curr' ptr in the list)
 * if the list is null or the curr elmt is null
 * then returns TRUE
 */
int
IsTailOptional PARAMS ((e),
    NamedTypeList *e)
{
    NamedType *elmt;
    void *tmp;
    int retVal;

    if (e == NULL)
        return TRUE;

    tmp = (void*)CURR_LIST_NODE (e);
    if (tmp == NULL)
        return TRUE;

    retVal = TRUE;
    FOR_REST_LIST_ELMT (elmt, e) {
        if ((!elmt->type->optional) && (elmt->type->defaultVal == NULL) &&
            (!elmt->type->extensionAddition)) {
            retVal = FALSE;
            break;
        }
    }
    SET_CURR_LIST_NODE (e, tmp); /* reset list to orig loc */
    return retVal;
} /* IsTailOptional */



/*
 * Returns TRUE if all elmts after but not including the curr list elmt
 * are optional otherwise returns FALSE.
 * (note: this relies on the 'curr' ptr in the list)
 * if the list is null or the curr elmt is null
 * then returns TRUE. if there are no elmts after the curr elmt
 * returns TRUE.
 */
int
NextIsTailOptional PARAMS ((e),
    NamedTypeList *e)
{
    NamedType *elmt;
    void *tmp;
    void *tmp2;
    int retVal;

    if ((e == NULL) || (LIST_EMPTY (e)))
        return TRUE;

    tmp = (void*)CURR_LIST_NODE (e);
    if (tmp == NULL)
        return TRUE;

    tmp2 = (void*)NEXT_LIST_NODE (e);
    if (tmp2 == NULL)
        return TRUE;

    SET_CURR_LIST_NODE (e, tmp2);

    retVal = TRUE;
    FOR_REST_LIST_ELMT (elmt, e) {
        if ((!elmt->type->optional) && (elmt->type->defaultVal == NULL) &&
            (!elmt->type->extensionAddition)) {
            retVal = FALSE;
            break;
        }
    }
    SET_CURR_LIST_NODE (e, tmp); /* reset list to orig loc */
    return retVal;
} /* NextIsTailOptional */


/*
 * Returns TRUE if all elmts of the curr list are optional
 * or have default values.  Useful with SET and SEQ elements.
 */
int
AllElmtsOptional PARAMS ((e),
    NamedTypeList *e)
{
    NamedType *elmt;
    void *tmp;
    int retVal;

    if ((e == NULL) || LIST_EMPTY (e))
        return TRUE;

    tmp = (void*)CURR_LIST_NODE (e);
    SET_CURR_LIST_NODE (e, FIRST_LIST_NODE (e));

    retVal = TRUE;
    FOR_REST_LIST_ELMT (elmt, e) {
        if ((!elmt->type->optional) && (elmt->type->defaultVal == NULL) &&
            (!elmt->type->extensionAddition)) {
            retVal = FALSE;
            break;
        }
    }
    SET_CURR_LIST_NODE (e, tmp); /* reset list to orig loc */
    return retVal;
} /* AllElmtsOptional */





/*
 * Follows single levely of type ref or library type and returns a
 * handle to its AnyRefList.  Typically used in do_macros.c to
 * add a hash key for the type that t is or refs.  Need to get
 * to the type def of type t to give the AnyRefListHndl.
 */
AnyRefList**
GetAnyRefListHndl PARAMS ((t),
    Type *t)
{
    TypeDef *td;

    if (IsDefinedByLibraryType (t))
        return LIBTYPE_GET_ANY_REFS_HNDL (t->basicType->choiceId);
    else
    {
        if (!IsTypeRef (t))
            return NULL;
        else
        {
            td = t->basicType->a.localTypeRef->link;
            return &td->anyRefs;
        }
    }
} /* GetAnyRefListHndl */


/*
 * Given a subtype list s (possibly empty *s == NULL) it tacks on
 * the newSubtype in a appropriate fashion, possible chaning *s.
 * Op can be SUBTYPE_AND or SUBTYPE_OR.
 *
 * e.g. Foo ::= INTEGER ((1..100) | 200)
 *
 * Add the subtypes by
 *   AppendSubtype (&t->subtypes, (1..100), SUBTYPE_AND)
 *   AppendSubtype (&t->subtypes, 200, SUBTYPE_OR)
 *
 * op is meaningless if s is empty
 */
void
AppendSubtype PARAMS ((s, newSubtype, op),
    Subtype **s _AND_
    Subtype *newSubtype _AND_
    enum SubtypeChoiceId op)
{
    void **tmpPtr;
    Subtype *sPtr;

    if (*s == NULL)
        *s = newSubtype;

    else if (op == SUBTYPE_AND)
    {
        if ((*s)->choiceId == SUBTYPE_AND)
        {
            tmpPtr = (void**)AsnListAppend ((*s)->a.and);
            *tmpPtr = (void*)newSubtype;
        }
        else
        {
            sPtr = (Subtype*)Malloc (sizeof (Subtype));
            sPtr->choiceId  = SUBTYPE_AND;
            sPtr->a.and = NEWLIST();
            tmpPtr = (void**)AsnListAppend (sPtr->a.and);
            *tmpPtr = (void*)*s;
            tmpPtr = (void**)AsnListAppend (sPtr->a.and);
            *tmpPtr = (void*)newSubtype;
            *s = sPtr;
        }
    }
    else if (op == SUBTYPE_OR)
    {
        if ((*s)->choiceId == SUBTYPE_OR)
        {
            tmpPtr = (void**)AsnListAppend ((*s)->a.or);
            *tmpPtr = (void*)newSubtype;
        }
        else
        {
            sPtr = (Subtype*)Malloc (sizeof (Subtype));
            sPtr->choiceId  = SUBTYPE_OR;
            sPtr->a.or = NEWLIST();
            tmpPtr = (void**)AsnListAppend (sPtr->a.or);
            *tmpPtr = (void*)*s;
            tmpPtr = (void**)AsnListAppend (sPtr->a.or);
            *tmpPtr = (void*)newSubtype;
            *s = sPtr;
        }
    }
    else
        /* NOT not supported here */
        fprintf (errFileG, "AppendSubtype - unknown operation\n");

}  /* AppendSubtype */

static int
TagCodeFound(AsnList *tags, AsnInt startingTag)
{
    Tag *tag;
    FOR_EACH_LIST_ELMT(tag, tags) {
        if (tag->code == startingTag)
            return 1;
    }
    return 0;
}

void
AutomaticTagNamed PARAMS ((l, all),
    NamedTypeList *l)
{
    AsnList *used_tag;
    NamedType *e;
    void *lcurr;
    AsnInt startingTag = 0;

    if (l == NULL) {
        fprintf(errFileG, "AutomaticTagNamed - NULL tag list.\n");
        return;
    }

    /* Two passes */
    used_tag = AsnListNew(sizeof(void*));
    /* First pass, identify all the tags, and all the elements which
       need tags */
    lcurr = l->curr;
    FOR_EACH_LIST_ELMT(e, l) {
        if (e->type->tags != NULL) {
            Tag *t;
            void *curr = e->type->tags->curr;
            FOR_EACH_LIST_ELMT(t, e->type->tags) {
                if (t->tclass == CNTX) {
                    APPEND(t, used_tag);
                    break;
                }
            }
            e->type->tags->curr = curr;
        }
    }
    FOR_EACH_LIST_ELMT(e, l) {
        int found = 0;
        Tag *t;
        FOR_EACH_LIST_ELMT(t, e->type->tags) {
            if (t->tclass == CNTX) {
                fprintf(errFileG, "CNTX tag exists\n");
                found = 1;
            }
        }
        if (!found) {
            Tag *newTag = Malloc(sizeof(Tag));
            newTag->tclass = CNTX;
            newTag->form = PRIM;
            newTag->valueRef = NULL;
            /* It seems strange to only use context tagging; however, after
             * some brief interop testing, that seems like the right thing to
             * do */
            if (!(e->type->tags))
                AsnListFree(e->type->tags);
            e->type->tags = AsnListNew(sizeof(void *));
            while(TagCodeFound(used_tag, startingTag))
                startingTag++;
            newTag->code = startingTag++;
            newTag->explicit = FALSE;
            PREPEND(newTag, e->type->tags);
        }
    }
    l->curr = lcurr;

    AsnListFree(used_tag);
}
