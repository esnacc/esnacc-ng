/*
 * compiler/back-ends/c-gen/gen-enc.c - routines for printing c encoders from type trees
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
 * $Header: /baseline/SNACC/compiler/back-ends/c-gen/gen-enc.c,v 1.18 2004/03/25 19:20:16 gronej Exp $
 * $Log: gen-enc.c,v $
 * Revision 1.18  2004/03/25 19:20:16  gronej
 * fixed some linux warnings
 *
 * Revision 1.17  2004/03/12 18:51:20  gronej
 * updated c-library to error on extension additions as it does with untagged elements
 *
 * Revision 1.16  2004/01/14 19:07:53  gronej
 * Updated Compiler to accept and process relative-oid's
 *
 * Revision 1.15  2003/08/04 10:35:23  colestor
 * Updated several improperly referenced "b" buffer parameters when dealing with
 * ANY load/unloads (encode/decodes).  This code has never been tested in the
 * "C" library.
 *
 * Revision 1.14  2003/07/31 18:33:23  colestor
 * Updated to reflect newly added ANY processing as AsnOcts.  All built code will
 * now compile directly.
 *
 * Revision 1.13  2003/07/30 19:01:40  colestor
 * (RWC)Added ANY (not ANY DEFINED BY) default to newly added
 * SetAnyTypeUnknown(...).  (UNTESTED)
 *
 * Revision 1.12  2003/07/28 11:13:51  colestor
 * Changes to complete handing of the "--snacc namespace" compiler directive.
 * Also, updates to handle ASN.1 constant integer tag designations for C++/C.
 *
 * Revision 1.11  2003/07/07 14:53:38  nicholar
 * Eliminated headers and cleaned up include references
 *
 * Revision 1.10  2003/04/29 21:03:24  leonberp
 * integerated Deepak's changes for IOB support
 *
 * Revision 1.9  2003/02/21 18:58:03  leonberp
 * removed for loop from around Asn1fFree(bufs)
 *
 * Revision 1.8  2002/10/22 14:39:40  mcphersc
 * Added modified DER encoding snacc generated code
 *
 * Revision 1.7  2002/10/21 17:15:43  mcphersc
 * fixed long int
 *
 * Revision 1.6  2002/09/16 17:34:54  mcphersc
 * Fixed warnings
 *
 * Revision 1.5  2002/09/04 17:59:33  vracarl
 * got rid of c++ comments
 *
 * Revision 1.4  2002/01/18 16:13:13  vracarl
 * fixed code to put in missing .bitLen code
 *
 * Revision 1.3  2000/10/24 14:54:47  rwc
 * Updated to remove high-level warnings (level 4 on MSVC++) for an easier build.
 * SOME warnings persist due to difficulty in modifying the SNACC compiler to
 * properly build clean source; also some files are built by Lex/Yacc.
 *
 * Revision 1.2  2000/10/16 18:10:42  rwc
 * removed most warnings from C++-lib, some C-lib.
 *
 * Revision 1.1.1.1  2000/08/21 20:36:05  leonberp
 * First CVS Version of SNACC.
 *
 * Revision 1.5  1997/10/23 01:15:55  povey
 * Fixed bug where default values weren't checked properly before encoding
 *
 * Revision 1.4  1997/08/28 07:26:08  povey
 * Changes to support DER encoding/decoding
 *
 * Revision 1.3.1.1  1997/08/20 23:14:41  povey
 *
 * Revision 1.3  1995/07/25 18:42:24  rj
 * file name has been shortened for redundant part: c-gen/gen-c-enc -> c-gen/gen-enc.
 *
 * changed `_' to `-' in file names.
 *
 * Revision 1.2  1994/09/01  00:23:10  rj
 * snacc_config.h and other superfluous .h files removed.
 *
 * Revision 1.1  1994/08/28  09:48:24  rj
 * first check-in. for a list of changes to the snacc-1.1 distribution please refer to the ChangeLog.
 *
 */

/*
	Deepak: Formatting improved as required on 11/Feb/2003
*/
#include "config.h"
#include "asn-incl.h"
#include "asn1module.h"
#include "rules.h"
#include "util.h"
#include "tag-util.h"
#include "snacc-util.h"
#include "enc-rules.h"


EncRulesType GetEncRulesType();
void PrintConstraintValueCheckingCode PROTO ((FILE *src, TypeDef *td, Type *t,
											 NamedType *nt));


//static int moduleImplicitTagsG;
static CRules *genEncCRulesG;
static char* bufNameG = "b";
extern char *valueArgNameG;

char *encodedLenVarNameG = "totalLen";
char *itemLenNameG = "itemLen";
char *listComponentNameG = "component";
char *listLenNameG = "listLen";
char *returnTypeG = "AsnLen";
extern char *bufTypeNameG;
extern char *lenTypeNameG;
extern char *tagTypeNameG;
extern char *envTypeNameG;


void PrintCValueInstantiation PROTO ((FILE *hdr, CRules *r, Value *v));

/* non-exported prototypes */
static void PrintCEncoderPrototype PROTO ((FILE *hdr, TypeDef *td));
static void PrintCEncoderDeclaration PROTO ((FILE *src, TypeDef *td));
static void PrintCEncoderDefine PROTO ((FILE *src, TypeDef *td));

static void PrintCEncoderLocals PROTO ((FILE *src, TypeDef *td));

static void PrintCElmtsEncodeCode PROTO ((FILE *src, TypeDef *td, Type *parent, NamedTypeList *e, int level, char *varName));
static void PrintCElmtEncodeCode PROTO ((FILE *src, TypeDef *td, Type *parent, NamedType *e, int level, char *varName));

static void PrintCListEncoderCode PROTO ((FILE *src, TypeDef *td, Type *t, int level, char *varName));
static void PrintCChoiceEncodeCode PROTO ((FILE *src, TypeDef *td, Type *t, int level, char *varName));

static void PrintCTagAndLenEncodingCode PROTO ((FILE *src, TypeDef *td, Type *t));

static void PrintEocEncoders PROTO ((FILE *src, TypeDef *td, Type *t));

static void PrintCLenEncodingCode PROTO ((FILE *f, int isCons, int isShort));

static void PrintCTagAndLenList PROTO ((FILE *src, Type *t,TagList *tg));

// Deepak:18/Apr/2003
static void PrintCMacroElmtsEncodeCode PROTO ((FILE *src, TypeDef *td, Type *parent, MacroType *mt, int level, char *varName));
static void PrintCRosOperationElmtsEncodeCode PROTO ((FILE *src, TypeDef *td, Type *parent, MacroType *mt, RosOperationMacroType *op, int level, char *varName));
// Deepak: 31/Mar/2003	// Following 4 func's are similar to that in gen-dec.c
static void PrintCEncoderTableConsType PROTO ((FILE *src, FILE *hdr, Module *m, TypeDef *td, Type *t, NamedType *nt));
static void PrintCEncoderTableConsBasicType PROTO ((FILE *src, FILE *hdr, Module *m, TypeDef *td, Type *t, NamedType *nt, BasicType *bt));
static void PrintCEncoderTableConsElmtTypes PROTO ((FILE *src, FILE *hdr, Module *m, TypeDef *td, NamedTypeList *e));
static void PrintCEncoderTableConsElmtType PROTO ((FILE *src, FILE *hdr, Module *m, TypeDef *td, NamedType *n));


void	// Deepak: 25/Mar/2003
PrintCTableConstraintEncoder PARAMS ((src, hdr, m, td),
	FILE *src _AND_
	FILE *hdr _AND_
	Module *m _AND_
	TypeDef *td)
{
	fprintf (hdr, "%s %s%s_TableCons(%s %s,%s *v);\n", returnTypeG, GetEncRulePrefix(), td->cTypeDefInfo->encodeRoutineName, bufTypeNameG, bufNameG, td->cTypeDefInfo->cTypeName);
	fprintf (hdr, "\n");

	fprintf (src, "%s %s%s_TableCons(%s %s,%s *v)\n", returnTypeG, GetEncRulePrefix(), td->cTypeDefInfo->encodeRoutineName, bufTypeNameG, bufNameG, td->cTypeDefInfo->cTypeName);
	fprintf (src, "{\n");
	fprintf (src, "\t%s totalLen = 0;\n", returnTypeG);
	fprintf (src, "\t%s itemLen = 0;\n", returnTypeG);
	fprintf (src, "\t%s index;\n", returnTypeG);
	fprintf (src, "\n");
	
	// Check each field here
	PrintCEncoderTableConsType(src, hdr, m, td, td->type, NULL);

	fprintf (src, "\treturn totalLen;\n");

	fprintf (src,"}  /* %s%s_TableCons */", GetEncRulePrefix(), td->cTypeDefInfo->encodeRoutineName);
	fprintf (src,"\n\n");
} /* PrintCTableConstraintEncoder */

static void 	// Deepak: 25/Mar/2003
PrintCEncoderTableConsType PARAMS ((src, hdr, m, td, t, nt),
	FILE *src _AND_
	FILE *hdr _AND_
	Module *m _AND_
	TypeDef *td _AND_
	Type *t _AND_
	NamedType *nt)
{
    if(t == NULL)
		return;

	PrintCEncoderTableConsBasicType(src, hdr, m, td, t, nt, t->basicType);

} /* PrintCEncoderTableConsType */

static void	// Deepak: 25/Mar/2003
PrintCEncoderTableConsBasicType PARAMS ((src, hdr, m, td, t, nt, bt),
	FILE *src _AND_
	FILE *hdr _AND_
	Module *m _AND_
	TypeDef *td _AND_
	Type *t _AND_
	NamedType *nt _AND_
	BasicType *bt)
{
    if(bt == NULL)
		return;

	switch (bt->choiceId)
    {
		case BASICTYPE_SEQUENCET:		// Deepak: added on 29/Nov/2002
        case BASICTYPE_SEQUENCE:
        case BASICTYPE_SET:
        case BASICTYPE_CHOICE:			// Deepak: process the elements in sequence etc here.
            PrintCEncoderTableConsElmtTypes (src, hdr, m, td, bt->a.set); 
            break;

        case BASICTYPE_SEQUENCEOF:
        case BASICTYPE_SETOF:
            //TypeLinkType (m, currMod, head, bt->a.setOf);
            break;

		case BASICTYPE_INTEGER:
			PrintConstraintValueCheckingCode(src, td, t, nt);
			break;

		case BASICTYPE_OBJECTCLASSFIELDTYPE:
		//	if(nt->type->basicType->choiceId == 0)
			{
				char* objSetName = t->tableConstraint->objSetAssignment->objectSetName;
				char* typeName = nt->type->typeName;
				fprintf (src, "\tif(%s[index].m.%sPresent)\n", objSetName, typeName);
				fprintf (src, "\t{\n");
				fprintf (src, "\t\titemLen = %s[index].m.encode%s(%s,%s[index].%s);\n", objSetName, typeName, bufNameG, objSetName, typeName);
				fprintf (src, "\t\tv->%s.octetLen = itemLen;\n", nt->fieldName);
				fprintf (src, "\t\tv->%s.octs = Asn1Alloc(itemLen);\n", nt->fieldName);
				fprintf (src, "\t\tmemcpy(v->%s.octs,b->dataStart,itemLen);\n", nt->fieldName);
				fprintf (src, "\t\ttotalLen += itemLen;\n");
				fprintf (src, "\t}\n");
				fprintf (src, "\n");
			}
			break;
    default:
      break;
	}
} /* PrintCEncoderTableConsBasicType */


static void	// Deepak: 25/Mar/2003
PrintCEncoderTableConsElmtTypes PARAMS ((src, hdr, m, td, e),
	FILE *src _AND_
	FILE *hdr _AND_
	Module *m _AND_
	TypeDef *td _AND_
	NamedTypeList *e)
{
	NamedType *n;

	FOR_EACH_LIST_ELMT (n, e)
	{
		PrintCEncoderTableConsElmtType (src, hdr, m, td, n);
	}
	
} /* PrintCEncoderTableConsElmtTypes */


static void	// Deepak: 25/Mar/2003
PrintCEncoderTableConsElmtType PARAMS ((src, hdr, m, td, n),
	FILE *src _AND_
	FILE *hdr _AND_
	Module *m _AND_
	TypeDef *td _AND_
	NamedType *n)
{
	if(n->type->tableConstraint)
	{
		fprintf (src, "\t/* Check %s */\n", n->fieldName);
		PrintCEncoderTableConsType (src, hdr, m, td, n->type, n);
	}
} /* PrintCEncoderTableConsElmtType */


void
PrintCEncoder PARAMS ((src, hdr, r, m, td),
    FILE *src _AND_
    FILE *hdr _AND_
    CRules *r _AND_
    Module *m _AND_
    TypeDef *td)
{
    CTDI *ctdi;
    TagList *tags = NULL;
    Tag *tag;
    char *formStr;
    char *classStr;
    int tagLen;
    int stoleChoiceTags;
    EncRulesType* encoding;
    char *pszCode=NULL;

    ctdi = td->cTypeDefInfo;
    if (!ctdi->genEncodeRoutine)
        return;

    /* Generate encoders for each encoding rule required */
    encoding = GetEncRules();
    while (SetEncRules(*encoding)) {
        encoding++;

        /*
         *  if is type that refs another pdu type or lib type
         *  without generating a new type via tagging or named elmts
         *  print define to the hdr file
         * (a type is a pdu by default if it is ref'd by an ANY)
         */
        if (!IsNewType (td->type)  &&
            (!IsTypeRef (td->type) ||
             (IsTypeRef (td->type) &&
              (td->type->basicType->a.localTypeRef->link->cTypeDefInfo->isPdu ||
               ((td->type->basicType->a.localTypeRef->link->anyRefs != NULL) &&
                !LIST_EMPTY (td->type->basicType->a.localTypeRef->link->anyRefs))))))
        {
            fprintf(hdr,"#define %s%s\t%s%s\n", GetEncRulePrefix(),
                    td->cTypeDefInfo->encodeRoutineName, GetEncRulePrefix(),
                    td->type->cTypeRefInfo->encodeRoutineName);
            return;
        }

        fprintf(hdr, "%s %s%s(%s b,%s *v);\n\n", lenTypeNameG,
                GetEncRulePrefix(), ctdi->encodeRoutineName, bufTypeNameG,
                ctdi->cTypeName);

        fprintf(src, "%s %s%s(", lenTypeNameG, GetEncRulePrefix(),
                ctdi->encodeRoutineName);
        fprintf(src, "%s b,",bufTypeNameG);
        fprintf(src, "%s *v)\n",ctdi->cTypeName);
        fprintf(src, "{\n");
        fprintf(src, "    %s l=0;\n", lenTypeNameG);

        PrintEocEncoders (src, td, td->type);

        fprintf (src, "    l = %s%sContent (b,v);\n", GetEncRulePrefix(), 
                 ctdi->encodeRoutineName);

        /* encode each tag/len pair if any */
        tags = GetTags (td->type, &stoleChoiceTags);
        if (! stoleChoiceTags) {
            FOR_EACH_LIST_ELMT_RVS (tag, tags) {
                classStr = Class2ClassStr (tag->tclass);
	    
                if (tag->form == ANY_FORM)
                    tag->form = PRIM;
                formStr = Form2FormStr (tag->form);
                tagLen = TagByteLen (tag->code);

                if (tag->form == CONS)
                    fprintf (src, "    l += %sEncConsLen(b,l);\n", 
                             GetEncRulePrefix());
                else
                    fprintf (src, "    l += %sEncDefLen(b,l);\n",
                             GetEncRulePrefix());
	    
                if (tag->tclass == UNIV)
                    pszCode = DetermineCode(tag, &tagLen, 0);
                else
                    pszCode = DetermineCode(tag, &tagLen, 1);

                fprintf (src, "    l += %sEncTag%d(b,%s,%s,%s);\n", 
                         GetEncRulePrefix(), tagLen, classStr, formStr,
                         pszCode);
            }
        }
        fprintf (src, "    return l;\n");
        fprintf (src, "} /* %s%s */\n\n", GetEncRulePrefix(), ctdi->encodeRoutineName);

        FreeTags (tags);
    }
}  /*  PrintCEncoder */

void
PrintCContentEncoder PARAMS ((src, hdr, r, m, td),
    FILE *src _AND_
    FILE *hdr _AND_
    CRules *r _AND_
    Module *m _AND_
    TypeDef *td)
{
    CTDI *ctdi;
    CTypeId rhsTypeId;  /* cTypeId of the type that defined this typedef */
    EncRulesType* encoding;

    genEncCRulesG = r;

    ctdi =  td->cTypeDefInfo;
    if (!ctdi->genEncodeRoutine)
        return;

    rhsTypeId = td->type->cTypeRefInfo->cTypeId;

    /* Generate encoders for each encoding rule required */
    encoding = GetEncRules();
    while (SetEncRules(*encoding)) {
      encoding++;

      switch(rhsTypeId) {
        case C_ANY:
	  fprintf (hdr, "/* ANY - Fix Me! */\n");
	  
	  /*
	   * Note - ANY's don't have the 'Content' suffix cause they
	   * encode their tags and lengths
	   */
	  fprintf(hdr, "#define %s%s\t%s%s\n", 
		  GetEncRulePrefix(), td->cTypeDefInfo->encodeRoutineName, 
		  GetEncRulePrefix(), td->type->cTypeRefInfo->encodeRoutineName);
	  
	  /*
            fprintf(hdr, "#define %s%s( b, v)  ", GetEncRulePrefix(), td->cTypeDefInfo->encodeRoutineName);
            fprintf (hdr, "%s%s (b, v)", GetEncRulePrefix(), td->type->cTypeRefInfo->encodeRoutineName);
	  */
	  
	  
	  break;
	  
      case C_LIB:
      case C_TYPEREF:
	PrintCEncoderDefine (hdr, td);
	fprintf (hdr,"\n");
	break;
	
	case C_CHOICE:
	  PrintCEncoderPrototype (hdr, td);
	  PrintCEncoderDeclaration (src, td);
	  fprintf (src,"{\n");
	  PrintCEncoderLocals (src, td);
	  fprintf (src,"\n\n");

	// Call Enc_TableCons function here
/*	if(td->bHasTableConstraint)
	{
		fprintf (src,"\tif(!%s%s_TableCons(b, v))\n", GetEncRulePrefix(), td->cTypeDefInfo->encodeRoutineName);
		fprintf (src,"\t\treturn 0;\n\n"); 
	}
  */
	  PrintCChoiceEncodeCode (src, td, td->type, FIRST_LEVEL, valueArgNameG);
	  fprintf (src,"\treturn %s;\n\n", encodedLenVarNameG);
	  fprintf (src,"}  /* %s%sContent */", GetEncRulePrefix(),
		   td->cTypeDefInfo->encodeRoutineName);
	  fprintf (hdr,"\n\n");
	  fprintf (src,"\n\n");
	  break;
	  
      case C_STRUCT:
	PrintCEncoderPrototype (hdr, td);
	PrintCEncoderDeclaration (src, td);
	fprintf (src,"{\n");
	PrintCEncoderLocals (src, td);
	fprintf (src,"\n\n");

	// Call Enc_TableCons function here
/*	if(td->bHasTableConstraint)
	{
		fprintf (src,"\tif(!%s%s_TableCons(b, v))\n", GetEncRulePrefix(), td->cTypeDefInfo->encodeRoutineName);
		fprintf (src,"\treturn 0;\n"); 
	}
  */
	PrintCElmtsEncodeCode (src, td, td->type, td->type->basicType->a.set, FIRST_LEVEL, valueArgNameG);
	fprintf (src,"\treturn %s;\n\n", encodedLenVarNameG);
	fprintf (src,"}  /* %s%sContent */", GetEncRulePrefix(),
		 td->cTypeDefInfo->encodeRoutineName);
	fprintf (hdr,"\n\n");
	fprintf (src,"\n\n");
	break;

	
      case C_LIST:
	PrintCEncoderPrototype (hdr, td);
	fprintf (hdr,"\n\n");
	
	PrintCEncoderDeclaration (src, td);
	fprintf (src,"{\n");
	PrintCEncoderLocals (src, td);
	fprintf (src,"\n\n");

	// Call Enc_TableCons function here
//	if(td->bHasTableConstraint)
//	{
//		fprintf (src,"\tif(!%s%s_TableCons(b, v))\n", GetEncRulePrefix(), td->cTypeDefInfo->encodeRoutineName);
//		fprintf (src,"\treturn 0;\n"); 
//	}

	PrintCListEncoderCode (src, td, td->type, FIRST_LEVEL, valueArgNameG);
	fprintf (src,"\treturn %s;\n\n", listLenNameG);
	fprintf (src,"}  /* %s%sContent */", GetEncRulePrefix(), 
		 td->cTypeDefInfo->encodeRoutineName);
	fprintf (src,"\n\n");
	break;
	

	  case C_MACROTYPE:
		PrintCEncoderPrototype (hdr, td);
		PrintCEncoderDeclaration (src, td);
		fprintf (src,"{\n");
		PrintCEncoderLocals (src, td);
		fprintf (src,"\n\n");

		PrintCMacroElmtsEncodeCode (src, td, td->type, td->type->basicType->a.macroType, FIRST_LEVEL, valueArgNameG);
		fprintf (src,"\treturn %s;\n\n", encodedLenVarNameG);
		fprintf (src,"}  /* %s%sContent */", GetEncRulePrefix(), td->cTypeDefInfo->encodeRoutineName);
		fprintf (hdr,"\n\n");
		fprintf (src,"\n\n");
		  break;


      case C_NO_TYPE:
	/*            fprintf (src," sorry, unsupported type \n\n"); */
	break;
	
      default:
	fprintf (errFileG, "PrintCEncoder: ERROR - unknown c type id\n");
	break;
      }
    } 
	     
    m = m;    /* AVOIDS warning. */
}  /*  PrintCContentEncoder */
    
    
    
    /*
 * Prints prototype for encode routine in hdr file
 */
static void
PrintCEncoderPrototype PARAMS ((hdr, td),
    FILE *hdr _AND_
    TypeDef *td)
{
    CTDI *ctdi;

    ctdi =  td->cTypeDefInfo;
    //fprintf (hdr,"%s %s%sContent PROTO ((%s b, %s *v));", returnTypeG, GetEncRulePrefix(), ctdi->encodeRoutineName, bufTypeNameG, ctdi->cTypeName);
	fprintf (hdr,"%s %s%sContent(%s b,%s *v);", returnTypeG, GetEncRulePrefix(), ctdi->encodeRoutineName, bufTypeNameG, ctdi->cTypeName);

}  /*  PrintCEncoderPrototype */



/*
 * Prints declarations of encode routine for the given type def
 */
static void
PrintCEncoderDeclaration PARAMS ((src, td),
    FILE *src _AND_
    TypeDef *td)
{
    CTDI *ctdi;

	ctdi =  td->cTypeDefInfo;
//	fprintf (src,"%s\n%s%sContent PARAMS ((b, v),\n%s b _AND_\n%s *v)\n", returnTypeG, GetEncRulePrefix(), ctdi->encodeRoutineName, bufTypeNameG, ctdi->cTypeName);ctdi =  td->cTypeDefInfo;
    fprintf (src,"%s %s%sContent(%s b,%s *v)\n", returnTypeG, GetEncRulePrefix(), ctdi->encodeRoutineName, bufTypeNameG, ctdi->cTypeName);

}  /*  PrintCEncoderDeclaration */




/*
 * makes a define for type refs or primitive type renaming
 * EG:
 * TypeX ::= INTEGER --> #define BerEncodeTypeX(b,v) BerEncodeInteger(b,v)
 * TypeX ::= TypeY --> #define BerEncodeTypeX(b,v) BerEncodeTypeY(b,v)
 */
static void
PrintCEncoderDefine PARAMS ((hdr, td),
    FILE *hdr _AND_
    TypeDef *td)
{
    fprintf(hdr, "#define %s%sContent %s%sContent", 
	    GetEncRulePrefix(),  td->cTypeDefInfo->encodeRoutineName, 
	    GetEncRulePrefix(), td->type->cTypeRefInfo->encodeRoutineName);

/*
    fprintf(hdr, "#define %s%sContent( b, v)  ", GetEncRulePrefix(),
    td->cTypeDefInfo->encodeRoutineName);
    fprintf (hdr, "%s%sContent (b, v)", GetEncRulePrefix(),
    td->type->cTypeRefInfo->encodeRoutineName);
*/
}  /*  PrintCEncoderDefine */




static void
PrintCEncoderLocals PARAMS ((src, td),
    FILE *src _AND_
    TypeDef *td)
{
    fprintf (src, "\tAsnLen %s = 0;\n", encodedLenVarNameG);
    fprintf (src, "\tAsnLen %s;\n", itemLenNameG);
    fprintf (src, "\tAsnLen %s;\n", listLenNameG);
    fprintf (src, "\tvoid *%s;", listComponentNameG);

    td = td;    /* AVOIDS warning. */
}  /*  PrintCEncoderLocals */



/* Compare the tags of two NamedTypes */
static int ElmtsTagCmp(const void *a, const void *b) {
  NamedType **at = (NamedType **)a;
  NamedType **bt = (NamedType **)b;

  return CmpTags((*at)->type, (*bt)->type);
}

/*
 * runs through elmts backwards and prints
 * encoding code for each one
 */
static void
PrintCElmtsEncodeCode PARAMS ((src, td, parent, elmts, level, varName),
    FILE *src _AND_
    TypeDef *td _AND_
    Type *parent _AND_
    NamedTypeList *elmts _AND_
    int level _AND_
    char *varName)
{
    NamedType *e;
    NamedType **elmtlist;
    int i=0;

    if (elmts == NULL)
    {
        fprintf (src,"/* ERROR? - expected elmts for this type*/\n");
        return;
    }

    
    /* If the encoding rules to be used are DER and we are generating
     * an encoder for a SET, the SET must be encoded in tag order, therefore
     * we must sort the elements
     */
    if (GetEncRulesType() == DER && 
	parent->basicType->choiceId == BASICTYPE_SET) {
      
      /* Put all the elements in the array */
      elmtlist = (NamedType **)Asn1Alloc(sizeof(NamedType *) * 
					 AsnListCount(elmts));
      FOR_EACH_LIST_ELMT (e, elmts) {
	elmtlist[i]=e;
	i++;
      }

      /* Sort list */
      qsort(elmtlist, i, sizeof(NamedType*), ElmtsTagCmp);
 
      /* Generate encoders (backwards) */
      for (i--; i>=0; i--) {
	PrintCElmtEncodeCode (src, td, parent, elmtlist[i], level, varName);
      }

      /* Free list */
      Asn1Free(elmtlist);
    } else {
      /*
       * remember! encoding "backwards" so recursively traverse
       * list backwards
       */
      FOR_EACH_LIST_ELMT_RVS (e, elmts) 
	  {
		  if(e->type->basicType->choiceId == BASICTYPE_OBJECTCLASSFIELDTYPE)
		  {	// Deepak: 01/Apr/2003
			  fprintf (src, "\t/*~~~~~~~~~~~~~~~~~~ Now encode the OpenType parameter here ~~~~~~~~~~~~~~~~~~*/\n");
			PrintConstraintValueCheckingCode(src, td, e->type, e);
			fprintf (src, "\titemLen = %s[index].encode%s(b,&v->%s);\n", e->type->tableConstraint->objSetAssignment->objectSetName, e->type->typeName, e->fieldName);
			fprintf (src, "\titemLen += BEncConsLen(b,itemLen);\n");
               // RWC;NOTE;  FOR constant value name interpretation to a value for
               // RWC;NOTE;   tags may have to be interpreted before the "%s[index]"
               // RWC;NOTE;   is loaded.  (NOT SURE YET!)
			fprintf (src, "\titemLen += BEncTag1(b,UNIV,PRIM,%s[index].tag);\n", e->type->tableConstraint->objSetAssignment->objectSetName);
			fprintf (src, "\ttotalLen += itemLen;\n");
			fprintf (src, "\n");
			fprintf (src, "\t/*~~~~~~~~~~~~~~~~~~ Till Here ~~~~~~~~~~~~~~~~~~*/\n");
			fprintf (src, "\n");
			continue;
		  }
		PrintCElmtEncodeCode (src, td, parent, e, level, varName);
      }
    }

}  /* PrintCElmtsEncodeCode */



/*
 * Prints code for encoding the elmts of a SEQ or SET
 */
static void
PrintCElmtEncodeCode PARAMS ((src, td, parent, e, level, varName),
    FILE *src _AND_
    TypeDef *td _AND_
    Type *parent _AND_
    NamedType *e _AND_
    int level _AND_
    char *varName)
{
    CTRI *ctri;
    char elmtVarRef[MAX_VAR_REF];
    char idVarRef[MAX_VAR_REF];
    enum BasicTypeChoiceId tmpTypeId;
    Type *tmpType;
    NamedType *idNamedType;
    Value* value;

    if ((e->type == NULL) || (e->type->cTypeRefInfo == NULL) || (e->type->extensionAddition) )
        return;

    ctri =  e->type->cTypeRefInfo;

    /* check if meant to be encoded */
    if (!ctri->isEncDec)
        return;


    MakeVarPtrRef (genEncCRulesG, td, parent, e->type, varName, elmtVarRef);

    /* If we are currently using DER and the value of a component with the 
     * DEFAULT qualifier is the same as the default value, then we should not
     * encode that value 
     */
    if (e->type->defaultVal != NULL) {
      if (GetEncRulesType() == DER) 
	  {
		fprintf (src, "\tif(%s (%s) && (", ctri->optTestRoutineName, elmtVarRef);
		if (GetBuiltinType (e->type) == BASICTYPE_BITSTRING)
		{
		   fprintf (src, "(*%s).bitLen != ", elmtVarRef);
		}
		else
		{
		   fprintf (src, "*%s != ", elmtVarRef);
		}

		/* Get default value */
		switch(e->type->defaultVal->value->basicValue->choiceId) 
		{
		case BASICVALUE_LOCALVALUEREF:
		  value = 
		 e->type->defaultVal->value->basicValue->a.localValueRef->link->value;
		  break;
		case BASICVALUE_IMPORTVALUEREF:
		  value = 
		e->type->defaultVal->value->basicValue->a.importValueRef->link->value;
		  break;
		default:
		  value = e->type->defaultVal->value;
		}

		PrintCValueInstantiation(src, genEncCRulesG, value);
		fprintf (src, " /* DEFAULT */))\n\t{\n");
      } 
	  else 
	  {
		fprintf (src, "\tif(%s (%s))\n\t{\n", ctri->optTestRoutineName, elmtVarRef);
      }
    }

    if (e->type->optional && e->type->defaultVal == NULL)
        fprintf (src, "\tif(%s (%s))\n\t{\n", ctri->optTestRoutineName, elmtVarRef);

    PrintEocEncoders (src, td, e->type);

    switch(ctri->cTypeId)
    {
        case C_ANYDEFINEDBY:

            /* get type of 'defining' field (int/enum/oid)*/
            idNamedType = e->type->basicType->a.anyDefinedBy->link;
            tmpTypeId = GetBuiltinType (idNamedType->type);

            if (tmpTypeId == BASICTYPE_OID || tmpTypeId == BASICTYPE_RELATIVE_OID)
            {
                MakeVarPtrRef (genEncCRulesG, td, parent, idNamedType->type, varName, idVarRef);
                fprintf (src, "\tSetAnyTypeByOid(%s,%s);\n", elmtVarRef, idVarRef);
            }
            else
            {
                /* want to ref int by value not ptr */
                MakeVarValueRef (genEncCRulesG, td, parent, idNamedType->type, varName, idVarRef);
                fprintf (src, "\tSetAnyTypeByInt(%s,%s);\n", elmtVarRef, idVarRef);
            }

            /* ANY's enc's do tag and len so zap the Content suffix */
            fprintf (src, "\t%s = %s%s(b,%s);\n", itemLenNameG, 
		     GetEncRulePrefix(), ctri->encodeRoutineName, elmtVarRef);
            break;

        case C_TYPEREF:
            tmpType = GetType (e->type);

             /* NOTE: ANY DEFINED BY must be directly in the parent (not ref)*/
            if (tmpType->cTypeRefInfo->cTypeId != C_ANY)
            {
				if(tmpType->basicType->choiceId == BASICTYPE_ENUMERATED) // Deepak: 19/Apr/2003 only if added,
					fprintf (src, "\t%s = %s%sContent(b,(AsnInt *)%s);\n", itemLenNameG, GetEncRulePrefix(), ctri->encodeRoutineName, elmtVarRef);
				else
					fprintf (src, "\t%s = %s%sContent(b,%s);\n", itemLenNameG, GetEncRulePrefix(), ctri->encodeRoutineName, elmtVarRef);
                break;
            }
            else  /* fall through */

        case C_ANY:
            /* ANY's enc's do tag and len so zap the Content suffix */
            //RWC;fprintf (src,"\t /* ANY - Fix Me! */\n");
            fprintf (src,"\tSetAnyTypeUnknown(%s);\n", elmtVarRef);
            //RWC;fprintf (src,"\tSetAnyTypeBy\?\?\?(%s, \?\?\?);\n", elmtVarRef);
            fprintf (src,"\t%s = %s%s (b, %s);\n", itemLenNameG, 
		     GetEncRulePrefix(), "EncAsnAny"/*RWC;NOT VALID FOR C_TYPEREF;ctri->encodeRoutineName*/, elmtVarRef);
            break;


        case C_LIB:
			fprintf (src, "\t%s = %s%sContent(b,%s);\n", itemLenNameG, 
		   GetEncRulePrefix(), ctri->encodeRoutineName, elmtVarRef);
	  break;

        case C_CHOICE:
            PrintCChoiceEncodeCode (src, td, e->type, level+1, elmtVarRef);
            break;

        case C_STRUCT:
            PrintCElmtsEncodeCode (src, td, e->type, e->type->basicType->a.set, level+1, elmtVarRef);
            break;

        case C_LIST:
            PrintCListEncoderCode (src, td, e->type, level+1, elmtVarRef);
            fprintf (src, "\t%s = %s;\n", itemLenNameG, listLenNameG);
            fprintf (src,"\n");
            break;

        case C_NO_TYPE:
            break;

//		case C_OBJECTCLASSFIELDTYPE:
//		  fprintf (src, "\t%s = %s%sContent (b, %s);\n", itemLenNameG,
//			   GetEncRulePrefix(), "AsnOcts" /*ctri->encodeRoutineName*/, elmtVarRef);
//			break;

        default:
            fprintf (errFileG, "PrintCElmtEncodeCode: ERROR - unknown c type id\n");
            break;
    }

    /*RWC;NOT ANY LONGER;if (ctri->cTypeId != C_ANY) / * ANY's do their own tag/lens */
    {
        PrintCTagAndLenEncodingCode (src, td, e->type);
        fprintf (src,"\t%s += %s;\n", encodedLenVarNameG, itemLenNameG);
    }

    if (e->type->optional || (e->type->defaultVal != NULL))
        fprintf (src, "\t}\n");

    fprintf (src,"\n");

}  /*  PrintCElmtEncodeCode */

/*
 * checks for which macroType is there
 * then calls specific function for encoding of that macro
 */
static void
PrintCMacroElmtsEncodeCode PARAMS ((src, td, parent, mt, level, varName),
    FILE *src _AND_
    TypeDef *td _AND_
    Type *parent _AND_
    MacroType *mt _AND_
    int level _AND_
    char *varName)
{
    if (mt == NULL)
    {
        fprintf (src,"/* ERROR? - expected macroType for this type*/\n");
        return;
    }

    switch (mt->choiceId)
    {	// This switch case copied from do-macros.c
	case MACROTYPE_ASNABSTRACTOPERATION:
	case MACROTYPE_ROSOPERATION:
		
		PrintCRosOperationElmtsEncodeCode (src, td, parent, mt, mt->a.rosOperation, level, varName);
		break;

		// Add code for other macro types here
	default:
		// Unsupported Macro Type
		break;
	}
}  /* PrintCMacroElmtsEncodeCode */


static void
PrintCRosOperationElmtsEncodeCode PARAMS ((src, td, parent, mt, op, level, varName),
    FILE *src _AND_
    TypeDef *td _AND_
    Type *parent _AND_
    MacroType *mt _AND_
	RosOperationMacroType *op _AND_
    int level _AND_
    char *varName)
{
	if(op->result != NULL)
	{
		PrintCElmtEncodeCode (src, td, parent, op->result, level, varName);
	}

	if(op->arguments != NULL)
	{
		PrintCElmtEncodeCode (src, td, parent, op->arguments, level, varName);
	}
	mt=mt;
} /* PrintCRosOperationElmtsEncodeCode */

/*
 * Generates code for internally defined lists
 * eg:
 * TypeX = SET { foo INTEGER, bar SEQUENCE OF INTEGER } -->
 * BerEncodeTypeX (b, v)
 * {
 *    ...
 *         listLen = 0;
 *         FOR_EACH_LIST_ELMT (component, v->bar)
 *         {
 *              itemLen = BerEncodeInteger (b, (int*) component);
 *              itemLen+= EncodeLen (b, itemLen)
 *              itemLen += ENCODE_TAG (b, INTEGER_TAG);
 *              listLen += itemLen;
 *         }
 *    ...
 * }
 */
static void
PrintCListEncoderCode PARAMS ((src, td, t, level, varName),
    FILE *src _AND_
    TypeDef *td _AND_
    Type *t _AND_
    int level _AND_
    char *varName)
{
    CTRI *ctri;
    char *elmtVarRef = "component";
    Type *tmpType;
    char *itemLenName=NULL;

    ctri =  t->basicType->a.setOf->cTypeRefInfo;

    if (ctri == NULL)
        return;
    
    /* IF DER, and SET OF, we need to encode in order of the DER encodings 
     * of the elements.  This first bit stuffs each encoded elmt into an
     * array of bufs.
     */
    if (GetEncRulesType() == DER && 
	t->basicType->choiceId == BASICTYPE_SETOF) {
      /* Print extra locals */
      fprintf(src, "\tEncodedElmt *bufs;\n");
	  fprintf(src, "\tExpBuf	*xbufp;\n");
      fprintf(src, "\tint i, len1;\n\n");

      fprintf (src, "\tlistLen = 0;\n");
      fprintf (src, "\t/* Encode elements and stuff them in an array */\n");
      fprintf (src, "\tbufs = (EncodedElmt* ) Asn1Alloc(sizeof(EncodedElmt)*AsnListCount(%s));\n", varName);
      fprintf (src, "\tfor (i=0; i < AsnListCount(%s); i++) {\n", 
	       varName);
      fprintf (src, "\t  xbufp = ExpBufAllocBufAndData();;\n");
      fprintf (src, "\t  ExpBufResetInWriteRvsMode(xbufp);// May not need - leave in for now \n ");
      fprintf (src, "\t  ExpBuftoGenBuf(xbufp, &bufs[i].b);\n   } \n");
      fprintf (src, "\ti = 0;\n");
      fprintf (src, "\tFOR_EACH_LIST_ELMT_RVS (component, %s)\n", varName);
      fprintf (src, "\t{\n");
      itemLenName = itemLenNameG;
      itemLenNameG = "bufs[i].len";
      bufNameG = "bufs[i].b";

    } else {
      fprintf (src, "\tlistLen = 0;\n");
      fprintf (src, "\tFOR_EACH_LIST_ELMT_RVS(component,%s)\n", varName);
      fprintf (src, "\t{\n");
      bufNameG = "b";
    }
	  fflush (src);

    PrintEocEncoders (src, td, t->basicType->a.setOf);

    /*
     * need extra case here for SET OF typedef not just SET OF typeref
     */
    switch(ctri->cTypeId)
    {

        case C_TYPEREF:
            tmpType = GetType (t->basicType->a.setOf);

             /* NOTE: ANY DEFINED BY must be directly in the parent (not ref)*/
            if (tmpType->cTypeRefInfo->cTypeId != C_ANY)
            {
                fprintf (src, "\t%s = %s%sContent(%s,%s);\n", itemLenNameG,
			 GetEncRulePrefix(), ctri->encodeRoutineName, 
			 bufNameG, elmtVarRef);
                break;
            }
            else  /* fall through */

        case C_ANY:
            /* ANY's enc's do tag and len so zap the Content suffix */
            fprintf (src,"\t /* ANY - Fix Me! */\n");
            fprintf (src,"\tSetAnyTypeUnknown(%s);\n", elmtVarRef);
            //RWC;fprintf (src, "\tSetAnyTypeBy???(%s,???);\n", elmtVarRef);
            fprintf (src, "\t%s = %s%s(%s,%s);\n", itemLenNameG,
				GetEncRulePrefix(), ctri->encodeRoutineName,
				bufNameG, elmtVarRef);
            break;

        default:
            fprintf (src, "\t%s = %s%sContent(%s,(%s*)%s);\n", 
		     itemLenNameG, GetEncRulePrefix(), 
		     ctri->encodeRoutineName, bufNameG, ctri->cTypeName, 
		     elmtVarRef);
            break;
    }

    PrintCTagAndLenEncodingCode (src, td, t->basicType->a.setOf);
    fprintf (src,"\n");
    fprintf (src, "\t%s += %s;\n", listLenNameG, itemLenNameG);
    if (GetEncRulesType() == DER && 
	t->basicType->choiceId == BASICTYPE_SETOF) {
 	  fprintf (src, "\t  BufResetInReadMode(bufs[i].b);\n");

      fprintf (src, "\t  i++;\n");
    }
    fprintf (src, "\t}\n");

    if (GetEncRulesType() == DER && 
	t->basicType->choiceId == BASICTYPE_SETOF) {
      /* Sort the encoded results in the array of bufs, then stuff everything
       * in the output buf
       */
      fprintf (src, "\tlen1 = i;\n");
      fprintf (src, "\t/* Sort elements */\n");
      fprintf (src, "\tqsort(bufs, i, sizeof(EncodedElmt), EncodedElmtCmp);\n");
      fprintf (src, "\tfor (i--; i>=0; i--) {\n");
      fprintf (src, "\t\tchar *dst = (char *) Asn1Alloc(bufs[i].len);\n");
      fprintf (src, "\t\tBufResetInReadMode(bufs[i].b);\n");
      fprintf (src, "\t\tBufCopy(dst, bufs[i].b, bufs[i].len);\n");
      fprintf (src, "\t\tBufPutSegRvs(b, dst, bufs[i].len);\n");
      fprintf (src, "\t\tExpBufFreeBufAndData(bufs[i].b->spare);\n");
      fprintf (src, "\t};\n");
	  fprintf (src, "\t/* Free the EncodedElmt's */\n");
	  fprintf (src, "\tAsn1Free(bufs);\n");
      itemLenNameG = itemLenName;
      bufNameG = "b";
    }

    level = level;  /* AVOIDS warning. */
}  /*  PrintCListEncoderCode */



static void
PrintCChoiceEncodeCode PARAMS ((src, td, t, level, varName),
    FILE *src _AND_
    TypeDef *td _AND_
    Type *t _AND_
    int level _AND_
    char *varName)
{
    NamedType *e;
    CTRI *ctri;
    void *tmp;

    ctri =  t->cTypeRefInfo;

    fprintf (src,"\tswitch(%s->%s)\n\t{\n", varName, ctri->choiceIdEnumFieldName);

    FOR_EACH_LIST_ELMT (e, t->basicType->a.choice)
    {
        tmp = (void*)CURR_LIST_NODE (t->basicType->a.choice);

        if (e->type == NULL)
            continue;

        ctri =  e->type->cTypeRefInfo;

        if (ctri != NULL)
            fprintf (src, "\t\tcase %s:\n", ctri->choiceIdSymbol);
        else
            fprintf (src, "\t\tcase ????:\n");



        PrintCElmtEncodeCode (src, td, t, e, level+1, varName);
        fprintf (src,"\tbreak;\n\n");

        SET_CURR_LIST_NODE (t->basicType->a.choice, tmp);
    }

    fprintf (src, "\t}\n");
}  /* PrintCChoiceEncodeCode */



/*
 *  prints DecodeBerEocIfNec (b) for each constructed len
 *  assoc with given type
 */
static void
PrintEocEncoders PARAMS ((src, td, t),
    FILE *src _AND_
    TypeDef *td _AND_
    Type *t)
{
    TagList *tl;
    Tag *tag;
    int stoleChoiceTags;

    /*
     * get all the tags on this type
     */
    tl = (TagList*) GetTags (t, &stoleChoiceTags);

    /*
     * leave choice elmt tag enc to encoding routine
     */

    if (!stoleChoiceTags)
    {
        FOR_EACH_LIST_ELMT (tag, tl)
        {
            if (tag->form == CONS)
                fprintf (src,"\t%sEncEocIfNec(b);\n", GetEncRulePrefix());
        }
    }
/*
    consTagCount = 0;
    if (!stoleChoiceTags)
    {
        FOR_EACH_LIST_ELMT (tag, tl)
            consTagCount++;
    }

    if (IsPrimitiveByDefOrRef (t))
        consTagCount--;

    for (; consTagCount > 0; consTagCount--)
        fprintf (src,"\t%sEncEocIfNec (b);\n", GetEncRulePrefix());

*/

    FreeTags (tl);

    td = td; /*AVOIDS warning*/
}  /* PrintEocEncoders */


/*
 *  Recursively walks throught type refs printing lower lvl tags
 *  first (since encoding is done backwards).
 *
 */
static void
PrintCTagAndLenEncodingCode PARAMS ((src, td, t),
    FILE *src _AND_
    TypeDef *td _AND_
    Type *t)
{
    TagList *tl;
    int stoleChoiceTags;

    /*
     * get all the tags on this type
     */
    tl = (TagList*) GetTags (t, &stoleChoiceTags);

    /*
     * leave choice elmt tag enc to encoding routine
     */
    if (!stoleChoiceTags)
        PrintCTagAndLenList (src, t, tl);

    FreeTags (tl);

    td = td; /*AVOIDS warning*/
}  /* PrintCTagAndLenEncodingCode */



/*
 * prints last tag's encoding code first
 */
static void
PrintCTagAndLenList PARAMS ((src, t, tagList),
    FILE *src _AND_
    Type *t _AND_
    TagList *tagList)
{
    char *classStr;
    char *formStr;
    Tag *tg;
    int tagLen;
    enum BasicTypeChoiceId typesType;
    int isShort;

    if ((tagList == NULL) || LIST_EMPTY (tagList))
        return;

    /*
     * efficiency hack - use simple length (1 byte)
     * encoded for type (almost) guaranteed to have
     * encoded lengths of 0 <= len <= 127
     */
    typesType = GetBuiltinType (t);
    if ((typesType == BASICTYPE_BOOLEAN) ||
        (typesType == BASICTYPE_INTEGER) ||
        (typesType == BASICTYPE_NULL) ||
        (typesType == BASICTYPE_REAL) ||
        (typesType == BASICTYPE_ENUMERATED))
        isShort = 1;
    else
        isShort = 0;

    /*
     * since encoding backward encode tags backwards
     */
    FOR_EACH_LIST_ELMT_RVS (tg, tagList)
    {
        classStr = Class2ClassStr (tg->tclass);

        if (tg->form == CONS)
        {
            formStr = Form2FormStr (CONS);
            PrintCLenEncodingCode (src, TRUE, isShort);
        }
        else /* PRIM or ANY_FORM */
        {
            formStr = Form2FormStr (PRIM);
            PrintCLenEncodingCode (src, FALSE, isShort);
        }

        fprintf (src,"\n");

        if (tg->code < 31)
            tagLen = 1;
        else if (tg->code < 128)
            tagLen = 2;
        else if (tg->code < 16384)
            tagLen = 3;
        else if (tg->code < 2097152)
            tagLen = 4;
        else
            tagLen = 5;

        fprintf (src,"    %s += %sEncTag%d(%s,%s,%s,%s);\n", itemLenNameG, 
                 GetEncRulePrefix(), tagLen, bufNameG, classStr, formStr, 
                 DetermineCode(tg, &tagLen, 0));
    }

}  /* PrintCTagAndLenList */

/*
 * prints length encoding code.  Primitives always use
 * definite length and constructors get "ConsLen"
 * which can be configured at compile to to be indefinite
 * or definite.  Primitives can also be "short" (isShort is true)
 * in which case a fast macro is used to write the length.
 * Types for which isShort apply are: boolean, null and
 * (almost always) integer and reals
 */
static void
PrintCLenEncodingCode PARAMS ((f, isCons, isShort),
    FILE *f _AND_
    int isCons _AND_
    int isShort)
{
   /* fprintf (f, "\t%sER_ENCODE_DEF_LEN (b, itemLen, itemLen);",
      GetEncRulePrefix()); */
    if (isCons)
        fprintf (f, "\t%s += %sEncConsLen(%s,%s);", 
		 itemLenNameG, GetEncRulePrefix(), bufNameG, itemLenNameG);
    else
    {
        if (isShort)
        {
            fprintf (f, "\t%sEncDefLenTo127(%s,%s);\n", 
		     GetEncRulePrefix(), bufNameG, itemLenNameG);
            fprintf (f, "\t%s++;", itemLenNameG);
        }
        else
            fprintf (f, "\t%s += %sEncDefLen(%s,%s);",
		     itemLenNameG, GetEncRulePrefix(), bufNameG, itemLenNameG);
    }
}  /* PrintCLenEncodingCode */
