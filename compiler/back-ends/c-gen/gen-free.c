/*
 * compiler/back-ends/c-gen/gen-free.c - routines for printing C hierachical free routines
 *
 * Mike Sample
 * 92/04
 * Copyright (C) 1991, 1992 Michael Sample
 *            and the University of British Columbia
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * $Header: /baseline/SNACC/compiler/back-ends/c-gen/gen-free.c,v 1.8 2004/03/25 19:20:16 gronej Exp $
 * $Log: gen-free.c,v $
 * Revision 1.8  2004/03/25 19:20:16  gronej
 * fixed some linux warnings
 *
 * Revision 1.7  2003/07/07 14:53:38  nicholar
 * Eliminated headers and cleaned up include references
 *
 * Revision 1.6  2003/04/29 21:02:57  leonberp
 * integerated Deepak's changes for IOB support
 *
 * Revision 1.5  2002/11/01 15:05:34  mcphersc
 * fixed compiler warnings and errors not caught by windows compiler
 *
 * Revision 1.4  2002/10/01 13:51:39  mcphersc
 * Modified snacc to accept either the -I import option or the original way to list the
 * asn modules on the command line
 *
 * Revision 1.3  2002/09/04 18:02:11  vracarl
 * got rid of c++ comments
 *
 * Revision 1.2  2000/10/24 14:54:47  rwc
 * Updated to remove high-level warnings (level 4 on MSVC++) for an easier build.
 * SOME warnings persist due to difficulty in modifying the SNACC compiler to
 * properly build clean source; also some files are built by Lex/Yacc.
 *
 * Revision 1.1.1.1  2000/08/21 20:36:05  leonberp
 * First CVS Version of SNACC.
 *
 * Revision 1.3  1995/07/25 18:42:47  rj
 * file name has been shortened for redundant part: c-gen/gen-c-free -> c-gen/gen-free.
 *
 * changed `_' to `-' in file names.
 *
 * Revision 1.2  1994/09/01  00:23:29  rj
 * snacc_config.h and other superfluous .h files removed.
 *
 * Revision 1.1  1994/08/28  09:48:26  rj
 * first check-in. for a list of changes to the snacc-1.1 distribution please refer to the ChangeLog.
 *
 */

/*
	Deepak: Formatting improved as required on 11/Feb/2003
*/
#include "config.h"
#include <stdio.h>

#include "asn-incl.h"
#include "asn1module.h"
#include "mem.h"
#include "define.h"
#include "rules.h"
#include "str-util.h"
#include "util.h"

#ifdef WIN32
//#pragma  warning( disable : 4706 )  /* IGNORE assign w/in conditional expression. */
#endif

extern FILE* errFileG;		// Defined in snacc.c

static char *returnTypeG = "void";
static char *valueArgNameG = "v";
static CRules *genFreeCRulesG;

/* non-exported prototypes */

static void PrintCFreePrototype PROTO ((FILE *hdr, TypeDef *td));
static void PrintCFreeDeclaration PROTO ((FILE *src, TypeDef *td));
static void PrintCFreeDefine PROTO ((FILE *hdr, TypeDef *td));
static void PrintCFreeLocals PROTO ((FILE *src,TypeDef *td));
static void PrintCFreeElmts PROTO ((FILE *src, TypeDef *td, Type *parent, NamedTypeList *elmts, char *varName));
static void PrintCElmtFree PROTO ((FILE *src, TypeDef *td, Type *parent, Type *e, char *varName));
static void PrintCFreeListElmts PROTO ((FILE *src, TypeDef *td, Type *t, char *varName));
static void PrintCFreeListRoutineBody PROTO ((FILE *src, TypeDef *td, Type *t,char *varName));
/*static void PrintCFreeListDefine PROTO ((FILE *hdr,TypeDef *td));*/
static void PrintCFreeChoiceElmts PROTO ((FILE *src, TypeDef *td, Type *t, char *varName));

//Deepak: 18/Apr/2003
static void PrintCFreeMacroElmts PROTO ((FILE *src, TypeDef *td, Type *parent, MacroType *mt, char *varName));
static void PrintCRosOperationElmtsFree PROTO ((FILE *src, TypeDef *td, Type *parent, MacroType *elmts, RosOperationMacroType *op, char *varName));


void
PrintCFree PARAMS ((src, hdr, r, mods, m, td),
    FILE *src _AND_
    FILE *hdr _AND_
    CRules *r _AND_
    ModuleList *mods _AND_
    Module *m _AND_
    TypeDef *td)
{
    CTDI *ctdi;
    CTypeId rhsTypeId;  /* cTypeId of the type that defined this typedef */

    genFreeCRulesG = r;

    ctdi =  td->cTypeDefInfo;
    if((ctdi == NULL) || (td->type->cTypeRefInfo == NULL))
    {
        fprintf (errFileG, "PrintCFree: ERROR - no type info\n");
        return;
    }

    if(!ctdi->genFreeRoutine)
        return;

    rhsTypeId = td->type->cTypeRefInfo->cTypeId;
    switch(rhsTypeId)
    {
        case C_ANY:
        case C_ANYDEFINEDBY:
        case C_LIB:
        case C_TYPEREF:
            PrintCFreeDefine (hdr, td);
            fprintf (hdr,"\n");
            break;

        case C_CHOICE:
            PrintCFreePrototype (hdr, td);
            PrintCFreeDeclaration (src, td);
            fprintf (src,"{\n");
            PrintCFreeLocals (src, td);
            fprintf (src,"\tif(%s == NULL)\n", valueArgNameG);
            fprintf (src,"\t\treturn;\n");
            PrintCFreeChoiceElmts (src, td, td->type, valueArgNameG);
            fprintf (src,"}  /* %s */",td->cTypeDefInfo->freeRoutineName);
            fprintf (hdr,"\n\n");
            fprintf (src,"\n\n");
            break;

        case C_STRUCT:
            PrintCFreePrototype (hdr, td);
            PrintCFreeDeclaration (src, td);
            fprintf (src,"{\n");
            PrintCFreeLocals (src, td);
            fprintf (src,"\tif(%s == NULL)\n", valueArgNameG);
            fprintf (src,"\t\treturn;\n");
            PrintCFreeElmts (src, td, td->type, td->type->basicType->a.set, valueArgNameG);
            fprintf (src,"}  /* %s */", td->cTypeDefInfo->freeRoutineName);
            fprintf (hdr,"\n\n");
            fprintf (src,"\n\n");
            break;


        case C_LIST:
            PrintCFreePrototype (hdr, td);
            PrintCFreeDeclaration (src, td);
            fprintf (src,"{\n");
            PrintCFreeLocals (src, td);
            fprintf (src,"\tif(%s == NULL)\n", valueArgNameG);
            fprintf (src,"\t\treturn;\n");
            PrintCFreeListRoutineBody (src, td, td->type, valueArgNameG);
            fprintf (src,"}  /* %s */", td->cTypeDefInfo->freeRoutineName);
            fprintf (hdr,"\n\n");
            fprintf (src,"\n\n");
            break;


		case C_MACROTYPE:
            PrintCFreePrototype (hdr, td);
            PrintCFreeDeclaration (src, td);
            fprintf (src,"{\n");
            PrintCFreeLocals (src, td);
            fprintf (src,"\tif(%s == NULL)\n", valueArgNameG);
            fprintf (src,"\t\treturn;\n");
            PrintCFreeMacroElmts (src, td, td->type, td->type->basicType->a.macroType, valueArgNameG);
            fprintf (src,"}  /* %s */", td->cTypeDefInfo->freeRoutineName);
            fprintf (hdr,"\n\n");
            fprintf (src,"\n\n");
            break;

        case C_NO_TYPE:
            break;

        default:
            fprintf (errFileG, "PrintCFree: ERROR - unknown c type id\n");
            break;
    }
    m=m;
    mods=mods; /*AVOIDS warning*/
}  /*  PrintCFree */


static void
PrintCFreeMacroElmts PARAMS ((src, td, parent, mt, varName),
    FILE *src _AND_
    TypeDef *td _AND_
    Type *parent _AND_
    MacroType *mt _AND_
    char *varName)
{
    if(mt == NULL)
    {
        fprintf (src,"/* ERROR? - expected macroType for this type*/\n");
        return;
    }

    switch (mt->choiceId)
    {	// This switch case copied from do-macros.c
	case MACROTYPE_ASNABSTRACTOPERATION:
	case MACROTYPE_ROSOPERATION:
		
		PrintCRosOperationElmtsFree (src, td, parent, mt, mt->a.rosOperation, varName);
			
		break;

		// Add code for other macro types here
	default:
		// Unsupported Macro Type
		break;
	}

    //PrintCElmtFree (src, td, parent, e->type, varName);

}  /* PrintCFreeMacroElmts */

static void
PrintCRosOperationElmtsFree PARAMS ((src, td, parent, mt, varName),
    FILE *src _AND_
    TypeDef *td _AND_
    Type *parent _AND_
    MacroType *mt _AND_
    RosOperationMacroType *op _AND_
	char *varName)
{
	if(op->arguments != NULL)
	{
		PrintCElmtFree (src, td, parent, op->arguments->type, varName);
	}

	if(op->result != NULL)
	{
		PrintCElmtFree (src, td, parent, op->result->type, varName);
	}
	mt=mt;
} /* PrintCRosOperationElmtsFree */

/*
 * Prints prototype for encode routine in hdr file
 */
static void
PrintCFreePrototype PARAMS ((hdr, td),
    FILE *hdr _AND_
    TypeDef *td)
{
    CTDI *ctdi;

    ctdi = td->cTypeDefInfo;
    //fprintf (hdr,"%s %s PROTO ((%s *v));\n", returnTypeG, ctdi->freeRoutineName, ctdi->cTypeName);
	fprintf (hdr,"%s %s(%s *v);\n", returnTypeG, ctdi->freeRoutineName, ctdi->cTypeName);

}  /*  PrintCFreePrototype */



/*
 * Prints declarations of encode routine for the given type def
 */
static void
PrintCFreeDeclaration PARAMS ((src, td),
    FILE *src _AND_
    TypeDef *td)
{
    CTDI *ctdi;

    ctdi =  td->cTypeDefInfo;
//	fprintf (src,"%s\n%s PARAMS ((v),\n%s *v)\n", returnTypeG, ctdi->freeRoutineName,  ctdi->cTypeName);
    fprintf (src,"%s %s(%s *v)\n", returnTypeG, ctdi->freeRoutineName,  ctdi->cTypeName);

}  /*  PrintCFreeDeclaration */




static void
PrintCFreeDefine PARAMS ((hdr, td),
    FILE *hdr _AND_
    TypeDef *td)
{

    fprintf(hdr, "#define %s %s ", td->cTypeDefInfo->freeRoutineName, td->type->cTypeRefInfo->freeRoutineName);

/*
    fprintf(hdr, "#define %s(v)  ", td->cTypeDefInfo->freeRoutineName);
    fprintf (hdr, "%s (v)", td->type->cTypeRefInfo->freeRoutineName);
*/
}  /*  PrintCFreeDefine */




static void
PrintCFreeLocals PARAMS ((src, td),
    FILE *src _AND_
    TypeDef *td)
{
    fprintf (src, "\n");

    if((td->type->basicType->choiceId == BASICTYPE_SETOF) ||
        (td->type->basicType->choiceId == BASICTYPE_SEQUENCEOF))
    {
        fprintf (src,"\tAsnListNode *l;\n");
        fprintf (src,"\tAsnListNode *tmp;\n");
    }

}  /*  PrintCFreeLocals */



static void
PrintCFreeElmts PARAMS ((src, td, parent, elmts, varName),
    FILE *src _AND_
    TypeDef *td _AND_
    Type *parent _AND_
    NamedTypeList *elmts _AND_
    char *varName)
{
    NamedType *e;

    if(elmts == NULL)
    {
        fprintf (src,"/* ERROR? - expected elmts for this type*/\n");
        return;
    }

    FOR_EACH_LIST_ELMT (e, elmts)
        PrintCElmtFree (src, td, parent, e->type, varName);

}  /* PrintCFreeElmts */



/*
 * Prints code for encoding the elmts of a SEQ or SET
 */
static void
PrintCElmtFree PARAMS ((src, td, parent, e, varName),
    FILE *src _AND_
    TypeDef *td _AND_
    Type *parent _AND_
    Type *e _AND_
    char *varName)
{
    CTRI *ctri;
    char elmtVarRef[MAX_VAR_REF];

    if((e == NULL) || (e->cTypeRefInfo == NULL))
        return;

    ctri =  e->cTypeRefInfo;

    /* build ref to the elmt */
    MakeVarPtrRef (genFreeCRulesG, td, parent, e, varName, elmtVarRef);

    /* if optional then put in NULL check */
    if(e->optional || (e->defaultVal != NULL))
        fprintf (src, "\tif(%s (%s))\n    {\n", ctri->optTestRoutineName, elmtVarRef);

    /* free contents of elmt first */
    switch(ctri->cTypeId)
    {
        case C_ANY:
        case C_ANYDEFINEDBY:
        case C_LIB:
        case C_TYPEREF:
            fprintf (src,"\t%s%s;", ctri->freeRoutineName, elmtVarRef);
            break;

        case C_LIST:
            PrintCFreeListElmts (src, td, e, elmtVarRef);
            break;

        /*
         * this  follwing shouldn't happen since embedded
         * choices/struct are moved to separate typedefs
         * in normalize.c.
         */
        case C_CHOICE:
            PrintCFreeChoiceElmts (src, td, e, elmtVarRef);
            break;

        case C_STRUCT:
            PrintCFreeElmts (src, td, e, e->basicType->a.set, elmtVarRef);
            break;

		case C_MACROTYPE:
            break;

        case C_NO_TYPE:
            break;

        default:
            fprintf (errFileG, "PrintCElmtFree: ERROR - unknown c type id\n");
            break;
    }

    /* free elmt itself if it is ref'd by ptr */
    if(ctri->isPtr)
        fprintf (src,"\n\tAsn1Free (%s);\n",elmtVarRef);

    /* write closing brkt for NULL check for optional elmts */
    if(e->optional || (e->defaultVal != NULL))
        fprintf (src, "\n\t}\n");

    fprintf (src,"\n");

}  /*  PrintCElmtFree */


/*static void
PrintCFreeListDefine PARAMS ((hdr, td),
    FILE *hdr _AND_
    TypeDef *td)
{
    fprintf(hdr, "#define %s(v)  ", td->cTypeDefInfo->freeRoutineName);
    fprintf (hdr, "ASN1_FREE_LIST (v, %s)", td->type->cTypeRefInfo->freeRoutineName);
}*/


static void
PrintCFreeListRoutineBody PARAMS ((src, td, t, varName),
    FILE *src _AND_
    TypeDef *td _AND_
    Type *t _AND_
    char *varName)
{
    Type *e;
    CTRI *ctri;
    char *elmtVarRef;

    fprintf (src,"\tfor (l = FIRST_LIST_NODE (%s); l != NULL; )\n", varName);
    fprintf (src,"\t{\n");

    e = t->basicType->a.setOf;
    ctri = e->cTypeRefInfo;
    elmtVarRef = "(l->data)";
    switch(ctri->cTypeId)
    {
		case C_ANY:
        case C_LIB:
        case C_TYPEREF:
            fprintf (src,"\t\t%s (%s);\n", ctri->freeRoutineName, elmtVarRef);
            break;

        case C_LIST:
            PrintCFreeListElmts (src, td, e, elmtVarRef);
            break;

        /*
         * this  follwing shouldn't happen since embedded
         * choices/struct are moved to separate typedefs
         * in normalize.c.
         */
        case C_CHOICE:
            PrintCFreeChoiceElmts (src, td, e, elmtVarRef);
            break;

        case C_STRUCT:
            PrintCFreeElmts (src, td, e, e->basicType->a.set, elmtVarRef);
            break;

		case C_MACROTYPE:
            break;

        case C_NO_TYPE:
            break;

        default:
            fprintf (errFileG, "PrintCElmtFree: ERROR - unknown c type id\n");
            break;
    }

    fprintf (src,"\t\ttmp = l->next;\n");
    fprintf (src,"\t\tAsn1Free (l->data);\n");
    fprintf (src,"\t\tAsn1Free (l);\n");
    fprintf (src,"\t\tl = tmp;\n");
    fprintf (src,"\t}\n");
}

static void
PrintCFreeListElmts PARAMS ((src, td, t, varName),
    FILE *src _AND_
    TypeDef *td _AND_
    Type *t _AND_
    char *varName)
{
    Type *e;
    CTRI *ctri;
    char *elmtVarRef;

    fprintf (src,"\t{\n");
    fprintf (src,"\t\tAsnListNode *l;\n");
    fprintf (src,"\t\tAsnListNode *tmp;\n");
    fprintf (src,"\t\tfor (l = FIRST_LIST_NODE (%s); l != NULL; )\n", varName);
    fprintf (src,"\t\t{\n");


    e = t->basicType->a.setOf;
    ctri = e->cTypeRefInfo;
    elmtVarRef = "(l->data)";
    switch(ctri->cTypeId)
    {
        case C_LIB:
        case C_TYPEREF:
            fprintf (src,"\t\t%s (%s);\n", ctri->freeRoutineName, elmtVarRef);
            break;

        case C_LIST:
            PrintCFreeListElmts (src, td, e, elmtVarRef);
            break;

        /*
         * this  follwing shouldn't happen since embedded
         * choices/struct are moved to separate typedefs
         * in normalize.c.
         */
        case C_CHOICE:
            PrintCFreeChoiceElmts (src, td, e, elmtVarRef);
            break;

        case C_STRUCT:
            PrintCFreeElmts (src, td, e, e->basicType->a.set, elmtVarRef);
            break;

		case C_MACROTYPE:
            break;

        case C_NO_TYPE:
            break;

        default:
            fprintf (errFileG, "PrintCElmtFree: ERROR - unknown c type id\n");
            break;
    }

    fprintf (src,"\t\t   tmp = l->next;\n");
    fprintf (src,"\t\t   Asn1Free (l->data);\n");
    fprintf (src,"\t\t   Asn1Free (l);\n");
    fprintf (src,"\t\t   l = tmp;\n");
    fprintf (src,"\t\t}\n");
    fprintf (src,"\t}\n");
} /* PrintCFreeListELmts */



static void
PrintCFreeChoiceElmts PARAMS ((src, td, t, varName),
    FILE *src _AND_
    TypeDef *td _AND_
    Type *t _AND_
    char *varName)
{
    NamedType *e;
    CTRI *ctri;
    void *tmp;

    ctri =  t->cTypeRefInfo;

    fprintf (src,"\tswitch(%s->%s)\n    {\n", varName, ctri->choiceIdEnumFieldName);

    FOR_EACH_LIST_ELMT (e, t->basicType->a.choice)
    {
        tmp = (void*)CURR_LIST_NODE (t->basicType->a.choice);

        if(e->type == NULL)
            continue;

        ctri =  e->type->cTypeRefInfo;

        if(e->type->cTypeRefInfo == NULL)
            fprintf (src, "\t   case ????:\n");
        else if(ctri->isPtr)
        {
            fprintf (src, "\t   case %s:\n", ctri->choiceIdSymbol);
            PrintCElmtFree (src, td, t, e->type, varName);
            fprintf (src,"\tbreak;\n\n");
        }

        SET_CURR_LIST_NODE (t->basicType->a.choice, tmp);
    }

    fprintf (src, "\t}\n");
}
