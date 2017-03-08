/*
 * compiler/back-ends/c-gen/gen-print.c - routines for printing C hierachical print routines
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
 * $Header: /baseline/SNACC/compiler/back-ends/c-gen/gen-print.c,v 1.8 2004/01/22 20:03:02 nicholar Exp $
 */
#include "config.h"
#include "asn-incl.h"
#include "asn1module.h"
#include "rules.h"
#include "util.h"
#include "snacc-util.h"

static char *returnTypeG = "void";
static char *valueArgNameG = "v";
static char *fileTypeNameG = "FILE*";
static char *indentTypeNameG = "unsigned int";
static CRules *genPrintCRulesG;
/* non-exported prototypes */

static void PrintCPrintPrototype PROTO ((FILE *hdr, TypeDef *td));
static void PrintCPrintDeclaration PROTO ((FILE *src, TypeDef *td));
static void PrintCPrintDefine PROTO ((FILE *hdr, TypeDef *td));
static void PrintCPrintLocals PROTO ((FILE *src,TypeDef *td));
/*
static void PrintCPrintElmts PROTO ((FILE *src, TypeDef *td, Type *parent, NamedTypeList *elmts, char *varName));
*/
static void PrintCChoiceElmtPrint PROTO ((FILE *src, TypeDef *td, Type *parent, NamedTypeList *elmts, NamedType *e, char *varName));


static void PrintCElmtPrintWithIndent PROTO ((FILE *src, TypeDef *td, Type *parent, NamedTypeList *elmts, NamedType *e, char *varName, int allOpt));

static void PrintCChoicePrintRoutine PROTO ((FILE *src, FILE *hdr, CRules *r, ModuleList *mods, Module *m, TypeDef *td));

static void PrintCSetPrintRoutine  PROTO ((FILE *src, FILE *hdr, CRules *r, ModuleList *mods, Module *m, TypeDef *td));
static void PrintCSeqPrintRoutine  PROTO ((FILE *src, FILE *hdr, CRules *r, ModuleList *mods, Module *m, TypeDef *td));
static void PrintCSeqOfPrintRoutine PROTO ((FILE *src, FILE *hdr, CRules *r, ModuleList *mods, Module *m, TypeDef *td));
static void PrintCSetOfPrintRoutine PROTO ((FILE *src, FILE *hdr, CRules *r, ModuleList *mods, Module *m, TypeDef *td));



void
PrintCPrinter PARAMS ((src, hdr, r, mods, m, td),
    FILE *src _AND_
    FILE *hdr _AND_
    CRules *r _AND_
    ModuleList *mods _AND_
    Module *m _AND_
    TypeDef *td)
{
    if ((td->cTypeDefInfo == NULL) || !(td->cTypeDefInfo->genPrintRoutine))
        return;

    genPrintCRulesG = r;
    switch (td->type->basicType->choiceId)
    {
        case BASICTYPE_IMPORTTYPEREF:  /* type references */
        case BASICTYPE_LOCALTYPEREF:
        case BASICTYPE_BOOLEAN:  /* library type */
        case BASICTYPE_REAL:  /* library type */
        case BASICTYPE_OCTETSTRING:  /* library type */
        case BASICTYPE_NULL:  /* library type */
        case BASICTYPE_OID:  /* library type */
        case BASICTYPE_RELATIVE_OID:
        case BASICTYPE_INTEGER:  /* library type */
/*        case BASICTYPE_BIGINT:  */ /* library type */			
        case BASICTYPE_BITSTRING:  /* library type */
        case BASICTYPE_ENUMERATED:  /* library type */
        case BASICTYPE_ANYDEFINEDBY:  /* ANY types */
        case BASICTYPE_ANY:
		case BASICTYPE_NUMERIC_STR:
		case BASICTYPE_PRINTABLE_STR:
		case BASICTYPE_IA5_STR:
		case BASICTYPE_BMP_STR:
		case BASICTYPE_UNIVERSAL_STR:
		case BASICTYPE_UTF8_STR:
		case BASICTYPE_T61_STR:
        case BASICTYPE_VISIBLE_STR:
        case BASICTYPE_GENERALIZEDTIME:
        case BASICTYPE_UTCTIME:
            PrintCPrintDefine (hdr, td);
            fprintf (hdr, "\n\n");
            break;

        case BASICTYPE_SETOF:
            PrintCSetOfPrintRoutine (src, hdr, r, mods, m, td);
            break;

        case BASICTYPE_SEQUENCEOF:
            PrintCSeqOfPrintRoutine (src, hdr, r, mods, m, td);
            break;

        case BASICTYPE_CHOICE:
            PrintCChoicePrintRoutine (src, hdr, r, mods, m, td);
            break;

        case BASICTYPE_SET:
            PrintCSetPrintRoutine (src, hdr, r, mods, m, td);
            break;


        case BASICTYPE_SEQUENCE:
            PrintCSeqPrintRoutine (src, hdr, r, mods, m, td);
            break;

        default:
            break;
    }
}  /*  PrintCPrint */


/*
 * Prints prototype for encode routine in hdr file
 */
static void
PrintCPrintPrototype PARAMS ((hdr, td),
    FILE *hdr _AND_
    TypeDef *td)
{
    CTDI *ctdi;

    ctdi = td->cTypeDefInfo;
    fprintf (hdr,"%s %s PROTO ((%s f, %s *v, %s indent));\n", returnTypeG,
		ctdi->printRoutineName, fileTypeNameG, ctdi->cTypeName,
		indentTypeNameG);

}  /*  PrintCPrintPrototype */



/*
 * Prints declarations of encode routine for the given type def
 */
static void
PrintCPrintDeclaration PARAMS ((src, td),
    FILE *src _AND_
    TypeDef *td)
{
    CTDI *ctdi;

    ctdi =  td->cTypeDefInfo;
    fprintf (src,"%s\n%s PARAMS ((f, v, indent),\n%s f _AND_\n%s *v _AND_"
		"\n%s indent)\n", returnTypeG, ctdi->printRoutineName, fileTypeNameG,
		ctdi->cTypeName, indentTypeNameG);

}  /*  PrintCPrintDeclaration */




static void
PrintCPrintDefine PARAMS ((hdr, td),
    FILE *hdr _AND_
    TypeDef *td)
{
    fprintf(hdr, "#define %s %s", td->cTypeDefInfo->printRoutineName, td->type->cTypeRefInfo->printRoutineName);
/*
    fprintf(hdr, "#define %s(f, v, indent)  ", td->cTypeDefInfo->printRoutineName);
    fprintf (hdr, "%s (f, v, indent)", td->type->cTypeRefInfo->printRoutineName);
*/
}  /*  PrintCPrintDefine */




static void
PrintCPrintLocals PARAMS ((src, td),
    FILE *src _AND_
    TypeDef *td)
{
    /* none yet */
    td = td;
    src = src; /*AVOIDS warning.*/
}  /*  PrintCPrintLocals */


/*
static void
PrintCPrintElmts PARAMS ((src, td, parent, elmts, varName),
    FILE *src _AND_
    TypeDef *td _AND_
    Type *parent _AND_
    NamedTypeList *elmts _AND_
    char *varName)
{
    NamedType *e;


    FOR_EACH_LIST_ELMT (e, elmts)
        PrintCElmtPrint (src, td, parent, elmts, e, varName);
}  PrintCPrintElmts */ 



/*
 * Prints code for printing a CHOICE element
 *
 */
static void
PrintCChoiceElmtPrint PARAMS ((src, td, parent, elmts,  e, varName),
    FILE *src _AND_
    TypeDef *td _AND_
    Type *parent _AND_
    NamedTypeList *elmts _AND_
    NamedType *e _AND_
    char *varName)
{
    char elmtVarRef[MAX_VAR_REF];

    /* build ref to the elmt */
    MakeVarPtrRef (genPrintCRulesG, td, parent, e->type, varName, elmtVarRef);

    if (e->fieldName != NULL)
        fprintf (src, "\t\tfprintf (f, \"%s \");\n", e->fieldName);

	fprintf (src, "\t\t%s (f, %s, indent);\n",
		e->type->cTypeRefInfo->printRoutineName, elmtVarRef);

    elmts = elmts; /*AVOIDS warning.*/
}  /*  PrintCChoiceElmtPrint */

/*
 * Prints code for printing an elmt of a SEQ or SET
 *
 * Does funny things to print commas correctly
 * eg for the following type
 * Foo ::= SET
 * {
 *      A,                   --> print   A ",\n"
 *      B,                               B ",\n"
 *      C OPTIONAL,                      C ",\n" if C present
 *      D,                               D ",\n"
 *      E,                               E ",\n"
 *      F,                               F       <- nothing after last non-opt
 *                                                  before tail opts.
 *      G OPTIONAL,                      ",\n" G
 *      H OPTIONAL                       ",\n" H "\n"
 * }

 */
static void
PrintCElmtPrintWithIndent PARAMS ((src, td, parent, elmts, e, varName, allOpt),
    FILE *src _AND_
    TypeDef *td _AND_
    Type *parent _AND_
    NamedTypeList *elmts _AND_
    NamedType *e _AND_
    char *varName _AND_
    int allOpt)
{
    CTRI *ctri;
    char elmtVarRef[MAX_VAR_REF];
    int inTailOpts;
	const char* tabStr = "\t";

    ctri =  e->type->cTypeRefInfo;

    /* this assumes the elmts->curr == e */
    inTailOpts = IsTailOptional (elmts);

    /* build ref to the elmt */
    MakeVarPtrRef (genPrintCRulesG, td, parent, e->type, varName, elmtVarRef);

    /* if optional then put in NULL check */
    if (e->type->optional || (e->type->defaultVal != NULL))
	{
        fprintf (src, "\tif (%s (%s))\n    {\n", ctri->optTestRoutineName,
			elmtVarRef);
		tabStr = "\t\t";
	}

    if (allOpt)
    {
        if (e != FIRST_LIST_ELMT (elmts))
        {
            fprintf (src, "%sif (!nonePrinted)\n", tabStr);
            fprintf (src, "%s\tfprintf (f, \",\\n\");\n", tabStr);
        }
        fprintf (src, "%snonePrinted = FALSE;\n", tabStr);
    }
    else if ((inTailOpts) && (e != FIRST_LIST_ELMT (elmts)))
        fprintf (src, "%sfprintf (f, \",\\n\");\n", tabStr);

    fprintf (src, "%sIndent (f, indent + 1);\n", tabStr);

    if (e->fieldName != NULL)
        fprintf (src, "%sfprintf (f, \"%s \");\n", tabStr, e->fieldName);

    fprintf (src, "%s%s (f, %s, indent + 1);\n", tabStr,
		e->type->cTypeRefInfo->printRoutineName, elmtVarRef);

    if ((e != LAST_LIST_ELMT (elmts)) &&
         (!inTailOpts) &&
         (!NextIsTailOptional (elmts)))
        fprintf (src, "%sfprintf (f, \",\\n\");\n", tabStr);


    /* write closing brkt for NULL check for optional elmts */
    if (e->type->optional || (e->type->defaultVal != NULL))
        fprintf (src, "\t}\n");

}  /*  PrintCElmtPrintWithIndent */


static void
PrintCChoicePrintRoutine PARAMS ((src, hdr, r, mods, m, td),
    FILE *src _AND_
    FILE *hdr _AND_
    CRules *r _AND_
    ModuleList *mods _AND_
    Module *m _AND_
    TypeDef *td)
{
    NamedType *e;

    PrintCPrintPrototype (hdr, td);

    PrintCPrintDeclaration (src, td);
    fprintf (src, "{\n");
    PrintCPrintLocals (src, td);
    fprintf (src, "\tswitch (%s->%s)\n", valueArgNameG,
		td->type->cTypeRefInfo->choiceIdEnumFieldName);
    fprintf (src, "\t{\n");

    FOR_EACH_LIST_ELMT (e, td->type->basicType->a.choice)
    {
        fprintf (src, "\tcase %s:\n",e->type->cTypeRefInfo->choiceIdSymbol);
        PrintCChoiceElmtPrint (src, td, td->type, td->type->basicType->a.choice, e, valueArgNameG);
        fprintf (src, "\t\tbreak;\n\n");
    }
    fprintf (src, "\t}\n");
    fprintf (src,"}  /* %s */\n\n", td->cTypeDefInfo->printRoutineName);

    m = m;
    mods = mods;
    r = r;    /*AVOIDS warning.*/
} /* PrintCChoicePrintRoutine */



static void
PrintCSetPrintRoutine PARAMS ((src, hdr, r, mods, m, td),
    FILE *src _AND_
    FILE *hdr _AND_
    CRules *r _AND_
    ModuleList *mods _AND_
    Module *m _AND_
    TypeDef *td)
{
    NamedType *e;
    int allOpt;

    PrintCPrintPrototype (hdr, td);

    PrintCPrintDeclaration (src, td);
    fprintf (src, "{\n");
    PrintCPrintLocals (src, td);

    allOpt = AllElmtsOptional (td->type->basicType->a.set);
    /*
     * print extra local variable so commas are handled correctly
     * when all elements are optional
     */
    if (allOpt)
        fprintf (src, "\tint nonePrinted = TRUE;\n\n");

    fprintf (src, "\tif (%s == NULL)\n", valueArgNameG);
    fprintf (src, "\t\treturn;\n\n");

    fprintf (src, "\tfprintf (f,\"{ -- SET --\\n\");\n\n");


    FOR_EACH_LIST_ELMT (e, td->type->basicType->a.set)
    {
		PrintCElmtPrintWithIndent (src, td, td->type,
			td->type->basicType->a.set, e, valueArgNameG, allOpt);
    }
	fprintf (src, "\tfprintf (f, \"\\n\");\n");
    fprintf (src, "\tIndent (f, indent);\n");
    fprintf (src, "\tfprintf (f, \"}\");\n");

    fprintf (src, "}  /* %s */\n\n", td->cTypeDefInfo->printRoutineName);

    m = m;
    mods = mods;r = r;   /*AVOIDS warning.*/
} /* PrintCSetPrintRoutine */



static void
PrintCSeqPrintRoutine PARAMS ((src, hdr, r, mods, m, td),
    FILE *src _AND_
    FILE *hdr _AND_
    CRules *r _AND_
    ModuleList *mods _AND_
    Module *m _AND_
    TypeDef *td)
{
    NamedType *e;
    int allOpt;

    PrintCPrintPrototype (hdr, td);

    PrintCPrintDeclaration (src, td);
    fprintf (src, "{\n");
    PrintCPrintLocals (src, td);

    allOpt = AllElmtsOptional (td->type->basicType->a.set);
    /*
     * print extra local variable so commas are handled correctly
     * when all elements are optional
     */
    if (allOpt)
        fprintf (src, "\tint nonePrinted = TRUE;\n\n");

    fprintf (src, "\tif (%s == NULL)\n", valueArgNameG);
    fprintf (src, "\t\treturn;\n\n");

    fprintf (src, "\tfprintf (f, \"{ -- SEQUENCE --\\n\");\n\n");

    FOR_EACH_LIST_ELMT (e, td->type->basicType->a.sequence)
    {
        PrintCElmtPrintWithIndent (src, td, td->type,
			td->type->basicType->a.sequence, e, valueArgNameG, allOpt);
    }
	fprintf (src, "\tfprintf (f, \"\\n\");\n");
    fprintf (src, "\tIndent (f, indent);\n");
    fprintf (src, "\tfprintf (f, \"}\");\n");

    fprintf (src, "}  /* %s */\n\n", td->cTypeDefInfo->printRoutineName);
    m = m;mods = mods;r = r;    /*AVOIDS warning.*/
} /* PrintCSeqPrintRoutine */



static void
PrintCSetOfPrintRoutine PARAMS ((src, hdr, r, mods, m, td),
    FILE *src _AND_
    FILE *hdr _AND_
    CRules *r _AND_
    ModuleList *mods _AND_
    Module *m _AND_
    TypeDef *td)
{

    PrintCPrintPrototype (hdr, td);

    PrintCPrintDeclaration (src, td);
    fprintf (src, "{\n");
    PrintCPrintLocals (src, td);

    fprintf (src, "\t%s *tmp;\n",
		td->type->basicType->a.setOf->cTypeRefInfo->cTypeName);

    fprintf (src, "\tif (%s == NULL)\n", valueArgNameG);
    fprintf (src, "\t\treturn;\n");

    fprintf (src, "\tfprintf (f, \"{ -- SET OF -- \\n\");\n");

    fprintf (src, "\tFOR_EACH_LIST_ELMT (tmp, %s)\n", valueArgNameG);
    fprintf (src, "\t{\n");
    fprintf (src, "\t\tIndent (f, indent + 1);\n");
    fprintf (src, "\t\t%s (f, tmp, indent + 1);\n",
		td->type->basicType->a.setOf->cTypeRefInfo->printRoutineName);
    fprintf (src, "\t\tif (tmp != (%s*)LAST_LIST_ELMT (%s))\n",
		td->type->basicType->a.setOf->cTypeRefInfo->cTypeName, valueArgNameG);
    fprintf (src, "\t\t\tfprintf (f, \",\\n\");\n");
    fprintf (src, "\t}\n");
    fprintf (src, "\tfprintf (f, \"\\n\");\n");
    fprintf (src, "\tIndent (f, indent);\n");
    fprintf (src, "\tfprintf (f, \"}\");\n");

    fprintf (src, "}  /* %s */\n\n", td->cTypeDefInfo->printRoutineName);

    m = m;mods = mods;r = r;    /*AVOIDS warning.*/
} /* PrintCSetOfPrintRoutine */

static void
PrintCSeqOfPrintRoutine PARAMS ((src, hdr, r, mods, m, td),
    FILE *src _AND_
    FILE *hdr _AND_
    CRules *r _AND_
    ModuleList *mods _AND_
    Module *m _AND_
    TypeDef *td)
{

    PrintCPrintPrototype (hdr, td);

    PrintCPrintDeclaration (src, td);
    fprintf (src, "{\n");
    PrintCPrintLocals (src, td);

    fprintf (src, "\t%s *tmp;\n",
		td->type->basicType->a.setOf->cTypeRefInfo->cTypeName);

    fprintf (src, "\tif (%s == NULL)\n", valueArgNameG);
    fprintf (src, "\t\treturn;\n");

    fprintf (src, "\tfprintf (f, \"{ -- SEQUENCE OF -- \\n\");\n");

    fprintf (src, "\tFOR_EACH_LIST_ELMT (tmp, %s)\n", valueArgNameG);
    fprintf (src, "\t{\n");
    fprintf (src, "\t\tIndent (f, indent+ 1);\n");
    fprintf (src, "\t\t%s (f, tmp, indent + 1);\n",
		td->type->basicType->a.setOf->cTypeRefInfo->printRoutineName);
    fprintf (src, "\t\tif (tmp != (%s*)LAST_LIST_ELMT (%s))\n",
		td->type->basicType->a.setOf->cTypeRefInfo->cTypeName, valueArgNameG);
    fprintf (src, "\t\t\tfprintf (f, \",\\n\");\n");
    fprintf (src, "\t}\n");
    fprintf (src, "\tfprintf (f, \"\\n\");\n");
    fprintf (src, "\tIndent (f, indent);\n");
    fprintf (src, "\tfprintf (f, \"}\");\n");

    fprintf (src, "}  /* %s */\n\n", td->cTypeDefInfo->printRoutineName);

    m=m;mods=mods;r=r;    /*AVOIDS warning.*/
} /* PrintCSeqOfPrintRoutine */
