/*
 * compiler/back-ends/c-gen/gen-type.c - routines for printing c types from  ASN.1 from type trees
 *
 * Mike Sample
 * 91/09/26
 * Copyright (C) 1991, 1992 Michael Sample
 *            and the University of British Columbia
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * $Header: /baseline/SNACC/compiler/back-ends/c-gen/gen-type.c,v 1.7 2004/03/25 19:20:16 gronej Exp $
 * $Log: gen-type.c,v $
 * Revision 1.7  2004/03/25 19:20:16  gronej
 * fixed some linux warnings
 *
 * Revision 1.6  2003/07/07 14:53:38  nicholar
 * Eliminated headers and cleaned up include references
 *
 * Revision 1.5  2003/04/29 21:02:36  leonberp
 * integerated Deepak's changes for IOB support
 *
 * Revision 1.4  2002/09/16 17:35:01  mcphersc
 * Fixed warnings
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
 * Revision 1.3  1995/07/25  18:43:44  rj
 * file name has been shortened for redundant part: c-gen/gen-c-type -> c-gen/gen-type.
 *
 * changed `_' to `-' in file names.
 *
 * Revision 1.2  1994/09/01  00:23:58  rj
 * snacc_config.h and other superfluous .h files removed.
 *
 * Revision 1.1  1994/08/28  09:48:31  rj
 * first check-in. for a list of changes to the snacc-1.1 distribution please refer to the ChangeLog.
 *
 */
#include "config.h"
#include <stdio.h>
#include <ctype.h>

#include "asn-incl.h"
#include "asn1module.h"
#include "rules.h"
#include "str-util.h"
#include "print.h"


// Deepak: 26/Mar/2003
extern char *valueArgNameG;
extern char *bufTypeNameG;
extern char *lenTypeNameG;
extern char *tagTypeNameG;
extern char *returnTypeG;


/* non-exported prototypes */
static void PrintCType PROTO ((FILE *f,  CRules *r, Module *m, TypeDef *td,
							  Type *parent, Type *t));
static void PrintCStructElmts PROTO ((FILE *f, CRules *r, Module *m,
									 TypeDef *td, Type *parent, Type *t));
static void PrintCChoiceIdEnum PROTO ((FILE *f, CRules *r, Module *m,
									  TypeDef *td, Type *parent, Type *t));
static void PrintCChoiceUnion  PROTO ((FILE *f, CRules *r, Module *m,
									  TypeDef *td, Type *parent, Type *t));
static void PrintCChoiceTypeDef PROTO ((FILE *f, CRules *r, Module *m,
									   TypeDef *td));
static void PrintTypeComment PROTO ((FILE *f, TypeDef *head, Type *t));
static void PrintPreTypeDefStuff PROTO ((FILE *f, CRules *r, Module *m,
										TypeDef *td, Type *parent, Type *t));

// Deepak: 11/Mar/2003
static void PrintCObjectClassElmts PROTO ((FILE *f, CRules *r, Module *m,
										  TypeDef *td, Type *parent, Type *t));

// Deepak: 17/Apr/2003
static void PrintCMacroTypeElmts PROTO ((FILE *f, CRules *r, Module *m,
										TypeDef *td, Type *parent, Type *t));
static void PrintCMacroRosOperationElmts PROTO ((FILE *f, CRules *r, Module *m,
												TypeDef *td, Type *parent,
												Type *t,
												RosOperationMacroType *op));


void
PrintCTypeDef PARAMS ((f, r, m, td),
    FILE *f _AND_
    CRules *r _AND_
    Module *m _AND_
    TypeDef *td)
{
    CTRI *ctri;
    CTDI *ctdi;
    Type *t;

    ctdi = td->cTypeDefInfo;
    if ((ctdi == NULL) || (!ctdi->genTypeDef))
        return;

    t = td->type;
    ctri = t->cTypeRefInfo;

    PrintPreTypeDefStuff (f, r, m, td, NULL, t);

    switch (ctri->cTypeId)
    {
        case C_TYPEREF:
        case C_LIB:
        case C_ANY:
        case C_ANYDEFINEDBY:
        case C_LIST:	// Deepak: following three stmts writes the equivalent C code in header file.
            fprintf (f, "typedef ");
            PrintCType (f, r, m, td, NULL, t);	// Deepak: Prints Basic ASN Data Type like NumericString or PrintableString or ENUMERATED etc...
            fprintf (f, " %s;", ctdi->cTypeName);	// Deepak: Prints User Defined ASN Data Type like Order-number, Item-code etc...
            PrintTypeComment (f, td, t);	// Deepak: actual asn code line is written in comments here
            fprintf (f, "\n\n");
            break;


        case C_CHOICE:
            PrintCChoiceTypeDef (f, r,  m, td);
            break;

        case C_STRUCT:
            fprintf (f, "typedef ");
            fprintf (f,"%s %s", "struct", t->cTypeRefInfo->cTypeName);
            PrintTypeComment (f, td, t);
            fprintf (f,"\n{\n");
            PrintCStructElmts (f, r, m, td, NULL, t);
            fprintf (f, "} %s;", ctdi->cTypeName);
            fprintf (f, "\n\n");
            break;

        case C_OBJECTCLASS:		// Deepak: 11/Mar/2003
            fprintf (f, "typedef ");
            fprintf (f,"%s %s", "struct", t->cTypeRefInfo->cTypeName);
            PrintTypeComment (f, td, t);
            fprintf (f,"\n{\n");
			
            PrintCObjectClassElmts (f, r, m, td, NULL, t);
            fprintf (f, "} %s;", ctdi->cTypeName);
            fprintf (f, "\n\n");
            break;
		case C_MACROTYPE:
            fprintf (f, "typedef ");
            fprintf (f,"%s %s", "struct", t->cTypeRefInfo->cTypeName);
            PrintTypeComment (f, td, t);
            fprintf (f,"\n{\n");
            PrintCMacroTypeElmts (f, r, m, td, NULL, t);
            fprintf (f, "} %s;", ctdi->cTypeName);
            fprintf (f, "\n\n");
			break;
        default:
            break;
        /* else do nothing - some unprocessed or unknown type (macros etc) */
    }

}  /* PrintCTypeDef */



static void
PrintCType PARAMS ((f, r, m, td, parent, t),
    FILE *f _AND_
    CRules *r _AND_
    Module *m _AND_
    TypeDef *td _AND_
    Type *parent _AND_
    Type *t)
{
    CTRI *ctri;
    CNamedElmt *n;

    ctri = t->cTypeRefInfo;

    if (ctri == NULL)
        return;

	if(t->basicType->choiceId == BASICTYPE_OBJECTCLASSFIELDTYPE)
	{	// Deepak: 01/Apr/2003
		fprintf (f,"void* ");
		return;
	}

    switch (ctri->cTypeId)
    {
        case C_TYPEREF:
            /*
             * put struct in front of def if
             * defined from a struct type (set/seq/choice)
             * but only if not a ref of a ref
             */
            if ((t->basicType->a.localTypeRef->link->type->cTypeRefInfo->cTypeId == C_STRUCT)||
                 (t->basicType->a.localTypeRef->link->type->cTypeRefInfo->cTypeId == C_CHOICE))
            {
                fprintf (f,"struct ");
            }

            fprintf (f,"%s", ctri->cTypeName);

            if (ctri->isPtr)
                fprintf (f,"*");
            break;

        case C_ANY:
            fprintf (f,"/* ANY- Fix Me ! */\n");
        case C_ANYDEFINEDBY:
        case C_LIST:
        case C_LIB:
            fprintf (f,"%s", ctri->cTypeName);
            /*
             * print enum constant defs
             */
            if ((ctri->cNamedElmts != NULL) &&
                (t->basicType->choiceId  == BASICTYPE_ENUMERATED))
            {
                fprintf (f, "\n    {\n");

                FOR_EACH_LIST_ELMT (n, ctri->cNamedElmts)
                {
                    fprintf (f,"        %s = %d", n->name, n->value);
                    if (n != (CNamedElmt*)LAST_LIST_ELMT (ctri->cNamedElmts))
                        fprintf (f,",");

                    fprintf (f,"\n");
                }
                fprintf (f, "    }");
            }

            if (ctri->isPtr)
               fprintf (f,"*");
            break;

       default:
           break;
             /* nothing */

    }
    parent = parent;td = td;m = m;r = r;    /*AVOIDS warning.*/

}  /* PrintCType */


static void
PrintCStructElmts PARAMS ((f, r, m, td, parent, t),
    FILE *f _AND_
    CRules *r _AND_
    Module *m _AND_
    TypeDef *td _AND_
    Type *parent _AND_
    Type *t)
{
    CTRI *ctri;
    NamedType *et;
    NamedTypeList *elmts;

    elmts = t->basicType->a.sequence;

    if ((elmts == NULL) || (LIST_EMPTY (elmts)))
    {
        fprintf (f, "    char unused; /* empty ASN1 SET/SEQ - not used */\n");
    }

    FOR_EACH_LIST_ELMT (et, elmts)
    {

        ctri =  et->type->cTypeRefInfo;
        fprintf (f,"\t");  /* cheap, fixed indent */
        PrintCType (f, r, m, td, t, et->type);
        fprintf (f, " %s;", ctri->cFieldName);	// Deepak: identifier of the structure
        PrintTypeComment (f, td, et->type);	// Deepak: actual asn code line is written in comments here
        fprintf (f, "\n");
    }
    parent = parent; /*AVOIDS warning.*/
}  /* PrintCStructElmts */


static void		// Deepak: 17/Apr/2003
PrintCMacroTypeElmts PARAMS ((f, r, m, td, parent, t),
    FILE *f _AND_
    CRules *r _AND_
    Module *m _AND_
    TypeDef *td _AND_
    Type *parent _AND_
    Type *t)
{

    switch (t->basicType->a.macroType->choiceId)
    {	// This switch case copied from do-macros.c
	case MACROTYPE_ASNABSTRACTOPERATION:
	case MACROTYPE_ROSOPERATION:
		
		PrintCMacroRosOperationElmts (f, r, m, td, parent, t, t->basicType->a.macroType->a.rosOperation);
		break;
		
	// Other Macro Types are not supported as for now as per changes done on 17/Apr/2003 
	// Add code for other macro types here

	default:
		// Unsupported Macro Type
		break;
	}

    parent=parent; /*AVOIDS warning.*/
}  /* PrintCMacroTypeElmts */


static void		// Deepak: 17/Apr/2003
PrintCMacroRosOperationElmts PARAMS ((f, r, m, td, parent, t, op),
    FILE *f _AND_
    CRules *r _AND_
    Module *m _AND_
    TypeDef *td _AND_
    Type *parent _AND_
    Type *t _AND_
	RosOperationMacroType *op)
{
    CTRI *ctri;
    NamedType *et;
	int digit = 1;

    if (op->arguments != NULL)
    {   
		et = op->arguments;
		if(et->type->basicType->choiceId == BASICTYPE_LOCALTYPEREF)
		{
			ctri =  et->type->cTypeRefInfo;
			fprintf (f,"\t");  /* cheap, fixed indent */
			PrintCType (f, r, m, NULL, NULL, et->type);
			ctri->cFieldName = et->type->basicType->a.localTypeRef->typeName;
			ctri->cFieldName[0] = (char)tolower (ctri->cFieldName[0]);
			AppendDigit (ctri->cFieldName, digit++);

			fprintf (f, " %s;", ctri->cFieldName);	// Deepak: identifier of the structure
			PrintTypeComment (f, td, et->type);	// Deepak: actual asn code line is written in comments here
			fprintf (f, "\n");
		}
	}

    if (op->result != NULL)
	{
		et = op->result;
		if(et->type->basicType->choiceId == BASICTYPE_LOCALTYPEREF)
		{
			ctri =  et->type->cTypeRefInfo;
			fprintf (f,"\t");  /* cheap, fixed indent */
			PrintCType (f, r, m, NULL, NULL, et->type);
			ctri->cFieldName = et->type->basicType->a.localTypeRef->typeName;
			ctri->cFieldName[0] = (char)tolower (ctri->cFieldName[0]);
			AppendDigit (ctri->cFieldName, digit++);

			fprintf (f, " %s;", ctri->cFieldName);	// Deepak: identifier of the structure
			PrintTypeComment (f, td, et->type);	// Deepak: actual asn code line is written in comments here
			fprintf (f, "\n");
		}
	}
	parent=parent;
	t=t;
}  /* PrintCMacroRosOperationElmts */

static void			   // Deepak: 11/Mar/2003
PrintCObjectClassElmts PARAMS ((f, r, m, td, parent, t),
    FILE *f _AND_
    CRules *r _AND_
    Module *m _AND_
    TypeDef *td _AND_
    Type *parent _AND_
    Type *t)
{
    CTRI *ctri;
    NamedType *et;
    NamedTypeList *elmts;
	int bOptionalElmtsStructWritten = FALSE;
	
    elmts = t->basicType->a.objectclass->classdef;

    if ((elmts == NULL) || (LIST_EMPTY (elmts)))
    {
        fprintf (f, "    char unused; /* empty ASN1 SET/SEQ - not used */\n");
    }

	// for struct enum of optional variables	// Deepak: 14/Mar/2003
    FOR_EACH_LIST_ELMT (et, elmts)
	{
		if(et->type->optional)
		{
			if(!bOptionalElmtsStructWritten)
			{
				bOptionalElmtsStructWritten = TRUE;
				fprintf (f,"\tstruct {\n");
			}
			fprintf (f,"\t\tunsigned int %sPresent : 1;\n", et->fieldName);
		}
	}
	if(bOptionalElmtsStructWritten)
		fprintf (f,"\t} m;\n\n");

    FOR_EACH_LIST_ELMT (et, elmts)
    {
        ctri =  et->type->cTypeRefInfo;
        
		
		/* ======== Deepak: 17/Mar/2003 Print Encode/Decode Functions for BASICTYPE_UNKNOWN ========*/
		if(et->type->basicType->choiceId == BASICTYPE_UNKNOWN)
		{
			fprintf (f, "\n    /* ========== %s: BASICTYPE_UNKNOWN ========== */\n", ctri->cFieldName);
			//fprintf (f, "\tchar* %sName;\n", ctri->cFieldName);
			fprintf (f, "\tvoid* %s;\n", ctri->cFieldName);
			fprintf (f, "\t%s %sSize;\n", returnTypeG, ctri->cFieldName);
			fprintf (f, "\t%s (*encode%s) (%s b, void *%s);\n", returnTypeG, ctri->cFieldName, bufTypeNameG, valueArgNameG);
			fprintf (f, "\t%s (*decode%s) (%s b, %s tagId0, %s elmtLen0, void *%s, %s *bytesDecoded);\n", returnTypeG, ctri->cFieldName, bufTypeNameG, tagTypeNameG, lenTypeNameG, valueArgNameG, lenTypeNameG);
			continue;
		}
		/* ======== Till here ======== */

		fprintf (f,"\t");  /* cheap, fixed indent */
        PrintCType (f, r, m, td, t, et->type);
        fprintf (f, " %s;", ctri->cFieldName);	// Deepak: identifier of the structure
        PrintTypeComment (f, td, et->type);	// Deepak: actual asn code line is written in comments here
        fprintf (f, "\n");
    }
//	fprintf (f, "\tAsnInt tagId;\n");
    parent=parent; /*AVOIDS warning.*/
}  /* PrintCObjectClassElmts */


static void
PrintCChoiceIdEnum PARAMS ((f, r, m, td, parent, t),
    FILE *f _AND_
    CRules *r _AND_
    Module *m _AND_
    TypeDef *td _AND_
    Type *parent _AND_
    Type *t)
{
    NamedType *et;
    NamedType *last=NULL;
    CTRI *ctri;

    ctri =  t->cTypeRefInfo;
    fprintf (f, "    enum %s\n    {\n", ctri->choiceIdEnumName);

    if ((t->basicType->a.choice != NULL) &&
        !(LIST_EMPTY (t->basicType->a.choice)))
        last = (NamedType*)LAST_LIST_ELMT (t->basicType->a.choice);

    FOR_EACH_LIST_ELMT (et, t->basicType->a.choice)
    {
        ctri =  et->type->cTypeRefInfo;
        fprintf (f,"        %s", ctri->choiceIdSymbol);
        if (et == last)
            fprintf (f, "\n");
        else
            fprintf (f, ",\n");
    }

    ctri =  t->cTypeRefInfo;
    fprintf (f, "    } %s;", ctri->choiceIdEnumFieldName);

    parent = parent;td = td;m = m;r = r; /*AVOIDS warning.*/
}  /* PrintCChoiceIdEnum */


static void
PrintCChoiceUnion PARAMS ((f, r, m, td, parent, t),
    FILE *f _AND_
    CRules *r _AND_
    Module *m _AND_
    TypeDef *td _AND_
    Type *parent _AND_
    Type *t)
{
    CTRI *ctri;
    ctri = t->cTypeRefInfo;

    fprintf (f,"    union %s\n    {\n",  ctri->cTypeName);
    PrintCStructElmts (f, r, m, td, parent, t);
    fprintf (f, "    }");
}  /* PrintCChoiceUnion */


static void
PrintCChoiceTypeDef PARAMS ((f, r, m, td),
    FILE *f _AND_
    CRules *r _AND_
    Module *m _AND_
    TypeDef *td)
{
    CTRI *ctri;
    char *choiceName;
    Type *t;

    t = td->type;
    ctri =  t->cTypeRefInfo;
    choiceName = td->cTypeDefInfo->cTypeName;

    fprintf (f, "typedef ");
    fprintf (f, "struct %s", choiceName);
    PrintTypeComment (f, td, t);
    fprintf (f,"\n{\n");
    PrintCChoiceIdEnum (f, r, m, td, NULL, t);
    fprintf (f,"\n");
    PrintCChoiceUnion (f, r, m, td, NULL, t);
    fprintf (f, " %s;", ctri->cFieldName);
    fprintf (f,"\n} %s;\n\n", choiceName);
}  /* PrintCChoiceDef */



/*
 * used to print snippet of the defining ASN.1  after the
 * C type.
 */
static void
PrintTypeComment PARAMS ((f, td, t),
    FILE *f _AND_
    TypeDef *td _AND_
    Type *t)
{
    fprintf (f," /* ");
    SpecialPrintType (f, td, t);
    fprintf (f," */");
}



/*
 * print any #defines for integers/bits with named elements
 * (currenly only the first option will fire due to the
 *  steps taken in normalize.c)
 */
static void
PrintPreTypeDefStuff PARAMS ((f, r, m, td, parent, t),
    FILE *f _AND_
    CRules *r _AND_
    Module *m _AND_
    TypeDef *td _AND_
    Type *parent _AND_
    Type *t)
{
    CTRI *ctri;
    NamedType *et;
    CNamedElmt *n;

    ctri = td->type->cTypeRefInfo;

    /*
     * print defined stmts for non enumerated type with named elmts
     */
    if ((ctri->cNamedElmts != NULL) &&
        (t->basicType->choiceId  != BASICTYPE_ENUMERATED))
    {
        FOR_EACH_LIST_ELMT (n, ctri->cNamedElmts)
        {
            fprintf(f, "\n#define %s %d", n->name, n->value);
        }
        fprintf (f, "\n\n");
    }

    else if ((t->basicType->choiceId == BASICTYPE_SET) ||
             (t->basicType->choiceId == BASICTYPE_SEQUENCE) ||
             (t->basicType->choiceId == BASICTYPE_CHOICE))
    {

        FOR_EACH_LIST_ELMT (et, t->basicType->a.set)
            PrintPreTypeDefStuff (f, r, m, td, t, et->type);
    }

    else if ((t->basicType->choiceId == BASICTYPE_SETOF) ||
             (t->basicType->choiceId == BASICTYPE_SEQUENCEOF))
    {
        PrintPreTypeDefStuff (f, r,  m, td, t, t->basicType->a.setOf);
    }
    parent = parent; /*AVOIDS warning.*/
}  /* PrintPreTypeDefStuff */
