/*
 *   compiler/back_ends/c++_gen/gen_code.c - routines for printing C++ code from type trees
 *
 *   assumes that the type tree has already been run through the
 *   c++ type generator (c++_gen/types.c).
 *
 *  This was hastily written - it has some huge routines in it.
 *  Needs a lot of cleaning up and modularization...
 *
 * Mike Sample
 * 92
 * Copyright (C) 1991, 1992 Michael Sample
 *           and the University of British Columbia
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * $Header: /baseline/SNACC/compiler/back-ends/c++-gen/gen-code.c,v 1.129 2004/04/06 15:13:41 gronej Exp $
 *
 */

#include "snacc.h"

#if TIME_WITH_SYS_TIME
# include <sys/time.h>
# include <time.h>
#else
# if HAVE_SYS_TIME_H
#  include <sys/time.h>
# else
#  include <time.h>
# endif
#endif

#include <string.h>

#include "asn-incl.h"
#include "asn1module.h"
#include "rules.h"
#include "snacc-util.h"
#include "print.h"
#include "tag-util.h"  /* get GetTags/FreeTags/CountTags/TagByteLen */
#include "cxxconstraints.h"
#include "cxxmultipleconstraints.h"


#if META
#include "meta.h"
#endif


enum BasicTypeChoiceId ParanoidGetBuiltinType PARAMS ((t),Type *t);
void PrintConditionalIncludeOpen PROTO ((FILE *f, char *fileName));
void PrintConditionalIncludeClose PROTO ((FILE *f, char *fileName));
void PrintCxxAnyCode PROTO ((FILE *src, FILE *hdr, CxxRules *r,
							ModuleList *mods, Module *m));
void PrintCxxValueDef PROTO ((FILE *src, CxxRules *r, ValueDef *v));
void PrintCxxValueExtern PROTO ((FILE *hdr, CxxRules *r, ValueDef *v));
char *LookupNamespace PROTO ((Type *t, ModuleList *mods));
static void PrintCxxSeqSetPrintFunction(FILE* src, FILE* hdr,
										MyString className,
										BasicType *pBasicType);

/* PER specific methods*/
static void
PrintCxxDefCode_SetSeqPEREncode (FILE *src, FILE *hdr, CxxRules *r, TypeDef *td, 
    NamedType **pSetElementNamedType,
    int iElementCount);      /* IN, ELEMENT Count to process in arrays */

static void
PrintCxxDefCode_SetSeqPERDecode (FILE *src, FILE *hdr, CxxRules *r, TypeDef *td, 
    NamedType **pSetElementNamedType,
    int iElementCount);      /* IN, ELEMENT Count to process in arrays */

static void
PrintCxxDefCode_PERSort (
    NamedType ***pppElementNamedType, /* OUT, array of sorted NameType(s) */
    int **ppElementTag,      /* OUT, actual tag for sorted. */
    AsnList *pElementList);  /* IN, actual eSNACC defs for NameType(s). */


/* flag to see if constraints were present */
int constraints_flag;
long lconstraintvar=0;

extern char *bVDAGlobalDLLExport;
extern int gNO_NAMESPACE;
extern const char *gAlternateNamespaceString;
extern int genPERCode;
//extern short ImportedFilesG;

static const char	bufTypeNameG[] = "AsnBuf";
static const char	lenTypeNameG[] = "AsnLen";
static const char	tagTypeNameG[] = "AsnTag";
static const char	envTypeNameG[] = "ENV_TYPE";
static long 		longJmpValG  = -100;
static const char	baseClassesG[] = ": public AsnType";

static int		printTypesG;
static int		printEncodersG;
static int		printDecodersG;
static int		printPrintersG;
static int		printFreeG;

#if META
static MetaNameStyle	printMetaG;
static MetaPDU		*meta_pdus_G;
#if TCL
static int		printTclG;
#endif
#endif /* META */


/* PIERCE added 10-27-2002 */
void PrintCxxEncodeContaining(Type *t, CxxRules *r, FILE *src);
void PrintCxxDecodeContaining(Type *t, CxxRules *r, FILE *src);
void PrintCxxPEREncodeContaining(Type *t, CxxRules *r, FILE *src);
void PrintCxxPERDecodeContaining(Type *t, CxxRules *r, FILE *src);

/* PIERCE added 9-4-2002 */

void PrintCxxSetTypeByCode(NamedType *defByNamedType, CxxTRI *cxxtri, FILE *src);

// normalizeValue
// 
// strip whitespace and { } from valueNation values.
//
void normalizeValue(char **normalized, char *input)
{
   int i;
   while(*input == ' ' || *input == '{' )
      input++;

   *normalized = strdup(input);
   
   i = strlen(*normalized) - 1;
   while ( (*normalized)[i] == ' ' || (*normalized)[i] == '}' )
   {
      (*normalized)[i] = 0;
      i--;
   }
}

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
			fileName = currMod->cxxHdrFileName;
			break;
		}
	}
	return fileName;
}

/* RWC; added 7/25/03 */
static Module *GetImportModuleRef (char *Impname, ModuleList *mods)
{
	Module *currMod=NULL;
    FOR_EACH_LIST_ELMT (currMod, mods)
	{
		/* Find the import Module in the Modules and
		 * return the header file name
		 */
		if ((strcmp(Impname, currMod->modId->name) == 0))
		{
			break;
		}
	}
	return currMod;
}

/* PIERCE added 8-22-2001 */
void PrintCopyConstructor(FILE *hdr, FILE *src, char *className)
{
   fprintf(hdr, "   %s(const %s& that);\n", className, className);
   fprintf(src,"%s::%s(const %s &that)\n{\n",className, className,className);
   /*fprintf(src,"   Clear();\n");*/
   fprintf(src,"   Init();\n");
   fprintf(src,"   *this = that;\n}\n");
}

void PrintSimpleMeta(FILE *hdr, char *className,int exportMember)
{
	char *ptr="";   /* NOT DLL Exported, or ignored on Unix. */

	if (bVDAGlobalDLLExport != NULL && exportMember == 1)
		ptr = bVDAGlobalDLLExport;

	fprintf(hdr, "public:\n\t%s virtual const char* typeName() const\t{ return \"%s\"; }\n",
		ptr, className);
}

void PrintSimpleCheck(FILE *hdr, FILE* src, char *className,int exportMember)
{

	char *ptr="";   /* NOT DLL Exported, or ignored on Unix. */

	if (bVDAGlobalDLLExport != NULL && exportMember == 1)
        ptr = bVDAGlobalDLLExport;

	fprintf(hdr, "\t%s virtual int checkConstraints(ConstraintFailList* pConstraintFails) const;\n",
		ptr);
	fprintf(src, "int %s::checkConstraints(ConstraintFailList* pConstraintFails) const\n{\n",
		className);
	fprintf(src, "\treturn checkListConstraints(pConstraintFails);\n");
	fprintf(src, "}\n");
}

static void
PrintHdrComment PARAMS ((hdr, m),
    FILE *hdr _AND_
    Module *m)
{
    time_t now = time (NULL);

    fprintf (hdr, "// %s - class definitions for ASN.1 module %s\n",
             m->cxxHdrFileName, m->modId->name);
    fprintf (hdr, "//\n");
    fprintf (hdr, "//   This file was generated by esnacc on %s\n",
             ctime(&now));
    fprintf (hdr, "//   NOTE: this is a machine generated file-"
             "-editing not recommended\n");
    fprintf (hdr, "\n");

} /* PrintHdrComment */

static void
PrintSrcComment PARAMS ((src, m),
    FILE *src _AND_
    Module *m)
{
    time_t now = time (NULL);

    fprintf (src, "// %s - class definitions for ASN.1 module %s\n",
             m->cxxSrcFileName, m->modId->name);
    fprintf (src, "//\n");
    fprintf (src, "//   This file was generated by esnacc on %s\n",
             ctime(&now));
    fprintf (src, "//   NOTE: this is a machine generated file-"
             "-editing not recommended\n");
    fprintf (src, "\n");

} /* PrintSrcComment */


static void
PrintSrcIncludes PARAMS ((src, mods, m),
    FILE *src _AND_
    ModuleList *mods _AND_
    Module *m)
{

	mods = mods;
	if (m->cxxHdrFileName)
		fprintf (src, "#include \"%s\"\n", m->cxxHdrFileName);

    m = m;  /*AVOIDS warning.*/
} /* PrintSrcIncludes */


static void
PrintTypeDecl PARAMS ((f, td),
    FILE *f _AND_
    TypeDef *td)
{
    switch (td->type->basicType->choiceId)
    {   
        case BASICTYPE_COMPONENTSOF:
        case BASICTYPE_SELECTION:
        case BASICTYPE_UNKNOWN:
        case BASICTYPE_MACRODEF:
        case BASICTYPE_MACROTYPE:
            return; /* do nothing */

        default:
            if (IsNewType (td->type))
                fprintf (f, "class %s;\n", td->cxxTypeDefInfo->className);
    }
	
} /* PrintTypeDecl */


static void
PrintCxxType PARAMS ((hdr, mods, m, r, td, parent, t),
    FILE *hdr _AND_
    ModuleList *mods _AND_
    Module *m _AND_
    CxxRules *r _AND_
    TypeDef *td _AND_
    Type *parent _AND_
    Type *t)
{
    char *pszNamespace=NULL;
    pszNamespace = LookupNamespace(t, mods);

    if (pszNamespace)
    {
        fprintf (hdr, "%s::%s       ", pszNamespace, t->cxxTypeRefInfo->className);
    }
    else
    {
        fprintf (hdr, "%s       ", t->cxxTypeRefInfo->className);
    }           // END IF BASICTYPE_IMPORTTYPEREF

    if (t->cxxTypeRefInfo->isPtr)
        fprintf (hdr, "*");

    parent = parent;td = td;r = r;m = m;mods=mods;  /*AVOIDS warning.*/
} /* PrintCxxType */



/*
 *  Uses the Constructor that takes no args.
 *  Assumes file f is positioned inside a class definition.
 *  All Classes get this to support the ANY type.
 */
static void
PrintCloneMethod PARAMS ((hdr, src, td),
    FILE *hdr _AND_
    FILE *src _AND_
    TypeDef *td)
{
//    fprintf (hdr, "  AsnType		*Clone() const;\n\n", td->cxxTypeDefInfo->className);
    fprintf (hdr, "  AsnType		*Clone() const;\n\n");
    fprintf (src, "AsnType *%s::Clone() const\n", td->cxxTypeDefInfo->className);
    fprintf (src, "{\n");
    fprintf (src, "  return new %s(*this);\n", td->cxxTypeDefInfo->className);
    fprintf (src, "}\n\n");

} /* PrintCloneMethod */


/*
 * prints inline definition of constructors if this class is
 * derived from a library class.
 * assumes FILE *f is positioned in the derived class definition (.h)
 *
 * 12/92 MS - added overloaded "=" ops for string types.
 */
static void
PrintDerivedConstructors PARAMS ((f, r, td),
    FILE *f _AND_
    CxxRules *r _AND_
    TypeDef *td)
{
    enum BasicTypeChoiceId typeId;
    char *derivedClassName;
    char *baseClassName;

    typeId = GetBuiltinType (td->type);
    derivedClassName = td->cxxTypeDefInfo->className;
    baseClassName = td->type->cxxTypeRefInfo->className;

    /* every class gets the no-arg constructor */
    #if TCL
        if (printTclG && typeId == BASICTYPE_ENUMERATED)
        {
	        fprintf (f, "#if TCL\n");
	        fprintf (f, "			%s(): %s (_nmdescs[0].value) {}\n", derivedClassName, baseClassName);
	        fprintf (f, "#else\n");
        }
        if (printTclG && typeId == BASICTYPE_ENUMERATED)
          fprintf (f, "#endif\n");
    #endif /* TCL */

    switch (typeId)
    {
        case BASICTYPE_BOOLEAN:
            fprintf (f, "			%s(): %s() {}\n", derivedClassName, baseClassName);
            fprintf (f, "			%s (bool _b): %s (_b) {}\n", derivedClassName, baseClassName);
            break;

        case BASICTYPE_ENUMERATED:
        case BASICTYPE_INTEGER:
            fprintf (f, "			%s(): %s() {}\n", derivedClassName, baseClassName);
            fprintf (f, "			%s (int i): %s (i) {}\n", derivedClassName, baseClassName);
            break;

        case BASICTYPE_REAL:
            fprintf (f, "			%s(): %s() {}\n", derivedClassName, baseClassName);
            fprintf (f, "			%s (double d): %s (d) {}\n", derivedClassName, baseClassName);
            break;

	     case BASICTYPE_OCTETSTRING:
            fprintf (f, "			%s(): %s() {}\n", derivedClassName, baseClassName);
            fprintf (f, "			%s (const char *str): %s (str) {}\n", derivedClassName, baseClassName);

            fprintf (f, "			%s (const char *str, const size_t len): %s (str, len) {}\n", derivedClassName, baseClassName);

            fprintf (f, "			%s (const %s &o): %s (o) {}\n", derivedClassName, derivedClassName, baseClassName);

            /* include overloading of = op. MS 12/92 */
			
            fprintf (f, "        %s		&operator = (const %s &o)	{ if (this != &o ) AsnOcts::operator=(o); return *this; }\n", derivedClassName, derivedClassName);
			fprintf (f, "        %s		&operator = (const char *str)	{ AsnOcts::operator=(str); return *this; }\n", derivedClassName);
            break;

        case BASICTYPE_BITSTRING:
			{
				const char *nblStr = "nblFlag = false;";
				if (HasNamedElmts (td->type))
					nblStr = "nblFlag = true;";
				fprintf(f, "\t%s() : %s()\t\t\t{ %s }\n", derivedClassName,
					baseClassName, nblStr);
				fprintf(f, "\t%s(size_t bits) : %s(bits)\t{ %s }\n",
					derivedClassName, baseClassName, nblStr);
				fprintf(f, "\t%s(const unsigned char* str, const size_t bitLen) :\n",
					derivedClassName);
				fprintf(f, "\t\t%s(str, bitLen)\t\t\t\t{ %s }\n", baseClassName,
					nblStr);
            //	Copy constructor not needed
            //				fprintf(f, "\t%s (const %s &o) : %s(o)\n",
            //					derivedClassName, baseClassName, baseClassName);
			}
			break;

        case BASICTYPE_OID:
            /* TBD: Fix this code -PIERCE
             
            fprintf (f, "			%s(): %s() {}\n", derivedClassName, baseClassName);
            fprintf (f, "			%s (const char *encOid, size_t len): %s (encOid, len) {}\n", derivedClassName, baseClassName);
            fprintf (f, "			%s (const %s &o): %s (o) {}\n", derivedClassName, baseClassName, baseClassName);
            fprintf (f, "			%s (unsigned long  a1, unsigned long  a2, long a3=-1, long a4=-1, long a5=-1, long a6=-1, long a7=-1, long a8=-1, long a9=-1, long a10=-1, long a11=-1): %s (a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11) {}\n", baseClassName, derivedClassName, baseClassName);
            fprintf (f, "  %s		&operator = (const %s &o)	{ ReSet (o); return *this; }\n", derivedClassName, derivedClassName);
            */
            printf("TBD: Attempt was made to generate code for a tagged OID outside of a SEQ or SET\n");
            break;
        default:
            break;
    }

    r = r;  /*AVOIDS warning.*/
} /* PrintDerivedConstructors */

#if DEPRECATED
static void
PrintCxxEocEncoders PARAMS ((src, td, t, bufVarName),
    FILE *src _AND_
    TypeDef *td _AND_
    Type *t _AND_
    char *bufVarName)
{
    TagList *tl;
    Tag *tag;
    int stoleChoiceTags;
    
    /* get all the tags on this type*/
    tl = (TagList*) GetTags (t, &stoleChoiceTags);
    FreeTags (tl);
    bufVarName=bufVarName;td=td;  /*AVOIDS warning.*/

} /* PrintCxxEocEncoders */
#endif

static int
HasShortLen PARAMS ((t),
    Type *t)
{
    enum BasicTypeChoiceId typesType;
    /*
     * efficiency hack - use simple length (1 byte)
     * encoded for type (almost) guaranteed to have
     * encoded lengths of 0 <= len <= 127
     */
    typesType = GetBuiltinType (t);
    /*RWC;8/9/01;REMOVED when INTEGER made AsnOcts;return typesType == BASICTYPE_BOOLEAN || typesType == BASICTYPE_INTEGER || typesType == BASICTYPE_NULL || typesType == BASICTYPE_REAL || typesType == BASICTYPE_ENUMERATED; */
    return typesType == BASICTYPE_BOOLEAN || typesType == BASICTYPE_NULL || typesType == BASICTYPE_REAL || typesType == BASICTYPE_ENUMERATED;
} /* HasShortLen */


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
PrintCxxLenEncodingCode PARAMS ((f, isCons, isShort, lenVarName, bufVarName),
    FILE *f _AND_
    int isCons _AND_
    int isShort _AND_
    char *lenVarName _AND_
    char *bufVarName)
{
    if (isCons)
        fprintf (f, "    %s += BEncConsLen (%s, %s);\n", lenVarName, bufVarName, lenVarName);
    else
    {
        if (isShort)
        {
            fprintf (f, "    BEncDefLenTo127 (%s, %s);\n", bufVarName, lenVarName);
            fprintf (f, "    %s++;\n", lenVarName);
        }
        else
            fprintf (f, "    %s += BEncDefLen (%s, %s);\n", lenVarName, bufVarName, lenVarName);
    }
} /* PrintCxxLenEncodingCode */


/* prints last tag's encoding code first */
static void
PrintCxxTagAndLenList PARAMS ((src, t, tagList, lenVarName, bufVarName),
    FILE *src _AND_
    Type *t _AND_
    TagList *tagList _AND_
    char *lenVarName _AND_
    char *bufVarName)
{
    char *classStr;
    char *formStr;
    Tag *tg;
    int tagLen;
    int isShort;

    if ((tagList == NULL) || LIST_EMPTY (tagList))
        return;

    /*
     * efficiency hack - use simple length (1 byte)
     * encoded for type (almost) guaranteed to have
     * encoded lengths of 0 <= len <= 127
     */
    isShort = HasShortLen (t);

    /*
     * since encoding backward encode tags backwards
     */
    FOR_EACH_LIST_ELMT_RVS (tg, tagList)
    {
        classStr = Class2ClassStr (tg->tclass);

        if (tg->form == CONS)
        {
            formStr = Form2FormStr (CONS);
            PrintCxxLenEncodingCode (src, TRUE, isShort, lenVarName, bufVarName);
        }
        else /* PRIM or ANY_FORM */
        {
            formStr = Form2FormStr (PRIM);
            PrintCxxLenEncodingCode (src, FALSE, isShort, lenVarName, bufVarName);
        }

        fprintf (src, "\n");

        //RWC;tagLen = TagByteLen (tg->code);

        if (tg->tclass == UNIV)
        {
            const char* ptr = DetermineCode(tg, &tagLen, 0);
            fprintf (src, "    %s += BEncTag%d (%s, %s, %s, %s);\n", lenVarName, tagLen, bufVarName, classStr, formStr, ptr);
        }                                                       //RWC;Code2UnivCodeStr (tg->code));
        else
        {
            const char* ptr = DetermineCode(tg, &tagLen, 1);
            fprintf (src, "    %s += BEncTag%d (%s, %s, %s, %s);\n", lenVarName, tagLen, bufVarName, classStr, formStr, ptr);
        }                                                       //RWC;tg->code);
    }
} /* PrintCxxTagAndLenList */

/*
 *  Recursively walks through tags, printing lower lvl tags
 *  first (since encoding is done backwards).
 *
 */
static void
PrintCxxTagAndLenEncodingCode PARAMS ((src, td, t, lenVarName, bufVarName),
    FILE *src _AND_
    TypeDef *td _AND_
    Type *t _AND_
    char *lenVarName _AND_
    char *bufVarName)
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
        PrintCxxTagAndLenList (src, t, tl, lenVarName, bufVarName);

    FreeTags (tl);

    td=td;  /*AVOIDS warning.*/
} /* PrintCxxTagAndLenEncodingCode */


/*
 * used to figure out local variables to declare
 * for decoding tags/len pairs on type t
 */
static int
CxxCountVariableLevels PARAMS ((t),
    Type *t)
{
    if (GetBuiltinType (t) == BASICTYPE_CHOICE)
        return CountTags (t) +1; /* since must decode 1 internal tag type */
    else
        return CountTags (t);
} /* CxxCountVariableLevels */


/*
 * returns true if elmts curr following
 *  onward are all optional ow. false
 */
static int
RestAreTailOptional PARAMS ((e),
    NamedTypeList *e)
{
    NamedType *elmt;
    void *tmp;
    int retVal;

    if (e == NULL)
        return TRUE;

    tmp = (void*)CURR_LIST_NODE (e);
    retVal = TRUE;
    AsnListNext (e);
    FOR_REST_LIST_ELMT (elmt, e)
    {
        if ((!elmt->type->optional) && (elmt->type->defaultVal == NULL)&&(!elmt->type->extensionAddition))
        {
            retVal = FALSE;
            break;
        }
    }
    SET_CURR_LIST_NODE (e, tmp); /* reset list to orig loc */
    return retVal;
}


/*
 * prints typedef or new class given an ASN.1  type def of a primitive type
 * or typeref.  Uses inheritance to cover re-tagging and named elmts.
 */
static void
PrintCxxSimpleDef PARAMS ((hdr, src, m, r, td),
    FILE *hdr _AND_
    FILE *src _AND_
    Module *m _AND_
    CxxRules *r _AND_
    TypeDef *td)
{
    Tag *tag;
    TagList *tags;
    char *formStr;
    char *classStr;
    int tagLen;
    int i;
    CNamedElmt *n;
    int stoleChoiceTags;
    int elmtLevel;
    enum BasicTypeChoiceId typeId;
    m = m;
    
    fprintf (hdr, "/* ");
    SpecialPrintType (hdr, td, td->type);
    fprintf (hdr, " */\n");

    /* check if has been re-tagged
     *   eg Foo ::= [APPLICATION 2] IMPLICIT REAL
     * or if it has named elmts in which case a new class must
     * be defined
     *  eg Foo ::= INTEGER { one (1), two (2), three (3) }
     */

    if (IsNewType (td->type))
    {
	     int	hasNamedElmts;

        char *ptr="";   /* NOT DLL Exported, or ignored on Unix. */
        if (bVDAGlobalDLLExport) 
            ptr = bVDAGlobalDLLExport;

        fprintf (hdr, "class %s %s: public %s\n", ptr, td->cxxTypeDefInfo->className, td->type->cxxTypeRefInfo->className);
        fprintf (hdr, "{\n");
        fprintf (hdr, "public:\n");

        /*
         * must explicitly call constructors for base class
         */
        PrintDerivedConstructors (hdr, r, td);

        /* do named elmts enum if any */
        /* for types with named elements, inherit from the base
         * class and define and enum eg:
         * Foo ::= INTEGER { one (1), two (2), five (5) }
         *  ->
         * class Foo: public AsnInt
         * {
         * public:
         *		Foo(): AsnInt() {}
         *		Foo (int val): AsnInt (int val) {}
         *    enum { one = 1, two = 2, five = 5 };
         * };
         * or
         * Foo2 ::= [APPLICATION 2] INTEGER
         * -->
         * class Foo: public AsnInt
         * {
         * public:
         *		Foo(): AsnInt() {}
         *		Foo (int val): AsnInt (int val) {}
         *     AsnLen	BEnc { ....... } <-- holds new tag enc/dec
         *     void	BDec { ....... }   <--/
         *     int	BEncPdu { ....... }
         *     int	BDecPdu { ....... }
         * };
         * (must 'inherit' constructors explicitly)
         */

        if ((hasNamedElmts = HasNamedElmts (td->type)) != 0)
        {   
            int count = 0;

            fprintf (hdr, "\n\tenum\n");
            fprintf (hdr, "\t{\n");
            FOR_EACH_LIST_ELMT (n, td->type->cxxTypeRefInfo->namedElmts)
            {
                fprintf (hdr, "\t\t%s = %d", n->name, n->value);
                if (n != (CNamedElmt *)LAST_LIST_ELMT (td->type->cxxTypeRefInfo->namedElmts))
                    fprintf (hdr, ",\n");
                else
                    fprintf (hdr, "\n");

                count++;
            }
            fprintf (hdr, "\t};\n");

            if(td->type->basicType->choiceId == BASICTYPE_ENUMERATED)
            {
                fprintf (hdr, "  AsnLen PEnc(AsnBufBits &_b)const{\n");
                fprintf (hdr, "     long enumList[] = {");
                FOR_EACH_LIST_ELMT (n, td->type->cxxTypeRefInfo->namedElmts)
                {
                    fprintf (hdr, "%d", n->value);
                    if (n != (CNamedElmt *)LAST_LIST_ELMT (td->type->cxxTypeRefInfo->namedElmts))
                        fprintf (hdr, ", ");
                }
                fprintf (hdr, " };\n");
                fprintf (hdr, "       AsnInt index = IndexedVal(enumList, %d);\n", count);
                fprintf (hdr, "       AsnLen len = index.PEncFullyConstrained(_b, 0, %d);\n", count - 1);
                fprintf (hdr, "       return len;}\n");
                  
                fprintf (hdr, "  void PDec(AsnBufBits &_b, AsnLen &bitsDecoded){\n");
                fprintf (hdr, "     long enumList[] = {");
                FOR_EACH_LIST_ELMT (n, td->type->cxxTypeRefInfo->namedElmts)
                {
                    fprintf (hdr, "%d", n->value);
                    if (n != (CNamedElmt *)LAST_LIST_ELMT (td->type->cxxTypeRefInfo->namedElmts))
                        fprintf (hdr, ", ");
                }
                fprintf (hdr, " };\n");
                fprintf (hdr, "     AsnInt index;\n");
                fprintf (hdr, "     index.PDecFullyConstrained(_b, 0, %d, bitsDecoded);\n", count - 1);
                fprintf (hdr, "     SetIndex(enumList, %d, index);}\n", count);
                  
            }
        }

        #if META
	        if (printMetaG)
	        {
	            PrintCxxSimpleDefMeta_1(hdr, src, td, hasNamedElmts, n, m);
	        }
        #endif /* META */

        /*
         * Re-do BerEncode, BerDeocode, BerDecodePdu and BerDecodePdu
         * if this type has been re-tagged
         */
        if ((IsDefinedByLibraryType (td->type) && !HasDefaultTag (td->type))
	        || (IsTypeRef (td->type) && ((td->type->tags != NULL) 
            && !LIST_EMPTY (td->type->tags))))
        {
            /* only BerEn/Decode BerEn/DecodePdu need to be re-done if tags are different */

            /* print clone routine for ANY mgmt */
            PrintCloneMethod (hdr, src, td);

            tags = GetTags (td->type, &stoleChoiceTags);
            typeId = GetBuiltinType (td->type);

            /* do BerEncode function */
            if (printEncodersG)
            {
				fprintf (hdr, "  %s		B%s (%s &_b) const;\n", lenTypeNameG, r->encodeBaseName, bufTypeNameG);
                fprintf (src, "%s %s::B%s (%s &_b) const\n", lenTypeNameG, td->cxxTypeDefInfo->className, r->encodeBaseName, bufTypeNameG);
                fprintf (src, "{\n");
                fprintf (src, "    %s l=0;\n", lenTypeNameG);
                fprintf (src, "    l = BEncContent (_b);\n");

                /* encode each tag/len pair if any */
                if (!stoleChoiceTags)
                {
                    FOR_EACH_LIST_ELMT_RVS (tag, tags)
                    {
                        classStr = Class2ClassStr (tag->tclass);

                        if (tag->form == ANY_FORM)
                        {
                            formStr = Form2FormStr (PRIM);
                            PrintCxxLenEncodingCode (src, FALSE, HasShortLen (td->type), "l", "_b");
                        }
                        else
                        {
                            formStr = Form2FormStr (tag->form);
                            PrintCxxLenEncodingCode (src, TRUE, HasShortLen (td->type), "l", "_b");
                        }

                        fprintf (src, "\n");
                        //RWC;tagLen = TagByteLen (tag->code);

                        if (tag->tclass == UNIV)
                        {
                            const char* ptr = DetermineCode(tag, &tagLen, 0);
                            fprintf (src, "    l += BEncTag%d (_b, %s, %s, %s);\n", tagLen, classStr, formStr, ptr);//RWC;Code2UnivCodeStr (tag->code));
                        }
                        else
                        {
                            const char* ptr = DetermineCode(tag, &tagLen, 1);
                            fprintf (src, "    l += BEncTag%d (_b, %s, %s, %s);\n", tagLen, classStr, formStr, ptr);//RWC;tag->code);
                        }
                    }
                }
                fprintf (src, "    return l;\n");
                fprintf (src, "}\n\n");
            }
            /* end of BEnc function */

            /* Do BDec function */
            if (printDecodersG)
            {
                fprintf (hdr, "  void			B%s (const %s &_b, %s &bytesDecoded);\n", r->decodeBaseName, bufTypeNameG, lenTypeNameG);// envTypeNameG);
                fprintf (src, "void %s::B%s (const %s &_b, %s &bytesDecoded)\n", td->cxxTypeDefInfo->className, r->decodeBaseName, bufTypeNameG, lenTypeNameG);//, envTypeNameG);
                fprintf (src, "{\n");
                fprintf (src, "    FUNC(\"%s::B%s\");\n", td->cxxTypeDefInfo->className, r->decodeBaseName); 
                fprintf (src, "    %s tag;\n", tagTypeNameG);

                /* PL: removed to avoid unused variable warning
                  fprintf (src, "    AsnBufLoc readLoc = _b.GetReadLoc();\n");
                 */

                /* print extra locals for redundant lengths */
                for (i = 1; (tags != NULL) && (i <= LIST_COUNT (tags)); i++)
                    fprintf (src, "    %s elmtLen%d;\n", lenTypeNameG, i);
                if (typeId == BASICTYPE_CHOICE)
                    fprintf (src, "    %s elmtLen%d;\n", lenTypeNameG, i++);
                fprintf (src, "\n");

                /*  decode tag/length pair (s) */
                elmtLevel = 0;
                if (!stoleChoiceTags)
                {
                    FOR_EACH_LIST_ELMT (tag, tags)
                    {
                        classStr = Class2ClassStr (tag->tclass);


                        if (tag->form == ANY_FORM)
                            formStr = Form2FormStr (PRIM);
                        else
                            formStr = Form2FormStr (tag->form);

                        fprintf (src, "    if (((tag = BDecTag (_b, bytesDecoded)) != ");

                        if (tag->tclass == UNIV)
                        {
                            fprintf (src, "MAKE_TAG_ID (%s, %s, %s))", classStr, formStr, DetermineCode(tag, NULL, 0));//RWC;Code2UnivCodeStr (tag->code));
                            if (tag->form == ANY_FORM)
                                fprintf (src, "\n        && (tag != MAKE_TAG_ID (%s, %s, %s)))\n", classStr, Form2FormStr (CONS), DetermineCode(tag, NULL, 0));//RWC;Code2UnivCodeStr (tag->code));
                            else
                                fprintf (src, ")\n");
                        }
                        else
                        {
                            fprintf (src, "MAKE_TAG_ID (%s, %s, %s))", classStr, formStr, DetermineCode(tag, NULL, 1));//RWC;tag->code);
                            if (tag->form == ANY_FORM)
                                fprintf (src, "\n        && (tag != MAKE_TAG_ID (%s, %s, %s)))\n", classStr, Form2FormStr (CONS), DetermineCode(tag, NULL, 1));//RWC;tag->code);
                            else
                                fprintf (src, ")\n");
                        }

                        fprintf (src, "    {\n");
                        fprintf (src, "        throw InvalidTagException(typeName(), tag, STACK_ENTRY);\n");
                        fprintf (src, "    }\n");
                        fprintf (src, "    elmtLen%d = BDecLen (_b, bytesDecoded);\n", ++elmtLevel);
                    }
                }

                /* decode first tag from CHOICE's content */
                if (typeId == BASICTYPE_CHOICE)
                {
                    fprintf (src, "    tag = BDecTag (_b, bytesDecoded);\n");
                    fprintf (src, "    elmtLen%d = BDecLen (_b, bytesDecoded);\n", ++elmtLevel);
                }

                fprintf (src, "    B%s (_b, tag, elmtLen%d, bytesDecoded);\n", r->decodeContentBaseName, i-1);

                /* grab any EOCs that match redundant, indef lengths */
                for (i = elmtLevel-1; i > 0; i--)
                {
                    fprintf (src, "    if (elmtLen%d == INDEFINITE_LEN)\n", i);
                    fprintf (src, "        BDecEoc (_b, bytesDecoded);\n");
                }

                fprintf (src, "}\n\n");
            }
            /* end of BDec function */

            FreeTags (tags);
        }
        /* close class def */
        fprintf (hdr, "};\n\n\n");

    }
    else  /* isomorphic with referenced type, so just to a typedef */
    {

        #if META
	        if (printMetaG)
	        {
                PrintCxxSimpleDefMeta_2(hdr, src, td, hasNamedElmts, n, m, r);
	        }
        #endif /* META */

		/* JKG 7/31/03 */
		/* The following code enclosed in this if/else statement */
		/* is constructed for constraint handling capability     */
		/* for primitives found outside of                       */
		/* sequences or sets                                     */

        if (td->type->subtypes != NULL)
		{
			switch (td->type->subtypes->choiceId)
            {
			    case SUBTYPE_AND:
			    case SUBTYPE_OR:
                case SUBTYPE_SINGLE:
				    {   
				    struct NamedType temp;
				    NamedType* tmp;
				    tmp=&temp;
				    tmp->type=td->type;
				    tmp->type->cxxTypeRefInfo->fieldName=td->definedName;
				    tmp->fieldName=td->definedName;

				    if (!PrintCxxMultiConstraintOrHandler(hdr, src, NULL, tmp, 0))
					    PrintTypeDefDefault(hdr, src, td);
				    break;
				    }

			    default:
                {
				    PrintTypeDefDefault(hdr, src, td);			   				
				    break;
                }
			}
		}
		else
		{
			PrintTypeDefDefault(hdr, src, td);
		}

        #if META
		        if (printMetaG)
			        fprintf (hdr, "#endif // META\n\n");
        #endif /* META */
	}
} /* PrintCxxSimpleDef */

static void
PrintCxxChoiceDefCode (FILE *src, FILE *hdr, ModuleList *mods, Module *m, CxxRules *r ,
    TypeDef *td,Type *parent, Type *choice, int novolatilefuncs)
{
    NamedType *e;
    char *classStr;
    char *formStr;
    char *codeStr;
    int tagLen=0, i;
    Tag *tag;
    TagList *tags;
    char *varName;
    CxxTRI *cxxtri;
    int elmtLevel=0;
    int varCount, tmpVarCount;
    int stoleChoiceTags;
    enum BasicTypeChoiceId tmpTypeId;
    NamedType *defByNamedType;
    NamedType **ppElementNamedType;
    int *pElementTag;
    int ii;
    char *ptr="";   /* NOT DLL Exported, or ignored on Unix. */
    int extensionsExist = FALSE;

    /* put class spec in hdr file */
    
    if (bVDAGlobalDLLExport != NULL) 
       ptr = bVDAGlobalDLLExport;

    fprintf (hdr, "class %s %s%s\n", ptr, td->cxxTypeDefInfo->className, baseClassesG);
    fprintf (hdr, "{\n");
    fprintf (hdr, "public:\n");

    /* write out choiceId enum type */

        FOR_EACH_LIST_ELMT (e, choice->basicType->a.choice)
        {
            if(e->type->subtypes != NULL)
            {    switch(e->type->subtypes->choiceId)
                {   
                    case SUBTYPE_AND:
                    case SUBTYPE_OR:
                    case SUBTYPE_SINGLE:
                        {
                            PrintCxxMultiConstraintOrHandler(hdr, src, td->cxxTypeDefInfo->className, e, 3);
                            break;
                        }
                    default:
                        {
                            break;
                        }
                }
            }

        }
   

    fprintf (hdr, "  enum %s\n", r->choiceIdEnumName);
    fprintf (hdr, "  {\n");
    FOR_EACH_LIST_ELMT (e, choice->basicType->a.choice)
    {
        fprintf (hdr, "     %s = %d", e->type->cxxTypeRefInfo->choiceIdSymbol, e->type->cxxTypeRefInfo->choiceIdValue);
        if (e != (NamedType*)LAST_LIST_ELMT (choice->basicType->a.choice))
            fprintf (hdr, ",\n");
        else
            fprintf (hdr, "\n");
    }
    fprintf (hdr, "  };\n\n");

    /* write out the choice Id field */
    fprintf (hdr, "  enum %s	%s;\n", r->choiceIdEnumName, r->choiceIdFieldName);

    /* write out the choice element anonymous union */
    fprintf (hdr, "  union\n");
    fprintf (hdr, "  {\n");
    FOR_EACH_LIST_ELMT (e, choice->basicType->a.choice)
    {
        fprintf (hdr, "  ");
        PrintCxxType (hdr, mods, m, r, td, choice, e->type);
	    fprintf (hdr, "%s;\n\n", e->type->cxxTypeRefInfo->fieldName);
    }
    fprintf (hdr, "  };\n\n");

    #if META
        if (printMetaG)
        {
            PrintCxxChoiceDefCodeMeta_1(hdr, src, td, choice, m, e);
        }
    #endif /* META */

    fprintf (hdr, "\n");

    /* constructors and destructor */
    fprintf (hdr, "   %s() {Init();}\n", td->cxxTypeDefInfo->className);

    /* PIERCE 8-22-2001 */
    PrintCopyConstructor(hdr, src, td->cxxTypeDefInfo->className);
	PrintSimpleMeta(hdr,td->cxxTypeDefInfo->className,0);

	/* Init() member function*/
    fprintf (hdr, "   void Init(void);\n");
    fprintf (src, "void %s::Init(void)\n", td->cxxTypeDefInfo->className);
    fprintf (src, "{\n");
    fprintf (src, "   // initialize choice to no choiceId to first choice and set pointer to NULL\n");
    e = FIRST_LIST_ELMT (choice->basicType->a.choice);
    fprintf (src, "  choiceId = %sCid;\n", e->type->cxxTypeRefInfo->fieldName);
    fprintf (src, "  %s = NULL;\n", e->type->cxxTypeRefInfo->fieldName);
    fprintf (src, "}\n\n");
    fprintf (hdr, "\n   virtual int checkConstraints(ConstraintFailList* pConstraintFails)const;\n\n");
	fprintf(src, "\nint %s::checkConstraints(ConstraintFailList* pConstraintFails) const\n{\n",
		td->cxxTypeDefInfo->className);

	FOR_EACH_LIST_ELMT (e, choice->basicType->a.choice)
	{
		if (e->type->cxxTypeRefInfo->isPtr)
		{
			fprintf(src, "\tif (%s != NULL)\n",
				e->type->cxxTypeRefInfo->fieldName);
			fprintf(src, "\t\t%s->checkConstraints(pConstraintFails);\n\n",
				e->type->cxxTypeRefInfo->fieldName);
		}
		else
		{
			fprintf(src, "\t%s.checkConstraints(pConstraintFails);\n\n",
				e->type->cxxTypeRefInfo->fieldName);
		}
	}
	fprintf(src, "\treturn 0;\n");
	fprintf(src, "}\n\n\n");

    /* virtual destructor*/
    fprintf (hdr, "   virtual ~%s() {Clear();}\n\n", td->cxxTypeDefInfo->className);

    /* Clear() member*/
    fprintf (hdr, "   void Clear();\n\n");
    fprintf (src, "void %s::Clear()\n", td->cxxTypeDefInfo->className);
    fprintf (src, "{\n");
    fprintf (src, "  switch (choiceId)\n");
    fprintf (src, "  {\n");

    FOR_EACH_LIST_ELMT (e, choice->basicType->a.choice)
    {
        tmpTypeId = GetBuiltinType (e->type);

	    fprintf (src, "    case %s:\n", e->type->cxxTypeRefInfo->choiceIdSymbol);
	    if (e->type->cxxTypeRefInfo->isPtr)
       {
	      fprintf (src, "      delete %s;\n", e->type->cxxTypeRefInfo->fieldName);
          fprintf (src, "      %s = NULL;\n", e->type->cxxTypeRefInfo->fieldName);
       }
        else if(!e->type->cxxTypeRefInfo->isPtr && 
            ((tmpTypeId == BASICTYPE_CHOICE) ||
                (tmpTypeId == BASICTYPE_SET) ||
                (tmpTypeId == BASICTYPE_SEQUENCE)) )
        {
            fprintf (src, "  %s.Clear();\n", e->type->cxxTypeRefInfo->fieldName);
        }
	    fprintf (src, "      break;\n");
    }

    fprintf (src, "  } // end of switch\n");
    fprintf (src, "} // end of Clear()\n");
    fprintf (src, "\n");

    /* print clone routine for ANY mgmt */
    PrintCloneMethod (hdr, src, td);

    fprintf (hdr, "  %s		&operator = (const %s &that);\n",
             td->cxxTypeDefInfo->className, td->cxxTypeDefInfo->className);
    fprintf (src, "%s &%s::operator = (const %s &that)\n",
             td->cxxTypeDefInfo->className,
             td->cxxTypeDefInfo->className,
             td->cxxTypeDefInfo->className);
    fprintf (src, "{\n");
    fprintf (src, "    if (this != &that) {\n");
    fprintf (src, "        Clear();\n");
   
    e = FIRST_LIST_ELMT (choice->basicType->a.choice);
    fprintf (src, "        if (that.%s != NULL) {\n",
             e->type->cxxTypeRefInfo->fieldName);
        fprintf (src, "            switch (choiceId = that.choiceId) {\n");

    FOR_EACH_LIST_ELMT (e, choice->basicType->a.choice) {
        fprintf (src, "                case %s:\n",
                 e->type->cxxTypeRefInfo->choiceIdSymbol);
        if (e->type->cxxTypeRefInfo->isPtr) {
            fprintf (src, "                    %s = new %s(*that.%s);\n",
                     e->type->cxxTypeRefInfo->fieldName,
                     e->type->cxxTypeRefInfo->className,
                     e->type->cxxTypeRefInfo->fieldName);
        } else {
            fprintf (src, "                    %s = that.%s;\n",
                     e->type->cxxTypeRefInfo->fieldName,
                     e->type->cxxTypeRefInfo->fieldName);
        }
        fprintf (src, "                break;\n");
    }

    fprintf (src, "            }\n");
    fprintf (src, "        }\n");
    fprintf (src, "    }\n");
    fprintf (src, "\n");
    fprintf (src, "    return *this;\n");
    fprintf (src, "}\n\n");

    /* BerEncodeContent */
    if (printEncodersG)
    {
        fprintf (hdr, "  %s		B%s (%s &_b) const;\n", lenTypeNameG, r->encodeContentBaseName, bufTypeNameG);

        fprintf (src, "%s\n", lenTypeNameG);
        fprintf (src, "%s::B%s (%s &_b) const\n", td->cxxTypeDefInfo->className, r->encodeContentBaseName, bufTypeNameG);
        fprintf (src, "{\n");
        fprintf (src, "    FUNC(\"%s::B%s (%s &_b)\");\n", td->cxxTypeDefInfo->className, r->encodeContentBaseName, bufTypeNameG);

        /* print local vars */
        fprintf (src, "  %s l=0;\n", lenTypeNameG);

        fprintf (src, "  switch (%s)\n", r->choiceIdFieldName);
        fprintf (src, "  {\n");
        FOR_EACH_LIST_ELMT (e, choice->basicType->a.choice)
        {
            cxxtri =  e->type->cxxTypeRefInfo;
            fprintf (src, "    case %s:\n", cxxtri->choiceIdSymbol);

            varName = cxxtri->fieldName;

            /* eSNACC 1.5 does not encode indefinite length
             *
             * PrintCxxEocEncoders (src, td, e->type, "_b");
             */

            /* encode content */
            tmpTypeId = GetBuiltinType (e->type);
            if (tmpTypeId == BASICTYPE_ANYDEFINEDBY)
            {
               defByNamedType = e->type->basicType->a.anyDefinedBy->link;
               PrintCxxSetTypeByCode(defByNamedType, cxxtri, src);
            }
            else if (tmpTypeId == BASICTYPE_ANY)
            {
                fprintf (src, "    l = %s", varName);
                if (cxxtri->isPtr)
                    fprintf (src, "->");
                else
                    fprintf (src, ".");

                fprintf (src, "B%s (_b);\n", r->encodeBaseName);
            }
            else if ( (tmpTypeId == BASICTYPE_OCTETCONTAINING) ||
                      (tmpTypeId == BASICTYPE_BITCONTAINING))
            {
               PrintCxxEncodeContaining(e->type, r, src);
            }
            else if ( tmpTypeId == BASICTYPE_EXTENSION )
            {
                fprintf (src, "      l = %s", varName);
                if (cxxtri->isPtr)
                    fprintf (src, "->");
                else
                    fprintf (src, ".");

                fprintf (src, "B%s (_b);\n", r->encodeBaseName);
            }
            else
            {
                fprintf (src, "      l = %s", varName);
                if (cxxtri->isPtr)
                    fprintf (src, "->");
                else
                    fprintf (src, ".");

                fprintf (src, "B%s (_b);\n", r->encodeContentBaseName);
            }

            /* encode tag (s) & len (s) */
            PrintCxxTagAndLenEncodingCode (src, td, e->type, "l", "_b");

            fprintf (src, "      break;\n\n");
        }
        fprintf (src, "      default:\n");
		fprintf (src, "         throw EXCEPT(\"Choice is empty\", ENCODE_ERROR);\n");
        fprintf (src, "  } // end switch\n");

        fprintf (src, "  return l;\n");
        fprintf (src, "} // %s::B%s\n\n\n", td->cxxTypeDefInfo->className, r->encodeContentBaseName);
    }
    /* end of BerEncodeContent method */

    /* BerDecodeContent */
    if (printDecodersG)
    {
        fprintf (hdr, "  void			B%s (const %s &_b, %s tag, %s elmtLen, %s &bytesDecoded /*, s env*/);\n", r->decodeContentBaseName, bufTypeNameG, tagTypeNameG, lenTypeNameG, lenTypeNameG);//, envTypeNameG);
        fprintf (src, "void %s::B%s (const %s &_b, %s tag, %s elmtLen0, %s &bytesDecoded /*, s env*/)\n", td->cxxTypeDefInfo->className, r->decodeContentBaseName, bufTypeNameG, tagTypeNameG, lenTypeNameG, lenTypeNameG);//, envTypeNameG);
        fprintf (src, "{\n");
        fprintf (src, "   FUNC(\"%s::B%s()\");\n",td->cxxTypeDefInfo->className, r->decodeContentBaseName); 
        fprintf (src, "   Clear();\n");
        //fprintf (src, "   Init();\n");
        /* print local vars */
        /* count max number of extra length var nec
         * by counting tag/len pairs on components of the CHOICE
         */

        varCount = 0;
        FOR_EACH_LIST_ELMT (e, choice->basicType->a.choice)
        {
            tmpVarCount = CxxCountVariableLevels (e->type);
            if (tmpVarCount > varCount)
                varCount = tmpVarCount;
        }
        /* write extra length vars - remember choice content
         * decoders are passed the 'key' tag so need one less
         * than max var count.
         */
        for (i = 1; i < varCount; i++)
            fprintf (src, "  %s elmtLen%d = 0;\n", lenTypeNameG, i);

        /* switch on given tag - choices always have the key tag decoded */
        fprintf (src, "  switch (tag)\n");
        fprintf (src, "  {\n");
        FOR_EACH_LIST_ELMT (e, choice->basicType->a.choice)
        {
            if( e->type->basicType->choiceId == BASICTYPE_EXTENSION )
            {
                extensionsExist = TRUE;
            }
            else
            {
            
                cxxtri =  e->type->cxxTypeRefInfo;
                tags = GetTags (e->type, &stoleChoiceTags);

                if (LIST_EMPTY (tags))
                {
                    fprintf (src, "    // ANY Type?\n");
                    fprintf (src, "    case MAKE_TAG_ID (?, ?, ?):\n");
                }
                else
                {
                    tag = (Tag*)FIRST_LIST_ELMT (tags);
                    classStr = Class2ClassStr (tag->tclass);
                    formStr = Form2FormStr (tag->form);

                    if (tag->tclass == UNIV)
                    {
                        codeStr = DetermineCode(tag, NULL, 0);//RWC;Code2UnivCodeStr (tag->code);
                    }
                    else
                    {
                        codeStr = DetermineCode(tag, NULL, 1);
                    }

                    if (tag->form == ANY_FORM)
                    {
                        fprintf (src, "    case MAKE_TAG_ID (%s, %s, %s):\n", classStr, Form2FormStr (PRIM), codeStr);
                        fprintf (src, "    case MAKE_TAG_ID (%s, %s, %s):\n", classStr, Form2FormStr (CONS), codeStr);
                    }
                    else
                    {
                        fprintf (src, "    case MAKE_TAG_ID (%s, %s, %s):\n", classStr, formStr, codeStr);
                    }

                    /* now decode extra tags/length pairs */
                    AsnListFirst (tags);
                    AsnListNext (tags);
                    elmtLevel = 0;
                    if (stoleChoiceTags)
                    {
                        FOR_REST_LIST_ELMT (tag, tags)
                        {
                            classStr = Class2ClassStr (tag->tclass);

                            formStr = Form2FormStr (tag->form);

                            if (tag->tclass == UNIV)
                                codeStr = DetermineCode(tag, NULL, 0);//RWC;Code2UnivCodeStr (tag->code);
                            else
                                codeStr = DetermineCode(tag, NULL, 1);

                            if (tag->form == ANY_FORM)
                            {
                                fprintf (src, "    case MAKE_TAG_ID (%s, %s, %s):\n", classStr, Form2FormStr (PRIM), codeStr);
                                fprintf (src, "    case MAKE_TAG_ID (%s, %s, %s):\n", classStr, Form2FormStr (CONS), codeStr);
                            }
                            else
                            {
                                fprintf (src, "    case MAKE_TAG_ID (%s, %s, %s):\n", classStr, formStr, codeStr);
                            }
                        }
                    }
                    else /* didn't steal nested choice's tags */
                    {
                        FOR_REST_LIST_ELMT (tag, tags)
                        {
                            classStr = Class2ClassStr (tag->tclass);
                            codeStr = DetermineCode(tag, NULL, 0);//RWC;Code2UnivCodeStr (tag->code);
                            formStr = Form2FormStr (tag->form);

                            fprintf (src, "      tag = BDecTag (_b, bytesDecoded);\n");
                            if (tag->form == ANY_FORM)
                            {
                                if (tag->tclass == UNIV)
                                {
                                    fprintf (src, "      if ((tag != MAKE_TAG_ID (%s, %s, %s))\n",  classStr, Form2FormStr (PRIM), codeStr);
                                    fprintf (src, "          && (tag != MAKE_TAG_ID (%s, %s, %s)))\n", classStr, Form2FormStr (CONS), codeStr);
                                }
                                else
                                {
                                    fprintf (src, "      if ((tag != MAKE_TAG_ID (%s, %s, %s))\n", classStr, Form2FormStr (PRIM), DetermineCode(tag, NULL, 1));//RWC;tag->code);
                                    fprintf (src, "          && (tag != MAKE_TAG_ID (%s, %s, %s)))\n", classStr, Form2FormStr (CONS), DetermineCode(tag, NULL, 1));//RWC;tag->code);
                                }
                            }
                            else
                            {
                                if (tag->tclass == UNIV)
                                    fprintf (src, "      if (tag != MAKE_TAG_ID (%s, %s, %s))\n", classStr, formStr, codeStr);
                                else
                                    fprintf (src, "      if (tag != MAKE_TAG_ID (%s, %s, %s))\n", classStr, formStr, DetermineCode(tag, NULL, 1));//RWC;tag->code);
                            }

                            fprintf (src, "      {\n");
                        
                            fprintf (src, "        throw InvalidTagException(typeName(), tag, STACK_ENTRY);\n");
                            fprintf (src, "      }\n\n");

                            fprintf (src, "      elmtLen%d = BDecLen (_b, bytesDecoded);\n", ++elmtLevel);
                        }
                    }
                }
                /*
                 * if the choices element is another choice &&
                 * we didn't steal its tags then we must grab
                 * the key tag out of the contained CHOICE
                 */
                if (!stoleChoiceTags && (GetBuiltinType (e->type) == BASICTYPE_CHOICE))
                {
                    fprintf (src, "      tag = BDecTag (_b, bytesDecoded);\n");
                    fprintf (src, "      elmtLen%d = BDecLen (_b, bytesDecoded);\n", ++elmtLevel);
                }

                varName = cxxtri->fieldName;
                /* set choice id for to this elment */
                fprintf (src, "      %s = %s;\n", r->choiceIdFieldName, cxxtri->choiceIdSymbol);

                /* alloc elmt if nec */
                if (cxxtri->isPtr)
                {
                    fprintf (src, "        %s = new %s;\n", varName, cxxtri->className);
                }
                /* decode content */
                tmpTypeId = GetBuiltinType (e->type);
                if (tmpTypeId == BASICTYPE_ANYDEFINEDBY)
                {
                    /*
                     * must check for another EOC for ANYs
                     * since the any decode routines decode
                     * their own first tag/len pair
                     */
                    elmtLevel++;
                    defByNamedType = e->type->basicType->a.anyDefinedBy->link;
                    PrintCxxSetTypeByCode(defByNamedType, cxxtri, src);

                    fprintf (src, "      %s", varName);
                    if (cxxtri->isPtr)
                        fprintf (src, "->");
                    else
                        fprintf (src, ".");
                    fprintf (src, "B%s (_b, bytesDecoded);\n",  r->decodeBaseName);
                }
                else if (tmpTypeId == BASICTYPE_ANY)
                {
                    elmtLevel++;
                    fprintf (src, "        %s", varName);
                    if (cxxtri->isPtr)
                       fprintf (src, "->");
                    else
                       fprintf (src, ".");
                    fprintf (src, "B%s (_b, bytesDecoded);\n",  r->decodeBaseName);
                }
                else if ( (tmpTypeId == BASICTYPE_OCTETCONTAINING) ||
							     (tmpTypeId == BASICTYPE_BITCONTAINING) )
                {
                   PrintCxxDecodeContaining(e->type, r, src);
                }
                else
                {
                    fprintf (src, "        %s", varName);
                    if (cxxtri->isPtr)
                        fprintf (src, "->");
                    else
                        fprintf (src, ".");

                    fprintf (src, "B%s (_b, tag, elmtLen%d, bytesDecoded);\n",  r->decodeContentBaseName, elmtLevel);
                }

                /* decode Eoc (s) */
                for (i = elmtLevel-1; i >= 0; i--)
                {
                    fprintf (src, "      if (elmtLen%d == INDEFINITE_LEN)\n", i);
                    fprintf (src, "        BDecEoc (_b, bytesDecoded);\n");
                }

                fprintf (src, "      break;\n\n");
                FreeTags (tags);
            }
        }

        fprintf (src, "    default:\n");
        fprintf (src, "    {");

        if(extensionsExist)
        {
            fprintf (src, "     AsnAny extAny;\n");
            fprintf (src, "     extension = new AsnExtension;\n");
            fprintf (src, "     choiceId = extensionCid;\n");
            fprintf (src, "     extAny.BDecContent(_b, tag, elmtLen0, bytesDecoded);\n");
            fprintf (src, "     extension->extList.insert( extension->extList.end(), extAny );\n");
        }
        else
        {
            fprintf (src, "        throw InvalidTagException(typeName(), tag, STACK_ENTRY);\n");
        }

        fprintf (src, "      break;\n");
        fprintf (src, "    }\n");
        fprintf (src, "  } // end switch\n");
        fprintf (src, "} // %s::B%s\n\n\n", td->cxxTypeDefInfo->className, r->decodeContentBaseName);
    }
    /* end of code for printing BDecodeContent method */

    /* do BEnc function */
    if (printEncodersG)
    {
        fprintf (hdr, "  %s		B%s (%s &_b) const;\n", lenTypeNameG, r->encodeBaseName, bufTypeNameG);
        fprintf (src, "%s %s::B%s (%s &_b) const\n", lenTypeNameG, td->cxxTypeDefInfo->className, r->encodeBaseName, bufTypeNameG);
        fprintf (src, "{\n");
        fprintf (src, "    %s l=0;\n", lenTypeNameG);
        fprintf (src, "    l = B%s (_b);\n", r->encodeContentBaseName);

        /* encode each tag/len pair if any */
        FOR_EACH_LIST_ELMT_RVS (tag, choice->tags)
        {
            classStr = Class2ClassStr (tag->tclass);
            formStr = Form2FormStr (CONS);  /* choices are constructed */
            //RWC;tagLen = TagByteLen (tag->code);
            fprintf (src, "    l += BEncConsLen (_b, l);\n");

            if (tag->tclass == UNIV)
            {
                const char* ptr = DetermineCode(tag, &tagLen, 1);
                fprintf (src, "    l += BEncTag%d (_b, %s, %s, %s);\n", tagLen, classStr, formStr, ptr);//RWC;Code2UnivCodeStr (tag->code));
            }
            else
            {
                const char* ptr = DetermineCode(tag, &tagLen, 1);
                fprintf (src, "    l += BEncTag%d (_b, %s, %s, %s);\n", tagLen, classStr, formStr, ptr);//RWC;tag->code);
            }
        }
        fprintf (src, "    return l;\n");
        fprintf (src, "}\n\n");
    }
    /* end of BEnc function */

    /* Do BDec function */
    if (printDecodersG)
    {
        fprintf (hdr, "  void			B%s (const %s &_b, %s &bytesDecoded);\n", r->decodeBaseName, bufTypeNameG, lenTypeNameG);
        fprintf (src, "void %s::B%s (const %s &_b, %s &bytesDecoded)\n", td->cxxTypeDefInfo->className, r->decodeBaseName, bufTypeNameG, lenTypeNameG);//, envTypeNameG);
        fprintf (src, "{\n");

        if (choice->tags->count > 0)
           fprintf (src, "    FUNC(\"%s::B%s\")\n", td->cxxTypeDefInfo->className, r->decodeBaseName);

        fprintf (src, "    %s elmtLen = 0;\n", lenTypeNameG);
        fprintf (src, "    %s tag;\n", tagTypeNameG);

        /* print extra locals for redundant lengths */
        for (i = 1; (choice->tags != NULL) && (i <= LIST_COUNT (choice->tags)); i++)
        {
            fprintf (src, "    %s extraLen%d = 0;\n", lenTypeNameG, i);
        }
        fprintf (src, "\n");

        /*  decode tag/length pair (s) */
        elmtLevel = 0;
        FOR_EACH_LIST_ELMT (tag, choice->tags)
        {
            classStr = Class2ClassStr (tag->tclass);
            formStr = Form2FormStr (CONS);  /* choices are constructed */
            fprintf (src, "    AsnTag tagId = BDecTag (_b, bytesDecoded);\n");
            fprintf (src, "    if (tagId != ");
            if (tag->tclass == UNIV)
            {
                fprintf (src, "MAKE_TAG_ID (%s, %s, %s))", classStr, formStr, DetermineCode(tag, NULL, 0));//RWC;Code2UnivCodeStr (tag->code));
            }
            else
            {
                fprintf (src, "MAKE_TAG_ID (%s, %s, %s))", classStr, formStr, DetermineCode(tag, NULL, 1));//RWC;tag->code);
            }
            fprintf (src, "    {\n");
            fprintf (src, "          throw InvalidTagException(typeName(), tagId, STACK_ENTRY);\n");
            fprintf (src, "    }\n");
            fprintf (src, "    extraLen%d = BDecLen (_b, bytesDecoded);\n", ++elmtLevel);
        }

        /* decode identifying tag from choice body */
        fprintf (src, "    /*  CHOICEs are a special case - grab identifying tag */\n");
        fprintf (src, "    /*  this allows easier handling of nested CHOICEs */\n");
        fprintf (src, "    tag = BDecTag (_b, bytesDecoded);\n");
        fprintf (src, "    elmtLen = BDecLen (_b, bytesDecoded);\n");
        fprintf (src, "    B%s (_b, tag, elmtLen, bytesDecoded);\n", r->decodeContentBaseName);

        /* grab any EOCs that match redundant, indef lengths */
        for (i = elmtLevel; i > 0; i--)
        {
            fprintf (src, "    if (extraLen%d == INDEFINITE_LEN)\n", i);
            fprintf (src, "        BDecEoc (_b, bytesDecoded);\n");
        }

        fprintf (src, "}\n\n");
    }
    /* end of BDec function */


   if(genPERCode)
   {
    /* do PER Encode, PEnc function */
    if (printEncodersG)
    {
        /****************************/
        /*** FIRST, handle index encoding for PER Choice.  Taking advantage of 
           * the AsnInt class with constraints for the detailed encoding 
           * details.  Declare outside scope of source method for PEnc/PDec. */
        fprintf (src, "class AsnIntChoice_%s: public AsnInt  {\n", td->cxxTypeDefInfo->className);
        fprintf (src, "  public:\n");
        fprintf (src, "  AsnIntChoice_%s(AsnIntType val=0):AsnInt(val){ }\n", 
                         td->cxxTypeDefInfo->className);
        fprintf (src, "  ValueRange* ValueRanges(int &sizeVRList)\n");
        fprintf (src, "  {\n");
        fprintf (src, "  	static ValueRange INT1_ValueRangeList[] = \n");
        fprintf (src, "  		{{ 0, %d, 1 }};\n",
                 choice->basicType->a.choice->count);  /* CONSTANT value for this CHOICE. */
        fprintf (src, "  	sizeVRList = 1;\n");
        fprintf (src, "  	return INT1_ValueRangeList;\n");
        fprintf (src, "  }\n");
        fprintf (src, "};\n\n");

		//RWC;fprintf (hdr, "  AsnLen		PEnc(AsnBufBits &_b, bool bAlign = false) const {AsnLen len; len = 1;return len;};\n");
        fprintf (hdr, "  %s		P%s (AsnBufBits &_b) const;\n", lenTypeNameG, r->encodeBaseName);
        fprintf (src, "%s %s::P%s (AsnBufBits &_b) const\n", lenTypeNameG, td->cxxTypeDefInfo->className, r->encodeBaseName);
        fprintf (src, "{\n");
        fprintf (src, "    %s l=0;\n", lenTypeNameG);
        fprintf (src, "    FUNC(\"%s::P%s (AsnBufBits &_b)\");\n", td->cxxTypeDefInfo->className, r->encodeBaseName);

        /****************************/
        /*** PERFORM sorting of Choice elements for proper index setting. */
        PrintCxxDefCode_PERSort(&ppElementNamedType, &pElementTag, choice->basicType->a.choice);
        fprintf (src, "  AsnIntChoice_%s TmpAsnIntChoice(%d);\n", 
                 td->cxxTypeDefInfo->className, 
                 choice->basicType->a.choice->count);  /* CONSTANT value for this CHOICE. */
        for (ii=0; ii < choice->basicType->a.choice->count; ii++)
        {
            fprintf (src, "   if (%s == %s::%s)\n", r->choiceIdFieldName, 
                  td->cxxTypeDefInfo->className, 
                  ppElementNamedType[ii]->type->cxxTypeRefInfo->choiceIdSymbol);
            fprintf (src, "      TmpAsnIntChoice.Set(%d); // SORTED index value.\n", ii);
        }       // END FOR ii
        free(ppElementNamedType);
        free(pElementTag);

        /*** SETUP specific sorted index value. */
        fprintf (src, "  l = TmpAsnIntChoice.PEnc(_b); // LOAD PER encoded, constrained Choice index value.\n");

        /****************************/
        /*** NOW, setup each individual Choice element.*/
        //RWC;fprintf (src, "    l = P%s (_b);\n", r->encodeContentBaseName);
        /* print local vars */
        //RWC;fprintf (src, "  %s l=0;\n", lenTypeNameG);

        /* encode tag (s) & len (s) */
        //PrintCxxTagAndLenEncodingCode (src, td, e->type, "l", "_b");
        // RWC; TAGS already encoded if necessary above (non-UNIV).

        fprintf (src, "  switch (%s)\n", r->choiceIdFieldName);
        fprintf (src, "  {\n");
        FOR_EACH_LIST_ELMT (e, choice->basicType->a.choice)
        {
            cxxtri =  e->type->cxxTypeRefInfo;
            fprintf (src, "    case %s:\n", cxxtri->choiceIdSymbol);
      
            varName = cxxtri->fieldName;

            /* encode content */
            tmpTypeId = GetBuiltinType (e->type);
            if (tmpTypeId == BASICTYPE_ANYDEFINEDBY)
            {
               defByNamedType = e->type->basicType->a.anyDefinedBy->link;
               PrintCxxSetTypeByCode(defByNamedType, cxxtri, src);
            }
            else if (tmpTypeId == BASICTYPE_ANY)
            {
                fprintf (src, "    l += %s", varName);
                if (cxxtri->isPtr)
                    fprintf (src, "->");
                else
                    fprintf (src, ".");

                fprintf (src, "P%s (_b);\n", r->encodeBaseName);
            }
            else if ( (tmpTypeId == BASICTYPE_OCTETCONTAINING) ||
                      (tmpTypeId == BASICTYPE_BITCONTAINING))
            {
               PrintCxxPEREncodeContaining(e->type, r, src);
            }
            else
            {
                fprintf (src, "      l += %s", varName);
                if (cxxtri->isPtr)
                    fprintf (src, "->");
                else
                    fprintf (src, ".");

                fprintf (src, "P%s (_b);\n", r->encodeBaseName);
            }
            fprintf (src, "      break;\n\n");
        }
        fprintf (src, "      default:\n");
		  fprintf (src, "         throw EXCEPT(\"Choice is empty\", ENCODE_ERROR);\n");
        fprintf (src, "  } // end switch\n");
        /****************************/

        fprintf (src, "    return l;\n");
        fprintf (src, "}    //%s::P%s(...)\n\n", td->cxxTypeDefInfo->className, r->encodeBaseName);
    }       /* END IF printEncodersG */
    /* end of PEnc function */

    /* Do PDec function */
    if (printDecodersG)
    {
        fprintf (hdr, "  void			P%s (AsnBufBits &_b, %s &bitsDecoded);\n", r->decodeBaseName, lenTypeNameG);
        fprintf (src, "void %s::P%s (AsnBufBits &_b, %s &bitsDecoded)\n", td->cxxTypeDefInfo->className, r->decodeBaseName, lenTypeNameG);//, envTypeNameG);
        fprintf (src, "{\n");
        fprintf (src, "\tClear();\n");

        /* print extra locals for redundant lengths */
        for (i = 1; (choice->tags != NULL) && (i <= LIST_COUNT (choice->tags)); i++)
        {
            //fprintf (src, "    %s extraLen%d = 0; \n", lenTypeNameG, i);
        }
        fprintf (src, "\n");

        /*  decode tag/length pair (s) */
        elmtLevel = 0;

        /****************************/
        fprintf (src, "  AsnIntChoice_%s TmpAsnIntChoice;\n", td->cxxTypeDefInfo->className);

        /*** SETUP specific sorted index value. */
        fprintf (src, "  TmpAsnIntChoice.PDec(_b, bitsDecoded); // LOAD PER decoded, constrained Choice index value.\n");

        /* decode identifying tag from choice body */
        fprintf (src, "    /*  CHOICEs are a special case - grab identifying tag */\n");
        fprintf (src, "    /*  this allows easier handling of nested CHOICEs */\n");

        /* print local vars */
        /* count max number of extra length var nec
         * by counting tag/len pairs on components of the CHOICE
         */
        varCount = 0;
        FOR_EACH_LIST_ELMT (e, choice->basicType->a.choice)
        {
            tmpVarCount = CxxCountVariableLevels (e->type);
            if (tmpVarCount > varCount)
                varCount = tmpVarCount;
        }
 
        /*** PERFORM sorting of Choice elements for proper index setting, then 
             determine actual Choice chosen by the user. */
        PrintCxxDefCode_PERSort(&ppElementNamedType, &pElementTag, choice->basicType->a.choice);
        for (ii=0; ii < choice->basicType->a.choice->count; ii++)
        {
            fprintf (src, "   if (TmpAsnIntChoice == %d)\n", ii);
            fprintf (src, "   {\n"); 
            fprintf (src, "      %s = %s::%s;\n", r->choiceIdFieldName, 
                  td->cxxTypeDefInfo->className, 
                  ppElementNamedType[ii]->type->cxxTypeRefInfo->choiceIdSymbol);

            /* Process specific tag - choices always have the key tag decoded */
                e = ppElementNamedType[ii];
                cxxtri =  e->type->cxxTypeRefInfo;

                varName = cxxtri->fieldName;

                /* alloc elmt if nec */
                if (cxxtri->isPtr)
                {
                   fprintf (src, "      %s = new %s;\n", varName, cxxtri->className);
                }
                /* decode content */
                tmpTypeId = GetBuiltinType (e->type);
                if (tmpTypeId == BASICTYPE_ANYDEFINEDBY)
                {
                    /*
                     * must check for another EOC for ANYs
                     * since the any decode routines decode
                     * their own first tag/len pair
                     */
                    elmtLevel++;

                    defByNamedType = e->type->basicType->a.anyDefinedBy->link;
                    PrintCxxSetTypeByCode(defByNamedType, cxxtri, src);

                    fprintf (src, "      %s", varName);
                    if (cxxtri->isPtr)
                        fprintf (src, "->");
                    else
                        fprintf (src, ".");
                    fprintf (src, "P%s (_b, bitsDecoded);\n",  r->decodeBaseName);
                }
                else if (tmpTypeId == BASICTYPE_ANY)
                {
                    elmtLevel++;
                    fprintf (src, "        %s", varName);
                    if (cxxtri->isPtr)
                       fprintf (src, "->");
                    else
                       fprintf (src, ".");
                    fprintf (src, "P%s (_b, bitsDecoded);\n",  r->decodeBaseName);
                }
                else if ( (tmpTypeId == BASICTYPE_OCTETCONTAINING) ||
							     (tmpTypeId == BASICTYPE_BITCONTAINING) )
                {
                   PrintCxxPERDecodeContaining(e->type, r, src);
                }
                else
                {
                    fprintf (src, "      %s", varName);
                    if (cxxtri->isPtr)
                        fprintf (src, "->");
                    else
                        fprintf (src, ".");

                    fprintf (src, "P%s (_b, bitsDecoded);\n",  r->decodeBaseName);
                }


            fprintf (src, "   }  // END if this Choice ID chosen\n"); 
        }       // END FOR ii

        free(ppElementNamedType);
        free(pElementTag);

        fprintf (src, "}   // END %s::P%s(...)\n\n", td->cxxTypeDefInfo->className, r->decodeBaseName);
    }
    /* end of PDec function */
   } /* if genPERCode */


    /* ostream printing routine */
    if (printPrintersG)
    {
        fprintf(hdr, "\tvoid Print(std::ostream& os, unsigned short indent = 0) const;\n");
        fprintf(src, "void %s::Print(std::ostream& os, unsigned short indent) const\n",
			td->cxxTypeDefInfo->className);
        fprintf(src, "{\n");
        fprintf(src, "\tswitch (choiceId)\n");
        fprintf(src, "\t{\n");

        FOR_EACH_LIST_ELMT (e, choice->basicType->a.choice)
        {
            fprintf (src, "\tcase %s:\n", e->type->cxxTypeRefInfo->choiceIdSymbol);

            /* value notation so print the choice elmts field name */
            if (e->fieldName != NULL)
                fprintf(src, "\t\tos << \"%s \";\n", e->fieldName);

            if (e->type->cxxTypeRefInfo->isPtr)
			{
                fprintf(src, "\t\tif (%s)\n", e->type->cxxTypeRefInfo->fieldName);
                fprintf(src, "\t\t\t%s->Print(os, indent);\n",
					e->type->cxxTypeRefInfo->fieldName);
                fprintf(src, "\t\telse\n");
                fprintf(src, "\t\t\tos << \"<CHOICE value is missing>\\n\";\n");
			}
			else
			{
                fprintf(src, "\t\t%s.Print(os, indent);\n",
					e->type->cxxTypeRefInfo->fieldName);
			}

            fprintf (src, "\t\tbreak;\n\n");
        }
        fprintf (src, "\t} // end of switch\n");

        fprintf (src, "} // end of %s::Print()\n\n", td->cxxTypeDefInfo->className);

        fprintf(hdr, "  void			PrintXML (std::ostream &os, const char *lpszTitle=NULL) const;\n");

        fprintf(src, "void %s::PrintXML (std::ostream &os, const char *lpszTitle) const\n", td->cxxTypeDefInfo->className);
        fprintf(src, "{\n");
        fprintf(src, "    const char *tagName = typeName();\n");
        fprintf(src, "    if (lpszTitle)\n");
        fprintf(src, "        tagName = lpszTitle;");
        fprintf(src, "    os << \"<\" << tagName << \">\";\n");
        fprintf(src, "    switch (choiceId) {\n");

        FOR_EACH_LIST_ELMT (e, choice->basicType->a.choice)
        {
            fprintf(src, "    case %s:\n",
                     e->type->cxxTypeRefInfo->choiceIdSymbol);

            /* value notation so print the choice elmts field name */
            if (e->type->cxxTypeRefInfo->isPtr)
            {
                fprintf(src, "       if (%s) {\n",
                        e->type->cxxTypeRefInfo->fieldName);
                fprintf(src, "           %s->PrintXML(os", e->type->cxxTypeRefInfo->fieldName);
                if (e->fieldName != NULL)
                   fprintf(src, ",\"%s\");\n", e->fieldName);
                else
                   fprintf(src, ");\n");
                fprintf(src, "      }\n");
            } else
                fprintf(src, "      %s.PrintXML(os, \"%s\");\n",
                        e->type->cxxTypeRefInfo->fieldName, e->fieldName);

            fprintf(src, "      break;\n\n");
        }
        fprintf (src, "    } // end of switch\n");
        fprintf (src, "    os << \"</\" << tagName << \">\";\n");
        fprintf (src, "} // %s::PrintXML\n\n", td->cxxTypeDefInfo->className);
    }
    /* end of Print Method code */

    /* close class definition */
    fprintf (hdr, "};\n\n\n");
	novolatilefuncs = novolatilefuncs;

    parent=parent;  /*AVOIDS warning.*/
} /* PrintCxxChoiceDefCode */


static void
PrintCxxSeqDefCode (FILE *src, FILE *hdr, ModuleList *mods, Module *m,
                    CxxRules *r ,TypeDef *td, Type *parent ESNACC_UNUSED,
                    Type *seq, int novolatilefuncs ESNACC_UNUSED)
{
    NamedType *e;
    char *classStr;
    char *formStr;
    char *codeStr;
    int tagLen, i=0;
    Tag *tag;
    TagList *tags;
    char *varName;
    CxxTRI *cxxtri=NULL;
    int elmtLevel=0;
    int varCount, tmpVarCount;
    int stoleChoiceTags;
    int inTailOptElmts;
    enum BasicTypeChoiceId tmpTypeId;
    NamedType *defByNamedType;
    NamedType *tmpElmt;

    // DEFINE PER encode/decode tmp vars.
    NamedType **pSeqElementNamedType=NULL;
    int ii;
    int extensionAdditionFound = FALSE;

    /* put class spec in hdr file */
    char *ptr="";   /* NOT DLL Exported, or ignored on Unix. */
    if (bVDAGlobalDLLExport != NULL) 
       ptr = bVDAGlobalDLLExport;

    fprintf (hdr, "class %s %s%s\n", ptr, td->cxxTypeDefInfo->className, baseClassesG);
    fprintf (hdr, "{\n");
    fprintf (hdr, "public:\n");

    /* write out the sequence elmts */
    FOR_EACH_LIST_ELMT (e, seq->basicType->a.sequence)
    {
        fprintf (hdr, "  ");
    
		/* JKG 7/31/03 */
		/*The following code enclosed in this if/else statement */
		/*is constructed for constraint handling capability     */
		/*for primitives found in sequences or sets             */
            
            if(e->type->subtypes!=NULL)
		    {
			    switch(e->type->subtypes->choiceId)
			    {
			        case SUBTYPE_AND:
			        case SUBTYPE_OR:
                    case SUBTYPE_SINGLE:
			        {
				        if(!PrintCxxMultiConstraintOrHandler(hdr, src, td->cxxTypeDefInfo->className, e, 1))
				        {
					        PrintCxxType (hdr, mods, m, r, td, seq, e->type);
					        fprintf (hdr, "%s;\n\n", e->type->cxxTypeRefInfo->fieldName);
    			        }
 				        break;
			        }
			        default:
			        {
				        PrintCxxType (hdr, mods, m, r, td, seq, e->type);
				        fprintf (hdr, "%s;\n\n", e->type->cxxTypeRefInfo->fieldName);
				        break;
			        }
		        }
	        }
	        else
	        {
		        PrintCxxType (hdr, mods, m, r, td, seq, e->type);
		        fprintf (hdr, "%s;\n\n", e->type->cxxTypeRefInfo->fieldName);
	        }
        
            fprintf (hdr, "\n");

	}
    #if META
        if (printMetaG)
        {
            PrintCxxSeqDefCodeMeta_1(hdr, src, td, seq, m, e)
        }
    #endif /* META */

    /* constructors and destructor: */
	/* Default constructor
	 */
    fprintf (hdr, "   %s() {Init();}\n", td->cxxTypeDefInfo->className);

	/* Init() member function 
	 */
    fprintf (hdr, "   void Init(void);\n");
    fprintf (src, "void %s::Init(void)\n", td->cxxTypeDefInfo->className);
    fprintf (src, "{\n");
    FOR_EACH_LIST_ELMT (e, seq->basicType->a.sequence)
    {
	   if ((e->type->cxxTypeRefInfo->isPtr))
	   {

    #if TCL
		       fprintf (src, "#if TCL\n");
		       fprintf (src, "  %s = new %s;\n", e->type->cxxTypeRefInfo->fieldName, e->type->cxxTypeRefInfo->className);
		       fprintf (src, "#else\n");
		       fprintf (src, "  %s = NULL;\n", e->type->cxxTypeRefInfo->fieldName);
		       fprintf (src, "#endif // TCL\n");
    #else
   
       /* Default member initialization is a work in progress for eSNACC 1.6
        */

         /* initialize default members to their default values */
         if (e->type->defaultVal != NULL)
           {
               Value *defVal = GetValue(e->type->defaultVal->value);

               /* HANDLE DEFAULT VALUE DECODING FOR DER by initializing the respective members to their default
                * values.
                */
                switch(ParanoidGetBuiltinType(e->type))
                {
                case BASICTYPE_INTEGER:
                case BASICTYPE_ENUMERATED:
                     fprintf(src,"  %s = new %s(%d);\n", 
                        e->type->cxxTypeRefInfo->fieldName, 
                        e->type->cxxTypeRefInfo->className,
                        defVal->basicValue->a.integer);
                     break;
                case BASICTYPE_BOOLEAN:
                     fprintf(src,"  %s = new %s(%s);\n", 
                        e->type->cxxTypeRefInfo->fieldName, 
                        e->type->cxxTypeRefInfo->className,
                        defVal->basicValue->a.boolean == 0 ? "false": "true");
                     break;
                case BASICTYPE_BITSTRING:
                {
                   NamedNumberList *pNNL = GetNamedElmts(e->type);
                   BasicValue *pBV;
                   pBV = GetLastNamedNumberValue(pNNL);

                   if (defVal->basicValue->choiceId == BASICVALUE_VALUENOTATION &&
                       pBV != NULL)
                   {
                      char *defBitStr;
                      normalizeValue(&defBitStr, defVal->basicValue->a.valueNotation->octs);
                      
                      fprintf(src,"  %s = new %s(%d);\n", 
                        e->type->cxxTypeRefInfo->fieldName, 
                        e->type->cxxTypeRefInfo->className,
                       pBV->a.integer);
                    
                      fprintf(src, "  %s->SetBit(%s::%s);\n",
                        e->type->cxxTypeRefInfo->fieldName,
                        e->type->cxxTypeRefInfo->className,
                        defBitStr);
                      free(defBitStr);
                   }
                   else
                      printf("\nWARNING: unsupported use of default BIT STRING\n");
               }
               break;
                default:
                {
                      fprintf (src, "  %s = new %s;\n", e->type->cxxTypeRefInfo->fieldName, e->type->cxxTypeRefInfo->className);
                }

             } /* end switch */
         }
         else
         {
              if(e->type->optional)
             {
                fprintf (src, "  %s = NULL;\n", e->type->cxxTypeRefInfo->fieldName);
             }
             else
             {
                fprintf (src, "  %s = new %s;\n", e->type->cxxTypeRefInfo->fieldName, e->type->cxxTypeRefInfo->className);
             }
         }
        /* end of default member initialization */
   
    #endif /* TCL */
	   }
    }
    fprintf (src, "}\n\n");

	/* virtual destructor
	*/
    fprintf (hdr, "   virtual ~%s() {Clear();}\n", td->cxxTypeDefInfo->className);
    /* Clear() member
     */
    fprintf (hdr, "   void Clear();\n");

	/*JKG: 7/30/03 -- virtual check Constraints method*/

	fprintf(hdr, "\n\tint checkConstraints(ConstraintFailList* pConstraintFails) const;\n\n");
	fprintf(src, "\nint %s::checkConstraints(ConstraintFailList* pConstraintFails) const{\n",
		td->cxxTypeDefInfo->className);

	FOR_EACH_LIST_ELMT (e, seq->basicType->a.sequence)
	{
		if (e->type->cxxTypeRefInfo->isPtr)
		{
			fprintf(src, "\tif (%s != NULL)\n", e->type->cxxTypeRefInfo->fieldName);
			fprintf(src, "\t\t%s->checkConstraints(pConstraintFails);\n\n",
				e->type->cxxTypeRefInfo->fieldName);
		}
		else
		{
			fprintf(src, "\t%s.checkConstraints(pConstraintFails);\n\n",
				e->type->cxxTypeRefInfo->fieldName);
		}
	}

	fprintf(src, "\treturn 0;\n");
	fprintf(src, "}\n\n\n");

    fprintf (src, "void %s::Clear()\n", td->cxxTypeDefInfo->className);
    fprintf (src, "{\n");
    FOR_EACH_LIST_ELMT (e, seq->basicType->a.sequence)
    { 
        tmpTypeId = GetBuiltinType (e->type);
        if (e->type->cxxTypeRefInfo->isPtr)
        {
            fprintf (src, "  if(%s)\n", e->type->cxxTypeRefInfo->fieldName);
	        fprintf (src, "    delete %s;\n", e->type->cxxTypeRefInfo->fieldName);
            fprintf (src, "  %s = NULL;\n", e->type->cxxTypeRefInfo->fieldName);
        }
        else if(!e->type->cxxTypeRefInfo->isPtr && 
            ((tmpTypeId == BASICTYPE_CHOICE) ||
                (tmpTypeId == BASICTYPE_SET) ||
                (tmpTypeId == BASICTYPE_SEQUENCE)) )
        {
            fprintf (src, "  %s.Clear();\n", e->type->cxxTypeRefInfo->fieldName);
        }
    }

    fprintf (src, "}\n\n");

    /* PIERCE 8-22-2001 */
    PrintCopyConstructor(hdr, src, td->cxxTypeDefInfo->className);
    PrintSimpleMeta(hdr, td->cxxTypeDefInfo->className,0);

    /* print clone routine for ANY mgmt */
    PrintCloneMethod (hdr, src, td);
    
    /* print operator = overload */
    fprintf (hdr, "  %s		&operator = (const %s &that);\n", td->cxxTypeDefInfo->className, td->cxxTypeDefInfo->className);
    fprintf (src, "%s &%s::operator = (const %s &that)\n", td->cxxTypeDefInfo->className, td->cxxTypeDefInfo->className, td->cxxTypeDefInfo->className);
    fprintf (src, "{\n");
    fprintf (src, "  if (this != &that)\n");
    fprintf (src, "  {\n");
    fprintf (src, "    Clear();\n");

    FOR_EACH_LIST_ELMT (e, seq->basicType->a.sequence)
    {
	    if (e->type->cxxTypeRefInfo->isPtr)
       {
	    fprintf (src, "    if (that.%s)\n", e->type->cxxTypeRefInfo->fieldName);
	    fprintf (src, "    {\n");
	    fprintf (src, "      if (!%s)\n", e->type->cxxTypeRefInfo->fieldName);
    	fprintf (src, "        %s = new %s;\n", e->type->cxxTypeRefInfo->fieldName, e->type->cxxTypeRefInfo->className);
		fprintf (src, "      *%s = *that.%s;\n", e->type->cxxTypeRefInfo->fieldName, e->type->cxxTypeRefInfo->fieldName);
	    fprintf (src, "    }\n");
	    fprintf (src, "    else\n");
	    fprintf (src, "    {\n");
	    fprintf (src, "      delete %s;\n", e->type->cxxTypeRefInfo->fieldName);
	    fprintf (src, "      %s = NULL;\n", e->type->cxxTypeRefInfo->fieldName);
	    fprintf (src, "    }\n");
       }
	    else
       {
	       fprintf (src, "    %s = that.%s;\n", e->type->cxxTypeRefInfo->fieldName, 
             e->type->cxxTypeRefInfo->fieldName);
       }
    }
    fprintf (src, "  }\n");
    fprintf (src, "\n");
    fprintf (src, "  return *this;\n");
    fprintf (src, "}\n\n");

    /* BerEncodeContent method */
    if (printEncodersG) {
        fprintf(hdr, "  %s		B%s (%s &_b) const;\n", lenTypeNameG,
                r->encodeContentBaseName, bufTypeNameG);
        fprintf(src, "%s\n", lenTypeNameG);
        fprintf(src, "%s::B%s (%s &_b) const\n", td->cxxTypeDefInfo->className,
                r->encodeContentBaseName, bufTypeNameG);
        fprintf(src, "{\n");

        /* print local vars */
        fprintf(src, "  %s totalLen = 0;\n", lenTypeNameG);
        fprintf(src, "  %s l=0;\n\n", lenTypeNameG);

        FOR_EACH_LIST_ELMT_RVS (e, seq->basicType->a.sequence) {
            cxxtri =  e->type->cxxTypeRefInfo;
            varName = cxxtri->fieldName;

            /* print optional test if nec */
            if (e->type->defaultVal != NULL) {
                Value *defVal = GetValue(e->type->defaultVal->value);

                /* HANDLE DEFAULT VALUE ENCODING FOR DER */
                switch(ParanoidGetBuiltinType(e->type)) {
                case BASICTYPE_INTEGER:
                case BASICTYPE_ENUMERATED:
                    fprintf(src,"  if ( %s(%s) && *%s != %d )\n  {\n",
                            cxxtri->optTestRoutineName,
                            varName, varName, defVal->basicValue->a.integer);
                    break;
                case BASICTYPE_BITSTRING:
                    {
                        if (defVal->basicValue->choiceId ==
                            BASICVALUE_VALUENOTATION) {
                            char *defBitStr;
                            normalizeValue(&defBitStr,
                                           defVal->basicValue->a.valueNotation->octs);
                            fprintf(src, "    if (%s(%s) && (!%s->soloBitCheck(%s::%s))) {\n",
                                    cxxtri->optTestRoutineName, varName, varName,
                                    cxxtri->className, defBitStr);

                            free(defBitStr);
                        } else {
                            fprintf(errFileG,
                                    "WARNING: unsupported use of default BIT STRING\n");
                        }
                    }
                    break;
                case BASICTYPE_BOOLEAN:
                    fprintf(src, "    if (%s(%s) && *%s != %s ) {\n",
                            cxxtri->optTestRoutineName, varName, varName,
                            defVal->basicValue->a.boolean ? "true" : "false");
                    break;
                default:
                    fprintf(src, "    if (%s) {\n", varName);
                    break;
               }
            } else if (e->type->optional) {
                fprintf(src, "    if (%s(%s)) {\n", cxxtri->optTestRoutineName,
                        varName);
            }

            /* eSNACC does not encode indefinite length
             *
             * PrintCxxEocEncoders (src, td, e->type, "_b");
             */

            /* encode content */
            tmpTypeId = GetBuiltinType (e->type);
            if (tmpTypeId == BASICTYPE_ANYDEFINEDBY)
            {
                defByNamedType = e->type->basicType->a.anyDefinedBy->link;
                PrintCxxSetTypeByCode(defByNamedType, cxxtri, src);

                fprintf (src, "    l = %s", varName);
                if (cxxtri->isPtr)
                    fprintf (src, "->");
                else
                    fprintf (src, ".");
                fprintf (src, "B%s (_b);\n", r->encodeBaseName);
            }
            else if (tmpTypeId == BASICTYPE_ANY)
            {
                fprintf (src, "    l = %s", varName);
                if (cxxtri->isPtr)
                    fprintf (src, "->");
                else
                    fprintf (src, ".");

                fprintf (src, "B%s (_b);\n", r->encodeBaseName);
            }
            else if ( (tmpTypeId == BASICTYPE_OCTETCONTAINING) ||
                      (tmpTypeId == BASICTYPE_BITCONTAINING))
            {
               PrintCxxEncodeContaining(e->type, r, src);
            }
            else if ( tmpTypeId == BASICTYPE_EXTENSION )
            {
                fprintf (src, "    l = %s", varName);
                if (cxxtri->isPtr)
                    fprintf (src, "->");
                else
                    fprintf (src, ".");

                fprintf (src, "BEnc (_b);\n");
            }
            else
            {
                fprintf (src, "    l = %s", varName);
                if (cxxtri->isPtr)
                    fprintf (src, "->");
                else
                    fprintf (src, ".");

                fprintf (src, "B%s (_b);\n", r->encodeContentBaseName);
            }


            /* encode tag (s) & len (s) */
            PrintCxxTagAndLenEncodingCode (src, td, e->type, "l", "_b");
            fprintf (src, "    totalLen += l;\n");

            /* close optional test if nec */
            if (e->type->optional || (e->type->defaultVal != NULL))
                fprintf (src, "  }\n\n");
            else
                fprintf (src, "\n");
        }
        fprintf (src, "  return totalLen;\n");
        fprintf (src, "} // %s::B%s\n\n\n", td->cxxTypeDefInfo->className, r->encodeContentBaseName);
    }
    /* end of BerEncodeContent method printing code */

    /* write BerDecodeContent to src */
    if (printDecodersG)
    {
        fprintf (hdr, "  void			B%s (const %s &_b, %s tag, %s elmtLen, %s &bytesDecoded);\n\n", r->decodeContentBaseName, bufTypeNameG, tagTypeNameG, lenTypeNameG, lenTypeNameG);//, envTypeNameG);
        fprintf (src, "void %s::B%s (const %s &_b, %s /*tag0*/, %s elmtLen0, %s &bytesDecoded)\n", td->cxxTypeDefInfo->className, r->decodeContentBaseName, bufTypeNameG, tagTypeNameG, lenTypeNameG, lenTypeNameG);//, envTypeNameG);
        fprintf (src, "{\n");
        fprintf (src, "  FUNC(\" %s::B%s\");\n", td->cxxTypeDefInfo->className, r->decodeContentBaseName);
        fprintf (src, "  Clear();\n");

        /* PL: removed to avoid used variable warning
        fprintf (src, "    AsnBufLoc readLoc = _b.GetReadLoc();\n");
        */

        /* print local vars */
        fprintf (src, "  %s tag1 = %s();\n", tagTypeNameG, tagTypeNameG);
        fprintf (src, "  %s seqBytesDecoded = 0;\n", lenTypeNameG);
        /* count max number of extra length var nec */
        varCount = 0;

        FOR_EACH_LIST_ELMT (e, seq->basicType->a.sequence)
        {
            tmpVarCount = CxxCountVariableLevels (e->type);

            if (tmpVarCount > varCount)
                varCount = tmpVarCount;
        }
        
        /* write extra length vars */
        for (i = 1; i <= varCount; i++)
            fprintf (src, "  %s elmtLen%d = 0;\n", lenTypeNameG, i);

        /* handle empty seq */
        if ((seq->basicType->a.sequence == NULL) || LIST_EMPTY (seq->basicType->a.sequence))
        {
            fprintf (src, "  if (elmtLen0 == INDEFINITE_LEN)\n");
            fprintf (src, "    BDecEoc (_b, bytesDecoded);\n");
            fprintf (src, "  else if (elmtLen0 != 0)\n");
            fprintf (src, "  {\n");
            fprintf (src, "          throw EXCEPT(\"Expected an empty sequence\", DECODE_ERROR);\n");
            fprintf (src, "  }\n");
        }
        else
        {
            /*  check if all elmts are optional */
            AsnListFirst (seq->basicType->a.sequence);
            inTailOptElmts = IsTailOptional (seq->basicType->a.sequence);
            e = (NamedType*)FIRST_LIST_ELMT (seq->basicType->a.sequence);
            tmpTypeId = GetBuiltinType (e->type);

            if (!inTailOptElmts)
            {
                if (((tmpTypeId == BASICTYPE_ANY) || (tmpTypeId == BASICTYPE_ANYDEFINEDBY)) && (CountTags (e->type) == 0))
                {
                    if ((e->type->optional) && (e != (NamedType*)LAST_LIST_ELMT (seq->basicType->a.sequence)))
                        fprintf (src, "<untagged optional ANY - you must fix this>\n");
                }
                else
                    fprintf (src, "  tag1 = BDecTag (_b, seqBytesDecoded);\n\n");
            }
            else
            {
                fprintf (src, "  if (elmtLen0 == 0)\n");
                fprintf (src, "    return;\n");
                fprintf (src, "  else\n");
                fprintf (src, "  {\n");
                if (((tmpTypeId == BASICTYPE_ANY) || (tmpTypeId == BASICTYPE_ANYDEFINEDBY)) && (CountTags (e->type) == 0))
                {
                    if ((e->type->optional) && (e != (NamedType*)LAST_LIST_ELMT (seq->basicType->a.sequence)))
                        fprintf (src, "<untagged optional ANY - you must fix this>\n");
                }
                else
                    fprintf (src, "    tag1 = BDecTag (_b, seqBytesDecoded);\n\n");
                fprintf (src, "    if ((elmtLen0 == INDEFINITE_LEN) && (tag1 == EOC_TAG_ID))\n");
                fprintf (src, "    {\n");
                fprintf (src, "      BDEC_2ND_EOC_OCTET (_b, seqBytesDecoded);\n");
                fprintf (src, "      bytesDecoded += seqBytesDecoded;\n");
                fprintf (src, "      return;\n");
                fprintf (src, "    }\n");
                fprintf (src, "  }\n\n");
            }

        /***********************************************************************/
        /***********************************************************************/
        /***********************************************************************/
            FOR_EACH_LIST_ELMT (e, seq->basicType->a.sequence)
            {

               if( e->type->basicType->choiceId != BASICTYPE_EXTENSION )
               {
                cxxtri =  e->type->cxxTypeRefInfo;
                elmtLevel = 0;

                tags = GetTags (e->type, &stoleChoiceTags);

                if (LIST_EMPTY (tags))
                    fprintf (src, "  // ANY type\n");
                else
                {
                    tag = (Tag*)FIRST_LIST_ELMT (tags);
                    classStr = Class2ClassStr (tag->tclass);
                    codeStr = DetermineCode(tag, NULL, 0);//RWC;Code2UnivCodeStr (tag->code);
                    formStr = Form2FormStr (tag->form);

                    fprintf (src, "  if (");
                    if (tag->tclass == UNIV)
                    {
                        if (tag->form == ANY_FORM)
                        {
                            fprintf (src, "(tag1 == MAKE_TAG_ID (%s, %s, %s))\n", classStr, Form2FormStr (PRIM), codeStr);
                            fprintf (src, "    || (tag1 == MAKE_TAG_ID (%s, %s, %s))", classStr, Form2FormStr (CONS), codeStr);
                        }
                        else
                            fprintf (src, "(tag1 == MAKE_TAG_ID (%s, %s, %s))", classStr, formStr, codeStr);
                    }
                    else
                    {
                        if (tag->form == ANY_FORM)
                        {
                            fprintf (src, "(tag1 == MAKE_TAG_ID (%s, %s, %s))\n", classStr, Form2FormStr (PRIM), DetermineCode(tag, NULL, 1));//RWC;tag->code);
                            fprintf (src, "    || (tag1 == MAKE_TAG_ID (%s, %s, %s))", classStr, Form2FormStr (CONS), DetermineCode(tag, NULL, 1));//RWC;tag->code);
                        }
                        else
                            fprintf (src, "(tag1 == MAKE_TAG_ID (%s, %s, %s))", classStr, formStr, DetermineCode(tag, NULL, 1));//RWC;tag->code);
                    }

                    /* now decode extra tags/length pairs */
                    AsnListFirst (tags);
                    AsnListNext (tags);
                    if (stoleChoiceTags)
                    {
                        FOR_REST_LIST_ELMT (tag, tags)
                        {
                            fprintf (src, "\n    || ");
                            classStr = Class2ClassStr (tag->tclass);
                            formStr = Form2FormStr (tag->form);

                            if (tag->tclass == UNIV)
                                codeStr = DetermineCode(tag, NULL, 0);//RWC;Code2UnivCodeStr (tag->code);
                            else
                                codeStr = DetermineCode(tag, NULL, 1);
                            if (tag->form == ANY_FORM)
                            {
                                fprintf (src, "(tag1 == MAKE_TAG_ID (%s, %s, %s))\n", classStr, Form2FormStr (PRIM), codeStr);
                                fprintf (src, "    || (tag1 == MAKE_TAG_ID (%s, %s, %s))", classStr, Form2FormStr (CONS), codeStr);
                            }
                            else
                            {
                                fprintf (src, "(tag1 == MAKE_TAG_ID (%s, %s, %s))", classStr, formStr, codeStr);
                            }
                        }
                        fprintf (src, ")\n");
                        fprintf (src, "  {\n");
                        fprintf (src, "    elmtLen%d = BDecLen (_b, seqBytesDecoded);\n", ++elmtLevel);
                    }
                    else /* didn't steal nested choice's tags */
                    {
                        fprintf (src, ")\n");
                        fprintf (src, "  {\n");
                        fprintf (src, "    elmtLen%d = BDecLen (_b, seqBytesDecoded);\n", ++elmtLevel);

                        FOR_REST_LIST_ELMT (tag, tags)
                        {
                            classStr = Class2ClassStr (tag->tclass);
                            formStr = Form2FormStr (tag->form);

                            fprintf (src, "    tag1 = BDecTag (_b, seqBytesDecoded);\n\n");
                            if (tag->tclass == UNIV)
                                codeStr = DetermineCode(tag, NULL, 0);//RWC;Code2UnivCodeStr (tag->code);
                            else
                                codeStr = DetermineCode(tag, NULL, 1);
                            if (tag->form == ANY_FORM)
                            {
                                fprintf (src, "    if ((tag1 != MAKE_TAG_ID (%s, %s, %s))\n", classStr, Form2FormStr (PRIM), codeStr);
                                fprintf (src, "       && (tag1 != MAKE_TAG_ID (%s, %s, %s)))\n", classStr, Form2FormStr (CONS), codeStr);
                            }
                            else
                                fprintf (src, "    if (tag1 != MAKE_TAG_ID (%s, %s, %s))\n", classStr, formStr, codeStr);

                            fprintf (src, "    {\n");
                            fprintf (src, "          throw InvalidTagException(typeName(), tag1, STACK_ENTRY);\n");
                            fprintf (src, "    }\n\n");
                            fprintf (src, "    elmtLen%d = BDecLen (_b, seqBytesDecoded);\n", ++elmtLevel);
                        }
                    }
                }

                /*
                 * if this seq element is CHOICE &&
                 * we didn't steal its tags then we must grab
                 * the key tag out of the contained CHOICE
                 */
                if (!stoleChoiceTags && (GetBuiltinType (e->type) == BASICTYPE_CHOICE))
                {
                    fprintf (src, "    tag1 = BDecTag (_b, seqBytesDecoded);\n");
                    fprintf (src, "    elmtLen%d = BDecLen (_b, seqBytesDecoded);\n", ++elmtLevel);
                }

                varName = cxxtri->fieldName;

                /* decode content */
                if (cxxtri->isPtr)
                {
                   if (e->type->defaultVal)
                   {
                      fprintf(src, "  // delete default value\n");
                      fprintf(src, "  delete %s;\n", varName);
                   }
                   fprintf (src, "  %s = new %s;\n", varName, cxxtri->className);
				  }

                /* decode content */
                tmpTypeId = GetBuiltinType (e->type);
                if (tmpTypeId == BASICTYPE_ANYDEFINEDBY)
                {
                    /*
                     * must check for another EOC for ANYs
                     * since the any decode routines decode
                     * their own first tag/len pair
                     */
                    elmtLevel++;

                    defByNamedType = e->type->basicType->a.anyDefinedBy->link;
                    PrintCxxSetTypeByCode(defByNamedType, cxxtri, src);

                    fprintf (src, "  %s", varName);
                    if (cxxtri->isPtr)
                        fprintf (src, "->");
                    else
                        fprintf (src, ".");
                    fprintf (src, "B%s (_b, seqBytesDecoded);\n",  r->decodeBaseName);
                }
                else if (tmpTypeId == BASICTYPE_ANY)
                {
                   elmtLevel++;
                    fprintf (src, "  %s", varName);
                    if (cxxtri->isPtr)
                        fprintf (src, "->");
                    else
                        fprintf (src, ".");
                    fprintf (src, "B%s (_b, seqBytesDecoded);\n",  r->decodeBaseName);
                }
                else if ( (tmpTypeId == BASICTYPE_OCTETCONTAINING) ||
					   		  (tmpTypeId == BASICTYPE_BITCONTAINING) )
                {
                   PrintCxxDecodeContaining(e->type, r, src);
                }
                else
                {
                    fprintf (src, "    %s", varName);
                    if (cxxtri->isPtr)
                        fprintf (src, "->");
                    else
                        fprintf (src, ".");
             
                    fprintf (src, "B%s (_b, tag1, elmtLen%d, seqBytesDecoded);\n",  r->decodeContentBaseName, elmtLevel);
                }

                /* decode Eoc (s) */
                for (i = elmtLevel-1; i > 0; i--)
                {
                    fprintf (src, "    if (elmtLen%d == INDEFINITE_LEN)\n", i);
                    fprintf (src, "      BDecEoc (_b, seqBytesDecoded);\n\n");
                }

                /*
                 * print code for getting the next tag
                 */
                inTailOptElmts = RestAreTailOptional (seq->basicType->a.sequence);
                if  (e != (NamedType*)LAST_LIST_ELMT (seq->basicType->a.sequence))
                {
                    tmpElmt = (NamedType*)NEXT_LIST_ELMT (seq->basicType->a.sequence);
                    tmpTypeId = GetBuiltinType (tmpElmt->type);
                    if (!inTailOptElmts)
                    {
                        if ((tmpTypeId == BASICTYPE_ANY || tmpTypeId == BASICTYPE_ANYDEFINEDBY) && CountTags (tmpElmt->type) == 0)
                        {
                            /* don't get a tag since ANY's decode their own */
                            if (e->type->optional || (tmpElmt->type->optional && tmpElmt != (NamedType*)LAST_LIST_ELMT (seq->basicType->a.sequence)))
                                /* let this cause a compile error in the generated code */
                                fprintf (src, "  <problems with untagged ANY that is optional or follows an optional sequence element - you must fix this>\n");
                        }
                        else
                        {
                            if(!tmpElmt->type->extensionAddition)
                            {
                                fprintf (src, "    tag1 = BDecTag (_b, seqBytesDecoded);\n");
                            }
                        }
                    }
                    else
                    {
                        fprintf (src, "    if (seqBytesDecoded == elmtLen0)\n");
                        fprintf (src, "    {\n");
                        fprintf (src, "      bytesDecoded += seqBytesDecoded;\n");
                        fprintf (src, "      return;\n");
                        fprintf (src, "    }\n");
                        fprintf (src, "    else\n");
                        fprintf (src, "    {\n");

                        if ((tmpTypeId == BASICTYPE_ANY || tmpTypeId == BASICTYPE_ANYDEFINEDBY) && !CountTags (tmpElmt->type))
                        {
                            /* don't get a tag since ANY's decode their own */
                            if (e->type->optional || (tmpElmt->type->optional && tmpElmt != (NamedType *)LAST_LIST_ELMT (seq->basicType->a.sequence)))
                            {
                                /*
                                 * let this cause a compile error in the generated code
                                 */
                                fprintf (src, "  <problems with untagged ANY that is optional or follows an optional sequence element - you must fix this>\n");
                            }
                            fprintf (src, "      tag1 = _b.PeekByte();\n\n");
                            fprintf (src, "      if ((elmtLen0 == INDEFINITE_LEN) && (tag1 == EOC_TAG_ID))\n");
                            fprintf (src, "      {\n");
                            fprintf (src, "        BDecEoc (_b, seqBytesDecoded);\n\n");
                            fprintf (src, "        bytesDecoded += seqBytesDecoded;\n");
                            fprintf (src, "        return;\n");
                            fprintf (src, "      }\n");
                        }
                        else if (tmpTypeId == BASICTYPE_EXTENSION)
                        {
                            fprintf (src, "      tag1 = _b.PeekByte();\n\n");
                            fprintf (src, "      if ((elmtLen0 == INDEFINITE_LEN) && (tag1 == EOC_TAG_ID))\n");
                            fprintf (src, "      {\n");
                            fprintf (src, "        BDecEoc (_b, seqBytesDecoded);\n\n");
                            fprintf (src, "        bytesDecoded += seqBytesDecoded;\n");
                            fprintf (src, "        return;\n");
                            fprintf (src, "      }\n");
                        }
                        else
                        {
                            fprintf (src, "      tag1 = BDecTag (_b, seqBytesDecoded);\n\n");
                            fprintf (src, "      if ((elmtLen0 == INDEFINITE_LEN) && (tag1 == EOC_TAG_ID))\n");
                            fprintf (src, "      {\n");
                            fprintf (src, "        BDEC_2ND_EOC_OCTET (_b, seqBytesDecoded);\n");
                            fprintf (src, "        bytesDecoded += seqBytesDecoded;\n");
                            fprintf (src, "        return;\n");
                            fprintf (src, "      }\n");
                        }
                        fprintf (src, "    }\n");
                    }
                }

                /*
                 * close tag check if (if there is one) and
                 * print else clause to handle missing non-optional elmt
                 * errors
                 */
                tmpTypeId = GetBuiltinType (e->type);
                if ((tmpTypeId == BASICTYPE_ANYDEFINEDBY || tmpTypeId == BASICTYPE_ANY) && !CountTags (e->type))
                {
                    /* do nothing - no tag check if stmt to close */
                    fprintf (src, "\n\n");
                }
                else if (!e->type->optional && !e->type->defaultVal)
                {
                    fprintf (src, "  }\n"); /* end of tag check if */
                    fprintf (src, "  else\n");
                    fprintf (src, "  {\n");
                    
                    if(e->type->extensionAddition)
                    {
                        fprintf (src, "      throw EXCEPT(\"SEQUENCE is missing non-optional expected extension addition elmt\", DECODE_ERROR);\n");
                    }
                    else
                    {
                        fprintf (src,
                                 "      throw EXCEPT(\"SEQUENCE is missing non-optional root elmt (%s)\", DECODE_ERROR);\n",
                                 e->fieldName);
                    }
                    fprintf (src, "  }\n\n");
                }
                else
                    fprintf (src, "  }\n\n"); /* end of tag check if */

                FreeTags (tags);
               }
               else
               {
                    extensionAdditionFound = TRUE;
               }
            }

        /***********************************************************************/
        /***********************************************************************/
        /***********************************************************************/

            /* for last elmt only */
            fprintf (src, "  bytesDecoded += seqBytesDecoded;\n");
            fprintf (src, "  if (elmtLen0 == INDEFINITE_LEN)\n");
            fprintf (src, "  {\n");
            if(extensionAdditionFound)
            {
                fprintf (src, "    // keep grabbing any's into the extension list\n");
                fprintf (src, "    // until EOC is found\n");
                fprintf (src, "    bool b_not_EOC = true;\n");
                fprintf (src, "    while(b_not_EOC)\n");
                fprintf (src, "    {\n");
                fprintf (src, "      tag1 = _b.PeekByte();\n\n");
                fprintf (src, "      if ((elmtLen0 == INDEFINITE_LEN) && (tag1 == EOC_TAG_ID))\n");
                fprintf (src, "      {\n");
                fprintf (src, "        BDecEoc (_b, seqBytesDecoded);\n\n");
                fprintf (src, "        bytesDecoded += seqBytesDecoded;\n");
                fprintf (src, "        b_not_EOC = false;\n");
                fprintf (src, "      }\n");
                fprintf (src, "      else\n");
                fprintf (src, "      {\n");
                fprintf (src, "        AsnAny extAny;\n");
                fprintf (src, "        extAny.BDec(_b, seqBytesDecoded);\n");
                fprintf (src, "        extension.extList.insert(extension.extList.end(), extAny);\n");      
                fprintf (src, "      }\n");
                fprintf (src, "    }\n");
            }
            else
            {
                fprintf (src, "    BDecEoc (_b, bytesDecoded);\n");
                fprintf (src, "    return;\n");
            }
            fprintf (src, "  }\n");
            fprintf (src, "  else if (seqBytesDecoded != elmtLen0)\n");
            fprintf (src, "  {\n");
            
            if(extensionAdditionFound)
            {
              fprintf (src, "    while(seqBytesDecoded < elmtLen0)\n");
              fprintf (src, "    {\n");
              fprintf (src, "       AsnAny extAny;\n");
              fprintf (src, "       extAny.BDec(_b, seqBytesDecoded);\n");
              fprintf (src, "       extension.extList.insert(extension.extList.end(), extAny);\n");
              fprintf (src, "    }\n");
              fprintf (src, "    return;\n");
            }
            else
            {
                fprintf (src, "    throw EXCEPT(\"Length discrepancy on sequence\", DECODE_ERROR);\n");
            }
            fprintf (src, "  }\n");
            fprintf (src, "  else\n");
            fprintf (src, "    return;\n");

        } /* end of non-empty set else clause */

        fprintf (src, "} // %s::B%s\n\n", td->cxxTypeDefInfo->className, r->decodeContentBaseName);
    }
    /* end of code for printing ber decode content method */

    /* do BEnc function */
    if (printEncodersG)
    {
        fprintf (hdr, "  %s		B%s (%s &_b) const;\n", lenTypeNameG, r->encodeBaseName, bufTypeNameG);
        fprintf (src, "%s %s::B%s (%s &_b) const\n", lenTypeNameG, td->cxxTypeDefInfo->className, r->encodeBaseName,  bufTypeNameG);
        fprintf (src, "{\n");
        fprintf (src, "  %s l=0;\n", lenTypeNameG);
        fprintf (src, "  l = B%s (_b);\n", r->encodeContentBaseName);

        /* encode each tag/len pair if any */
        FOR_EACH_LIST_ELMT_RVS (tag, seq->tags)
        {
            classStr = Class2ClassStr (tag->tclass);
            formStr = Form2FormStr (CONS);  /* seq's are constructed */

            fprintf (src, "  l += BEncConsLen (_b, l);\n");

            if (tag->tclass == UNIV)
            {
                const char* ptr = DetermineCode(tag, &tagLen, 0);
                fprintf (src, "  l += BEncTag%d (_b, %s, %s, %s);\n", tagLen, classStr, formStr, ptr);//RWC;Code2UnivCodeStr (tag->code));
            }
            else
            {
                const char* ptr = DetermineCode(tag, &tagLen, 1);
                fprintf (src, "  l += BEncTag%d (_b, %s, %s, %s);\n", tagLen, classStr, formStr, ptr);//RWC;tag->code);
            }
        }

        fprintf (src, "  return l;\n");
        fprintf (src, "}\n\n");
    }
    /* end of BEnc function */

    /* Do BDec function */
    if (printDecodersG)
    {
        fprintf (hdr, "  void			B%s (const %s &_b, %s &bytesDecoded);\n", r->decodeBaseName, bufTypeNameG, lenTypeNameG);//, envTypeNameG);
        fprintf (src, "void %s::B%s (const %s &_b, %s &bytesDecoded)\n", td->cxxTypeDefInfo->className, r->decodeBaseName,  bufTypeNameG, lenTypeNameG);//, envTypeNameG);
        fprintf (src, "{\n");
        fprintf (src, "  FUNC(\" %s::B%s\");\n", td->cxxTypeDefInfo->className, r->decodeBaseName); 
        fprintf (src, "  %s tag;\n", tagTypeNameG);

        /* print extra locals for redundant lengths */
        for (i = 1; (seq->tags != NULL) && (i <= LIST_COUNT (seq->tags)); i++)
            fprintf (src, "  %s elmtLen%d;\n", lenTypeNameG, i);
        fprintf (src, "\n");

        /*  decode tag/length pair (s) */
        elmtLevel = 0;
        FOR_EACH_LIST_ELMT (tag, seq->tags)
        {
            classStr = Class2ClassStr (tag->tclass);
            formStr = Form2FormStr (CONS);  /* seqs are constructed */

            fprintf (src, "  if ((tag = BDecTag (_b, bytesDecoded)) != ");

            if (tag->tclass == UNIV)
                fprintf (src, "MAKE_TAG_ID (%s, %s, %s))\n", classStr, formStr, DetermineCode(tag, NULL, 0));//RWC;Code2UnivCodeStr (tag->code));
            else
                fprintf (src, "MAKE_TAG_ID (%s, %s, %s))\n", classStr, formStr, DetermineCode(tag, NULL, 1));//RWC;tag->code);
            fprintf (src, "  {\n");
            fprintf (src, "    throw InvalidTagException(typeName(), tag, STACK_ENTRY);\n");
            fprintf (src, "  }\n");

            fprintf (src, "  elmtLen%d = BDecLen (_b, bytesDecoded);\n", ++elmtLevel);
        }

        fprintf (src, "  B%s (_b, tag, elmtLen%d, bytesDecoded);\n", r->decodeContentBaseName, elmtLevel);
		
        /* grab any EOCs that match redundant, indef lengths */
        for (i = elmtLevel-1; i > 0; i--)
        {
            fprintf (src, "  if (elmtLen%d == INDEFINITE_LEN)\n", i);
            fprintf (src, "    BDecEoc (_b, bytesDecoded);\n");
        }

        fprintf (src, "}\n\n");
    }
    /* end of BDec function */

  if(genPERCode)
  {
    /* PER Encode, PerEnc */
    if (printEncodersG)     //RWC;TBD; SHOULD set printEncodersP with FLAG!!!!
    {
        /*** RWC;PRESENT. ***/
        /* FIRST, create array containing all SEQUENCE elements. */
        pSeqElementNamedType = (NamedType **)calloc(
                                seq->basicType->a.sequence->count, 
                                sizeof(NamedType *));
        ii = 0;
        FOR_EACH_LIST_ELMT (e, seq->basicType->a.sequence)
        {
            pSeqElementNamedType[ii++] = e;
        }       /* END FOR each SEQUENCE element. */
        /*RWC;ONLY FOR SET.PrintCxxDefCode_PERSort(&pSetElementNamedType, &pSetElementTag, 
             set->basicType->a.set);*/

        /* THIRD, perform actual encoding using SEQUENCE element pointers.
         * FOR_EACH_LIST_ELMT (e, set->basicType->a.set) */
        PrintCxxDefCode_SetSeqPEREncode (src, hdr, r, td, pSeqElementNamedType, 
                    seq->basicType->a.sequence->count);
    }       // END IF printEncodersG
    /* end of PerEncode */

    /* PER Decode, PerDecode */
    if (printDecodersG)
    {
        PrintCxxDefCode_SetSeqPERDecode (src, hdr, r, td, pSeqElementNamedType, 
                    seq->basicType->a.sequence->count);
    }
  
  }
    
    /* end of PerDecode */
    if (pSeqElementNamedType)
        free(pSeqElementNamedType);

    /* write code for printing */
    if (printPrintersG)
	{
		PrintCxxSeqSetPrintFunction(src, hdr, td->cxxTypeDefInfo->className,
                                    seq->basicType);

        fprintf(hdr, "  void		PrintXML (std::ostream &os, const char *lpszTitle=NULL) const;\n");
        fprintf(src, "void %s::PrintXML (std::ostream &os, const char *lpszTitle) const\n",
                 td->cxxTypeDefInfo->className);
        fprintf(src, "{\n");
        fprintf(src, "    const char *tagName = typeName();\n");
        fprintf(src, "    if (lpszTitle)\n");
        fprintf(src, "        tagName = lpszTitle;\n");
        fprintf(src, "    os << \"<\" << tagName << \">\";\n");

        FOR_EACH_LIST_ELMT (e, seq->basicType->a.sequence) {
            inTailOptElmts = IsTailOptional(seq->basicType->a.sequence);
            if (e->type->cxxTypeRefInfo->isPtr) {
                fprintf(src, "    if (%s (%s)) {\n",
                        cxxtri->optTestRoutineName,
                        e->type->cxxTypeRefInfo->fieldName);
                fprintf(src, "        %s->",
                        e->type->cxxTypeRefInfo->fieldName);
            } else {
                fprintf(src, "    %s.", e->type->cxxTypeRefInfo->fieldName);
            }
            fprintf(src, "PrintXML(os");
            if (e->fieldName != NULL)
                fprintf(src, ", \"%s\"", e->fieldName);
            fprintf(src, ");\n");
            if (e->type->cxxTypeRefInfo->isPtr)
                fprintf(src, "    }\n");
            fprintf(src, "\n");
        }
        fprintf(src, "    os << \"</\" << tagName << \">\";\n");
        fprintf(src, "} // %s::PrintXML\n\n\n", td->cxxTypeDefInfo->className);
    }
    /* end of print method code printer */

    /* close class definition */
    fprintf (hdr, "};\n\n\n");
} /* PrintCxxSeqDefCode */


static void
PrintCxxSetDefCode (FILE *src, FILE *hdr, ModuleList *mods, Module *m,
                    CxxRules *r, TypeDef *td, Type *parent ESNACC_UNUSED,
                    Type *set, int novolatilefuncs ESNACC_UNUSED)
{
    NamedType *e;
    char *classStr;
    char *formStr;
    char *codeStr;
    int tagLen;
    int i=0;
    Tag *tag;
    TagList *tags;
    char *varName;
    CxxTRI *cxxtri=NULL;
    int elmtLevel=0;
    int varCount, tmpVarCount;
    int stoleChoiceTags;
    int mandatoryElmtCount;
    enum BasicTypeChoiceId tmpTypeId;
    NamedType *defByNamedType;

    // DEFINE PER encode/decode tmp vars.
    int *pSetElementTag=NULL;
    NamedType **pSetElementNamedType=NULL;
    int extensionAdditions = FALSE;

    /* put class spec in hdr file */
    char *ptr="";   /* NOT DLL Exported, or ignored on Unix. */
    if (bVDAGlobalDLLExport != NULL)
       ptr = bVDAGlobalDLLExport;

    fprintf (hdr, "class %s %s%s\n", ptr, td->cxxTypeDefInfo->className, baseClassesG);
    fprintf (hdr, "{\n");
    fprintf (hdr, "public:\n");

    /* write out the set elmts */
    FOR_EACH_LIST_ELMT (e, set->basicType->a.set)
    {		
        fprintf (hdr, "    ");
    
		/* JKG 7/31/03 */
		/*The following code enclosed in this if/else statement */
		/*is constructed for constraint handling capability     */
		/*for primitives found in sequences or sets             */
	    
            if(e->type->subtypes!=NULL)
		    {
			    switch(e->type->subtypes->choiceId)
			    {
			        case SUBTYPE_AND:
			        case SUBTYPE_OR:
                    case SUBTYPE_SINGLE:
			        {
				        if(!PrintCxxMultiConstraintOrHandler(hdr, src, td->definedName, e, 1))
				        {
					        PrintCxxType (hdr, mods, m, r, td, set, e->type);
					        fprintf (hdr, "%s;\n\n", e->type->cxxTypeRefInfo->fieldName);
				        }
    			        break;
			        }
			        default:
			        {
				        PrintCxxType (hdr, mods, m, r, td, set, e->type);
				        fprintf (hdr, "%s;\n\n", e->type->cxxTypeRefInfo->fieldName);
				        break;
			        }
		        }
            }
		    else
	        {
	    	    PrintCxxType (hdr, mods, m, r, td, set, e->type);
		        fprintf (hdr, "%s;\n\n", e->type->cxxTypeRefInfo->fieldName);
    	    }
        
    }
    fprintf (hdr, "\n");

    #if META
        if (printMetaG)
        {
            PrintCxxSetDefCodeMeta_1(hdr, src, td, set, m, e);
        }
    #endif /* META */

    /* Default constructor
	*/
    fprintf (hdr, "     %s() { Init(); }\n", td->cxxTypeDefInfo->className);

	/* Init() member function
	 */
	fprintf (hdr, "     void Init(void);\n");
    fprintf (src, "void %s::Init(void)\n", td->cxxTypeDefInfo->className);
    fprintf (src, "{\n");
    FOR_EACH_LIST_ELMT (e, set->basicType->a.set)
    {
      if (e->type->cxxTypeRefInfo->isPtr)
      {
        #if TCL
                   fprintf (src, "#if TCL\n");
                   fprintf (src, "  %s = new %s;\n", e->type->cxxTypeRefInfo->fieldName, e->type->cxxTypeRefInfo->className);
                   fprintf (src, "#else // TCL\n");
                   fprintf (src, "  %s = NULL;\n", e->type->cxxTypeRefInfo->fieldName);
        #else

           if (e->type->defaultVal != NULL)
           {
               Value *defVal = GetValue(e->type->defaultVal->value);

               /* HANDLE DEFAULT VALUE DECODING FOR DER by initializing the respective members to their default
                * values.
                */
                switch(ParanoidGetBuiltinType(e->type))
                {
                case BASICTYPE_INTEGER:
                case BASICTYPE_ENUMERATED:
                     fprintf(src,"  %s = new %s(%d);\n", 
                        e->type->cxxTypeRefInfo->fieldName, 
                        e->type->cxxTypeRefInfo->className,
                        defVal->basicValue->a.integer);
                     break;
                case BASICTYPE_BOOLEAN:
                     fprintf(src,"  %s = new %s(%s);\n", 
                        e->type->cxxTypeRefInfo->fieldName, 
                        e->type->cxxTypeRefInfo->className,
                        defVal->basicValue->a.boolean == 0 ? "false": "true");
                     break;
                case BASICTYPE_BITSTRING:
                {
                   NamedNumberList *pNNL = GetNamedElmts(e->type);
                   BasicValue *pBV;
                   pBV = GetLastNamedNumberValue(pNNL);

                   if (defVal->basicValue->choiceId == BASICVALUE_VALUENOTATION &&
                       pBV != NULL)
                   {
                      char *defBitStr;
                      normalizeValue(&defBitStr, defVal->basicValue->a.valueNotation->octs);
                      
                      fprintf(src,"  %s = new %s(%d);\n", 
                        e->type->cxxTypeRefInfo->fieldName, 
                        e->type->cxxTypeRefInfo->className,
                       pBV->a.integer);
                    
                      fprintf(src, "  %s->SetBit(%s::%s);\n",
                        e->type->cxxTypeRefInfo->fieldName,
                        e->type->cxxTypeRefInfo->className,
                        defBitStr);
                      free(defBitStr);
                   }
                   else
                      printf("\nWARNING: unsupported use of default BIT STRING\n");
               }
               break;
                default:
                {
                      fprintf (src, "  %s = new %s;\n", e->type->cxxTypeRefInfo->fieldName, e->type->cxxTypeRefInfo->className);
                }
             } /* end switch */
         }
         else
         {
              if(e->type->optional)
             {
                fprintf (src, "  %s = NULL;\n", e->type->cxxTypeRefInfo->fieldName);
             }
             else
             {
                fprintf (src, "  %s = new %s;\n", e->type->cxxTypeRefInfo->fieldName, e->type->cxxTypeRefInfo->className);
             }
         }

        #endif
      }
    }
    fprintf (src, "}\n\n");

    /* PIERCE added 8-22-2001
     
       Simple META support */
	 PrintCopyConstructor(hdr, src, td->cxxTypeDefInfo->className);
    PrintSimpleMeta(hdr, td->cxxTypeDefInfo->className,0);

    /* virtual destructor */
    fprintf (hdr, "  virtual ~%s() {Clear();}\n", td->cxxTypeDefInfo->className);
    
    /* Clear() member*/
    fprintf (hdr, "  void Clear();\n");
    fprintf (src, "void %s::Clear()\n", td->cxxTypeDefInfo->className);
    fprintf (src, "{\n");
    
    FOR_EACH_LIST_ELMT (e, set->basicType->a.set)
    {
        tmpTypeId = GetBuiltinType (e->type);

        if(e->type->cxxTypeRefInfo->isPtr){
            /**
             * C++ > 1998 allows deletion of null pointer. 
             * This means i hate old c++
             */
	      fprintf (src, "  delete %s;\n", e->type->cxxTypeRefInfo->fieldName);
  	      fprintf (src, "  %s = NULL;\n", e->type->cxxTypeRefInfo->fieldName);
        }
        else if(!e->type->cxxTypeRefInfo->isPtr && 
            ((tmpTypeId == BASICTYPE_CHOICE) ||
                (tmpTypeId == BASICTYPE_SET) ||
                (tmpTypeId == BASICTYPE_SEQUENCE)) )
        {
            fprintf (src, "  %s.Clear();\n", e->type->cxxTypeRefInfo->fieldName);
        }
    }
    fprintf (src, "}\n\n");

	fprintf(hdr, "\n   virtual int checkConstraints(ConstraintFailList* pConstraintFails) const;\n\n");
	fprintf(src, "\nint %s::checkConstraints(ConstraintFailList* pConstraintFails) const\n{\n",
		td->cxxTypeDefInfo->className);
	FOR_EACH_LIST_ELMT (e, set->basicType->a.set)
	{
		if (e->type->cxxTypeRefInfo->isPtr)
		{
			fprintf(src, "\tif (%s != NULL)\n", e->type->cxxTypeRefInfo->fieldName);
			fprintf(src, "\t\t%s->checkConstraints(pConstraintFails);\n\n",
				e->type->cxxTypeRefInfo->fieldName);
		}
		else
		{
			fprintf(src, "\t\t%s.checkConstraints(pConstraintFails);\n\n",
				e->type->cxxTypeRefInfo->fieldName);
		}
	}

	fprintf(src, "\treturn 0;\n");
	fprintf(src, "}\n\n\n");
 
    /* print clone routine for ANY mgmt */
    PrintCloneMethod (hdr, src, td);

    fprintf (hdr, "  %s		&operator = (const %s &that);\n", td->cxxTypeDefInfo->className, td->cxxTypeDefInfo->className);
    fprintf (src, "%s &%s::operator = (const %s &that)\n", td->cxxTypeDefInfo->className, td->cxxTypeDefInfo->className, td->cxxTypeDefInfo->className);
    fprintf (src, "{\n");
    fprintf (src, "  if (this != &that)\n");
    fprintf (src, "  {\n");
    fprintf (src, "    Clear();\n");

    FOR_EACH_LIST_ELMT (e, set->basicType->a.set)
    {
	   if (e->type->cxxTypeRefInfo->isPtr)
      {
	      fprintf (src, "    if (that.%s)\n", e->type->cxxTypeRefInfo->fieldName);
	      fprintf (src, "    {\n");
	      fprintf (src, "      if (!%s)\n", e->type->cxxTypeRefInfo->fieldName);
          fprintf (src, "        %s = new %s;\n", e->type->cxxTypeRefInfo->fieldName, e->type->cxxTypeRefInfo->className);
 	      fprintf (src, "      *%s = *that.%s;\n", e->type->cxxTypeRefInfo->fieldName, e->type->cxxTypeRefInfo->fieldName);
	      fprintf (src, "    }\n");
	      fprintf (src, "    else\n");
	      fprintf (src, "    {\n");
	      fprintf (src, "      delete %s;\n", e->type->cxxTypeRefInfo->fieldName);
	      fprintf (src, "      %s = NULL;\n", e->type->cxxTypeRefInfo->fieldName);
	      fprintf (src, "    }\n");
	   }
	   else
      {
	    fprintf (src, "    %s = that.%s;\n", e->type->cxxTypeRefInfo->fieldName, e->type->cxxTypeRefInfo->fieldName);
      }
    }
    fprintf (src, "  }\n");
    fprintf (src, "\n");
    fprintf (src, "  return *this;\n");
    fprintf (src, "}\n\n");

    /* BerEncode content*/
    if (printEncodersG)
    {
        fprintf (hdr, "  %s		B%s (%s &_b) const;\n", lenTypeNameG, r->encodeContentBaseName, bufTypeNameG);
        fprintf (src, "%s %s::B%s (%s &_b) const\n", lenTypeNameG, td->cxxTypeDefInfo->className, r->encodeContentBaseName, bufTypeNameG);
        fprintf (src, "{\n");

        /* print local vars */
        fprintf (src, "  %s totalLen = 0;\n", lenTypeNameG);
        fprintf (src, "  %s l=0;\n\n", lenTypeNameG);
      
        /* PIERCE changed 11-05-2002
         *   use an std::list of AsnBuf instead of an array of CSM_Buffers to 
         *   sort the elements of the set.
         */
        fprintf (src, "    std::list<AsnBuf> bufList;\n");
        fprintf (src, "    std::list<AsnBuf>::iterator iBuf;\n");
        
        FOR_EACH_LIST_ELMT_RVS (e, set->basicType->a.set)
        {
            cxxtri =  e->type->cxxTypeRefInfo;
            varName = cxxtri->fieldName;

            /* print optional test if nec*/
            if (e->type->optional || (e->type->defaultVal != NULL))
            {
                fprintf (src, "  if (%s (%s))\n", cxxtri->optTestRoutineName, varName);
                fprintf (src, "  {\n");
            }

            /* encode content */
            tmpTypeId = GetBuiltinType (e->type);
            if (tmpTypeId == BASICTYPE_ANYDEFINEDBY)
            {
                fprintf(src, "    iBuf = bufList.insert(bufList.begin(), AsnBuf());\n");
                defByNamedType = e->type->basicType->a.anyDefinedBy->link;
                PrintCxxSetTypeByCode(defByNamedType, cxxtri, src);

                fprintf (src, "    l = %s", varName);
                if (cxxtri->isPtr)
                    fprintf (src, "->");
                else
                    fprintf (src, ".");
                fprintf (src, "B%s (*iBuf);\n", r->encodeBaseName);

            }
            else if (tmpTypeId == BASICTYPE_ANY)
            {
                fprintf(src, "    iBuf = bufList.insert(bufList.begin(), AsnBuf());\n");
                fprintf (src, "    l = %s", varName);
                if (cxxtri->isPtr)
                    fprintf (src, "->");
                else
                    fprintf (src, ".");

                fprintf (src, "B%s (*iBuf);\n", r->encodeBaseName);
            }
            else if ( (tmpTypeId == BASICTYPE_OCTETCONTAINING) ||
                      (tmpTypeId == BASICTYPE_BITCONTAINING))
            {
               fprintf(src, "    iBuf = bufList.insert(bufList.begin(), AsnBuf());\n");
               PrintCxxEncodeContaining(e->type, r, src);
            }
            else if ( tmpTypeId == BASICTYPE_EXTENSION )
            {
                fprintf (src, "       l = %s", varName);
                if (cxxtri->isPtr)
                    fprintf (src, "->");
                else
                    fprintf (src, ".");

                fprintf (src, "B%s (_b);\n", r->encodeBaseName);
            }
            else
            {
                fprintf(src, "    iBuf = bufList.insert(bufList.begin(), AsnBuf());\n");
                fprintf (src, "    l = %s", varName);
                if (cxxtri->isPtr)
                    fprintf (src, "->");
                else
                    fprintf (src, ".");

                fprintf (src, "B%s (*iBuf);\n", r->encodeContentBaseName);
            }

            /* encode tag (s) & len (s) */

            PrintCxxTagAndLenEncodingCode (src, td, e->type, "l", "(*iBuf)");
            fprintf (src, "    totalLen += l;\n");


            /** NOW, encode for SET DER rule ordering.*/
            /** RWC; Buffers handle files or memory. **/
            
             /* close optional test if nec */
            if (e->type->optional || (e->type->defaultVal != NULL))
                fprintf (src, "  }\n\n");
            else
                fprintf (src, "\n");
        }
    #if 0
            /** LAST, Order for SET DER rule ordering.*/
            /**   re-order all elements, add to "_b".*/
            fprintf (src, "      vdasnacc_sortSet(tmpEnc, iii);\n");
            
            /** These "SET" components are now ordered in ascending order, 
             ** ready to be loaded into the output buffer. (RWC; TBD; make output 
             ** buffers accept these allocated buffers directly, no copy). 
             **/ 

            fprintf (src, "      tmpCount = iii;  /** REMEMBER how many we have**/\n");
            fprintf (src, "      for (iii=0; iii < tmpCount; iii++)\n");
            fprintf (src, "         SM_WriteToAsnBuf(tmpEnc[iii], _b);\n");

            fprintf (src, "      for (iii=0; iii < tmpCount; iii++) delete tmpEnc[iii];\n");
            fprintf (src, "      free(outputBuf.DataPtr());\n");
    #endif      
            fprintf (src, "    sortSet(bufList);\n");
            fprintf (src, "    iBuf = bufList.begin();\n");
            fprintf (src, "    while(iBuf != bufList.end())\n");
            fprintf (src, "    {\n");
            fprintf (src, "       iBuf->ResetMode();\n");
            fprintf (src, "       _b.splice(*iBuf);\n");
            fprintf (src, "       iBuf++;\n");
            fprintf (src, "    }\n");

        /** internal definition bracket for "tmpCount".**/
        fprintf (src, "  return totalLen;\n");
        fprintf (src, "} // %s::B%s\n\n\n", td->cxxTypeDefInfo->className, r->encodeContentBaseName);
    }
    /* end of BerEncodeContent */

    /* write BerDecodeContent */
    if (printDecodersG)
    {
        fprintf (hdr, "  void			B%s (const %s &_b, %s tag, %s elmtLen, %s &bytesDecoded);\n\n", r->decodeContentBaseName, bufTypeNameG, tagTypeNameG, lenTypeNameG, lenTypeNameG);//, envTypeNameG);

        fprintf (src, "void %s::B%s (const %s &_b, %s /*tag0*/, %s elmtLen0, %s &bytesDecoded)\n", td->cxxTypeDefInfo->className, r->decodeContentBaseName, bufTypeNameG, tagTypeNameG, lenTypeNameG, lenTypeNameG);//, envTypeNameG);
        fprintf (src, "{\n");
        fprintf (src, "   FUNC(\"%s::B%s\");\n",td->cxxTypeDefInfo->className, r->decodeContentBaseName); 
        fprintf (src, "   Clear();\n");

        fprintf (src, "    AsnBufLoc readLoc;\n   readLoc = _b.GetReadLoc();\n");

        /* print local vars */
        fprintf (src, "  %s tag1 = %s();\n", tagTypeNameG, tagTypeNameG);
        fprintf (src, "  %s setBytesDecoded = 0;\n", lenTypeNameG);
        fprintf (src, "  unsigned int mandatoryElmtsDecoded = 0;\n");
        /* count max number of extra length var nec */
        varCount = 0;
        FOR_EACH_LIST_ELMT (e, set->basicType->a.set)
        {
            tmpVarCount = CxxCountVariableLevels (e->type);
            if (tmpVarCount > varCount)
                varCount = tmpVarCount;
        }
        /* write extra length vars */
        for (i = 1; i <= varCount; i++)
            fprintf (src, "  %s elmtLen%d;\n", lenTypeNameG, i);
        fprintf (src, "\n");

        /* handle empty set */
        if ((set->basicType->a.set == NULL) || LIST_EMPTY (set->basicType->a.set))
        {
            fprintf (src, "  if (elmtLen0 == INDEFINITE_LEN)\n");
            fprintf (src, "    BDecEoc (_b, bytesDecoded);\n");
            fprintf (src, "  else if (elmtLen0 != 0)\n");
            fprintf (src, "  {\n");
            
            fprintf (src, "    throw EXCEPT(\"Expected an empty sequence\", DECODE_ERROR);\n");
            fprintf (src, "  }\n");
        }
        else
        {
            fprintf (src, "  for (; (setBytesDecoded < elmtLen0) || (elmtLen0 == INDEFINITE_LEN); )\n");
            fprintf (src, "  {\n");
            fprintf (src, "    readLoc = _b.GetReadLoc();\n");
            fprintf (src, "    tag1 = BDecTag (_b, setBytesDecoded);\n\n");
            
            fprintf (src, "    if ((elmtLen0 == INDEFINITE_LEN) && (tag1 == EOC_TAG_ID))\n");
            fprintf (src, "    {\n");
            fprintf (src, "      BDEC_2ND_EOC_OCTET (_b, setBytesDecoded);\n");
            fprintf (src, "      break; /* exit for loop */\n");
            fprintf (src, "    }\n");

            fprintf (src, "    elmtLen1 = BDecLen (_b, setBytesDecoded);\n");
            fprintf (src, "    switch (tag1)\n");
            fprintf (src, "    {\n");
            mandatoryElmtCount = 0;
            FOR_EACH_LIST_ELMT (e, set->basicType->a.set)
            {
               if( e->type->basicType->choiceId != BASICTYPE_EXTENSION )
               {
                cxxtri =  e->type->cxxTypeRefInfo;
                tags = GetTags (e->type, &stoleChoiceTags);
               
                if (LIST_EMPTY (tags))
                {
                    fprintf (src, "      // ANY Type ?\n");
                    fprintf (src, "      case MAKE_TAG_ID (?, ?, ?):\n");
                }
                else
                {
                    tag = (Tag*)FIRST_LIST_ELMT (tags);
                    classStr = Class2ClassStr (tag->tclass);
                    formStr = Form2FormStr (tag->form);

                    if (tag->tclass == UNIV)
                        codeStr = DetermineCode(tag, NULL, 0);//RWC;Code2UnivCodeStr (tag->code);
                    else
                        codeStr = DetermineCode(tag, NULL, 1);
                    if (tag->form == ANY_FORM)
                    {
                        fprintf (src, "      case MAKE_TAG_ID (%s, %s, %s):\n", classStr, Form2FormStr (PRIM), codeStr);
                        fprintf (src, "      case MAKE_TAG_ID (%s, %s, %s):\n", classStr, Form2FormStr (CONS), codeStr);
                    }
                    else
                        fprintf (src, "      case MAKE_TAG_ID (%s, %s, %s):\n", classStr, formStr, codeStr);

                    /* now decode extra tags/length pairs */
                    AsnListFirst (tags);
                    AsnListNext (tags);
                    elmtLevel = 1;
                    if (stoleChoiceTags)
                    {
                        FOR_REST_LIST_ELMT (tag, tags)
                        {
                            classStr = Class2ClassStr (tag->tclass);
                            formStr = Form2FormStr (tag->form);

                            if (tag->tclass == UNIV)
                                codeStr = DetermineCode(tag, NULL, 0);//RWC;Code2UnivCodeStr (tag->code);
                            else
                                codeStr = DetermineCode(tag, NULL, 1);
                            if (tag->form == ANY_FORM)
                            {
                                fprintf (src, "      case MAKE_TAG_ID (%s, %s, %s):\n", classStr, Form2FormStr (PRIM), codeStr);
                                fprintf (src, "      case MAKE_TAG_ID (%s, %s, %s):\n", classStr, Form2FormStr (CONS), codeStr);
                            }
                            else
                                fprintf (src, "      case MAKE_TAG_ID (%s, %s, %s):\n", classStr, formStr, codeStr);
                        }
                    }
                    else /* didn't steal nested choice's tags */
                    {
                        FOR_REST_LIST_ELMT (tag, tags)
                        {
                            classStr = Class2ClassStr (tag->tclass);
                            codeStr = DetermineCode(tag, NULL, 0);//RWC;Code2UnivCodeStr (tag->code);
                            formStr = Form2FormStr (tag->form);

                            fprintf (src, "        tag1 = BDecTag (_b, setBytesDecoded);\n");
                            if (tag->form == ANY_FORM)
                            {
                                if (tag->tclass == UNIV)
                                {
                                    fprintf (src, "        if ((tag1 != MAKE_TAG_ID (%s, %s, %s))\n",  classStr, Form2FormStr (PRIM), codeStr);
                                    fprintf (src, "           && (tag1 != MAKE_TAG_ID (%s, %s, %s)))\n", classStr, Form2FormStr (CONS), codeStr);
                                }
                                else
                                {
                                    fprintf (src, "        if ((tag1 != MAKE_TAG_ID (%s, %s, %s))\n", classStr, Form2FormStr (PRIM), DetermineCode(tag, NULL, 1));//RWC;tag->code);
                                    fprintf (src, "           && (tag1 != MAKE_TAG_ID (%s, %s, %s)))\n", classStr, Form2FormStr (CONS), DetermineCode(tag, NULL, 1));//RWC;tag->code);
                                }
                            }
                            else
                            {
                                if (tag->tclass == UNIV)
                                    fprintf (src, "        if (tag1 != MAKE_TAG_ID (%s, %s, %s))\n", classStr, formStr, codeStr);
                                else
                                    fprintf (src, "        if (tag1 != MAKE_TAG_ID (%s, %s, %s))\n", classStr, formStr, DetermineCode(tag, NULL, 1));//RWC;tag->code);
                            }

                            fprintf (src, "        {\n");
                            fprintf (src, "           throw InvalidTagException(typeName(), tag1, STACK_ENTRY);\n");
                            fprintf (src, "        }\n\n");
                            fprintf (src, "        elmtLen%d = BDecLen (_b, setBytesDecoded);\n", ++elmtLevel);
                        }
                    }
                }
                /*
                 * if the choices element is another choice &&
                 * we didn't steal its tags then we must grab
                 * the key tag out of the contained CHOICE
                 */
                if (!stoleChoiceTags && (GetBuiltinType (e->type) == BASICTYPE_CHOICE))
                {
                    fprintf (src, "        tag1 = BDecTag (_b, setBytesDecoded);\n");
                    fprintf (src, "        elmtLen%d = BDecLen (_b, setBytesDecoded);\n", ++elmtLevel);
                }

                varName = cxxtri->fieldName;

                /* decode content */
                if (cxxtri->isPtr)
                {
                   if (e->type->defaultVal)
                   {
                      fprintf(src, "  // delete default value\n");
                      fprintf(src, "  delete %s;\n", varName);
                   }
                   fprintf (src, "\t\t%s = new %s;\n", varName, cxxtri->className);
                }

                /* decode content */
                tmpTypeId = GetBuiltinType (e->type);
                /*fprintf (src, "    tag1 = BDecTag (_b, setBytesDecoded);\n\n");*/
                /*fprintf (src, "    elmtLen1 = BDecLen (_b, setBytesDecoded);\n");*/
                if (tmpTypeId == BASICTYPE_ANYDEFINEDBY)
                {
                    /*
                     * must check for another EOC for ANYs
                     * since the any decode routines decode
                     * their own first tag/len pair
                     */
                    elmtLevel++;

                    fprintf (src, "        %s", varName);
                    if (cxxtri->isPtr)
                        fprintf (src, "->");
                    else
                        fprintf (src, ".");

                    defByNamedType = e->type->basicType->a.anyDefinedBy->link;
                    PrintCxxSetTypeByCode(defByNamedType, cxxtri, src);

                    fprintf (src, "        %s", varName);
                    if (cxxtri->isPtr)
                        fprintf (src, "->");
                    else
                        fprintf (src, ".");
                    fprintf (src, "B%s (_b, setBytesDecoded);\n",  r->decodeBaseName);
                }
                else if (tmpTypeId == BASICTYPE_ANY)
                {
                    elmtLevel++;

                    fprintf (src, "        %s", varName);
                    if (cxxtri->isPtr)
                        fprintf (src, "->");
                    else
                        fprintf (src, ".");
                    fprintf (src, "B%s (_b, setBytesDecoded);\n",  r->decodeBaseName);
                }
                else if ( (tmpTypeId == BASICTYPE_OCTETCONTAINING) ||
					   		  (tmpTypeId == BASICTYPE_BITCONTAINING) )
                {
                   PrintCxxDecodeContaining(e->type, r, src);
                }
                else
                {
                    fprintf (src, "        %s", varName);
                    if (cxxtri->isPtr)
                        fprintf (src, "->");
                    else
                        fprintf (src, ".");

                    fprintf (src, "B%s (_b, tag1, elmtLen%d, setBytesDecoded);\n",  r->decodeContentBaseName, elmtLevel);
                }

                /* decode Eoc (s) */
                for (i = elmtLevel-1; i >= 1; i--)
                {
                    fprintf (src, "          if (elmtLen%d == INDEFINITE_LEN)\n", i);
                    fprintf (src, "            BDecEoc (_b, setBytesDecoded);\n\n");
                }

                /* keep track of decoded non-optional elmts */
                if ( (!e->type->optional) && (e->type->defaultVal == NULL) && (!e->type->extensionAddition) )
                {
                    mandatoryElmtCount++;
                    fprintf (src, "          mandatoryElmtsDecoded++;\n");
                }
                fprintf (src, "          break;\n\n");

                FreeTags (tags);
               }
               else
               {
                    extensionAdditions = TRUE;
               }
            } /* for each elmt */
            fprintf (src, "       default:\n");
            if(extensionAdditions)
            {
                fprintf (src, "       {\n");
                fprintf (src, "         _b.SetReadLoc(readLoc);\n");
                fprintf (src, "         AsnAny extAny;\n");
                fprintf (src, "         extAny.BDec(_b, setBytesDecoded);\n");
                fprintf (src, "         extension.extList.insert(extension.extList.end(), extAny);\n");
                fprintf (src, "       }\n");
            }
            else
            {
                fprintf (src, "         throw InvalidTagException(typeName(), tag1, STACK_ENTRY);\n");
            }

            fprintf (src, "    } // end switch\n");
            fprintf (src, "  } // end for loop\n");
            fprintf (src, "  bytesDecoded += setBytesDecoded;\n");
            fprintf (src, "  if (mandatoryElmtsDecoded < %d)\n", mandatoryElmtCount);
            fprintf (src, "  {\n");
            
            fprintf (src, "     throw EXCEPT(\"non-optional SET element missing\", DECODE_ERROR);\n");
            fprintf (src, "  }\n");
        } /* if not empty set clause */

        fprintf (src, "} // %s::B%s\n\n", td->cxxTypeDefInfo->className, r->decodeContentBaseName);
    }
    /* end of decode content method code */
  if(genPERCode)
  {
    /* PerEncode */
    if (printEncodersG)     //RWC;TBD; SHOULD set printEncodersP with FLAG!!!!
    {
        PrintCxxDefCode_PERSort(&pSetElementNamedType, &pSetElementTag, 
             set->basicType->a.set);
        //
        // THIRD, perform actual encoding using re-arranged Set element 
        //    pointers.
        //FOR_EACH_LIST_ELMT (e, set->basicType->a.set)
        PrintCxxDefCode_SetSeqPEREncode (src, hdr, r, td, pSetElementNamedType, 
                    set->basicType->a.set->count);
    }       // END IF printEncodersG
    /* end of PerEncode */

    /* PerDecode */
    if (printDecodersG)
    {
        PrintCxxDefCode_SetSeqPERDecode (src, hdr, r, td, 
             pSetElementNamedType, set->basicType->a.set->count);
    }
  }
    
    /* end of PerDecode */
    if (pSetElementTag)
        free(pSetElementTag);
    if (pSetElementNamedType)
        free(pSetElementNamedType);

    /* BerEncode */
    if (printEncodersG)
    {
        fprintf (hdr, "  %s		B%s (%s &_b) const;\n", lenTypeNameG, r->encodeBaseName, bufTypeNameG);
        fprintf (src, "%s\n", lenTypeNameG);
        fprintf (src, "%s::B%s (%s &_b) const\n", td->cxxTypeDefInfo->className, r->encodeBaseName, bufTypeNameG);
        fprintf (src, "{\n");
        fprintf (src, "  %s l=0;\n", lenTypeNameG);
        fprintf (src, "  l = B%s (_b);\n", r->encodeContentBaseName);

        /* encode each tag/len pair if any */
        FOR_EACH_LIST_ELMT_RVS (tag, set->tags)
        {
            classStr = Class2ClassStr (tag->tclass);
            formStr = Form2FormStr (CONS);  /* set's are constructed */
            //RWC;tagLen = TagByteLen (tag->code);

            fprintf (src, "  l += BEncConsLen (_b, l);\n");

            if (tag->tclass == UNIV)
                fprintf (src, "  l += BEncTag%d (_b, %s, %s, %s);\n", tagLen, classStr, formStr, DetermineCode(tag, &tagLen, 0));//RWC;Code2UnivCodeStr (tag->code));
            else
                fprintf (src, "  l += BEncTag%d (_b, %s, %s, %s);\n", tagLen, classStr, formStr, DetermineCode(tag, &tagLen, 1));//RWC;tag->code);
        }
        fprintf (src, "  return l;\n");
        fprintf (src, "}\n\n");
    }
    /* end of BerEncode */

    /* BerDecode */
    if (printDecodersG)
    {
        fprintf (hdr, "  void			B%s (const %s &_b, %s &bytesDecoded);\n", r->decodeBaseName, bufTypeNameG, lenTypeNameG);//, envTypeNameG);
        fprintf (src, "void %s::B%s (const %s &_b, %s &bytesDecoded)\n", td->cxxTypeDefInfo->className, r->decodeBaseName, bufTypeNameG, lenTypeNameG);//, envTypeNameG);
        fprintf (src, "{\n");
        fprintf (src, "   FUNC(\"%s::B%s\");\n", td->cxxTypeDefInfo->className, r->decodeBaseName);
        fprintf (src, "   %s tag;\n", tagTypeNameG);

        /* print extra locals for redundant lengths */
        for (i = 1; (set->tags != NULL) && (i <= LIST_COUNT (set->tags)); i++)
            fprintf (src, "  %s elmtLen%d;\n", lenTypeNameG, i);
        fprintf (src, "\n");

        /*  decode tag/length pair (s) */
        elmtLevel = 0;
        FOR_EACH_LIST_ELMT (tag, set->tags)
        {
            classStr = Class2ClassStr (tag->tclass);
            formStr = Form2FormStr (CONS);  /* sets are constructed */

            fprintf (src, "  if ((tag = BDecTag (_b, bytesDecoded)) != ");

            if (tag->tclass == UNIV)
                fprintf (src, "MAKE_TAG_ID (%s, %s, %s))\n", classStr, formStr, DetermineCode(tag, NULL, 0));//RWC;Code2UnivCodeStr (tag->code));
            else
                fprintf (src, "MAKE_TAG_ID (%s, %s, %s))\n", classStr, formStr, DetermineCode(tag, NULL, 1));//RWC;tag->code);
            fprintf (src, "  {\n");
            fprintf (src, "        throw InvalidTagException(typeName(), tag, STACK_ENTRY);\n");
            fprintf (src, "  }\n");

            fprintf (src, "  elmtLen%d = BDecLen (_b, bytesDecoded);\n", ++elmtLevel);
        }

        fprintf (src, "  B%s (_b, tag, elmtLen%d, bytesDecoded);\n", r->decodeContentBaseName, i-1);

        /* grab any EOCs that match redundant, indef lengths */
        for (i = elmtLevel-1; i > 0; i--)
        {
            fprintf (src, "  if (elmtLen%d == INDEFINITE_LEN)\n", i);
            fprintf (src, "    BDecEoc (_b, bytesDecoded);\n");
        }

        fprintf (src, "}\n\n");
    }
    /* end of BerDecode */

    /* write code for printing */
    if (printPrintersG)
	{
		PrintCxxSeqSetPrintFunction(src, hdr, td->cxxTypeDefInfo->className,
			set->basicType);

        fprintf(hdr, "    void PrintXML (std::ostream &os, const char *lpszTitle=NULL) const;\n");
        fprintf(src,
                "void %s::PrintXML (std::ostream &os, const char *lpszTitle) const\n",
                td->cxxTypeDefInfo->className);
        fprintf(src, "{\n");
        fprintf(src, "    const char *tagName = typeName();\n");
        fprintf(src, "    if (lpszTitle) {\n");
        fprintf(src, "        tagName = lpszTitle;\n");
        fprintf(src, "    os << \"<\" << tagName << \">\";\n");
        FOR_EACH_LIST_ELMT (e, set->basicType->a.set) {
            const char *fieldString = "%s";
            if (e->type->cxxTypeRefInfo->isPtr) {
                fprintf(src, "    if (%s (%s)) {\n",
                        cxxtri->optTestRoutineName,
                        e->type->cxxTypeRefInfo->fieldName);
                fprintf(src, "        %s->",
                        e->type->cxxTypeRefInfo->fieldName);
            } else {
                fprintf(src, "    %s.",
                        e->type->cxxTypeRefInfo->fieldName);
            }
            fprintf(src, "PrintXML(os");
            if (e->fieldName != NULL)
                fprintf(src, ", \"%s\"", e->fieldName);
            fprintf (src, ");\n");
            if (e->type->cxxTypeRefInfo->isPtr)
                fprintf(src, "    }\n");
        }      /* END For each set element */
        fprintf(src, "    os << \"</\" << tagName << \">\";\n");
        fprintf(src, "} // %s::PrintXML\n\n\n", td->cxxTypeDefInfo->className);
    }
    /* end of print method code */

    /* close class definition */
    fprintf(hdr, "};\n\n\n");
} /* PrintCxxSetDefCode */


/*
 * PIERCE added 8-21-2001 template code to handle SET/SEQ OF
 *    
 */
static void
PrintCxxListClass (FILE *hdr, FILE * src, TypeDef *td, Type *lst, Module* m, ModuleList *mods)
{  
   int idecl;
   struct NamedType p_etemp;
   NamedType* p_e;
   char typeNameStr[256];
   char *lcn; /* list class name */
   char *ecn; /* (list) elmt class name */
   const char* exportStr;
   char *pszNamespace;
   char pcvarname[100];

   idecl=0;
   p_e=&p_etemp;
   p_e->type=lst->basicType->a.setOf;
   m = m;

   // Set DLL export string
   if (bVDAGlobalDLLExport == NULL)
	   exportStr = "";
   else
	   exportStr = bVDAGlobalDLLExport;

   ecn = lst->basicType->a.setOf->cxxTypeRefInfo->className;

    if(p_e->type->subtypes!=NULL)
    {
       sprintf(pcvarname, "PERGen_%s%ld", ecn, lconstraintvar);
       lconstraintvar+=1;
       p_e->type->cxxTypeRefInfo->fieldName=&pcvarname[0];

       switch (p_e->type->subtypes->choiceId)
	    {
	        case SUBTYPE_AND:
	        case SUBTYPE_OR:
            case SUBTYPE_SINGLE:
		        {
    		        if(PrintCxxMultiConstraintOrHandler(hdr, src, td->definedName, p_e, 0))
			        {
				        constraints_flag=1;
			        }
    		        break;
		        }
	        default:
	        {
                break;
	        }
	    }
	}

   
   ecn=lst->basicType->a.setOf->cxxTypeRefInfo->className;
   pszNamespace = LookupNamespace(lst, mods);
   lcn = td->cxxTypeDefInfo->className;
    
   fprintf (hdr, "class %s : public ", lcn);

   switch (lst->basicType->choiceId)
   {
   case BASICTYPE_SEQUENCEOF:
       sprintf (typeNameStr,"\"%s\"", lcn);
       if (pszNamespace)
           fprintf (hdr, "AsnSeqOf<%s::%s>\n", pszNamespace, ecn);
       else
           fprintf (hdr, "AsnSeqOf<%s>\n", ecn);
	    break;
    case BASICTYPE_SETOF:
       sprintf (typeNameStr,"\"%s\"", lcn);
       if (pszNamespace)
           fprintf (hdr, "AsnSetOf<%s::%s>\n", pszNamespace, ecn);
       else
           fprintf (hdr, "AsnSetOf<%s>\n", ecn);
	    break;

	 default:
		 /* TBD print error? */
		 break;
   }
   fprintf (hdr, "{\n");

   // Print typeName() and Clone() functions
	fprintf(hdr, "public:\n");
	fprintf(hdr, "\t%s virtual const char* typeName() const\t{ return \"%s\"; }\n",
		exportStr, lcn);
	fprintf(hdr, "\t%s virtual AsnType* Clone() const\t\t\t{ return new %s(*this); }\n",
		exportStr, lcn);

	// JKG--added functionality for sequence of and set of constraints
	if (td->type->subtypes != NULL)
	{
        /*Subtype* s_type;*/
        /*s_type = (Subtype*)td->type->subtypes->a.single->a.sizeConstraint->a.or->last->data;*/
		/* Only single size constraints that are themselves single are supported */
		if ((td->type->subtypes->choiceId == SUBTYPE_SINGLE) &&
			(td->type->subtypes->a.single->choiceId == SUBTYPEVALUE_SIZECONSTRAINT) &&
			(td->type->subtypes->a.single->a.sizeConstraint->choiceId == SUBTYPE_SINGLE))
		{
			PrintCxxSetOfSizeConstraint(hdr, 
				td->type->subtypes->a.single->a.sizeConstraint->a.single,
				m, td->type);
		}
		else
		{
			PrintErrLoc(m->asn1SrcFileName, (long)td->type->lineNo);
			fprintf(errFileG, "ERROR - unsupported constraint\n");
		}
	}

    fprintf(hdr, "};\n\n");
}

static void
PrintCxxSetOfDefCode PARAMS ((src, hdr, mods, m, r, td, parent, setOf, novolatilefuncs),
    FILE *src _AND_
    FILE *hdr _AND_
    ModuleList *mods _AND_
    Module *m _AND_
    CxxRules *r _AND_
    TypeDef *td _AND_
    Type *parent _AND_
    Type *setOf _AND_
    int novolatilefuncs)
{
	// Get rid of warnings
	novolatilefuncs = novolatilefuncs;
	parent = parent;
	m = m;
	r = r;
	mods = mods;
	src = src;

    /* do class */
	PrintCxxListClass (hdr, src, td, setOf, m, mods);

} /* PrintCxxSetOfDefCode */


static void
PrintCxxAnyDefCode PARAMS ((src, hdr, mods, m, r, td, parent, any),
    FILE *src _AND_
    FILE *hdr _AND_
    ModuleList *mods _AND_
    Module *m _AND_
    CxxRules *r _AND_
    TypeDef *td _AND_
    Type *parent _AND_
    Type *any)
{
    fprintf (hdr, "/* ");
    SpecialPrintType (hdr, td, td->type);
    fprintf (hdr, " */\n");
    fprintf (hdr, "typedef %s %s;\n\n", td->type->cxxTypeRefInfo->className, td->cxxTypeDefInfo->className);
    any=any;parent=parent;r=r;m=m;mods=mods;src=src;    /*AVOIDS warning.*/
} /* PrintCxxAnyDefCode */


static void
PrintCxxTypeDefCode PARAMS ((src, hdr, mods, m, r, td, novolatilefuncs),
    FILE *src _AND_
    FILE *hdr _AND_
    ModuleList *mods _AND_
    Module *m _AND_
    CxxRules *r _AND_
    TypeDef *td _AND_
    int novolatilefuncs)
{
    switch (td->type->basicType->choiceId)
    {
        case BASICTYPE_BOOLEAN:  /* library type */
        case BASICTYPE_REAL:  /* library type */
        case BASICTYPE_OCTETSTRING:  /* library type */
        case BASICTYPE_NULL:  /* library type */
        case BASICTYPE_EXTERNAL:		/* library type */
        case BASICTYPE_OID:  /* library type */
        case BASICTYPE_RELATIVE_OID:
        case BASICTYPE_INTEGER:  /* library type */
        case BASICTYPE_BITSTRING:  /* library type */
        case BASICTYPE_ENUMERATED:  /* library type */
        case BASICTYPE_NUMERIC_STR:  /* 22 */
        case BASICTYPE_PRINTABLE_STR: /* 23 */
        case BASICTYPE_UNIVERSAL_STR: /* 24 */
        case BASICTYPE_IA5_STR:      /* 25 */
        case BASICTYPE_BMP_STR:      /* 26 */
        case BASICTYPE_UTF8_STR:     /* 27 */
        case BASICTYPE_UTCTIME:      /* 28 tag 23 */
        case BASICTYPE_GENERALIZEDTIME: /* 29 tag 24 */
        case BASICTYPE_GRAPHIC_STR:     /* 30 tag 25 */
        case BASICTYPE_VISIBLE_STR:     /* 31 tag 26  aka ISO646String */
        case BASICTYPE_GENERAL_STR:     /* 32 tag 27 */
        case BASICTYPE_OBJECTDESCRIPTOR:	/* 33 tag 7 */
        case BASICTYPE_VIDEOTEX_STR:	/* 34 tag 21 */
        case BASICTYPE_T61_STR:			/* 35 tag 20 */
            PrintCxxSimpleDef (hdr, src, m, r, td);
            break;
        case BASICTYPE_SEQUENCEOF:  /* list types */
        case BASICTYPE_SETOF:
            PrintCxxSetOfDefCode (src, hdr, mods, m, r, td, NULL, td->type, novolatilefuncs);
            break;
        case BASICTYPE_IMPORTTYPEREF:  /* type references */
        case BASICTYPE_LOCALTYPEREF:
            /*
             * if this type has been re-tagged then
             * must create new class instead of using a typedef
             */
            PrintCxxSimpleDef (hdr, src, m, r, td);
            break;
        case BASICTYPE_ANYDEFINEDBY:  /* ANY types */
        case BASICTYPE_ANY:
            PrintCxxAnyDefCode (src, hdr, mods, m, r, td, NULL, td->type);
            break;
        case BASICTYPE_CHOICE:
            PrintCxxChoiceDefCode (src, hdr, mods, m, r, td, NULL, td->type, novolatilefuncs);
            break;
        case BASICTYPE_SET:
            PrintCxxSetDefCode (src, hdr, mods, m, r, td, NULL, td->type, novolatilefuncs);
            break;
        case BASICTYPE_SEQUENCE:
            PrintCxxSeqDefCode (src, hdr, mods, m, r, td, NULL, td->type, novolatilefuncs);
            break;
        case BASICTYPE_COMPONENTSOF:
        case BASICTYPE_SELECTION:
        case BASICTYPE_UNKNOWN:
        case BASICTYPE_MACRODEF:
        case BASICTYPE_MACROTYPE:
            /* do nothing */
            break;
        default:
				/* TBD: print error? */
				break;
    }
} /* PrintCxxTypeDefCode */

void
PrintCxxCode PARAMS ((src, hdr, if_META (printMeta COMMA meta COMMA meta_pdus COMMA)
					 mods, m, r, longJmpVal, printTypes, printValues,
					 printEncoders, printDecoders, printPrinters, printFree
					 if_TCL (COMMA printTcl), novolatilefuncs),
    FILE *src _AND_
    FILE *hdr _AND_
    if_META (MetaNameStyle printMeta _AND_)
    if_META (const Meta *meta _AND_)
    if_META (MetaPDU *meta_pdus _AND_)
    ModuleList *mods _AND_
    Module *m _AND_
    CxxRules *r _AND_
    long longJmpVal _AND_
    int printTypes _AND_
    int printValues _AND_
    int printEncoders _AND_
    int printDecoders _AND_
    int printPrinters _AND_
    int printFree
    if_TCL (_AND_ int printTcl) _AND_
    int novolatilefuncs)
{
	Module *currMod;
    AsnListNode *currModTmp;
    TypeDef *td;
    ValueDef *vd;

    longJmpValG = longJmpVal;
    printTypesG = printTypes;
    printEncodersG = printEncoders;
    printDecodersG = printDecoders;
    printPrintersG = printPrinters;
    printFreeG = printFree;

#if META
    printMetaG = printMeta;
    meta_pdus_G = meta_pdus;
#if TCL
    printTclG = printTcl;
#endif /* TCL */
#endif /* META */

    PrintSrcComment(src, m);
    PrintHdrComment(hdr, m);
    PrintConditionalIncludeOpen(hdr, m->cxxHdrFileName);

#if META
    if (printMetaG) {
	    fprintf(src, "\n");
	    fprintf(src, "#ifndef META\n");
	    fprintf(src, "#define META	1\n");
	    fprintf(src, "#endif\n");
#if TCL
	    if (printTclG) {
            fprintf(src, "#ifndef TCL\n");
            fprintf(src, "#define TCL	META\n");
            fprintf(src, "#endif\n");
        }
#endif /* TCL */
    }
#endif /* META */

    fprintf(hdr, "#include \"asn-incl.h\"\n");
    fprintf(hdr, "#include \"asn-listset.h\"\n");
    fprintf(src, "\n");

    PrintSrcIncludes(src, mods, m);

    FOR_EACH_LIST_ELMT (currMod, mods) {
		if (!strcmp(m->cxxHdrFileName, currMod->cxxHdrFileName)) {
            ImportModuleList *ModLists;
            ImportModule *impMod;
            char *ImpFile = NULL;
            ModLists = currMod->imports;
            currModTmp = mods->curr;
            FOR_EACH_LIST_ELMT(impMod, ModLists) {
                ImpFile = GetImportFileName(impMod->modId->name, mods);
                if (ImpFile != NULL)
                    fprintf(hdr, "#include \"%s\"\n", ImpFile);
                if (impMod->moduleRef == NULL)
                    impMod->moduleRef = GetImportModuleRef(impMod->modId->name, mods);
                if (impMod->moduleRef && impMod->moduleRef->namespaceToUse) {
                    fprintf(src,"using namespace %s;\n",
                            impMod->moduleRef->namespaceToUse);
                }
            }
            mods->curr = currModTmp;
        }
    }

    fprintf(hdr, "\n");
    fprintf(src, "\n");

    if (gNO_NAMESPACE == 0) {
        fprintf(hdr, "#ifndef NO_NAMESPACE\n");
        fprintf(src, "#ifndef NO_NAMESPACE\n");
        if (gAlternateNamespaceString) {
            fprintf(hdr, "using namespace SNACC;\n");
            fprintf(src, "using namespace SNACC;\n");
            fprintf(hdr, "namespace %s {\n", gAlternateNamespaceString);
            fprintf(src, "namespace %s {\n", gAlternateNamespaceString);
        } else if (m->namespaceToUse) {
            fprintf(hdr, "using namespace SNACC;\n");
            fprintf(src, "using namespace SNACC;\n");
            fprintf(hdr, "namespace %s {\n", m->namespaceToUse);
            fprintf(src, "namespace %s {\n", m->namespaceToUse);
        } else {
            fprintf(hdr, "namespace SNACC {\n");
            fprintf(src, "namespace SNACC {\n");
        }
        fprintf(hdr, "#endif\n");
        fprintf(src, "#endif\n");
    }

    if (bVDAGlobalDLLExport) {
        fprintf(hdr, "#ifndef %s\n", bVDAGlobalDLLExport);
        fprintf(hdr, "#if defined(WIN32)\n");
        fprintf(hdr, "#pragma warning( disable : 4251)\n");
        if (!strcmp(bVDAGlobalDLLExport, "SNACCDLL_API")) {
            /* Special case for compatibility */
            fprintf(hdr, "#ifdef SNACCDLL_EXPORTS\n");
        } else {
            fprintf(hdr, "#ifdef %s_EXPORTS\n", bVDAGlobalDLLExport);
        }

        fprintf(hdr, "#define %s __declspec(dllexport)\n", bVDAGlobalDLLExport);
        fprintf(hdr, "#else\n");
        fprintf(hdr, "#define %s __declspec(dllimport)\n", bVDAGlobalDLLExport);
        fprintf(hdr, "#endif      // %s\n", bVDAGlobalDLLExport);
        fprintf(hdr, "#else       // Handle Unix...\n");
        fprintf(hdr, "#define %s \n", bVDAGlobalDLLExport);
        fprintf(hdr, "#endif      // WIN32\n");
        fprintf(hdr, "#endif      // %s\n", bVDAGlobalDLLExport);
    }

    fprintf (hdr,
             "//------------------------------------------------------------------------------\n");
    fprintf (hdr, "// class declarations:\n\n");
    FOR_EACH_LIST_ELMT (td, m->typeDefs) {
        PrintTypeDecl (hdr, td);
    }

    fprintf (hdr, "\n");

#if META
    if (printMeta) {
	    fprintf (hdr, "#if META\n");
	    fprintf (src, "#if META\n\n");

        fprintf (hdr, "//------------------------------------------------------------------------------\n");

        char *ptr="";   /* NOT DLL Exported, or ignored on Unix. */

        if (bVDAGlobalDLLExport != NULL) {
            ptr = bVDAGlobalDLLExport;
        }

        fprintf(hdr, "extern const %s AsnModuleDesc %sModuleDesc;\n",
                ptr, m->cxxname);

        fprintf(src, "//------------------------------------------------------------------------------\n");
        fprintf(src, "static const AsnTypeDesc *%sModuleTypes[] = {\n", m->cxxname);
        FOR_EACH_LIST_ELMT(td, m->typeDefs) {
            fprintf(src, "    &%s::_desc,\n", td->cxxTypeDefInfo->className);
        }

        fprintf(src, "    NULL\n");
        fprintf(src, "};\n");

	    fprintf(src,
                "const AsnModuleDesc %sModuleDesc = { \"%s\", %sModuleTypes };\n\n",
                m->cxxname, m->modId->name, m->cxxname);

        fprintf(hdr, "#endif // META\n\n");
        fprintf(src, "#endif // META\n\n");
    }
#endif /* META */

    if (printValues) {
        fprintf(src, "//------------------------------------------------------------------------------\n");
        fprintf(src, "// value defs\n\n");
        FOR_EACH_LIST_ELMT (vd, m->valueDefs) {
            PrintCxxValueDef(src, r, vd);
        }
        fprintf(src, "\n");
    }

    if (printValues) {
        fprintf(hdr, "//------------------------------------------------------------------------------\n");
        fprintf(hdr, "// externs for value defs\n\n");
        FOR_EACH_LIST_ELMT (vd, m->valueDefs) {
            PrintCxxValueExtern (hdr, r, vd);
        }
        fprintf(hdr, "//------------------------------------------------------------------------------\n");
    }

    fprintf(hdr, "//------------------------------------------------------------------------------\n");
    fprintf(hdr, "// class definitions:\n\n");
    fprintf(src, "//------------------------------------------------------------------------------\n");
    fprintf(src, "// class member definitions:\n\n");

    PrintCxxAnyCode (src, hdr, r, mods, m);

    FOR_EACH_LIST_ELMT (td, m->typeDefs) {
        PrintCxxTypeDefCode (src, hdr, mods, m, r, td, novolatilefuncs);
    }

    if (gNO_NAMESPACE == 0) {
        fprintf(hdr, "#ifndef NO_NAMESPACE\n");
        fprintf(hdr, "} // namespace close\n");
        fprintf(hdr, "#endif\n");
        fprintf(src, "#ifndef NO_NAMESPACE\n");
        fprintf(src, "} // namespace close\n");
        fprintf(src, "#endif\n");
    }

    PrintConditionalIncludeClose (hdr, m->cxxHdrFileName);

} /* PrintCxxCode */


void PrintCxxEncodeContaining(Type *t, CxxRules *r, FILE *src)
{
   fprintf(src, "    l += %s", t->cxxTypeRefInfo->fieldName);
   if (t->cxxTypeRefInfo->isPtr)
      fprintf(src, "->");
   else
      fprintf(src, ".");

   fprintf(src, "B%s(_b);\n", r->encodeBaseName);

   /* If this is a BITSTRING CONTAINING encode a NULL octet for the unused 
    * bits
    */
   if (t->basicType->choiceId == BASICTYPE_BITCONTAINING)
   {
      fprintf(src,"    _b.PutByteRvs((char) 0 ); //encode 0 for unused bits\n");
      fprintf(src,"    l++;\n");
   }
}

void PrintCxxDecodeContaining(Type *t, CxxRules *r, FILE *src)
{
   NamedType *defByNamedType;

   /* Encode Content of contained type */
   if (t->basicType->a.stringContaining->basicType->choiceId == 
			 BASICTYPE_ANYDEFINEDBY)
   {
      defByNamedType = 
			  t->basicType->a.stringContaining->basicType->a.anyDefinedBy->link;
      PrintCxxSetTypeByCode(defByNamedType, t->cxxTypeRefInfo, src);
   }

   if (t->basicType->choiceId == BASICTYPE_BITCONTAINING)
   {
      fprintf(src,"\n");
      fprintf(src,"    // Decode unused bits and make sure it's 0\n");
      fprintf(src,"    char unusedBits;\n");
      fprintf(src,"    unusedBits = _b.GetByte();\n");
      fprintf(src,"    seqBytesDecoded++;\n");
      fprintf(src,"    if (unusedBits != '0x0')\n");
      fprintf(src,"      throw DecodeException(STACK_ENTRY);\n");
      fprintf(src,"\n");
   }

   fprintf (src, "    %s", t->cxxTypeRefInfo->fieldName);
   if (t->cxxTypeRefInfo->isPtr)
      fprintf (src, "->");
   else
      fprintf (src, ".");

   fprintf (src, "B%s (_b, seqBytesDecoded);\n",  r->decodeBaseName);
}


void PrintCxxPEREncodeContaining(Type *t, CxxRules *r, FILE *src)
{
   fprintf(src, "    l += %s", t->cxxTypeRefInfo->fieldName);
   if (t->cxxTypeRefInfo->isPtr)
      fprintf(src, "->");
   else
      fprintf(src, ".");

   fprintf(src, "P%s(_b);\n", r->encodeBaseName);

   /* If this is a BITSTRING CONTAINING encode a NULL octet for the unused 
    * bits
    */
   if (t->basicType->choiceId == BASICTYPE_BITCONTAINING)
   {
      fprintf(src,"    unsigned char _tmp[] = {0x00};\n");
      fprintf(src,"    _b.PutBits(tmp , 8); //encode 0 for unused bits\n");
      fprintf(src,"    l++;\n");
   }
}

void PrintCxxPERDecodeContaining(Type *t, CxxRules *r, FILE *src)
{
   NamedType *defByNamedType;

   /* Encode Content of contained type */
   if (t->basicType->a.stringContaining->basicType->choiceId == 
			 BASICTYPE_ANYDEFINEDBY)
   {
      defByNamedType = 
			  t->basicType->a.stringContaining->basicType->a.anyDefinedBy->link;
      PrintCxxSetTypeByCode(defByNamedType, t->cxxTypeRefInfo, src);
   }

   if (t->basicType->choiceId == BASICTYPE_BITCONTAINING)
   {
      fprintf(src,"\n");
      fprintf(src,"    // Decode unused bits and make sure it's 0\n");
      fprintf(src,"    unsigned char* unusedBits;\n");
      fprintf(src,"    unusedBits = _b.GetBits(8);\n");
      fprintf(src,"    bitsDecoded++;\n");
      fprintf(src,"    if (unusedBits[0] != '0x0')\n");
      fprintf(src,"      throw DecodeException(STACK_ENTRY);\n");
      fprintf(src,"\n");
   }

   fprintf (src, "    %s", t->cxxTypeRefInfo->fieldName);
   if (t->cxxTypeRefInfo->isPtr)
      fprintf (src, "->");
   else
      fprintf (src, ".");

   fprintf (src, "P%s (_b, bitsDecoded);\n",  r->decodeBaseName);
}


void PrintCxxSetTypeByCode(NamedType *defByNamedType, CxxTRI *cxxtri, FILE *src)
{
     char *varName = cxxtri->fieldName;

     if (GetBuiltinType (defByNamedType->type) == BASICTYPE_OID)
     {
         fprintf (src, "    %s", varName);
         if (cxxtri->isPtr)
            fprintf (src, "->");
         else
            fprintf (src, ".");

         fprintf (src, "SetTypeByOid (");
         if (defByNamedType->type->cxxTypeRefInfo->isPtr)
             fprintf (src, " *");
         fprintf (src, "%s);\n", defByNamedType->type->cxxTypeRefInfo->fieldName);
     }
     else if (GetBuiltinType (defByNamedType->type) == BASICTYPE_INTEGER)
     {
         fprintf (src, "    %s", varName);
         if (cxxtri->isPtr)
            fprintf (src, "->");
         else
            fprintf (src, ".");

         fprintf (src, "SetTypeByInt (");
         if (defByNamedType->type->cxxTypeRefInfo->isPtr)
             fprintf (src, " *");
         fprintf (src, "%s);\n", defByNamedType->type->cxxTypeRefInfo->fieldName);
     }
     else if (GetBuiltinType (defByNamedType->type) == BASICTYPE_CHOICE)
     {
        NamedType *nt;
        Type      *t = GetType(defByNamedType->type);

        if (defByNamedType->type->cxxTypeRefInfo->isPtr)
            fprintf(src, "  switch (%s->choiceId)\n", defByNamedType->type->cxxTypeRefInfo->fieldName);
        else
            fprintf(src, "  switch (%s.choiceId)\n", defByNamedType->type->cxxTypeRefInfo->fieldName);
        fprintf(src, "  {\n");

        FOR_EACH_LIST_ELMT(nt, t->basicType->a.choice)
        {
           fprintf(src, "   case %s::%sCid:\n", defByNamedType->type->cxxTypeRefInfo->className, nt->fieldName);
           if (nt->type->basicType->choiceId == BASICTYPE_INTEGER ||
               nt->type->basicType->choiceId == BASICTYPE_ENUMERATED)
           {
              fprintf (src, "      %s", varName);
              if (cxxtri->isPtr)
                 fprintf (src, "->");
              else
                 fprintf (src, ".");

              if (defByNamedType->type->cxxTypeRefInfo->isPtr)
                  fprintf(src, "SetTypeByInt(*%s->%s);\n", defByNamedType->type->cxxTypeRefInfo->fieldName, nt->fieldName);
              else
                  fprintf(src, "SetTypeByInt(*%s.%s);\n", defByNamedType->type->cxxTypeRefInfo->fieldName, nt->fieldName);
           }
           else
           {
              fprintf (src, "      %s", varName);
              if (cxxtri->isPtr)
                 fprintf (src, "->");
              else
                 fprintf (src, ".");

              if (defByNamedType->type->cxxTypeRefInfo->isPtr)
                  fprintf(src, "SetTypeByOid(*%s->%s);\n", defByNamedType->type->cxxTypeRefInfo->fieldName, nt->fieldName);
              else
                  fprintf(src, "SetTypeByOid(*%s.%s);\n", defByNamedType->type->cxxTypeRefInfo->fieldName, nt->fieldName);
           }
           fprintf(src, "      break;\n");

        }
        fprintf(src, "  }\n");
     }
}


char *
LookupNamespace PARAMS ((t, mods),
    Type *t _AND_
    ModuleList *mods)
{
    char *pszNamespace=NULL;
    Module *mTmp=NULL;
    TypeDef *ptTmp=NULL;
    BasicType *pbtTmp2=NULL;

    //RWC; HANDLE namespace designations of specific modules on declarations,
    //      if necessary.  (May have to lookup module name to get namespace).
    pbtTmp2 = t->basicType;
    if (pbtTmp2->choiceId == BASICTYPE_SEQUENCEOF || 
        pbtTmp2->choiceId == BASICTYPE_SETOF)
        pbtTmp2 = pbtTmp2->a.sequenceOf->basicType;  // Burrow 1 more layer down for SequenceOf/SetOf
    if (pbtTmp2->choiceId == BASICTYPE_IMPORTTYPEREF)
    {                     // RWC; IF IMPORTED, then we need to check for 
                          //       optional namespace designation (only in .h)
            FOR_EACH_LIST_ELMT (mTmp, mods)
            {
                ptTmp = LookupType(mTmp->typeDefs, 
                          pbtTmp2->a.importTypeRef->typeName); //WHAT we are looking for...
                if (ptTmp != NULL)
                    break;      //FOUND the MODULE that contains our defninition...
            }       // END FOR each module.
            if (ptTmp != NULL && mTmp != NULL && mTmp->namespaceToUse)  // FOUND our MODULE...
            {
                pszNamespace = mTmp->namespaceToUse;    // DO NOT DELETE...
            }    
            //LookupType PARAMS ((typeDefList, typeName),
    }           // IF BASICTYPE_IMPORTTYPEREF

    return(pszNamespace);
}       /* END LookupNamespace(...)*/


void PrintCxxSeqSetPrintFunction(FILE* src, FILE* hdr, MyString className,
								 BasicType *pBasicType)
{
    int allOpt;
	int inTailOptElmts;
	NamedTypeList* pElmtList;
	NamedType *e;

	fprintf(hdr, "\tvoid Print(std::ostream& os, unsigned short indent = 0) const;\n");

	fprintf(src, "void %s::Print(std::ostream& os, unsigned short indent) const\n",
		className);
	fprintf(src, "{\n");

	if (pBasicType->choiceId == BASICTYPE_SEQUENCE)
	{
		fprintf(src, "\tos << \"{ -- SEQUENCE --\" << std::endl;\n");
		pElmtList = pBasicType->a.sequence;
	}
	else if (pBasicType->choiceId == BASICTYPE_SET)
	{
		fprintf(src, "\tos << \"{ -- SET --\" << std::endl;\n");
		pElmtList = pBasicType->a.set;
	}
	else
		abort();

	allOpt = AllElmtsOptional(pElmtList);
	if (allOpt)
		fprintf(src, "\tint nonePrinted = true;\n");

	fprintf(src, "++indent;\n\n");

	FOR_EACH_LIST_ELMT (e, pElmtList)
	{
		inTailOptElmts = IsTailOptional(pElmtList);

		if (e->type->cxxTypeRefInfo->isPtr)
		{
			fprintf(src, "\tif (%s (%s))\n",
				e->type->cxxTypeRefInfo->optTestRoutineName,
				e->type->cxxTypeRefInfo->fieldName);
			fprintf (src, "\t{\n");

			if (allOpt)
			{
				if (e != FIRST_LIST_ELMT (pElmtList))
				{
					fprintf(src, "\t\tif (!nonePrinted)\n");
					fprintf(src, "\t\t\tos << \",\" << std::endl;\n");
				}
				fprintf(src, "\t\tnonePrinted = false;\n");
			}
			/* cannot be first elmt ow allOpt is true */
			else if (inTailOptElmts) 
				fprintf (src, "\t\tos << \",\"<< std::endl;\n");

			fprintf(src, "\t\tIndent(os, indent);\n");

			if (e->fieldName != NULL)
				fprintf(src, "\t\tos << \"%s \";\n", e->fieldName);

			fprintf(src, "\t\t%s->Print(os, indent);\n",
				e->type->cxxTypeRefInfo->fieldName);

			fprintf(src, "\t}\n");
		}
		else
		{
			fprintf(src, "\tIndent(os, indent);\n");

			if (e->fieldName != NULL)
				fprintf(src, "\tos << \"%s \";\n", e->fieldName);

			fprintf(src, "\t%s.Print(os, indent);\n",
				e->type->cxxTypeRefInfo->fieldName);

			if (e != LAST_LIST_ELMT (pElmtList))
				fprintf(src, "\tos << ',' << std::endl;\n");
		}

		fprintf (src, "\n");

		if (e == LAST_LIST_ELMT (pElmtList))
			fprintf(src, "\tos << std::endl;\n");
	}

	fprintf(src, "\t--indent;\n");
	fprintf(src, "\tIndent(os, indent);\n");
	fprintf(src, "\tos << \"}\\n\";\n");
	fprintf (src, "} // end of %s::Print()\n\n", className);
} /* end of PrintCxxSeqSetPrintFunction() */


/*
 * RWC;   */
static void
PrintCxxDefCode_SetSeqPEREncode (FILE *src, FILE *hdr, CxxRules *r, TypeDef *td, 
    NamedType **pSetElementNamedType,
    int iElementCount)      /* IN, ELEMENT Count to process in array*/
{
	NamedType *e;
	char *varName;
	CxxTRI *cxxtri=NULL;
	enum BasicTypeChoiceId tmpTypeId;
	NamedType *defByNamedType;
    int extensionsExist = FALSE;
	
	// DEFINE PER encode/decode tmp vars.
	int ii=0;
	long lOptional_Default_ElmtCount=0;
    const char* tabAndlenVar = "\tl";
		
	fprintf(hdr, "\t%s\t\tP%s(AsnBufBits &_b) const;\n", lenTypeNameG,
		r->encodeBaseName);
      /*RWC; {AsnLen len; len = 1;return len;};\n");
		RWC; MUST sort the results by tag; usually explicit except for 
		RWC;  untagged Choices which can be nested.  We must determine which 
		RWC;  tag can go 1st from any Choice, potentially nested. 
	RWC;  (FORWARD ENCODING FOR PER!)*/
	
	fprintf(src, "%s %s::P%s(AsnBufBits &_b) const\n", lenTypeNameG,
		td->cxxTypeDefInfo->className, r->encodeBaseName);
	fprintf(src, "{\n\t%s l = 0;\n", lenTypeNameG);

	/* SECOND, determine ahead of time the bit count for OPTIONAL/DEFAULT values. */
	for (ii=0; ii < iElementCount; ii++)
	{
		e = pSetElementNamedType[ii];
		
		//RWC; ALSO, count any OPTIONAL/DEFAULT ASN.1 elements.
		if ( (e->type->optional || e->type->defaultVal != NULL) && (!e->type->extensionAddition))
			lOptional_Default_ElmtCount++;
	}
	
	/* NEXT, decode this number of bits, if any, to determine the 
	presence/absence of OPTIONAL/DEFAULT elements.*/
	if (lOptional_Default_ElmtCount)
	{	/* NOW, load PER encoding flag to indicate what OPTIONAL/DEFAULT
		fields are actually present.*/
		/* FOR PER ENCODING OF Set, we must load a bitfield of length 
		"lOptional_Default_ElmtCount", indicating presence of optional 
		or default field data. */
		int iOptional_Default_ElementIndex=0;
		fprintf(src, "\n\t// Build and encode preamble");
		fprintf(src, "\n\tAsnBits SnaccOptionalDefaultBits(%ld);\n", lOptional_Default_ElmtCount);
		for (ii=0; ii < iElementCount; ii++)
		{
			e = pSetElementNamedType[ii];
			if ( (e->type->optional || e->type->defaultVal != NULL) && (!e->type->extensionAddition))
			{
				fprintf (src, "\tif (%s != NULL)\n",
					e->type->cxxTypeRefInfo->fieldName);
				fprintf (src, "\t\tSnaccOptionalDefaultBits.SetBit(%d);\n",
					iOptional_Default_ElementIndex++);
			}		/* END IF OPTIONAL/DEFAULT */
		}	/* END FOR each element. */
		fprintf (src, "\t_b.PutBits(SnaccOptionalDefaultBits.data(), %ld);\n",
			lOptional_Default_ElmtCount);
		fprintf (src, "\tl += %ld;\n", lOptional_Default_ElmtCount);
       
	}		/* END IF lOptional_Default_ElmtCount */
	
	/* NEXT, process each element of the Set/Sequence for decoding. */
	fprintf(src, "\n\t// Encode the elements of the SEQUENCE\n");
	for (ii=0; ii < iElementCount; ii++)
	{
		e = pSetElementNamedType[ii];
        if(!e->type->extensionAddition)
        {


		    if ( (e->type->optional || e->type->defaultVal != NULL) && (!e->type->extensionAddition))
		    {
			    fprintf(src, "\tif (%s != NULL)\t// Optional OR Default\n",
				    e->type->cxxTypeRefInfo->fieldName);
			    tabAndlenVar = "\t\tl";
		    }
		    
		    cxxtri =  e->type->cxxTypeRefInfo;
		    varName = cxxtri->fieldName;
		    
		    /* encode tag(s), not UNIV but APL, CNTX or PRIV tags ONLY for PER */
		    
		    /*RWC;TBD; UPDATE to reflect PER encoding rules for encoding length, no tags
		    RWC;TBD;  unless explicit (probably need to write a PER version, checking type).*/
		    
		    //RWC;NOT FOR PER;PrintCxxTagAndLenEncodingCode (src, td, e->type, "l", "(*iBuf)");
		    
		    /* encode content */
		    tmpTypeId = GetBuiltinType (e->type);
		    if (tmpTypeId == BASICTYPE_ANYDEFINEDBY)
		    {
			    //RWC;TBD; we may have to investigate individual types here to 
			    //RWC;TBD;	restrict which codes are printed for PER...
			    defByNamedType = e->type->basicType->a.anyDefinedBy->link;
			    PrintCxxSetTypeByCode(defByNamedType, cxxtri, src);
			    
			    fprintf(src, "%s += %s", tabAndlenVar, varName);
			    if (cxxtri->isPtr)
				    fprintf (src, "->");
			    else
				    fprintf (src, ".");
			    fprintf (src, "P%s(_b);\n", r->encodeBaseName);
		    }
		    else if (tmpTypeId == BASICTYPE_ANY)
		    {
			    //RWC;NOTE:  we will assume here that the ANY buffer is already
			    //RWC;NOTE:    properly PER encoder; we have no way of checking.
			    fprintf(src, "%s += %s", tabAndlenVar, varName);
			    if (cxxtri->isPtr)
				    fprintf (src, "->");
			    else
				    fprintf (src, ".");
			    
			    fprintf (src, "P%s(_b);\n", r->encodeBaseName);
		    }
		    else if ( (tmpTypeId == BASICTYPE_OCTETCONTAINING) ||
			    (tmpTypeId == BASICTYPE_BITCONTAINING))
		    {
			    PrintCxxPEREncodeContaining(e->type, r, src);
			    //RWC;TBD; THIS CALL WILL NOT UPDATE THE COUNT VALUE PROPERLY; must reflect
			    //RWC;TBD;	PER encoding forward, l+=, instead of l=
		    }
		    else
		    {
			    fprintf(src, "%s += %s", tabAndlenVar, varName);
			    if (cxxtri->isPtr)
				    fprintf (src, "->");
			    else
				    fprintf (src, ".");
			    
			    fprintf (src, "P%s(_b);\n", r->encodeBaseName);/*RWC;r->encodeContentBaseName);*/
		    }
        }
        else
        {
            extensionsExist = TRUE;
        }
	}			/* END FOR iElementCount */
	

    if(extensionsExist)
    {
        fprintf (src, " \t/*   WARNING:  PER does not yet support extensibility */\n");
    }

	fprintf(src, "\n\treturn l;\n");
	fprintf(src, "}\t// %s::P%s\n\n\n", td->cxxTypeDefInfo->className,
		r->encodeBaseName);
} /* END PrintCxxDefCode_SetSeqPEREncode(...) */


/*
 * RWC;  This method only handles the entire routine decode operations for both Set and Sequence PER Decode.
 * element decodes, not the wrapping logic. */
static void
PrintCxxDefCode_SetSeqPERDecode (FILE *src, FILE *hdr, CxxRules *r, TypeDef *td, 
    NamedType **pSetElementNamedType,
    int iElementCount)      /* IN, ELEMENT Count to process in arrays */
{
	NamedType *e;
	char *varName;
	CxxTRI *cxxtri=NULL;
	int elmtLevel=0;
	int varCount, tmpVarCount;
    enum BasicTypeChoiceId tmpTypeId;
	NamedType *defByNamedType;

    // DEFINE PER encode/decode tmp vars.
	int ii=0;
	int iOptional_Default_ElementIndex=0;
	long lOptional_Default_ElmtCount=0;
	
	if (pSetElementNamedType == NULL)
	{
		printf("****** PrintCxxDefCode_SetSeqPERDecode: MUST HAVE PER Encoders as well as PER Decoders! *****\n");
		return;
	}
	
	/***RWC; PERFORM PDec operations first... */
	fprintf(hdr, "\tvoid\t\tP%s(AsnBufBits& _b, %s& bitsDecoded);\n\n",
		r->decodeBaseName, lenTypeNameG);
	
	fprintf(src, "void %s::P%s(AsnBufBits& _b, %s& bitsDecoded)\n",
		td->cxxTypeDefInfo->className, r->decodeBaseName, lenTypeNameG);
	fprintf(src, "{\n");
	fprintf(src, "\tClear();\n");

	/* count max number of extra length var nec */
	varCount = 0;
	
    /*	decode tag/length pair (s) */
	elmtLevel = 0;
	
	for (ii=0; ii < iElementCount; ii++)
		//FOR_EACH_LIST_ELMT (e, set->basicType->a.set)
	{
		e = pSetElementNamedType[ii];
		tmpVarCount = CxxCountVariableLevels (e->type);
		if (tmpVarCount > varCount)
			varCount = tmpVarCount;
		if ( (e->type->optional || e->type->defaultVal != NULL) && (!e->type->extensionAddition))
		{
			lOptional_Default_ElmtCount++;
		}
	}
	
	/* NEXT, decode this number of bits, if any, to determine the 
	presence/absence of OPTIONAL/DEFAULT elements.	MUST BE DONE BEFORE 
	TAGs.*/
	if (lOptional_Default_ElmtCount)
	{
		fprintf(src, "\n\t// Decode the preamble\n");
		fprintf(src, "\tAsnBits SnaccOptionalDefaultBits;\n");
		fprintf(src, "\tbitsDecoded += _b.GetBits(SnaccOptionalDefaultBits, %ld);\n",
			lOptional_Default_ElmtCount);
	}
	
	//******************
	/****RWC; PERFORM PDecContent operations here... ***/
	/* print content local vars */
    //	fprintf (src, "  unsigned int mandatoryElmtsDecoded = 0;\n");
	
	//******************
	/* write extra length vars */
    //	fprintf (src, "{\n");		// RWC; Temporary until I figure out the local var names from combining PDec with PDecContent
    //	for (i = 1; i <= varCount; i++)
    //		fprintf (src, "  %s elmtLen%d = 0; //RWC;default to infinite for now.\n", lenTypeNameG, i);
    //	fprintf (src, "\n");
	
	/* handle empty set */
	//RWC;if ((set->basicType->a.set == NULL) || LIST_EMPTY (set->basicType->a.set))
	if (iElementCount == 0)
	{
		// RWC; Allow for "{" editing...
		/*fprintf (src, "    throw EXCEPT(\"Expected an empty sequence\", DECODE_ERROR);\n");
		fprintf (src, "  }\n");*/
	}
	else
	{
		fprintf(src, "\n\t// Decode each of the elements\n");
		for (ii=0; ii < iElementCount; ii++)
		{
			const char* tabStr = "\t";
			e = pSetElementNamedType[ii];

            if(!e->type->extensionAddition)
            {
			    cxxtri =  e->type->cxxTypeRefInfo;
			    if (e->type->optional || (e->type->defaultVal != NULL))
			    {
				    tabStr = "\t\t";
				    fprintf(src, "\tif (SnaccOptionalDefaultBits.GetBit(%d))\n",
					    iOptional_Default_ElementIndex++);
				    fprintf(src, "\t{\n");
			    }
			    
			    varName = cxxtri->fieldName;
			    
			    /* decode content */
			    if (cxxtri->isPtr)
			    {
				    //fprintf(src, "%sif(%s)\n", tabStr, varName);
                    //fprintf(src, "%s%sdelete %s;\n", tabStr, tabStr, varName);
			    
				    fprintf(src, "%s%s = new %s;\n", tabStr, varName,
						    cxxtri->className);
					     /* END IF subtypes, PER-Visible */
			    }
			    
			    /* decode content */
			    tmpTypeId = GetBuiltinType (e->type);
			    if ((tmpTypeId == BASICTYPE_OCTETCONTAINING) ||
				    (tmpTypeId == BASICTYPE_BITCONTAINING))
			    {
				    PrintCxxPERDecodeContaining(e->type, r, src);
			    }
			    else
			    {
				    if (tmpTypeId == BASICTYPE_ANYDEFINEDBY)
				    {
					    elmtLevel++;
				    
					    defByNamedType = e->type->basicType->a.anyDefinedBy->link;
					    PrintCxxSetTypeByCode(defByNamedType, cxxtri, src);
				    }
				    else if (tmpTypeId == BASICTYPE_ANY)
				    {
					    elmtLevel++;
				    }

				    if (cxxtri->isPtr)
					    fprintf(src, "%s%s->", tabStr, varName);
				    else
					    fprintf(src, "%s%s.", tabStr, varName);
				    fprintf(src, "P%s(_b, bitsDecoded);\n",	r->decodeBaseName);
			    }
			    
			    if (e->type->optional || (e->type->defaultVal != NULL))
				    fprintf (src, "\t}\n\n");
            }				
		} /* for each elmt */
    } /* if not empty set clause */

	fprintf (src, "} // %s::P%s()\n\n", td->cxxTypeDefInfo->className,
		r->decodeBaseName);
		
}		/* END PrintCxxDefCode_SetSeqPERDecode(...) */

/*** This routine handles sorting of groups of NameType element(s) based on the
 *   Set and Choice sorting rules.
 */
static void
PrintCxxDefCode_PERSort (
    NamedType ***pppElementNamedType, /* OUT, array of sorted NameType(s) */
    int **ppElementTag,      /* OUT, actual tag for sorted. */
    AsnList *pElementList)   /* IN, actual eSNACC defs for NameType(s). */
{
    NamedType **pElementNamedType;
    int *pElementTag;
    NamedType *e;
    NamedType *pnamedTypeTmp;
    Tag *tag;
    TagList *tags;
    int tagTmp;
    int stoleChoiceTags;
    int ii=0, iii;

        /*
         * FIRST, determine encode order by looking at each element tag/type.
         *  (careful with untagged Choice elements, may be nested).
         *  If not tagged in the ASN.1 syntax, then we sort based on the IMPLICIT
         *  tag, even though it may not be encoded for PER.
         * pElementList->count total elements for PER encode sorting.*/
        pElementTag = *ppElementTag = (int *)calloc(pElementList->count, sizeof(int));
        pElementNamedType = *pppElementNamedType = 
            (NamedType **)calloc(pElementList->count, sizeof(NamedType *));
        FOR_EACH_LIST_ELMT (e, pElementList)
        {
            /*RWC;SEE tag-utils.c, line 175 for example of looking at nested
             *RWC;  untagged Choice(s).  For PER, NEED to return lowest tag
             *RWC;  value in nested untagged Choice for sorting.
             *RWC;  The call to GetTags will only return tags with non-tagged
             *RWC;  "Choice" elements if present (flagged by "stoleChoiceTags").*/
            tags = GetTags (e->type, &stoleChoiceTags);

            if (LIST_EMPTY (tags))
            {
                pElementTag[ii] = 0;
                /* RWC; IGNORE; for now */
            }       /* END IF (LIST_EMPTY (tags))*/
            else if (stoleChoiceTags)
            {
                /* FOR untagged Choice, determine lowest possible tag for
                 *  PER sorting order.*/
                pElementTag[ii] = 9999;
                FOR_EACH_LIST_ELMT (tag, tags)
                {
                    if (tag->code < pElementTag[ii])
                       pElementTag[ii] = tag->code;  /* ONLY 1st element for sorting.*/
                }
            }
            else
            {
                tag = (Tag*)FIRST_LIST_ELMT (tags);
                pElementTag[ii] = tag->code;  // ONLY 1st element for sorting.
            }
            pElementNamedType[ii] = e;
            ii++;
        }       // END FOR each element.

        // SECOND, sort this group of elements based on these tags.
        for (ii=0; ii < pElementList->count-1; ii++)
        {
            for (iii=ii+1; iii < pElementList->count; iii++)
            {   // LOCATE smallest tag value
                if (pElementTag[iii] < pElementTag[ii])
                {   // THEN switch them.
                    tagTmp = pElementTag[ii];
                    pnamedTypeTmp = pElementNamedType[ii];
                    pElementTag[ii] = pElementTag[iii];
                    pElementNamedType[ii] = pElementNamedType[iii];
                    pElementTag[iii] = tagTmp;
                    pElementNamedType[iii] = pnamedTypeTmp;
                }
            }   // END for remaining elements (for sorting)
        }       // END FOR each element
}       /* END PrintCxxDefCode_PERSort(...) */

void PrintCxxSimpleDefMeta_1(FILE * hdr, FILE* src, TypeDef* td, int hasNamedElmts, CNamedElmt *n, Module* m)
{
    hdr = hdr;
    src = src;
    td = td;
    hasNamedElmts = hasNamedElmts;
    n = n;
    m = m;

    #if META
            const char	*T, *t;
	        int a3;

	        fprintf (hdr, "\n");
	        fprintf (hdr, "#if META\n");
	        fprintf (src, "#if META\n\n");

	        fprintf (src, "static AsnType *create%s()\n", td->cxxTypeDefInfo->className);
	        fprintf (src, "{\n");
	        fprintf (src, "  return new %s;\n", td->cxxTypeDefInfo->className);
	        fprintf (src, "}\n\n");

	        if ((hasNamedElmts = HasNamedElmts (td->type)))
	        {
		    fprintf (hdr, "  static const AsnNameDesc	_nmdescs[];\n");

		    fprintf (src, "const AsnNameDesc %s::_nmdescs[] =\n", td->cxxTypeDefInfo->className);
		    fprintf (src, "{\n");
		    FOR_EACH_LIST_ELMT (n, td->type->cxxTypeRefInfo->namedElmts)
    #if 0 /* (no asn1 names available!) */
		        if (printMetaG == META_backend_names)
		        else /* META_asn1_names */
    #endif
			    fprintf (src, "  \"%s\", %s, // %d\n", n->name, n->name, n->value);
		    fprintf (src, "  NULL, -1\n");
		    fprintf (src, "};\n\n");
	        }

	        switch (GetBuiltinType (td->type))
	        {
		    case BASICTYPE_BOOLEAN:
		        T = "BOOLEAN";
		        t = "Bool";
		        a3 = FALSE;
		        break;
		    case BASICTYPE_ENUMERATED:
		        T = "ENUMERATED";
		        t = "Enum";
		        a3 = TRUE;
		        break;
		    case BASICTYPE_INTEGER:
		        T = "INTEGER";
		        t = "Int";
		        a3 = TRUE;
		        break;
		    case BASICTYPE_REAL:
		        T = "REAL";
		        t = "Real";
		        a3 = FALSE;
		        break;
		    case BASICTYPE_OCTETSTRING:
		        T = "OCTET_STRING";
		        t = "Octs";
		        a3 = FALSE;
		        break;
		    case BASICTYPE_BITSTRING:
		        T = "BIT_STRING";
              t = "Bits";
		        a3 = TRUE;
		        break;
		    case BASICTYPE_OID:
		        T = "OID";
		        t = "Oid";
		        a3 = FALSE;
            case BASICTYPE_RELATIVE_OID:
                T = "RELATIVE_OID";
                t = "RelativeOid";
                a3 = FALSE;
	    	    default:
		        T =
		        t = "?";
		        a3 = FALSE;
	        }

	        fprintf (hdr, "  static const Asn%sTypeDesc	_desc;\n", t);
	        fprintf (hdr, "  const AsnTypeDesc	*_getdesc() const;\n");

	        fprintf (src, "const Asn%sTypeDesc %s::_desc\n", t, td->cxxTypeDefInfo->className);
	        fprintf (src, "(\n");
	        fprintf (src, "  &%sModuleDesc,\n", m->cxxname);
	        if (printMetaG == META_backend_names)
		    fprintf (src, "  \"%s\", // `%s'\n", td->cxxTypeDefInfo->className, td->definedName);
	        else /* META_asn1_names */
		    fprintf (src, "  \"%s\", // `%s'\n", td->definedName, td->cxxTypeDefInfo->className);
	        fprintf (src, "  %s,\n", isMetaPDU (m->modId->name, td->definedName, meta_pdus_G) ? "true" : "false");
	        fprintf (src, "  AsnTypeDesc::%s,\n", T);
	        fprintf (src, "  create%s", td->cxxTypeDefInfo->className);
	        if (a3)
		    fprintf (src, ",\n  %s", hasNamedElmts ? "_nmdescs" : "NULL");
	        fprintf (src, "\n);\n\n");

	        fprintf (src, "const AsnTypeDesc *%s::_getdesc() const\n", td->cxxTypeDefInfo->className);
	        fprintf (src, "{\n");
	        fprintf (src, "  return &_desc;\n");
	        fprintf (src, "}\n\n");

	        fprintf (hdr, "#endif // META\n");
	        fprintf (src, "#endif // META\n\n");

    #if TCL
    #endif

#endif //META
}

void PrintCxxSimpleDefMeta_2(FILE * hdr, FILE* src, TypeDef* td, int hasNamedElmts, CNamedElmt *n, Module* m, CxxRules *r)
{
    hdr = hdr;
    src = src;
    td = td;
    hasNamedElmts = hasNamedElmts;
    n = n;
    m = m;
    r = r;

   #if META
        fprintf (hdr, "#if META\n");
	    fprintf (src, "#if META\n\n");

	    fprintf (src, "static AsnType *create%s()\n", td->cxxTypeDefInfo->className);
	    fprintf (src, "{\n");
	    fprintf (src, "  return new %s;\n", td->cxxTypeDefInfo->className);
	    fprintf (src, "}\n\n");

	    fprintf (hdr, "struct %s: public %s\n", td->cxxTypeDefInfo->className, td->type->cxxTypeRefInfo->className);
	    fprintf (hdr, "{\n");

	    PrintDerivedConstructors (hdr, r, td);
        PrintCloneMethod (hdr, src, td);

	    fprintf (hdr, "  static const AsnAliasTypeDesc	_desc;\n");
	    fprintf (hdr, "  const AsnTypeDesc	*_getdesc() const;\n");

	    fprintf (src, "const AsnAliasTypeDesc %s::_desc\n", td->cxxTypeDefInfo->className);
	    fprintf (src, "(\n");
	    fprintf (src, "  &%sModuleDesc,\n", m->cxxname);
	    if (printMetaG == META_backend_names)
		fprintf (src, "  \"%s\", // `%s'\n", td->cxxTypeDefInfo->className, td->definedName);
	    else /* META_asn1_names */
		fprintf (src, "  \"%s\", // `%s'\n", td->definedName, td->cxxTypeDefInfo->className);
	    fprintf (src, "  %s,\n", isMetaPDU (m->modId->name, td->definedName, meta_pdus_G) ? "true" : "false");
	    fprintf (src, "  AsnTypeDesc::ALIAS,\n");
	    fprintf (src, "  create%s,\n", td->cxxTypeDefInfo->className);
	    fprintf (src, "  &%s::_desc\n);\n\n", td->type->cxxTypeRefInfo->className);

	    fprintf (src, "const AsnTypeDesc *%s::_getdesc() const\n", td->cxxTypeDefInfo->className);
	    fprintf (src, "{\n");
	    fprintf (src, "  return &_desc;\n");
	    fprintf (src, "}\n\n");

	    fprintf (hdr, "};\n\n");

	    fprintf (hdr, "#else // META\n\n");
	    fprintf (src, "#endif // META\n\n");
    #endif //META
}

void PrintCxxChoiceDefCodeMeta_1(FILE* hdr, FILE* src, TypeDef* td, Type* choice, Module* m, NamedType* e)
{
    hdr = hdr;
    src = src;
    td = td;
    choice = choice;
    m = m;
    e = e;

    #if META
	    fprintf (hdr, "#if META\n");
	    fprintf (src, "#if META\n\n");

	    fprintf (hdr, "  static const AsnChoiceTypeDesc	_desc;\n");
	    fprintf (hdr, "  static const AsnChoiceMemberDesc	_mdescs[];\n\n");
	    fprintf (hdr, "  const AsnTypeDesc		*_getdesc() const;\n");
	    fprintf (hdr, "  AsnType			*_getref (const char *membername, bool create = false);\n\n");

	    fprintf (src, "static AsnType *create%s()\n", td->cxxTypeDefInfo->className);
	    fprintf (src, "{\n");
	    fprintf (src, "  return new %s;\n", td->cxxTypeDefInfo->className);
	    fprintf (src, "}\n\n");

	    fprintf (src, "const AsnChoiceMemberDesc %s::_mdescs[] =\n", td->cxxTypeDefInfo->className);
	    fprintf (src, "{\n");
	    FOR_EACH_LIST_ELMT (e, choice->basicType->a.choice)
	        if (printMetaG == META_backend_names)
		    fprintf (src, "  AsnChoiceMemberDesc (\"%s\", &%s::_desc), // `%s'\n", e->type->cxxTypeRefInfo->fieldName, e->type->cxxTypeRefInfo->className, e->fieldName ? e->fieldName : "");
	        else /* META_asn1_names */
		    fprintf (src, "  AsnChoiceMemberDesc (\"%s\", &%s::_desc), // `%s'\n", e->fieldName ? e->fieldName : e->type->cxxTypeRefInfo->fieldName, e->type->cxxTypeRefInfo->className, e->type->cxxTypeRefInfo->fieldName);
	    fprintf (src, "  AsnChoiceMemberDesc()\n");
	    fprintf (src, "};\n\n");

	    fprintf (src, "const AsnChoiceTypeDesc %s::_desc\n", td->cxxTypeDefInfo->className);
	    fprintf (src, "(\n");
	    fprintf (src, "  &%sModuleDesc,\n", m->cxxname);
	    if (printMetaG == META_backend_names)
	        fprintf (src, "  \"%s\", // `%s'\n", td->cxxTypeDefInfo->className, td->definedName);
	    else /* META_asn1_names */
	        fprintf (src, "  \"%s\", // `%s'\n", td->definedName, td->cxxTypeDefInfo->className);
	    fprintf (src, "  %s,\n", isMetaPDU (m->modId->name, td->definedName, meta_pdus_G) ? "true" : "false");
	    fprintf (src, "  AsnTypeDesc::CHOICE,\n");
	    fprintf (src, "  create%s,\n", td->cxxTypeDefInfo->className);
	    fprintf (src, "  _mdescs\n");
	    fprintf (src, ");\n\n");

	    fprintf (src, "const AsnTypeDesc *%s::_getdesc() const\n", td->cxxTypeDefInfo->className);
	    fprintf (src, "{\n");
	    fprintf (src, "  return &_desc;\n");
	    fprintf (src, "}\n\n");

	    fprintf (src, "AsnType *%s::_getref (const char *membername, bool create)\n", td->cxxTypeDefInfo->className);
	    fprintf (src, "{\n");
	    fprintf (src, "  ChoiceIdEnum newCid = (ChoiceIdEnum)_desc.choicebyname (membername);\n");
	    fprintf (src, "  if (newCid == -1)\n");
	    fprintf (src, "    return NULL;\n");
	    fprintf (src, "  if (newCid == choiceId)\n");
	    fprintf (src, "  {\n");
	    fprintf (src, "    switch (choiceId)\n");
	    fprintf (src, "    {\n");
	    FOR_EACH_LIST_ELMT (e, choice->basicType->a.choice)
	    {
	        fprintf (src, "      case %sCid:\n", e->type->cxxTypeRefInfo->fieldName);
	        fprintf (src, "        return %s;\n", e->type->cxxTypeRefInfo->fieldName);
	    }
	    fprintf (src, "      default:\n");
	    fprintf (src, "        return NULL;\n");
	    fprintf (src, "    }\n");
	    fprintf (src, "  }\n");
	    fprintf (src, "  else\n");
	    fprintf (src, "  {\n");
	    fprintf (src, "    if (create)\n");
	    fprintf (src, "    {\n");
	    fprintf (src, "//      switch (choiceId)\n");
	    fprintf (src, "//      {\n");
	    FOR_EACH_LIST_ELMT (e, choice->basicType->a.choice)
	    {
	        fprintf (src, "//        case %sCid:\n", e->type->cxxTypeRefInfo->fieldName);
	        fprintf (src, "//          delete %s;\n", e->type->cxxTypeRefInfo->fieldName);
	        fprintf (src, "//          %s = NULL;\n", e->type->cxxTypeRefInfo->fieldName);
	        fprintf (src, "//          break;\n");
	    }
	    fprintf (src, "//        default:\n");
	    fprintf (src, "//          return NULL;\n");
	    fprintf (src, "//      }\n");
	    e = FIRST_LIST_ELMT (choice->basicType->a.choice);
	    fprintf (src, "      // simply delete any member, the virtual function table takes care of the rest:\n");
	    fprintf (src, "      delete %s;\n", e->type->cxxTypeRefInfo->fieldName);
	    fprintf (src, "      %s = NULL;\n", e->type->cxxTypeRefInfo->fieldName);
	    fprintf (src, "      switch (choiceId = newCid)\n");
	    fprintf (src, "      {\n");
	    FOR_EACH_LIST_ELMT (e, choice->basicType->a.choice)
	    {
	        fprintf (src, "        case %sCid:\n", e->type->cxxTypeRefInfo->fieldName);
	        fprintf (src, "          return %s = new %s;\n", e->type->cxxTypeRefInfo->fieldName, e->type->cxxTypeRefInfo->className);
	    }
	    fprintf (src, "        default: // internal error!\n");
	    fprintf (src, "          return NULL;\n");
	    fprintf (src, "      }\n");
	    fprintf (src, "    }\n");
	    fprintf (src, "    else\n");
	    fprintf (src, "      return NULL;\n");
	    fprintf (src, "  }\n");
	    fprintf (src, "}\n\n");

        #if TCL
	    if (printTclG)
	    {
	        fprintf (hdr, "#if TCL\n");
	        fprintf (src, "#if TCL\n\n");

	        fprintf (hdr, "  int			TclGetDesc (Tcl_DString *) const;\n");
	        fprintf (hdr, "  int			TclGetVal (Tcl_Interp *) const;\n");
	        fprintf (hdr, "  int			TclSetVal (Tcl_Interp *, const char *valstr);\n\n");

	        fprintf (src, "int %s::TclGetDesc (Tcl_DString *valstr) const\n", td->cxxTypeDefInfo->className);
	        fprintf (src, "{\n");
	        fprintf (src, "  Tcl_DStringAppendElement (valstr, (char*)_desc.choicebyvalue (choiceId));\n");
	        fprintf (src, "  // hack: since all members are pointers, we don't have to check for its type via choiceId, because all we want to know is whether it's NULL or not:\n");
	        e = FIRST_LIST_ELMT (choice->basicType->a.choice);
	        fprintf (src, "  Tcl_DStringAppendElement (valstr, %s ? \"valid\" : \"void\");\n", e->type->cxxTypeRefInfo->fieldName);
	        fprintf (src, "  return TCL_OK;\n");
	        fprintf (src, "}\n\n");

	        fprintf (src, "int %s::TclGetVal (Tcl_Interp *interp) const\n", td->cxxTypeDefInfo->className);
	        fprintf (src, "{\n");
	        fprintf (src, "  switch (choiceId)\n");
	        fprintf (src, "  {\n");
	        FOR_EACH_LIST_ELMT (e, choice->basicType->a.choice)
	        {
		    fprintf (src, "    case %sCid:\n", e->type->cxxTypeRefInfo->fieldName);
		    fprintf (src, "      if (%s)\n", e->type->cxxTypeRefInfo->fieldName);
		    fprintf (src, "        if (%s->TclGetVal (interp) != TCL_OK)\n", e->type->cxxTypeRefInfo->fieldName);
		    fprintf (src, "          return TCL_ERROR;\n");
		    fprintf (src, "      break;\n");
	        }
	        fprintf (src, "    default:\n");
	        fprintf (src, "      Tcl_SetResult (interp, \"illegal choiceId in %s\", TCL_STATIC);\n", td->cxxTypeDefInfo->className);
	        fprintf (src, "      Tcl_SetErrorCode (interp, \"SNACC\", \"ILLCHOICE\", NULL);\n");
	        fprintf (src, "      return TCL_ERROR;\n");
	        fprintf (src, "  }\n");
	        fprintf (src, "  Tcl_DString valstr;\n");
	        fprintf (src, "  Tcl_DStringInit (&valstr);\n");
	        fprintf (src, "  Tcl_DStringAppendElement (&valstr, (char*)_desc.choicebyvalue (choiceId));\n");
	        fprintf (src, "  Tcl_DStringAppendElement (&valstr, interp->result);\n");
	        fprintf (src, "  Tcl_ResetResult (interp);\n");
	        fprintf (src, "  Tcl_DStringResult (interp, &valstr);\n");
	        fprintf (src, "  return TCL_OK;\n");
	        fprintf (src, "}\n\n");

	        fprintf (src, "int %s::TclSetVal (Tcl_Interp *interp, const char *valstr)\n", td->cxxTypeDefInfo->className);
	        fprintf (src, "{\n");
	        fprintf (src, "  Args elem;\n");
	        fprintf (src, "  if (Tcl_SplitList (interp, (char*)valstr, &elem.c, &elem.v) != TCL_OK)\n");
	        fprintf (src, "    return TCL_ERROR;\n");
	        fprintf (src, "  if (elem.c != 2)\n");
	        fprintf (src, "  {\n");
	        fprintf (src, "    sprintf (interp->result, \"syntax error: expected a pair, but it's got %%d element(s)\", elem.c);\n");
	        fprintf (src, "    return TCL_ERROR;\n");
	        fprintf (src, "  }\n");
	        fprintf (src, "  AsnType *member = _getref (elem.v[0], true);\n");
	        fprintf (src, "  if (!member)\n");
	        fprintf (src, "  {\n");
	        fprintf (src, "    Tcl_AppendResult (interp, \"illegal choice \", elem.v[0], \" for %s\", NULL);\n", td->cxxTypeDefInfo->className);
	        fprintf (src, "    Tcl_SetErrorCode (interp, \"SNACC\", \"ILLCHOICE\", NULL);\n");
	        fprintf (src, "    return TCL_ERROR;\n");
	        fprintf (src, "  }\n");
	        fprintf (src, "  return member->TclSetVal (interp, elem.v[1]);\n");
	        fprintf (src, "}\n\n");

	        fprintf (hdr, "#endif // TCL\n");
	        fprintf (src, "#endif // TCL\n\n");
	    }
        #endif /* TCL */

	    fprintf (hdr, "#endif // META\n");
	    fprintf (src, "#endif // META\n\n");
    #endif /* META */
}

void PrintCxxSeqDefCodeMeta_1(FILE* hdr, FILE* src, TypeDef* td, Type* seq, Module* m, NamedType* e)
{
    hdr = hdr;
    src = src;
    td = td;
    seq = seq;
    m = m;
    e = e;

    #if META
	        fprintf (hdr, "#if META\n");
	        fprintf (src, "#if META\n\n");

	        fprintf (hdr, "  static const AsnSequenceTypeDesc	_desc;\n");
	        fprintf (hdr, "  static const AsnSequenceMemberDesc	_mdescs[];\n");
	        fprintf (hdr, "  const AsnTypeDesc		*_getdesc() const;\n");
	        fprintf (hdr, "  AsnType			*_getref (const char *membername, bool create = false);\n\n");

	        fprintf (src, "static AsnType *create%s()\n", td->cxxTypeDefInfo->className);
	        fprintf (src, "{\n");
	        fprintf (src, "  return new %s;\n", td->cxxTypeDefInfo->className);
	        fprintf (src, "}\n\n");

	        fprintf (src, "const AsnSequenceMemberDesc %s::_mdescs[] =\n", td->cxxTypeDefInfo->className);
	        fprintf (src, "{\n");
	        FOR_EACH_LIST_ELMT (e, seq->basicType->a.sequence)
	            if (printMetaG == META_backend_names)
		        fprintf (src, "  AsnSequenceMemberDesc (\"%s\", &%s::_desc, %s), // `%s'\n", e->type->cxxTypeRefInfo->fieldName, e->type->cxxTypeRefInfo->className, e->type->optional || e->type->defaultVal ? "true" : "false", e->fieldName ? e->fieldName : "");
	            else /* META_asn1_names */
		        fprintf (src, "  AsnSequenceMemberDesc (\"%s\", &%s::_desc, %s), // `%s'\n", e->fieldName ? e->fieldName : e->type->cxxTypeRefInfo->fieldName, e->type->cxxTypeRefInfo->className, e->type->optional || e->type->defaultVal ? "true" : "false", e->type->cxxTypeRefInfo->fieldName);
	        fprintf (src, "  AsnSequenceMemberDesc()\n");
	        fprintf (src, "};\n\n");

	        fprintf (src, "const AsnSequenceTypeDesc %s::_desc\n", td->cxxTypeDefInfo->className);
	        fprintf (src, "(\n");
	        fprintf (src, "  &%sModuleDesc,\n", m->cxxname);
	        if (printMetaG == META_backend_names)
	            fprintf (src, "  \"%s\", // `%s'\n", td->cxxTypeDefInfo->className, td->definedName);
	        else /* META_asn1_names */
	            fprintf (src, "  \"%s\", // `%s'\n", td->definedName, td->cxxTypeDefInfo->className);
	        fprintf (src, "  %s,\n", isMetaPDU (m->modId->name, td->definedName, meta_pdus_G) ? "true" : "false");
	        fprintf (src, "  AsnTypeDesc::SEQUENCE,\n");
	        fprintf (src, "  create%s,\n", td->cxxTypeDefInfo->className);
	        fprintf (src, "  _mdescs\n");
	        fprintf (src, ");\n\n");

	        fprintf (src, "const AsnTypeDesc *%s::_getdesc() const\n", td->cxxTypeDefInfo->className);
	        fprintf (src, "{\n");
	        fprintf (src, "  return &_desc;\n");
	        fprintf (src, "}\n\n");

	        fprintf (src, "AsnType *%s::_getref (const char *membername, bool create)\n", td->cxxTypeDefInfo->className);
	        fprintf (src, "{\n");
	        FOR_EACH_LIST_ELMT (e, seq->basicType->a.sequence)
	        {
	            fprintf (src, "  if (!strcmp (membername, \"%s\"))\n", e->type->cxxTypeRefInfo->fieldName);
	            if (e->type->cxxTypeRefInfo->isPtr)
	            {
		        fprintf (src, "  {\n");
		        fprintf (src, "    if (!%s && create)\n", e->type->cxxTypeRefInfo->fieldName);
                fprintf (src, "      %s = new %s;\n", e->type->cxxTypeRefInfo->fieldName, e->type->cxxTypeRefInfo->className);
                fprintf (src, "    return %s;\n", e->type->cxxTypeRefInfo->fieldName);
		        fprintf (src, "  }\n");
	            }
	            else
		        fprintf (src, "    return &%s;\n", e->type->cxxTypeRefInfo->fieldName);
	        }
	        fprintf (src, "  return NULL;\n");
	        fprintf (src, "}\n\n");

        #if TCL
	        if (printTclG)
	        {
	            fprintf (hdr, "#if TCL\n");
	            fprintf (src, "#if TCL\n\n");

	            fprintf (hdr, "  int			TclGetDesc (Tcl_DString *) const;\n");
	            fprintf (hdr, "  int			TclGetVal (Tcl_Interp *) const;\n");
	            fprintf (hdr, "  int			TclSetVal (Tcl_Interp *, const char *valstr);\n");
	            fprintf (hdr, "  int			TclUnsetVal (Tcl_Interp *, const char *membname);\n\n");

	            fprintf (src, "int %s::TclGetDesc (Tcl_DString *valstr) const\n", td->cxxTypeDefInfo->className);
	            fprintf (src, "{\n");
	            fprintf (src, "  Tcl_DStringStartSublist (valstr);\n\n");
	            FOR_EACH_LIST_ELMT (e, seq->basicType->a.sequence)
	            {
		        fprintf (src, "  Tcl_DStringStartSublist (valstr);\n");
		        fprintf (src, "  Tcl_DStringAppendElement (valstr, \"%s\");\n", e->type->cxxTypeRefInfo->fieldName);
		        if (e->type->cxxTypeRefInfo->isPtr)
		            fprintf (src, "  Tcl_DStringAppendElement (valstr, %s ? \"valid\" : \"void\");\n", e->type->cxxTypeRefInfo->fieldName);
		        else
		            fprintf (src, "  Tcl_DStringAppendElement (valstr, %s \"valid\");\n", e->type->cxxTypeRefInfo->fieldName);
		        fprintf (src, "  Tcl_DStringEndSublist (valstr);\n\n");
	            }
	            fprintf (src, "  Tcl_DStringEndSublist (valstr);\n\n");
	            fprintf (src, "  return TCL_OK;\n");
	            fprintf (src, "}\n\n");

	            fprintf (src, "int %s::TclGetVal (Tcl_Interp *interp) const\n", td->cxxTypeDefInfo->className);
	            fprintf (src, "{\n");
	            fprintf (src, "  Tcl_DString valstr;\n\n");
	            fprintf (src, "  Tcl_DStringInit (&valstr);\n\n");
	            FOR_EACH_LIST_ELMT (e, seq->basicType->a.sequence)
	            {
		        if (e->type->cxxTypeRefInfo->isPtr)
		        {
		            fprintf (src, "  if (%s)\n", e->type->cxxTypeRefInfo->fieldName);
		            fprintf (src, "  {\n");
		            fprintf (src, "    Tcl_DStringStartSublist (&valstr);\n");
		            fprintf (src, "    Tcl_DStringAppendElement (&valstr, \"%s\");\n", e->type->cxxTypeRefInfo->fieldName);
		            fprintf (src, "    if (%s->TclGetVal (interp) != TCL_OK)\n", e->type->cxxTypeRefInfo->fieldName);
		            fprintf (src, "      goto Error;\n");
		            fprintf (src, "    Tcl_DStringAppendElement (&valstr, interp->result);\n");
		            fprintf (src, "    Tcl_ResetResult (interp);\n");
		            fprintf (src, "    Tcl_DStringEndSublist (&valstr);\n");
		            fprintf (src, "  }\n\n");
		        }
		        else
		        {
		            fprintf (src, "  Tcl_DStringStartSublist (&valstr);\n");
		            fprintf (src, "  Tcl_DStringAppendElement (&valstr, \"%s\");\n", e->type->cxxTypeRefInfo->fieldName);
		            fprintf (src, "  if (%s.TclGetVal (interp) != TCL_OK)\n", e->type->cxxTypeRefInfo->fieldName);
		            fprintf (src, "    goto Error;\n");
		            fprintf (src, "  Tcl_DStringAppendElement (&valstr, interp->result);\n");
		            fprintf (src, "  Tcl_ResetResult (interp);\n");
		            fprintf (src, "  Tcl_DStringEndSublist (&valstr);\n\n");
		        }
	            }
	            fprintf (src, "  Tcl_DStringResult (interp, &valstr);\n");
	            fprintf (src, "  return TCL_OK;\n\n");
	            fprintf (src, "Error:\n");
	            fprintf (src, "  Tcl_DStringFree (&valstr);\n");
	            fprintf (src, "  return TCL_ERROR;\n");
	            fprintf (src, "}\n\n");

	            fprintf (src, "int %s::TclSetVal (Tcl_Interp *interp, const char *valstr)\n", td->cxxTypeDefInfo->className);
	            fprintf (src, "{\n");
	            fprintf (src, "  int i;\n");
	            fprintf (src, "  Args elems;\n");
	            fprintf (src, "  if (Tcl_SplitList (interp, (char*)valstr, &elems.c, &elems.v) != TCL_OK)\n");
	            fprintf (src, "    return TCL_ERROR;\n");
	            fprintf (src, "  Args* elempairs = new Args[elems.c];\n");
	            fprintf (src, "  for (i=0; i<elems.c; i++)\n");
	            fprintf (src, "  {\n");
	            fprintf (src, "    if (Tcl_SplitList (interp, elems.v[i], &elempairs[i].c, &elempairs[i].v) != TCL_OK)\n");
	            fprintf (src, "    {\n");
	            fprintf (src, "      delete elempairs;\n");
	            fprintf (src, "      return TCL_ERROR;\n");
	            fprintf (src, "    }\n");
	            fprintf (src, "    if (elempairs[i].c != 2)\n");
	            fprintf (src, "    {\n");
	            fprintf (src, "      sprintf (interp->result, \"syntax error in element #%%d: expected a pair, but it's got %%d element(s)\", i, elempairs[i].c);\n");
	            fprintf (src, "      delete elempairs;\n");
	            fprintf (src, "      return TCL_ERROR;\n");
	            fprintf (src, "    }\n");
	            fprintf (src, "  }\n");
	            fprintf (src, "\n");
	            fprintf (src, "  for (const AsnSequenceMemberDesc *m=_mdescs; m->name; m++)\n");
	            fprintf (src, "  {\n");
	            fprintf (src, "    int count = 0;\n");
	            fprintf (src, "    for (i=0; i<elems.c; i++)\n");
	            fprintf (src, "      if (!strcmp (elempairs[i].v[0], m->name))\n");
	            fprintf (src, "        count++;\n");
	            fprintf (src, "    if (count > 1)\n");
	            fprintf (src, "    {\n");
	            fprintf (src, "      sprintf (interp->result, \"duplicate value for member \\\"%%s\\\" in list\", m->name);\n");
	            fprintf (src, "      Tcl_SetErrorCode (interp, \"SNACC\", \"DUPMEMB\", NULL);\n");
	            fprintf (src, "      delete elempairs;\n");
	            fprintf (src, "      return TCL_ERROR;\n");
	            fprintf (src, "    }\n");
	            fprintf (src, "    if (!m->optional && count < 1)\n");
	            fprintf (src, "    {\n");
	            fprintf (src, "      sprintf (interp->result, \"mandatory member \\\"%%s\\\" is missing in list\", m->name);\n");
	            fprintf (src, "      Tcl_SetErrorCode (interp, \"SNACC\", \"MISSMAND\", NULL);\n");
	            fprintf (src, "      delete elempairs;\n");
	            fprintf (src, "      return TCL_ERROR;\n");
	            fprintf (src, "    }\n");
	            fprintf (src, "    \n");
	            fprintf (src, "  }\n");
	            fprintf (src, "\n");
	            fprintf (src, "  for (i=0; i<elems.c; i++)\n");
	            fprintf (src, "  {\n");
	            FOR_EACH_LIST_ELMT (e, seq->basicType->a.sequence)
	            {
		        fprintf (src, "    if (!strcmp (elempairs[i].v[0], \"%s\"))\n", e->type->cxxTypeRefInfo->fieldName);
		        fprintf (src, "    {\n");
		        if (e->type->cxxTypeRefInfo->isPtr)
		        {
		            fprintf (src, "      if (!%s)\n", e->type->cxxTypeRefInfo->fieldName);
                    fprintf (src, "        %s = new %s;\n", e->type->cxxTypeRefInfo->fieldName, e->type->cxxTypeRefInfo->className);
    	            fprintf (src, "      if (%s->TclSetVal (interp, elempairs[i].v[1]))\n", e->type->cxxTypeRefInfo->fieldName);
		            fprintf (src, "      {\n");
		            fprintf (src, "        delete elempairs;\n");
		            fprintf (src, "        return TCL_ERROR;\n");
		            fprintf (src, "      }\n");
		        }
		        else
		        {
		            fprintf (src, "      if (%s.TclSetVal (interp, elempairs[i].v[1]))\n", e->type->cxxTypeRefInfo->fieldName);
		            fprintf (src, "      {\n");
		            fprintf (src, "        delete elempairs;\n");
		            fprintf (src, "        return TCL_ERROR;\n");
		            fprintf (src, "      }\n");
		        }
		        fprintf (src, "    }\n");
	            }
	            fprintf (src, "  }\n");
	            fprintf (src, "\n");
	            fprintf (src, "  // look for unmentioned optional members and delete them:\n");
	            FOR_EACH_LIST_ELMT (e, seq->basicType->a.sequence)
	            {
		        if (e->type->optional || e->type->defaultVal)
		        {
		            fprintf (src, "  {\n");
		            fprintf (src, "    bool present = false;\n");
		            fprintf (src, "    for (i=0; i<elems.c; i++)\n");
		            fprintf (src, "      if (!strcmp (elempairs[i].v[0], \"%s\"))\n", e->type->cxxTypeRefInfo->fieldName);
		            fprintf (src, "        present = true;\n");
		            fprintf (src, "    if (!present)\n");
		            fprintf (src, "    {\n");
		            fprintf (src, "      delete %s;\n", e->type->cxxTypeRefInfo->fieldName);
		            fprintf (src, "      %s = NULL;\n", e->type->cxxTypeRefInfo->fieldName);
		            fprintf (src, "    }\n");
		            fprintf (src, "  }\n");
		        }
	            }
	            fprintf (src, "\n");
	            fprintf (src, "  delete elempairs;\n");
	            fprintf (src, "  return TCL_OK;\n");
	            fprintf (src, "}\n\n");

	            fprintf (src, "int %s::TclUnsetVal (Tcl_Interp *interp, const char *membernames)\n", td->cxxTypeDefInfo->className);
	            fprintf (src, "{\n");
	            fprintf (src, "  Args elems;\n");
	            fprintf (src, "  if (Tcl_SplitList (interp, (char*)membernames, &elems.c, &elems.v) != TCL_OK)\n");
	            fprintf (src, "    return TCL_ERROR;\n");
	            fprintf (src, "\n");
	            fprintf (src, "  for (int i=0; i<elems.c; i++)\n");
	            fprintf (src, "  {\n");
	            FOR_EACH_LIST_ELMT (e, seq->basicType->a.sequence)
	            {
		        fprintf (src, "    if (!strcmp (elems.v[i], \"%s\"))\n", e->type->cxxTypeRefInfo->fieldName);
		        fprintf (src, "    {\n");
		        if (e->type->optional || e->type->defaultVal)
		        {
		            fprintf (src, "        delete %s;\n", e->type->cxxTypeRefInfo->fieldName);
		            fprintf (src, "        %s = NULL;\n", e->type->cxxTypeRefInfo->fieldName);
		        }
		        else
		        {
		            fprintf (src, "        return _desc.mandatmemberr (interp, elems.v[i]);\n");
		        }
		        fprintf (src, "    }\n");
	            }
	            fprintf (src, "  }\n");
	            fprintf (src, "  return TCL_OK;\n");
	            fprintf (src, "}\n\n");

	            fprintf (hdr, "#endif // TCL\n");
	            fprintf (src, "#endif // TCL\n\n");
	        }
        #endif /* TCL */

	        fprintf (hdr, "#endif // META\n\n");
	        fprintf (src, "#endif // META\n\n");

    #endif /* META*/
}

void PrintCxxSetDefCodeMeta_1(FILE* hdr, FILE* src, TypeDef* td, Type* set, Module* m, NamedType* e)
{
    hdr = hdr;
    src = src;
    td = td;
    set = set;
    m = m;
    e = e;

    #if META
	        fprintf (hdr, "#if META\n");
	        fprintf (src, "#if META\n\n");

	        fprintf (hdr, "  static const AsnSetTypeDesc	_desc;\n");
	        fprintf (hdr, "  static const AsnSetMemberDesc	_mdescs[];\n");
	        fprintf (hdr, "  const AsnTypeDesc		*_getdesc() const;\n");
	        fprintf (hdr, "  AsnType			*_getref (const char *membername, bool create = false);\n\n");

	        fprintf (src, "static AsnType *create%s()\n", td->cxxTypeDefInfo->className);
	        fprintf (src, "{\n");
	        fprintf (src, "  return new %s;\n", td->cxxTypeDefInfo->className);
	        fprintf (src, "}\n\n");

	        fprintf (src, "const AsnSetMemberDesc %s::_mdescs[] =\n", td->cxxTypeDefInfo->className);
	        fprintf (src, "{\n");
	        FOR_EACH_LIST_ELMT (e, set->basicType->a.set)
	            if (printMetaG == META_backend_names)
		        fprintf (src, "  AsnSetMemberDesc (\"%s\", &%s::_desc, %s), // `%s'\n", e->type->cxxTypeRefInfo->fieldName, e->type->cxxTypeRefInfo->className, e->type->optional || e->type->defaultVal ? "true" : "false", e->fieldName ? e->fieldName : "");
	            else /* META_asn1_names */
		        fprintf (src, "  AsnSetMemberDesc (\"%s\", &%s::_desc, %s), // `%s'\n", e->fieldName ? e->fieldName : e->type->cxxTypeRefInfo->fieldName, e->type->cxxTypeRefInfo->className, e->type->optional || e->type->defaultVal ? "true" : "false", e->type->cxxTypeRefInfo->fieldName);
	        fprintf (src, "  AsnSetMemberDesc()\n");
	        fprintf (src, "};\n\n");

	        fprintf (src, "const AsnSetTypeDesc %s::_desc\n", td->cxxTypeDefInfo->className);
	        fprintf (src, "(\n");
	        fprintf (src, "  &%sModuleDesc,\n", m->cxxname);
	        if (printMetaG == META_backend_names)
	            fprintf (src, "  \"%s\", // `%s'\n", td->cxxTypeDefInfo->className, td->definedName);
	        else /* META_asn1_names */
	            fprintf (src, "  \"%s\", // `%s'\n", td->definedName, td->cxxTypeDefInfo->className);
	        fprintf (src, "  %s,\n", isMetaPDU (m->modId->name, td->definedName, meta_pdus_G) ? "true" : "false");
	        fprintf (src, "  AsnTypeDesc::SET,\n");
	        fprintf (src, "  create%s,\n", td->cxxTypeDefInfo->className);
	        fprintf (src, "  _mdescs\n");
	        fprintf (src, ");\n\n");

	        fprintf (src, "const AsnTypeDesc *%s::_getdesc() const\n", td->cxxTypeDefInfo->className);
	        fprintf (src, "{\n");
	        fprintf (src, "  return &_desc;\n");
	        fprintf (src, "}\n\n");

	        fprintf (src, "AsnType *%s::_getref (const char *membername, bool create)\n", td->cxxTypeDefInfo->className);
	        fprintf (src, "{\n");
	        FOR_EACH_LIST_ELMT (e, set->basicType->a.set)
	        {
	            fprintf (src, "  if (!strcmp (membername, \"%s\"))\n", e->type->cxxTypeRefInfo->fieldName);
	            if (e->type->cxxTypeRefInfo->isPtr)
	            {
		        fprintf (src, "  {\n");
		        fprintf (src, "    if (!%s && create)\n", e->type->cxxTypeRefInfo->fieldName);
                fprintf (src, "      %s = new %s;\n", e->type->cxxTypeRefInfo->fieldName, e->type->cxxTypeRefInfo->className);
                fprintf (src, "    return %s;\n", e->type->cxxTypeRefInfo->fieldName);
		        fprintf (src, "  }\n");
	            }
	            else
		        fprintf (src, "    return &%s;\n", e->type->cxxTypeRefInfo->fieldName);
	        }
	        fprintf (src, "  return NULL;\n");
	        fprintf (src, "}\n\n");

        #if TCL
	        if (printTclG)
	        {
	            fprintf (hdr, "#if TCL\n");
	            fprintf (src, "#if TCL\n\n");

	            fprintf (hdr, "  int			TclGetDesc (Tcl_DString *) const;\n");
	            fprintf (hdr, "  int			TclGetVal (Tcl_Interp *) const;\n");
	            fprintf (hdr, "  int			TclSetVal (Tcl_Interp *, const char *valstr);\n");
	            fprintf (hdr, "  int			TclUnsetVal (Tcl_Interp *, const char *membernames);\n\n");

	            fprintf (src, "int %s::TclGetDesc (Tcl_DString *valstr) const\n", td->cxxTypeDefInfo->className);
	            fprintf (src, "{\n");
	            fprintf (src, "  Tcl_DStringStartSublist (valstr);\n\n");
	            FOR_EACH_LIST_ELMT (e, set->basicType->a.set)
	            {
		        fprintf (src, "  Tcl_DStringStartSublist (valstr);\n");
		        fprintf (src, "  Tcl_DStringAppendElement (valstr, \"%s\");\n", e->type->cxxTypeRefInfo->fieldName);
		        if (e->type->cxxTypeRefInfo->isPtr)
		            fprintf (src, "  Tcl_DStringAppendElement (valstr, %s ? \"valid\" : \"void\");\n", e->type->cxxTypeRefInfo->fieldName);
		        else
		            fprintf (src, "  Tcl_DStringAppendElement (valstr, \"valid\");\n", e->type->cxxTypeRefInfo->fieldName);
		        fprintf (src, "  Tcl_DStringEndSublist (valstr);\n\n");
	            }
	            fprintf (src, "  Tcl_DStringEndSublist (valstr);\n\n");
	            fprintf (src, "  return TCL_OK;\n");
	            fprintf (src, "}\n\n");

	            fprintf (src, "int %s::TclGetVal (Tcl_Interp *interp) const\n", td->cxxTypeDefInfo->className);
	            fprintf (src, "{\n");
	            fprintf (src, "  Tcl_DString valstr;\n\n");
	            fprintf (src, "  Tcl_DStringInit (&valstr);\n\n");
	            FOR_EACH_LIST_ELMT (e, set->basicType->a.set)
	            {
		        if (e->type->cxxTypeRefInfo->isPtr)
		        {
		            fprintf (src, "  if (%s)\n", e->type->cxxTypeRefInfo->fieldName);
		            fprintf (src, "  {\n");
		            fprintf (src, "    Tcl_DStringStartSublist (&valstr);\n");
		            fprintf (src, "    Tcl_DStringAppendElement (&valstr, \"%s\");\n", e->type->cxxTypeRefInfo->fieldName);
		            fprintf (src, "    if (%s->TclGetVal (interp) != TCL_OK)\n", e->type->cxxTypeRefInfo->fieldName);
		            fprintf (src, "      goto Error;\n");
		            fprintf (src, "    Tcl_DStringAppendElement (&valstr, interp->result);\n");
		            fprintf (src, "    Tcl_ResetResult (interp);\n");
		            fprintf (src, "    Tcl_DStringEndSublist (&valstr);\n");
		            fprintf (src, "  }\n\n");
		        }
		        else
		        {
		            fprintf (src, "  Tcl_DStringStartSublist (&valstr);\n");
		            fprintf (src, "  Tcl_DStringAppendElement (&valstr, \"%s\");\n", e->type->cxxTypeRefInfo->fieldName);
		            fprintf (src, "  if (%s.TclGetVal (interp) != TCL_OK)\n", e->type->cxxTypeRefInfo->fieldName);
		            fprintf (src, "    goto Error;\n");
		            fprintf (src, "  Tcl_DStringAppendElement (&valstr, interp->result);\n");
		            fprintf (src, "  Tcl_ResetResult (interp);\n");
		            fprintf (src, "  Tcl_DStringEndSublist (&valstr);\n\n");
		        }
	            }
	            fprintf (src, "  Tcl_DStringResult (interp, &valstr);\n");
	            fprintf (src, "  return TCL_OK;\n\n");
	            fprintf (src, "Error:\n");
	            fprintf (src, "  Tcl_DStringFree (&valstr);\n");
	            fprintf (src, "  return TCL_ERROR;\n");
	            fprintf (src, "}\n\n");

	            fprintf (src, "int %s::TclSetVal (Tcl_Interp *interp, const char *valstr)\n", td->cxxTypeDefInfo->className);
	            fprintf (src, "{\n");
	            fprintf (src, "  int i;\n");
	            fprintf (src, "  Args elems;\n");
	            fprintf (src, "  if (Tcl_SplitList (interp, (char*)valstr, &elems.c, &elems.v) != TCL_OK)\n");
	            fprintf (src, "    return TCL_ERROR;\n");
	            fprintf (src, "  Args* elempairs = new Args[elems.c];\n");
	            fprintf (src, "  for (i=0; i<elems.c; i++)\n");
	            fprintf (src, "  {\n");
	            fprintf (src, "    if (Tcl_SplitList (interp, elems.v[i], &elempairs[i].c, &elempairs[i].v) != TCL_OK)\n");
	            fprintf (src, "    {\n");
	            fprintf (src, "      delete elempairs;\n");
	            fprintf (src, "      return TCL_ERROR;\n");
	            fprintf (src, "    }\n");
	            fprintf (src, "    if (elempairs[i].c != 2)\n");
	            fprintf (src, "    {\n");
	            fprintf (src, "      sprintf (interp->result, \"syntax error in element #%%d: expected a pair, but it's got %%d element(s)\", i, elempairs[i].c);\n");
	            fprintf (src, "      delete elempairs;\n");
	            fprintf (src, "      return TCL_ERROR;\n");
	            fprintf (src, "    }\n");
	            fprintf (src, "  }\n");
	            fprintf (src, "\n");
	            fprintf (src, "  for (const AsnSetMemberDesc *m=_mdescs; m->name; m++)\n");
	            fprintf (src, "  {\n");
	            fprintf (src, "    int count = 0;\n");
	            fprintf (src, "    for (i=0; i<elems.c; i++)\n");
	            fprintf (src, "      if (!strcmp (elempairs[i].v[0], m->name))\n");
	            fprintf (src, "        count++;\n");
	            fprintf (src, "    if (count > 1)\n");
	            fprintf (src, "    {\n");
	            fprintf (src, "      sprintf (interp->result, \"duplicate value for member \\\"%%s\\\" in list\", m->name);\n");
	            fprintf (src, "      Tcl_SetErrorCode (interp, \"SNACC\", \"DUPMEMB\", NULL);\n");
	            fprintf (src, "      delete elempairs;\n");
	            fprintf (src, "      return TCL_ERROR;\n");
	            fprintf (src, "    }\n");
	            fprintf (src, "    if (!m->optional && count < 1)\n");
	            fprintf (src, "    {\n");
	            fprintf (src, "      sprintf (interp->result, \"mandatory member \\\"%%s\\\" is missing in list\", m->name);\n");
	            fprintf (src, "      Tcl_SetErrorCode (interp, \"SNACC\", \"MISSMAND\", NULL);\n");
	            fprintf (src, "      delete elempairs;\n");
	            fprintf (src, "      return TCL_ERROR;\n");
	            fprintf (src, "    }\n");
	            fprintf (src, "    \n");
	            fprintf (src, "  }\n");
	            fprintf (src, "\n");
	            fprintf (src, "  for (i=0; i<elems.c; i++)\n");
	            fprintf (src, "  {\n");
	            FOR_EACH_LIST_ELMT (e, set->basicType->a.set)
	            {
		        fprintf (src, "    if (!strcmp (elempairs[i].v[0], \"%s\"))\n", e->type->cxxTypeRefInfo->fieldName);
		        fprintf (src, "    {\n");
		        if (e->type->cxxTypeRefInfo->isPtr)
		        {
		            fprintf (src, "      if (!%s)\n", e->type->cxxTypeRefInfo->fieldName);
                    fprintf (src, "        %s = new %s;\n", e->type->cxxTypeRefInfo->fieldName, e->type->cxxTypeRefInfo->className);
		            fprintf (src, "      if (%s->TclSetVal (interp, elempairs[i].v[1]))\n", e->type->cxxTypeRefInfo->fieldName);
		            fprintf (src, "      {\n");
		            fprintf (src, "        delete elempairs;\n");
		            fprintf (src, "        return TCL_ERROR;\n");
		            fprintf (src, "      }\n");
		        }
		        else
		        {
		            fprintf (src, "      if (%s.TclSetVal (interp, elempairs[i].v[1]))\n", e->type->cxxTypeRefInfo->fieldName);
		            fprintf (src, "      {\n");
		            fprintf (src, "        return TCL_ERROR;\n");
		            fprintf (src, "        delete elempairs;\n");
		            fprintf (src, "      }\n");
		        }
		        fprintf (src, "    }\n");
	            }
	            fprintf (src, "  }\n");
	            fprintf (src, "\n");
	            fprintf (src, "  // look for unmentioned optional members and delete them:\n");
	            FOR_EACH_LIST_ELMT (e, set->basicType->a.set)
	            {
		        if (e->type->optional || e->type->defaultVal)
		        {
		            fprintf (src, "  {\n");
		            fprintf (src, "    bool present = false;\n");
		            fprintf (src, "    for (i=0; i<elems.c; i++)\n");
		            fprintf (src, "      if (!strcmp (elempairs[i].v[0], \"%s\"))\n", e->type->cxxTypeRefInfo->fieldName);
		            fprintf (src, "        present = true;\n");
		            fprintf (src, "    if (!present)\n");
		            fprintf (src, "    {\n");
		            fprintf (src, "      delete %s;\n", e->type->cxxTypeRefInfo->fieldName);
		            fprintf (src, "      %s = NULL;\n", e->type->cxxTypeRefInfo->fieldName);
		            fprintf (src, "    }\n");
		            fprintf (src, "  }\n");
		        }
	            }
	            fprintf (src, "\n");
	            fprintf (src, "  delete elempairs;\n");
	            fprintf (src, "  return TCL_OK;\n");
	            fprintf (src, "}\n\n");

	            fprintf (src, "int %s::TclUnsetVal (Tcl_Interp *interp, const char *membernames)\n", td->cxxTypeDefInfo->className);
	            fprintf (src, "{\n");
	            fprintf (src, "  Args elems;\n");
	            fprintf (src, "  if (Tcl_SplitList (interp, (char*)membernames, &elems.c, &elems.v) != TCL_OK)\n");
	            fprintf (src, "    return TCL_ERROR;\n");
	            fprintf (src, "\n");
	            fprintf (src, "  for (int i=0; i<elems.c; i++)\n");
	            fprintf (src, "  {\n");
	            FOR_EACH_LIST_ELMT (e, set->basicType->a.set)
	            {
		        fprintf (src, "    if (!strcmp (elems.v[i], \"%s\"))\n", e->type->cxxTypeRefInfo->fieldName);
		        fprintf (src, "    {\n");
		        if (e->type->optional || e->type->defaultVal)
		        {
		            fprintf (src, "      delete %s;\n", e->type->cxxTypeRefInfo->fieldName);
		            fprintf (src, "      %s = NULL;\n", e->type->cxxTypeRefInfo->fieldName);
		        }
		        else
		        {
		            fprintf (src, "      return _desc.mandatmemberr (interp, elems.v[i]);\n");
		        }
		        fprintf (src, "    }\n");
	            }
	            fprintf (src, "  }\n");
	            fprintf (src, "  return TCL_OK;\n");
	            fprintf (src, "}\n\n");

	            fprintf (hdr, "#endif // TCL\n");
	            fprintf (src, "#endif // TCL\n\n");
	        }
        #endif /* TCL */
        fprintf (hdr, "#endif // META\n\n");
        fprintf (src, "#endif // META\n\n");
    #endif /* META */
}
/* EOF gen-code.c (for back-ends/c++-gen) */

