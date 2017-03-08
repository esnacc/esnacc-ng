/*
 * compiler/back-ends/py-gen/types.c  - fills in python type information
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
#include <ctype.h>
#include <string.h>
#include "asn-incl.h"
#include "asn1module.h"
#include "snacc-util.h"
#include "str-util.h"
#include "rules.h"

static DefinedObj *definedNamesG;

/* unexported prototypes */

void FillPyTypeDefInfo PROTO ((PyRules *r, Module *m, TypeDef *td));
static void FillPyFieldNames PROTO ((PyRules *r,
                                      NamedTypeList *firstSibling));
static void FillPyTypeRefInfo PROTO ((PyRules *r, Module *m,
                                       TypeDef *head, Type *parent, Type *t));
static void FillPyStructElmts PROTO ((PyRules *r, Module *m,
                                       TypeDef *head, Type *parent,
                                       NamedTypeList *t));
static void FillPyChoiceElmts PROTO ((PyRules *r, Module *m, TypeDef *head,
                                       Type *parent, NamedTypeList *first));
static int IsPyNone PROTO ((PyRules *r, TypeDef *td, Type *parent, Type *t));
void FillPyTDIDefaults PROTO ((PyRules *r, CxxTDI *ctdi, TypeDef *td));


static char *pykwds[] = {
    "False", 	"class", 	"finally", 	"is", 	"return",
    "None", 	"continue", 	"for", 	"lambda", 	"try",
    "True", 	"def", 	"from", 	"nonlocal", 	"while",
    "and", 	"del", 	"global", 	"not", 	"with",
    "as", 	"elif", 	"if", 	"or", 	"yield",
    "assert", 	"else", 	"import", 	"pass",
    "break", 	"except", 	"in", 	"raise", NULL
};


/*
 * returns non-zero if the given str is a C++ key word
 */
int
IsPyKeyWord PARAMS ((str),
    char *str)
{
    size_t i;

    for (i=0; pykwds[i] != NULL && strcmp(pykwds[i], str); i++);

    return pykwds[i] != NULL;
}


/*
 *  allocates and fills all the cxxTypeInfos
 *  in the type trees for every module in the list
 */
void
FillPyTypeInfo PARAMS ((r, modList),
    PyRules *r _AND_
    ModuleList *modList)
{
    TypeDef *td;
    Module *m;

    /*
     * go through each module's type defs and fill
     * in the C type and enc/dec routines etc
     */
    definedNamesG = NULL;

    FOR_EACH_LIST_ELMT (m, modList) {
        FOR_EACH_LIST_ELMT (td, m->typeDefs)
            FillPyTypeDefInfo (r, m, td);
    }

    /*
     * now that type def info is filled in
     * set up set/seq/list/choice elements that ref
     * those definitions
     */
    FOR_EACH_LIST_ELMT (m, modList) {
        FOR_EACH_LIST_ELMT (td, m->typeDefs)
            FillPyTypeRefInfo (r, m, td, NULL, td->type);
    }

    /* done with checking for name conflicts */
    FreeDefinedObjs (&definedNamesG);
    definedNamesG = NULL;

}


/*
 *  allocates and fills structure holding C type definition information
 *  fo the given ASN.1 type definition.  Does not fill CTRI for contained
 *  types etc.
 */
void
FillPyTypeDefInfo PARAMS ((r, m, td),
    PyRules *r _AND_
    Module *m _AND_
    TypeDef *td)
{
    char *tmpName;
    CxxTDI *cxxtdi;

    /*
     * if CxxTDI is present this type def has already been 'filled'
     */
    if (td->cxxTypeDefInfo != NULL)
        return;


    cxxtdi = MT (CxxTDI);
    td->cxxTypeDefInfo = cxxtdi;

    /* get default type def attributes from table for type on rhs of ::= */

    FillPyTDIDefaults (r, cxxtdi, td);


    /*
     * if defined by a ref to another type definition fill in that type
     * def's CxxTDI so can inherit (actully completly replace default
     * attributes) from it
     */
    if ((td->type->basicType->choiceId == BASICTYPE_LOCALTYPEREF) ||
        (td->type->basicType->choiceId == BASICTYPE_IMPORTTYPEREF))
    {
        /*
         * Fill in CxxTDI for defining type if nec.
         * this works for importTypeRef as well since both a.localTypeRef
         * and a.importTypeRef are of type TypeRef
         */
        FillPyTypeDefInfo(r, td->type->basicType->a.localTypeRef->module,
                          td->type->basicType->a.localTypeRef->link);

        tmpName = cxxtdi->className; /* save className */
        /* copy all type def info and restore name related stuff - hack*/
        *cxxtdi = *td->type->basicType->a.localTypeRef->link->cxxTypeDefInfo;
        cxxtdi->className = tmpName; /* restore className */
    }
}  /* FillCxxTypeDefInfo */


static void
FillPyTypeRefInfo PARAMS ((r, m, head, parent, t),
    PyRules *r _AND_
    Module *m _AND_
    TypeDef *head _AND_
    Type *parent _AND_
    Type *t)
{
    CxxTRI *cxxtri;
    CxxTDI *tmpCxxtdi;
    ValueDef *namedElmt;
    CNamedElmt *cne;
    CNamedElmt **cneHndl;
    char       *elmtName;
    int         len;
    enum BasicTypeChoiceId basicTypeId;

    /*
     * you must check for cycles yourself before calling this
     */
    if (t->cxxTypeRefInfo == NULL) {
        cxxtri = MT (CxxTRI);
        t->cxxTypeRefInfo = cxxtri;
    } else
        cxxtri = t->cxxTypeRefInfo;

    basicTypeId = t->basicType->choiceId;

    tmpCxxtdi = &r->typeConvTbl[basicTypeId];

    /* get base type def info from the conversion table in the rules */
    cxxtri->isEnc = tmpCxxtdi->isEnc;
    if ((basicTypeId == BASICTYPE_OCTETCONTAINING) ||
        (basicTypeId == BASICTYPE_BITCONTAINING)) {
        cxxtri->className =
            r->typeConvTbl[t->basicType->a.stringContaining->basicType->
                           choiceId].className;
    } else
        cxxtri->className = tmpCxxtdi->className;

    cxxtri->optTestRoutineName = tmpCxxtdi->optTestRoutineName;


    /*
     * convert named elmts to C++ names.
     * check for name conflict with other defined Types/Names/Values
     */
    if (((basicTypeId == BASICTYPE_INTEGER) ||
         (basicTypeId == BASICTYPE_ENUMERATED) ||
         (basicTypeId == BASICTYPE_BITSTRING)) &&
        (!(LIST_EMPTY (t->basicType->a.integer)))) {
        cxxtri->namedElmts = AsnListNew (sizeof (void*));

        FOR_EACH_LIST_ELMT (namedElmt, t->basicType->a.integer) {
            cneHndl = (CNamedElmt**)AsnListAppend (cxxtri->namedElmts);
            cne = *cneHndl = MT(CNamedElmt);
            elmtName = Asn1ValueName2CValueName(namedElmt->definedName);
            cne->name = elmtName; /* not very efficient */

            if (namedElmt->value->basicValue->choiceId == BASICVALUE_INTEGER)
                cne->value = namedElmt->value->basicValue->a.integer;
            else {
                fprintf (errFileG, "Warning: unlinked defined value. Using -9999999\n");
                cne->value = -9999999;
            }

            if (r->capitalizeNamedElmts)
                Str2UCase(cne->name, len);

            /*
             * append digits if enum value name is a keyword
             */
            MakePyStrUnique(definedNamesG, cne->name, r->maxDigitsToAppend, 1);
        }
    }

    /* fill in rest of type info depending on the type */
    switch (basicTypeId)
    {
    default:
    case BASICTYPE_UNKNOWN:
    case BASICTYPE_MACRODEF:
    case BASICTYPE_MACROTYPE:
        /* do nothing */
    case BASICTYPE_BOOLEAN:  /* library types */
    case BASICTYPE_INTEGER:
    case BASICTYPE_BITSTRING:
    case BASICTYPE_OCTETSTRING:
    case BASICTYPE_NULL:
    case BASICTYPE_OID:
    case BASICTYPE_RELATIVE_OID:
    case BASICTYPE_REAL:
    case BASICTYPE_ENUMERATED:
    case BASICTYPE_ANY:
    case BASICTYPE_ANYDEFINEDBY:  /* ANY types */
        /* don't need to do anything else */
        break;

    case BASICTYPE_SEQUENCEOF:  /* list types */
    case BASICTYPE_SETOF:
        /* fill in component type */
        FillPyTypeRefInfo (r, m, head, t, t->basicType->a.setOf);
        break;

    case BASICTYPE_IMPORTTYPEREF:  /* type references */
    case BASICTYPE_LOCALTYPEREF:
        /*
         * grab class name from link (link is the def of the
         * the ref'd type)
         */
        if (t->basicType->a.localTypeRef->link != NULL) {
            /* inherit attributes from referenced type */
            tmpCxxtdi=  t->basicType->a.localTypeRef->link->cxxTypeDefInfo;
            cxxtri->className = tmpCxxtdi->className;
            cxxtri->isEnc = tmpCxxtdi->isEnc;
            cxxtri->optTestRoutineName = tmpCxxtdi->optTestRoutineName;
        }
        break;  /* these are handled now */

    case BASICTYPE_CHOICE:
        /*
         * must fill field names BEFORE filling choice elmts
         * (allows better naming for choice ids)
         */
        FillPyFieldNames(r, t->basicType->a.choice);
        FillPyChoiceElmts(r, m, head, t, t->basicType->a.choice);
        break;

    case BASICTYPE_SET:
    case BASICTYPE_SEQUENCE:
        FillPyStructElmts (r, m, head, t, t->basicType->a.set);
        FillPyFieldNames (r, t->basicType->a.set);
        break;

    case BASICTYPE_COMPONENTSOF:
    case BASICTYPE_SELECTION:
        fprintf(errFileG,
                "Compiler error - COMPONENTS OF or SELECTION type slipped through normalizing phase.\n");
        exit(-1);
        break;
    }

    /*
     * figure out whether this is a ptr based on the enclosing
     * type (if any) and optionality/default
     */
    cxxtri->isPtr = (unsigned char)IsPyNone(r, head, parent, t);
}  /* FillCxxTypeRefInfo */



static void
FillPyStructElmts PARAMS ((r, m, head, parent, elmts),
    PyRules *r _AND_
    Module *m _AND_
    TypeDef *head _AND_
    Type *parent _AND_
    NamedTypeList *elmts)
{
    NamedType *et;

    FOR_EACH_LIST_ELMT (et, elmts) {
        FillPyTypeRefInfo(r, m, head, parent, et->type);
    }
}

/*
 *  Figures out non-conflicting enum names for the
 *  choice id's
 */
static void
FillPyChoiceElmts PARAMS ((r, m, head, parent, elmts),
    PyRules *r _AND_
    Module *m _AND_
    TypeDef *head _AND_
    Type *parent _AND_
    NamedTypeList *elmts)
{
    NamedType *et;
    int idCount = 0;
    CxxTRI *cxxtri;
    int   len;

    /*
     * fill in type info for elmt types first
     */
    FOR_EACH_LIST_ELMT (et, elmts)
        FillPyTypeRefInfo (r, m, head, parent, et->type);

    FOR_EACH_LIST_ELMT (et, elmts) {
        cxxtri =  et->type->cxxTypeRefInfo;

        if (cxxtri == NULL)
            continue; /* wierd type */

        cxxtri->choiceIdValue = idCount++;

        len = strlen (cxxtri->fieldName);
        cxxtri->choiceIdSymbol = Malloc (len + 4);
        strcpy (cxxtri->choiceIdSymbol, cxxtri->fieldName);
        strcat (cxxtri->choiceIdSymbol, "Cid");

        if (r->capitalizeNamedElmts)
            Str2UCase (cxxtri->choiceIdSymbol, len);
    }
}

/*
 * takes a list of "sibling" (eg same level in a structure)
 * ElmtTypes and fills sets up the c field names in
 * the CxxTRI struct
 */
static void
FillPyFieldNames PARAMS ((r, elmts),
    PyRules *r _AND_
    NamedTypeList *elmts)
{
    NamedType  *et;
    CxxTRI *cxxtri;
    DefinedObj *fieldNames;
    int        len;
    char      *tmpName;
    char      *asn1FieldName;
    char      *cFieldName = NULL;

    /*
     * Initialize fieldname data
     * allocate (if nec) and fill in CTRI fieldname if poss
     * from asn1 field name.  leave blank otherwise
     */
    fieldNames = NewObjList();
    FOR_EACH_LIST_ELMT (et, elmts) {
        cxxtri = et->type->cxxTypeRefInfo;
        if (cxxtri == NULL) {
            cxxtri = MT(CxxTRI);
            et->type->cxxTypeRefInfo = cxxtri;
        }

        if (et->fieldName != NULL) {
            /*
             * can assume that the field names are
             * distinct because they have passed the
             * error checking step.
             * However, still call MakeStrUnique
             * to change any field names that
             * conflict with C++ keywords
             */
            asn1FieldName = et->fieldName;
            cxxtri->fieldName = Asn1FieldName2CFieldName(asn1FieldName);
            MakePyStrUnique(fieldNames, cxxtri->fieldName,
                            r->maxDigitsToAppend, 1);
            DefineObj(&fieldNames, cxxtri->fieldName);
        }

        /*
         * generate field names for those without them
         */
        cFieldName = cxxtri->fieldName;
        if (cxxtri->fieldName == NULL) {
            if ((et->type->basicType->choiceId == BASICTYPE_LOCALTYPEREF) ||
                (et->type->basicType->choiceId == BASICTYPE_IMPORTTYPEREF)) {
                /*
                 * take ref'd type name as field name
                 * convert first let to lower case
                 */
                tmpName = et->type->basicType->a.localTypeRef->link->
                    cxxTypeDefInfo->className;
                tmpName = Asn1TypeName2CTypeName(tmpName);
            } else {
                /*
                 * get default field name for this type
                 */
                tmpName = Strdup(r->typeConvTbl[et->type->basicType->choiceId].
                                 defaultFieldName);
            }
            cFieldName = tmpName;
            if (isupper(cFieldName[0]))
                cFieldName[0] = (char)tolower(cFieldName[0]);

            len = strlen(cFieldName);

            /*
             * try to use just the type name (with lower case first char).
             * if that is already used in this type or a C++ keyword,
             * append ascii digits to field name until unique
             * in this type
             */
            MakePyStrUnique(fieldNames, cFieldName, r->maxDigitsToAppend, 1);
            DefineObj(&fieldNames, cFieldName);
            cxxtri->fieldName = cFieldName;
        }
    }
    FreeDefinedObjs (&fieldNames);
}

/*
 * returns true if this c type for this type should be
 * be ref'd as a ptr
 */
static int
IsPyNone PARAMS ((r, td, parent, t),
    PyRules *r _AND_
    TypeDef *td _AND_
    Type *parent _AND_
    Type *t)
{
    CxxTDI *cxxtdi;
    int retVal = FALSE;

    /*
     * inherit ptr attriubutes from ref'd type if any
     * otherwise grab lib c type def from the PyRules
     */
    if ((t->basicType->choiceId == BASICTYPE_LOCALTYPEREF) ||
        (t->basicType->choiceId == BASICTYPE_IMPORTTYPEREF)) {
        cxxtdi = t->basicType->a.localTypeRef->link->cxxTypeDefInfo;
    } else
        cxxtdi = &r->typeConvTbl[GetBuiltinType (t)];

    /* no parent means t is the root of a typedef */
    if ((parent == NULL) && (cxxtdi->isPtrForTypeDef))
        retVal = TRUE;
    else if ((parent != NULL) &&
             ((parent->basicType->choiceId == BASICTYPE_SET) ||
             (parent->basicType->choiceId == BASICTYPE_SEQUENCE)) &&
             (cxxtdi->isPtrInSetAndSeq))
        retVal = TRUE;
    else if ((parent != NULL) &&
             ((parent->basicType->choiceId == BASICTYPE_SETOF) ||
             (parent->basicType->choiceId == BASICTYPE_SEQUENCEOF)) &&
             (cxxtdi->isPtrInList))
        retVal = TRUE;
    else if ((parent != NULL) &&
             (parent->basicType->choiceId == BASICTYPE_CHOICE) &&
             (cxxtdi->isPtrInChoice))
        retVal = TRUE;
    else if (((t->optional) || (t->defaultVal != NULL)) && (cxxtdi->isPtrForOpt))
        retVal = TRUE;

    return retVal;
}

/* fill given cxxtdi with defaults from table for given typedef */
void
FillPyTDIDefaults PARAMS ((r, cxxtdi, td),
    PyRules *r _AND_
    CxxTDI *cxxtdi _AND_
    TypeDef *td)
{
    CxxTDI *tblCxxtdi;
    int typeIndex;
    char *tmpName;

    typeIndex = GetBuiltinType (td->type);

    if (typeIndex < 0)
        return;

    tblCxxtdi = &r->typeConvTbl[typeIndex];

    memcpy (cxxtdi, tblCxxtdi, sizeof (CxxTDI));

    /* make sure class name is unique wrt to previously defined classes */
    tmpName = Asn1TypeName2CTypeName (td->definedName);
    cxxtdi->className = Malloc (strlen (tmpName) + r->maxDigitsToAppend +1);
    strcpy (cxxtdi->className, tmpName);
    Free (tmpName);

    MakePyStrUnique (definedNamesG, cxxtdi->className, r->maxDigitsToAppend, 1);
    DefineObj (&definedNamesG, cxxtdi->className);

}
