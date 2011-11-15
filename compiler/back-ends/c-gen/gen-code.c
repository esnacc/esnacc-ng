/*
 * compiler/back-ends/c-gen/gen-code.c - generate C hdr and src files
 *
 * Assumes you have called FillCTypeInfo
 *
 * MS 92
 * Copyright (C) 1991, 1992 Michael Sample
 *            and the University of British Columbia
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * $Header: /baseline/SNACC/compiler/back-ends/c-gen/gen-code.c,v 1.17 2004/03/25 19:20:16 gronej Exp $
 * $Log: gen-code.c,v $
 * Revision 1.17  2004/03/25 19:20:16  gronej
 * fixed some linux warnings
 *
 * Revision 1.16  2004/03/12 18:51:20  gronej
 * updated c-library to error on extension additions as it does with untagged elements
 *
 * Revision 1.15  2004/01/14 19:07:53  gronej
 * Updated Compiler to accept and process relative-oid's
 *
 * Revision 1.14  2003/12/17 19:05:04  gronej
 * SNACC baseline merged with PER v1_7 tag
 *
 * Revision 1.13.2.1  2003/11/05 14:58:57  gronej
 * working PER code merged with esnacc_1_6
 *
 * Revision 1.13  2003/07/30 15:20:22  colestor
 * Added "#pragma" to disable MS Windows compiler warning on produced .c
 * source file for unused local variables.
 *
 * Revision 1.12  2003/07/30 00:48:15  colestor
 * Removed final init_* method declarations.  Also, for "C" code generation, to move
 * all IMPORT include references to the module .h file (as in C++).  This allows the resulting
 * .c source to properly build with appropriately ordered IMPORT references.
 *
 * Revision 1.11  2003/07/28 11:13:51  colestor
 * Changes to complete handing of the "--snacc namespace" compiler directive.
 * Also, updates to handle ASN.1 constant integer tag designations for C++/C.
 *
 * Revision 1.10  2003/07/07 14:53:38  nicholar
 * Eliminated headers and cleaned up include references
 *
 * Revision 1.9  2003/04/29 21:05:15  leonberp
 * integerated Deepak's changes for IOB support
 *
 * Revision 1.8  2003/02/07 17:03:12  leonberp
 * added #ifdef __cplusplus extern "C"
 *
 * Revision 1.7  2002/11/01 16:19:54  mcphersc
 * Fixed compiler warnings and errors not caught by microsoft
 *
 * Revision 1.6  2002/10/30 13:39:56  mcphersc
 * added include references to sources include file, and only produce src and include files for non imported files - files not referenced by the -I option
 *
 * Revision 1.5  2002/10/07 13:45:25  mcphersc
 * When a "-u filename" is on the argument line, the executable will go into an infinite
 * loop. This fixes that problem
 *
 * Revision 1.4  2002/10/01 13:51:34  mcphersc
 * Modified snacc to accept either the -I import option or the original way to list the
 * asn modules on the command line
 *
 * Revision 1.3  2002/09/04 17:59:34  vracarl
 * got rid of c++ comments
 *
 * Revision 1.2  2000/10/24 14:54:46  rwc
 * Updated to remove high-level warnings (level 4 on MSVC++) for an easier build.
 * SOME warnings persist due to difficulty in modifying the SNACC compiler to
 * properly build clean source; also some files are built by Lex/Yacc.
 *
 * Revision 1.1.1.1  2000/08/21 20:36:05  leonberp
 * First CVS Version of SNACC.
 *
 * Revision 1.4  1995/07/25 18:39:46  rj
 * file name has been shortened for redundant part: c-gen/gen-c-code -> c-gen/gen-code.
 *
 * PrintConditionalIncludeOpen() and PrintConditionalIncludeClose() moved to back-ends/cond.c
 *
 * changed `_' to `-' in file names.
 *
 * Revision 1.3  1995/02/18  12:50:53  rj
 * typo fixed.
 *
 * Revision 1.2  1994/09/01  00:21:54  rj
 * snacc_config.h and other superfluous .h files removed.
 *
 * Revision 1.1  1994/08/28  09:48:17  rj
 * first check-in. for a list of changes to the snacc-1.1 distribution please refer to the ChangeLog.
 *
 */

#include <time.h>
#include <string.h>
#include "asn-incl.h"
#include "asn1module.h"
#include "rules.h"
#include "str-util.h"
#include "enc-rules.h"
#include "lib-types.h"
#include "tag-util.h"


/* Function Prototypes */
char *Code2UnivCodeStr PROTO ((BER_UNIV_CODE code));
void PrintCAnyCode(FILE *src, FILE *hdr, CRules *r, ModuleList *mods, 
				   Module *m, int printEncoders, int printDecoders, 
				   int printPrinters, int printFree);
void PrintCContentDecoder PROTO ((FILE *src, FILE *hdr, CRules *r, Module *m,
								 TypeDef *td, long int *longJmpVal));
void PrintCContentEncoder PROTO ((FILE *src, FILE *hdr, CRules *r, Module *m,
								 TypeDef *td));
void PrintCDecoder PROTO ((FILE *src, FILE *hdr, CRules *r, Module *m,
						  TypeDef *td, long int *longJmpVal));
void PrintCEncoder PROTO ((FILE *src, FILE *hdr, CRules *r, Module *m,
						  TypeDef *td));
void PrintCFree PROTO ((FILE *src, FILE *hdr, CRules *r, ModuleList *mods,
					   Module *m, TypeDef *td));
void PrintCPrinter PROTO ((FILE *src, FILE *hdr, CRules *r, ModuleList *mods,
						  Module *m, TypeDef *td));
void PrintCTypeDef PROTO ((FILE *f, CRules *r, Module *m, TypeDef *td));
void PrintCValueDef PROTO ((FILE *src, CRules *r, ValueDef *v));
void PrintCValueExtern  PROTO ((FILE *hdr, CRules *r, ValueDef *v));
void PrintCValueInstantiation PROTO ((FILE *hdr, CRules *r, Value *v));
void PrintConditionalIncludeOpen PROTO ((FILE *f, char *fileName));
void PrintConditionalIncludeClose PROTO ((FILE *f, char *fileName));


/* Global Variables */
extern int isTableConstraintAllowed;	/* defined in snacc.c */


/* unexported prototypes */
static void PrintCSrcComment PROTO ((FILE *src, Module *m));
static void PrintCSrcIncludes PROTO ((FILE *src, Module *m, ModuleList *mods));
static void PrintCHdrComment PROTO ((FILE *hdr, Module *m));
static void PrintCHdrObjectDeclaration_and_Init PROTO ((FILE *hdr, Module *m, CRules *r));
//RWC;static void PrintCHdrObjectField PROTO ((FILE *hdr, Module *m, CRules *r, char *objName, ObjectAssignmentField *oaf));
//extern short ImportedFilesG;
/*
 * Fills the hdr file with the C type and encode/decode prototypes
 * Fills the src file with the encoded/decode routine definitions
 */
void
PrintCCode PARAMS ((src, hdr, mods, m, r, longJmpVal, printTypes, printValues, printEncoders, printDecoders, printPrinters, printFree),
    FILE *src _AND_
    FILE *hdr _AND_
    ModuleList *mods _AND_
    Module *m _AND_
    CRules *r _AND_
    long int longJmpVal _AND_
    int printTypes _AND_
    int printValues _AND_
    int printEncoders _AND_
    int printDecoders _AND_
    int printPrinters _AND_
    int printFree)
{
    TypeDef *td;
    ValueDef *vd;

	/* Deepak: suppose the asn source file is test.asn
	 * then the C source file name is test.c and C header file is test.h
	 */
    PrintCSrcComment (src, m);	// Deepak: Write the comments in the source file.
    PrintCSrcIncludes (hdr/*RWC;src*/, m, mods);	// Deepak: #include "asn-incl.h" and #include "test.h".

    PrintCHdrComment (hdr, m);	// Deepak: Write the comments in the header file.
    PrintConditionalIncludeOpen (hdr, m->cHdrFileName);	

	/* Deepak: the above fn writes 
	 * #ifndef _test_h_ 
	 * #define _test_h_
     */

	/* PIERCE TBD: Is this necessary still after Deepak's mods?
     *
	 * Add include reference to source file
	 * 
     */
    fprintf (src, "#include \"%s\"\n", m->cHdrFileName);
    fprintf (hdr,"\n\n");
    fprintf (hdr,"#ifdef __cplusplus\n");
    fprintf (hdr,"extern \"C\" {\n");
    fprintf (hdr,"#endif\n");
    // RWC; ADDED to remove warning about "unreferenced local variable" for 
    //   variables that are hardcoded by the eSNACC compiler; depending on 
    //   the recursed data types (e.g. in a CHOICE), the variables may not be
    //   used.  We just ignore the warning.
    fprintf (hdr,"#ifdef _WIN32\n");
    fprintf (hdr,"#pragma warning( disable : 4101 )\n");
    fprintf (hdr,"#endif\n");

    fprintf (src,"\n\n");

    if (printValues)
    {
        /* put value defs at beginning of .c file */
        FOR_EACH_LIST_ELMT (vd, m->valueDefs)
        {
            PrintCValueDef (src, r, vd);
        }
    }

    PrintCAnyCode (src, hdr, r, mods, m, printEncoders, printDecoders, 
		printPrinters, printFree);

    FOR_EACH_LIST_ELMT (td, m->typeDefs)
    {
        if (printTypes)
            PrintCTypeDef (hdr, r, m, td);

        /* for PDU type or types ref'd with ANY/ANY DEF BY */
        if (printEncoders && ((td->anyRefs != NULL) || td->cTypeDefInfo->isPdu))
            PrintCEncoder (src, hdr, r, m, td);

        /* for PDU type or types ref'd with ANY/ANY DEF BY */
        if (printDecoders && ((td->anyRefs != NULL) || td->cTypeDefInfo->isPdu))
            PrintCDecoder (src, hdr, r, m, td, &longJmpVal);

        if (printEncoders)
		{
            PrintCContentEncoder (src, hdr, r, m, td);
			//if (td->bHasTableConstraint)
			//	PrintCTableConstraintEncoder (src, hdr, m, td);		// Deepak: 25/Mar/2003
		}

        if (printDecoders)
		{
            PrintCContentDecoder (src, hdr, r, m, td, &longJmpVal);
			//if (td->bHasTableConstraint)
			//	PrintCTableConstraintDecoder (src, hdr, m, td);		// Deepak: 25/Mar/2003
		}

        if (printPrinters)
            PrintCPrinter (src, hdr, r, mods, m, td);

        if (printFree)
            PrintCFree (src, hdr, r, mods, m, td);

        /* only print new lines for normal types */
        switch (td->type->basicType->choiceId)
        {
            case BASICTYPE_SEQUENCEOF:  /* list types */
            case BASICTYPE_SETOF:
            case BASICTYPE_CHOICE:
            case BASICTYPE_SET:
            case BASICTYPE_SEQUENCE:
			case BASICTYPE_SEQUENCET:	// Deepak: 30/Nov/2002
			case BASICTYPE_OBJECTCLASS:	// Deepak: 14/Mar/2003
                fprintf (src, "\n");
                /* fall through */

            case BASICTYPE_IMPORTTYPEREF:  /* type references */
            case BASICTYPE_LOCALTYPEREF:
            case BASICTYPE_BOOLEAN:  /* library type */
            case BASICTYPE_REAL:  /* library type */
            case BASICTYPE_OCTETSTRING:  /* library type */
            case BASICTYPE_NULL:  /* library type */
            case BASICTYPE_OID:  /* library type */
            case BASICTYPE_RELATIVE_OID: /* library type */
            case BASICTYPE_INTEGER:  /* library type */
            case BASICTYPE_BITSTRING:  /* library type */
            case BASICTYPE_ENUMERATED:  /* library type */
            case BASICTYPE_ANYDEFINEDBY:  /* ANY types */
            case BASICTYPE_ANY:
			case BASICTYPE_NUMERIC_STR:		/* library type */
			case BASICTYPE_PRINTABLE_STR:	/* library type */
			case BASICTYPE_IA5_STR:			/* library type */
			case BASICTYPE_BMP_STR:			/* library type */
			case BASICTYPE_UNIVERSAL_STR:	/* library type */
			case BASICTYPE_UTF8_STR:		/* library type */
			case BASICTYPE_T61_STR:			/* library type */
                fprintf (hdr, "\n");
                break;
		
	default:
	  break;
        }
    }

	// Declare ObjectAssignment, ObjestSetAssignment, & initialize them.
	if(isTableConstraintAllowed)
	{
		PrintCHdrObjectDeclaration_and_Init (hdr, m, r);	// Deepak: 24/Mar/2003
	}

    if (printValues)
    {
        /* put value externs at end of .h file */
        FOR_EACH_LIST_ELMT (vd, m->valueDefs)
        {
            PrintCValueExtern (hdr, r, vd);
        }
    }

    fprintf (hdr,"#ifdef __cplusplus\n");
    fprintf (hdr,"extern \"C\" {\n");
    fprintf (hdr,"#endif\n");


    PrintConditionalIncludeClose (hdr, m->cHdrFileName);

} /* PrintCCode */

void
PrintConstraintValueCheckingCode PARAMS ((src, td, t, nt),
	FILE *src _AND_
	TypeDef *td _AND_
	Type *t _AND_
	NamedType *nt)
{
	if(t == NULL || nt == NULL)
		return;

	fprintf (src, "\tfor(index=0; index < %s_Size; index++)\n", t->tableConstraint->objSetAssignment->objectSetName);
	fprintf (src, "\t{\n");
	fprintf (src, "\t\tif(&v->%s == &%s[index].%s)\n", nt->fieldName, t->tableConstraint->objSetAssignment->objectSetName, nt->fieldName);
	fprintf (src, "\t\t\tbreak;\n");
	fprintf (src, "\t}\n");
	fprintf (src, "\n");

	fprintf (src, "\tif(&v->%s == %s_Size)\n", nt->fieldName, t->tableConstraint->objSetAssignment->objectSetName);
	fprintf (src, "\t\treturn 0;\n");
	fprintf (src, "\n");

	td=td; 
} /* PrintConstraintValueCheckingCode */

static void		// Deepak: 24/Mar/2003
PrintCHdrObjectDeclaration_and_Init PARAMS ((hdr, m, r), 
	FILE *hdr _AND_
	Module *m _AND_
	CRules *r)
{
	ObjectAssignment *oa;
	ObjectSetAssignment *osa;
	//RWC;ObjectAssignmentField *oaf;
	//RWC;TypeOrValue* tOrV;
	char* moduleName;
	int osaCount=0, currOsa=0;
	char *osaName="";
	//RWC;char *objName="";
	
	////////////////////////////////////////////////////////////////////////////////////
	/* Declare Object Assignments */		// Deepak: 13/Mar/2003
	fprintf (hdr, "/* ========== Object Declarations ========== */\n");
	FOR_EACH_LIST_ELMT (oa, m->objAssignments)
	{
		fprintf (hdr, "%s %s;\n", Asn1ValueName2CValueName(oa->objectClassName), Asn1ValueName2CValueName(oa->objectName));
	}
	fprintf (hdr, "\n");
	////////////////////////////////////////////////////////////////////////////////////

	////////////////////////////////////////////////////////////////////////////////////
	/* Declare Object Set Assignments */	// Deepak: 13/Mar/2003
	fprintf (hdr, "\n/* ========== Object Set Declarations ========== */\n");
	FOR_EACH_LIST_ELMT (osa, m->objSetAssignments)
	{
		currOsa = 0;
		osaCount = osa->objectNameList->count;
		osaName = Asn1ValueName2CValueName(osa->objectSetName);
		fprintf (hdr, "%s %s[%d];\n", Asn1ValueName2CValueName(osa->objectClassName), osaName, osaCount);
		fprintf (hdr, "int %s_Size = %d;\n", osaName, osaCount);
		fprintf (hdr, "\n");
	}
	////////////////////////////////////////////////////////////////////////////////////

	////////////////////////////////////////////////////////////////////////////////////
	// Init Module Objects
	moduleName = Asn1ValueName2CValueName(m->modId->name);
	//RWC;REMOVED;fprintf (hdr, "/* ========== init_%sObjects() ========== */\n", moduleName);
    /*RWC;REMOVED;
	fprintf (hdr, "void init_%sObjects()\n", moduleName);
	fprintf (hdr, "{\n");
	
	FOR_EACH_LIST_ELMT (oa, m->objAssignments)
	{
		objName = Asn1ValueName2CValueName(oa->objectName);
		FOR_EACH_LIST_ELMT (oaf, oa->objectAssignmentField)
		{
			PrintCHdrObjectField (hdr, m, r, objName, oaf);
		}
		fprintf (hdr, "\n");
	}
	fprintf (hdr, "}\n");
	fprintf (hdr, "\n");
	////////////////////////////////////////////////////////////////////////////////////

	////////////////////////////////////////////////////////////////////////////////////
	// Init Module Object Sets
	
	FOR_EACH_LIST_ELMT (osa, m->objSetAssignments)
	{
		currOsa = 0;
		osaCount = osa->objectNameList->count;
		osaName = Asn1ValueName2CValueName(osa->objectSetName);
		fprintf (hdr, "void init_%s()\n", osaName);
		fprintf (hdr, "{\n");
		FOR_EACH_LIST_ELMT (tOrV, osa->objectNameList)		// Deepak: 24/Mar/2003
		{
			if(tOrV->choiceId)
				oa = tOrV->a.value->basicValue->a.objAssignment;
			else	// this tOrV can never be type as per my knowledge.
				continue;

			sprintf(osaName, "%s[%d]", Asn1ValueName2CValueName(osa->objectSetName), currOsa);
			FOR_EACH_LIST_ELMT (oaf, oa->objectAssignmentField)
			{
				PrintCHdrObjectField (hdr, m, r, osaName, oaf);
			}
			fprintf (hdr, "\n");
			currOsa++;
		}
		fprintf (hdr, "}\n");
		fprintf (hdr, "\n");
	}
	////////////////////////////////////////////////////////////////////////////////////

	////////////////////////////////////////////////////////////////////////////////////
	// Module Init
       
    
	fprintf (hdr, "void %s_init()\n", moduleName);
	fprintf (hdr, "{\n");
	fprintf (hdr, "\tinit_%sValue();\n", moduleName);
	fprintf (hdr, "\tinit_%sObjects();\n", moduleName);
	FOR_EACH_LIST_ELMT (osa, m->objSetAssignments)
	{
		fprintf (hdr, "\tinit_%s()\n", Asn1ValueName2CValueName(osa->objectSetName));
	}
	fprintf (hdr, "}\n");
	fprintf (hdr, "\n");
	////////////////////////////////////////////////////////////////////////////////////
     *RWC;REMOVED;*/

	r=r;

} /* PrintCHdrObjectDeclaration_and_Init */


/*RWC;REMOVED;
static void		// Deepak: 24/Mar/2003
PrintCHdrObjectField PARAMS ((hdr, m, r, objName, oaf), 
	FILE *hdr _AND_
	Module *m _AND_
	CRules *r _AND_
	char *objName _AND_
	ObjectAssignmentField *oaf)
{
	char *objFieldName="";
	objFieldName = Asn1ValueName2CValueName(oaf->objectFieldName);
	if(oaf->bPresent && oaf->typeOrValue->choiceId)	
	{	// value
		fprintf (hdr, "\t%s.%s = ", objName, objFieldName);
		PrintCValueInstantiation (hdr, r, oaf->typeOrValue->a.value);
		fprintf (hdr, ";\n");
	}
	
	// Print here if optional arguments are present or not
	if(oaf->bOptional)	// Deepak: 22/Mar/2003
	{
		fprintf (hdr, "\t%s.m.%sPresent = %d;\n", objName, objFieldName, oaf->bPresent?1:0);
	}
	
	// Now write the BasicType_Unknown initialisation here...
	if(oaf->bPresent && oaf->bUnknownType)	// Unknown Type	 Deepak: 22/Mar/2003
	{	// value
		char *typeName = Asn1ValueName2CValueName(oaf->typeOrValue->a.type->basicType->a.localTypeRef->typeName);
		//fprintf (hdr, "\t%s.%sName = \"%s\";\n", objName, objFieldName, typeName);
		fprintf (hdr, "\t%s.%s = %s;\n", objName, objFieldName, typeName);
		fprintf (hdr, "\t%s.%sSize = sizeof(%s);\n", objName, objFieldName, typeName);
		fprintf (hdr, "\t%s.encode%s = &%s%s%sContent;\n", objName, objFieldName, GetEncRulePrefix(), r->encodeRoutineBaseName, typeName);
		fprintf (hdr, "\t%s.decode%s = &%s%s%sContent;\n", objName, objFieldName, GetEncRulePrefix(), r->decodeRoutineBaseName, typeName);
//		fprintf (hdr, "\t%s.tag = %d;\n", objName, LIBTYPE_GET_UNIV_TAG_CODE(oaf->typeOrValue->a.type->basicType->a.localTypeRef->link->type->basicType->choiceId));

		typeName = Code2UnivCodeStr(LIBTYPE_GET_UNIV_TAG_CODE(oaf->typeOrValue->a.type->basicType->a.localTypeRef->link->type->basicType->choiceId));
		fprintf (hdr, "\t%s.tagId = %s;\n", objName, typeName);
	}
	m;
} / * PrintCHdrObjectField * /
***RWC;REMOVED **/

static void
PrintCSrcComment PARAMS ((src, m),
    FILE *src _AND_
    Module *m)
{
    long int t;

    t = time (0);
    fprintf (src, "/*\n");
    fprintf (src, " *    %s\n", m->cSrcFileName);
    fprintf (src, " *    \"%s\" ASN.1 module encode/decode/print/free C src.\n", m->modId->name);
    fprintf (src, " *    This file was generated by Coral WinSnacc on %s", ctime (&t));
    fprintf (src, " *    Coral WinSnacc written by Deepak Gupta\n");
    fprintf (src, " *    NOTE: This is a machine generated file - editing not recommended\n");
    fprintf (src, " */\n\n");

} /* PrintSrcComment */

static char *GetImportFileName (char *Impname, ModuleList *mods)
{
	Module *currMod;
	char *fileName = NULL;
    FOR_EACH_LIST_ELMT (currMod, mods)
	{
		/* Find the import Module in the Modules and
		 * return the header file name
		 */
		if ((strcmp(Impname, currMod->modId->name) == 0))
		{
			/* Set the file name and break */
			fileName = currMod->cHdrFileName;
			break;
		}
	}
	return fileName;
}


/* RWC; CHANGED to print to hdr file instead of src file to avoid
*  interaction of multiple include references; this way all files
*  will appropriately pull in their respective references.
*/
static void
PrintCSrcIncludes PARAMS ((inFile, m, mods ),
    FILE *inFile _AND_
    Module *m _AND_
    ModuleList *mods /* _AND_
	char *srcref*/)
{
    void *tmp;
    Module *currMod;
    AsnListNode *currModTmp;

    /*
     * include snacc runtime library related hdrs
     */
    fprintf (inFile, "\n#include \"asn-incl.h\"\n");

    /*
     * print out include files in same order of the module
     * list. every module in the list includes the others and it's
     * own .h
     */
    tmp = (void*)CURR_LIST_NODE (mods);
    FOR_EACH_LIST_ELMT (currMod, mods)
    {
		if ((strcmp(m->cHdrFileName, currMod->cHdrFileName) == 0))
		{
			ImportModuleList *ModLists;
			ImportModule *impMod;
			char *ImpFile = NULL;
			ModLists = currMod->imports;
            currModTmp = mods->curr;    //RWC;FIXES infinite loop problem.
                                        //  IN case changed inside loop.
			FOR_EACH_LIST_ELMT(impMod, ModLists)
			{
				ImpFile = GetImportFileName (impMod->modId->name, mods);
				if (ImpFile != NULL)
					fprintf (inFile, "#include \"%s\"\n", ImpFile);
			}
            mods->curr = currModTmp;    // RWC;RESET loop control
        }
//		if ((ImportedFilesG == FALSE) || (impMod->ImportedFlag == TRUE))
//		{
//			// Only include if Module was exported
//			if (impMod->exportStatus != 0)
//			{
//				// Check that the source header is not part of 
//				// These references.
//				if ((strcmp(impMod->cHdrFileName, srcref) != 0))
//					fprintf (src, "#include \"%s\"\n", impMod->cHdrFileName);
//			}
//		} // endif
 //       fprintf (src, "#include \"%s\"\n", impMod->cHdrFileName);
    }

	//RWC;if (m->cHdrFileName != NULL)
	//RWC;	fprintf (inFile, "#include \"%s\"\n", m->cHdrFileName);


    SET_CURR_LIST_NODE (mods, tmp);
   m=m; /* AVOIDS warning. */
}  /* PrintCSrcIncludes */


static void
PrintCHdrComment PARAMS ((f, m),
    FILE *f _AND_
    Module *m)
{
    long int t;

    t = time (0);
    fprintf (f, "/*\n");
    fprintf (f, " *    %s\n", m->cHdrFileName);
    fprintf (f, " *    \"%s\" ASN.1 module C type definitions and prototypes\n", m->modId->name);
    fprintf (f, " *    This .h file was generated by Coral WinSnacc on %s", ctime (&t));
    fprintf (f, " *    Coral WinSnacc written compiler by Deepak Gupta\n");
    fprintf (f, " *    NOTE: This is a machine generated file--editing not recommended\n");
    fprintf (f, " */\n\n");
} /* PrintCHdrComment */
