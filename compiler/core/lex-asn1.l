/* compiler/core/lex-asn1.l */
/* AUTHOR: Mike Sample */
/* DATE:   91/92 */

/* Copyright (C) 1991, 1992 Michael Sample */
/*               and the University of British Columbia */
/* This program is free software; you can redistribute it and/or modify */
/* it under the terms of the GNU General Public License as published by */
/* the Free Software Foundation; either version 2 of the License, or */
/* (at your option) any later version. */

/* these comments must only be a single line each - lex blows it otherwise */
/* due to this claim, the rcs log is at the end of this file. */
/* $Header: /baseline/SNACC/compiler/core/lex-asn1.l,v 1.14 2004/03/25 19:20:16 gronej Exp $ */

/* This lex spec should compile under either lex or flex. */

/* There arGe three modes to the lexical analyzer, INITIAL, MACRO_DEF, */
/* and BRACE_BAL.  INITIAL is the normal mode. MACRO_DEF is used by   */
/* a lexical tie-in from the yacc code to eat a MACRO DEFINTION as a  */
/* single blk of text.  BRACE_BAL is used to by a lexical tie-in for  */
/* eating values inside { }'s.  */

/* if your ASN.1 source file has control characters that cause      */
/* snacc to choke, use a program like 'tr' to condition them first. */


%{

#include <string.h>
#include <errno.h>
#include "asn-incl.h"
#include "asn1module.h"
#include "exports.h"
#include "y.tab.h"  /* defines the returned token values */
#include "lex-stuff.h"

unsigned long int myLineNoG = 0;

%}

%s COMMENT MACRO_DEF BRACE_BAL SNACC_ATTRIBS

%option noyywrap


WHITESPC	[ \t\n\r]


%%

<INITIAL>"[C]"       return BOXC_SYM;
<INITIAL>"[S]"       return BOXS_SYM;

<INITIAL>{WHITESPC}+ { COUNT_NEWLINES (myLineNoG, yytext);}
<INITIAL>"..."    return ELLIPSIS_SYM;
<INITIAL>"."      return DOT_SYM;
<INITIAL>","      return COMMA_SYM;
<INITIAL>"{"      return LEFTBRACE_SYM;
<INITIAL>"}"      return RIGHTBRACE_SYM;
<INITIAL>"("      return LEFTPAREN_SYM;
<INITIAL>")"      return RIGHTPAREN_SYM;
<INITIAL>"["      return LEFTBRACKET_SYM;
<INITIAL>"]"      return RIGHTBRACKET_SYM;
<INITIAL>"<"      return LESSTHAN_SYM;
<INITIAL>"-"      return MINUS_SYM;
<INITIAL>"::="    return GETS_SYM;
<INITIAL>"|"      return BAR_SYM;
<INITIAL>";"      return SEMI_COLON_SYM;
<INITIAL>":"	  return COLON_SYM;
<INITIAL>TAGS     return TAGS_SYM;
<INITIAL>BOOLEAN  return BOOLEAN_SYM;
<INITIAL>INTEGER  return INTEGER_SYM;
<INITIAL>BIT      return BIT_SYM;
<INITIAL>STRING   return STRING_SYM;
<INITIAL>OCTET    return OCTET_SYM;
<INITIAL>CONTAINING return CONTAINING_SYM;
<INITIAL>ENCODED  return ENCODED_SYM;
<INITIAL>NULL     return NULL_SYM;
<INITIAL>SEQUENCE return SEQUENCE_SYM;
<INITIAL>OF       return OF_SYM;
<INITIAL>SET      return SET_SYM;
<INITIAL>IMPLICIT return IMPLICIT_SYM;
<INITIAL>CHOICE   return CHOICE_SYM;
<INITIAL>ANY      return ANY_SYM;
<INITIAL>OBJECT{WHITESPC}*IDENTIFIER {
      COUNT_NEWLINES (myLineNoG, yytext);
      return OBJECT_IDENTIFIER_SYM;}

<INITIAL>RELATIVE-OID {
	  COUNT_NEWLINES (myLineNoG, yytext);
	  return RELATIVE_OID_SYM;}

<INITIAL>OPTIONAL return OPTIONAL_SYM;
<INITIAL>DEFAULT  return DEFAULT_SYM;
<INITIAL>COMPONENTS return COMPONENTS_SYM;
<INITIAL>UNIVERSAL return UNIVERSAL_SYM;
<INITIAL>APPLICATION return APPLICATION_SYM;
<INITIAL>PRIVATE  return PRIVATE_SYM;
<INITIAL>TRUE     return TRUE_SYM;
<INITIAL>FALSE    return FALSE_SYM;
<INITIAL>BEGIN    return BEGIN_SYM;
<INITIAL>END      return END_SYM;
<INITIAL>DEFINITIONS return DEFINITIONS_SYM;
<INITIAL>EXPLICIT return EXPLICIT_SYM;
<INITIAL>ENUMERATED return ENUMERATED_SYM;
<INITIAL>EXPORTS  return EXPORTS_SYM;
<INITIAL>EXTERNAL return EXTERNAL_SYM;
<INITIAL>IMPORTS  return IMPORTS_SYM;
<INITIAL>REAL     return REAL_SYM;
<INITIAL>INCLUDES return INCLUDES_SYM;
<INITIAL>MIN      return MIN_SYM;
<INITIAL>MAX      return MAX_SYM;
<INITIAL>SIZE     return SIZE_SYM;
<INITIAL>FROM     return FROM_SYM;
<INITIAL>WITH     return WITH_SYM;
<INITIAL>COMPONENT return COMPONENT_SYM;
<INITIAL>PRESENT  return PRESENT_SYM;
<INITIAL>ABSENT   return ABSENT_SYM;
<INITIAL>DEFINED  return DEFINED_SYM;
<INITIAL>BY       return BY_SYM;
<INITIAL>PLUS-INFINITY return PLUS_INFINITY_SYM;
<INITIAL>MINUS-INFINITY return MINUS_INFINITY_SYM;

<INITIAL>NumericString		return NUMERICSTRING_SYM;
<INITIAL>PrintableString	return PRINTABLESTRING_SYM;
<INITIAL>IA5String			return IA5STRING_SYM;
<INITIAL>BMPString			return BMPSTRING_SYM;
<INITIAL>UniversalString	return UNIVERSALSTRING_SYM;
<INITIAL>UTF8String			return UTF8STRING_SYM;
<INITIAL>TeletexString		return TELETEXSTRING_SYM;
<INITIAL>T61String			return T61STRING_SYM;
<INITIAL>VideotexString     return VIDEOTEXSTRING_SYM;
<INITIAL>GraphicString      return GRAPHICSTRING_SYM;
<INITIAL>ISO646String       return ISO646STRING_SYM;
<INITIAL>VisibleString      return VISIBLESTRING_SYM;
<INITIAL>GeneralString      return GENERALSTRING_SYM;
<INITIAL>GeneralizedTime    return GENERALIZEDTIME_SYM;
<INITIAL>UTCTime            return UTCTIME_SYM;
<INITIAL>ObjectDescriptor   return OBJECTDESCRIPTOR_SYM;




<MACRO_DEF>(.|\n) {
    int i;
    char *buf;
    int bufSize;
    int inComment;
    int inStr;
    unsigned int c, c1, c2;

    /*
     * matches any first char, then
     * copies everything until an uncommented,
     * unquoted END.  This Lex state is started
     * from the yacc src (lexical tie in)
     * from the MACRO_DEF production.
     *
     * if you don't like realloc and don't care about
     * macro defs just have this return a constant string
     * like "BEGIN <not parsed> END" after eating the definition
     */

    unput (yytext[0]);

    bufSize = 1024;
    buf = Malloc (1024);

    i = 0;          /* put BEGIN str at beginning */
    buf[i++] = 'B';
    buf[i++] = 'E';
    buf[i++] = 'G';
    buf[i++] = 'I';
    buf[i++] = 'N';
    buf[i++] = '\n';

    inStr = FALSE;
    inComment = FALSE;
    for ( ; ; i++)
    {
        c = input();

        if (i >= (bufSize - 4))
        {
            bufSize += 512;
            buf = (char*) Realloc (buf, bufSize);
        }

        buf[i] = (char)c;

        if ((inComment) && (c == '\n'))
              inComment = FALSE;
        else if (!(inStr) &&  (c == '-'))
        {
            c = input();
            if (c  == '-')
            {
                buf[++i] = (char)c;
                inComment = !inComment;
            }
            else
                unput (c);
        }
        else  if (inComment)
            continue;
        else if (c == '"')
              inStr = !inStr;
        else if (inStr)
            continue;
        else if (c == 'E')
        {
            c1 = input();
            c2 = input();
            if ((c1 == 'N') && (c2 == 'D'))
            {
                buf[++i] = 'N';
                buf[++i] = 'D';
                buf[++i] = '\0';
                yylval.charPtr = buf;
                COUNT_NEWLINES (myLineNoG, buf);
                myLineNoG -=1; /* take off 1 added after "BEGIN" */
                return MACRODEFBODY_SYM;
            }
            else
            {
                unput (c2);
                unput (c1);
            }
        }
    }
   /* not reached */
}



<INITIAL>[A-Z](-[A-Z0-9]|[A-Z0-9])*{WHITESPC}*MACRO {
         int i;
        /* copy and return the Macro's name only */
        /* doesn't handle comments between macro name and MACRO sym */
         for (i = 0; (yytext[i] != ' ') &&
                     (yytext[i] != '\t') &&
                     (yytext[i] != '\n') &&
                     (yytext[i] != '\r'); i++);
         yylval.charPtr = Malloc (i+1);
         strncpy (yylval.charPtr, yytext, i);
         yylval.charPtr[i] = '\0';
         return NAMEDMACRO_SYM; }


<INITIAL>OPERATION        return OPERATION_SYM;
<INITIAL>ARGUMENT         return ARGUMENT_SYM;
<INITIAL>RESULT           return RESULT_SYM;
<INITIAL>ERRORS           return ERRORS_SYM;
<INITIAL>LINKED           return LINKED_SYM;

<INITIAL>ERROR            return ERROR_SYM;
<INITIAL>PARAMETER        return PARAMETER_SYM;

<INITIAL>BIND             return BIND_SYM;
<INITIAL>BIND-ERROR       return BINDERROR_SYM;
<INITIAL>UNBIND           return UNBIND_SYM;
<INITIAL>UNBIND-ERROR     return UNBINDERROR_SYM;

<INITIAL>APPLICATION-CONTEXT          return AC_SYM;
<INITIAL>APPLICATION-SERVICE-ELEMENTS return ASES_SYM;
<INITIAL>REMOTE                       return REMOTE_SYM;
<INITIAL>INITIATOR                    return INITIATOR_SYM;
<INITIAL>RESPONDER                    return RESPONDER_SYM;
<INITIAL>ABSTRACT{WHITESPC}*SYNTAXES {
        COUNT_NEWLINES (myLineNoG, yytext);
        return ABSTRACTSYNTAXES_SYM;}

<INITIAL>APPLICATION-SERVICE-ELEMENT return ASE_SYM;
<INITIAL>OPERATIONS                  return OPERATIONS_SYM;
<INITIAL>CONSUMER{WHITESPC}*INVOKES {
        COUNT_NEWLINES (myLineNoG, yytext);
        return CONSUMERINVOKES_SYM;}

<INITIAL>SUPPLIER{WHITESPC}*INVOKES  {
        COUNT_NEWLINES (myLineNoG, yytext);
        return SUPPLIERINVOKES_SYM;}

<INITIAL>EXTENSION-ATTRIBUTE          return EXTENSIONATTRIBUTE_SYM;
<INITIAL>EXTENSIONS                   return EXTENSIONS_SYM;
<INITIAL>CHOSEN                       return CHOSEN_SYM;

<INITIAL>EXTENSION                   return EXTENSION_SYM;
<INITIAL>CRITICAL                    return CRITICAL_SYM;
<INITIAL>FOR                         return FOR_SYM;
<INITIAL>SUBMISSION                  return SUBMISSION_SYM;
<INITIAL>DELIVERY                    return DELIVERY_SYM;
<INITIAL>TRANSFER                    return TRANSFER_SYM;

<INITIAL>OBJECT                  return OBJECT_SYM;
<INITIAL>PORTS                   return PORTS_SYM;

<INITIAL>PORT                      return PORT_SYM;
<INITIAL>ABSTRACT{WHITESPC}*OPERATIONS  {
       COUNT_NEWLINES (myLineNoG, yytext);
       return ABSTRACTOPS_SYM;}


<INITIAL>REFINE                    return REFINE_SYM;
<INITIAL>AS                        return AS_SYM;
<INITIAL>RECURRING                 return RECURRING_SYM;
<INITIAL>VISIBLE                   return VISIBLE_SYM;
<INITIAL>PAIRED                    return PAIRED_SYM;

<INITIAL>ABSTRACT-BIND             return ABSTRACTBIND_SYM;
<INITIAL>TO                        return TO_SYM;

<INITIAL>ABSTRACT-UNBIND           return ABSTRACTUNBIND_SYM;

<INITIAL>ABSTRACT-ERROR            return ABSTRACTERROR_SYM;

<INITIAL>ABSTRACT-OPERATION        return ABSTRACTOPERATION_SYM;

<INITIAL>TOKEN                     return TOKEN_SYM;

<INITIAL>TOKEN-DATA                return TOKENDATA_SYM;

<INITIAL>SECURITY-CATEGORY         return SECURITYCATEGORY_SYM;

<INITIAL>ALGORITHM         return ALGORITHM_SYM;
<INITIAL>ENCRYPTED         return ENCRYPTED_SYM;
<INITIAL>SIGNED            return SIGNED_SYM;
<INITIAL>SIGNATURE         return SIGNATURE_SYM;
<INITIAL>PROTECTED         return PROTECTED_SYM;

<INITIAL>OBJECT-TYPE       return OBJECTTYPE_SYM;
<INITIAL>SYNTAX            return SYNTAX_SYM;
<INITIAL>ACCESS            return ACCESS_SYM;
<INITIAL>STATUS            return STATUS_SYM;
<INITIAL>DESCRIPTION       return DESCRIPTION_SYM;
<INITIAL>REFERENCE         return REFERENCE_SYM;
<INITIAL>INDEX             return INDEX_SYM;
<INITIAL>DEFVAL            return DEFVAL_SYM;



<BRACE_BAL>(.|\n) {
    int i;
    char *buf;
    int bufSize;
    int inComment;
    int inStr;
    int braceDepth;
    char c;

    /*
     * matches any first char, then
     * copies everything until an ending "}"
     * Assumes that initially parsed a "{"
     * and puts one at beg. of returned string
     */
    unput (yytext[0]);

    bufSize = 256;
    buf = Malloc (256);

    i = 0;          /* put openning brace at beginning */
    buf[i++] = '{';
    buf[i++] = ' ';

    inStr = FALSE;
    inComment = FALSE;
    braceDepth = 1;
    for ( ; ; i++)
    {
        c = (char)input();

        if (i >= (bufSize - 2))
        {
            bufSize += 256;
            buf = (char*) Realloc (buf, bufSize);
        }

        buf[i] = c;

        if ((inComment) && (c == '\n'))
              inComment = FALSE;
        else if (!(inStr) &&  (c == '-'))
        {
            c = (char)input();
            if (c  == '-')
            {
                buf[++i] = c;
                inComment = !inComment;
            }
            else
                unput (c);
        }
        else  if (inComment)
            continue;
        else if (c == '"')
              inStr = !inStr;
        else if (inStr)
            continue;
        else if (c == '{')
            braceDepth++;
        else if (c == '}')
        {
            braceDepth--;
            if (braceDepth == 0)
            {
                buf[++i] = '\0';
                yylval.charPtr = buf;
                COUNT_NEWLINES (myLineNoG, buf);
                return BRACEBAL_SYM;
            }
        }
    }
   /* not reached */
}



<INITIAL>\'[0-1 \t\r\v\f\n]*\'B {
          COUNT_NEWLINES (myLineNoG, yytext);
          yylval.charPtr = (char*)Malloc (yyleng);
          strncpy (yylval.charPtr, yytext+1, yyleng -1);  /* strip "'"s */
          yylval.charPtr[yyleng-2] = '\0';
          return BSTRING_SYM;}

<INITIAL>\'[0-9A-Fa-f \r\t\v\f\n]*\'H {
          COUNT_NEWLINES (myLineNoG, yytext);
          yylval.charPtr = (char*)Malloc (yyleng);
          strncpy (yylval.charPtr, yytext+1, yyleng -1);   /* strip "'"s */
          yylval.charPtr[yyleng-2] = '\0';
          return HSTRING_SYM;}

<INITIAL>\"([^\"]|"\"\"")*\"  {
          COUNT_NEWLINES (myLineNoG, yytext);
          yylval.charPtr = (char*)Malloc (yyleng);
          strncpy (yylval.charPtr, yytext+1, yyleng -1);       /* strip '"'s */
          yylval.charPtr[yyleng-2] = '\0'; /* 2 quotes == quote in a quote */
          return CSTRING_SYM;}

<INITIAL>[A-Z][a-zA-Z0-9]*([-][a-zA-Z0-9]+)* {
          yylval.charPtr = (char*)Malloc (yyleng+1);
          strcpy (yylval.charPtr, yytext);
          yylval.charPtr[yyleng] = '\0';
          return UCASEFIRST_IDENT_SYM;}


<INITIAL>[a-z][a-zA-Z0-9]*([-][a-zA-Z0-9]+)* {
          yylval.charPtr = (char*)Malloc (yyleng+1);
          strcpy (yylval.charPtr, yytext);
          yylval.charPtr[yyleng] = '\0';
          return LCASEFIRST_IDENT_SYM;}

<INITIAL>[1-9][0-9]* { /*first digit cannot be zero on multi-digit #'s*/
	  errno = 0;
	  {
	      unsigned long ul = (unsigned long) strtol(yytext,NULL,10);
	      if (!errno && ul>(unsigned long)0xFFFFFFFF) {
		  errno = ERANGE;
	      }
	      if (!errno) {
		  yylval.uintVal = (unsigned int) ul;
		  return NUMBER_SYM;
	      }
	  }
          yylval.charPtr = (char*)Malloc (yyleng+1);
          strcpy (yylval.charPtr, yytext);
          yylval.charPtr[yyleng] = '\0';
          return NUMBER_ERANGE;}

<INITIAL>0 {  /*allow zero as first digit on single digit #'s*/
          yylval.uintVal = 0;
          return NUMBER_SYM;}

<INITIAL>"--"			BEGIN(COMMENT);

<INITIAL>"--snacc"		BEGIN(SNACC_ATTRIBS);

 /* Bob's fix
<INITIAL>"--snacc namespace"(-[^-\n]|[^\-\n])*("--"|\n) { 
    char *ptr1, *ptr2;
    printf("RWC --snacc namespace found\n");
        COUNT_NEWLINES (myLineNoG, yytext);
        yylval.charPtr = (char*)Malloc (yyleng-4);
        
        ptr1 = strchr(yytext+7+10, '"');
        ptr2 = strchr(ptr1+1, '"');
        if (ptr1 && ptr2)
        {
            ptr1++;
            strncpy (yylval.charPtr, ptr1, ptr2-ptr1);
            modulePtrG->namespaceToUse = strdup(yylval.charPtr);
                                
        }
    return ATTRIB_NAMESPACE_SYM; }
 */

 /* OLD SNACC compiler directive parser
<INITIAL>"--snacc"(-[^-\n]|[^\-\n])*("--"|\n) {
         * this must be before the normal comment eater so that snacc attribs
          * are not treated as normal comments
          *
        * eat comments, update line no *
        int len;
        COUNT_NEWLINES (myLineNoG, yytext);
        yylval.charPtr = (char*)Malloc (yyleng-4);
        * skip first "--snacc" in copy to ret val *
        strcpy (yylval.charPtr, yytext + 7);
        len = strlen (yylval.charPtr);
        * strip off newline or -- terminator for comment *
        if (yylval.charPtr[len-1] == '\n')
           yylval.charPtr[len-1] = '\0';
        else
           yylval.charPtr[len-2] = '\0';
        return SNACC_ATTRIBUTES; }

<INITIAL>"--"(-[^\-\n]|[^\-\n])*("--"|\n|"-\n") {
        * eat comments, update line no *
        COUNT_NEWLINES (myLineNoG, yytext);}
 */


<COMMENT>(-[^\-\n]|[^\-\n])*("--"|\n|"-\n")		{
        /* eat comments, update line no */
		COUNT_NEWLINES (myLineNoG, yytext);
		BEGIN(INITIAL);  }


<SNACC_ATTRIBS>{	/* Begin SNACC_ATTRIBS start condition */

asn1TypeId			return ATTRIB_ASN1_TYPE_ID;
cTypeId				return ATTRIB_C_TYPE_ID;
cTypeName			return ATTRIB_C_TYPE_NAME_SYM;
cFieldName			return ATTRIB_C_FIELD_NAME_SYM;
isPdu				return ATTRIB_IS_PDU_SYM;
isPtr				return ATTRIB_IS_PTR_SYM;
isPtrForTypeDef		return ATTRIB_IS_PTR_TYPEDEF_SYM;
isPtrForTypeRef		return ATTRIB_IS_PTR_TYPE_REF_SYM;
isPtrInChoice		return ATTRIB_IS_PTR_IN_CHOICE_SYM;
isPtrForOpt			return ATTRIB_IS_PTR_FOR_OPT_SYM;
optTestRoutineName	return ATTRIB_OPT_TEST_ROUTINE_SYM;
defaultFieldName	return ATTRIB_DEFAULT_FIELD_SYM;
printRoutineName	return ATTRIB_PRINT_ROUTINE_SYM;
encodeRoutineName	return ATTRIB_ENCODE_ROUTINE_SYM;
decodeRoutineName	return ATTRIB_DECODE_ROUTINE_SYM;
freeRoutineName		return ATTRIB_FREE_ROUTINE_SYM;
isEncDec			return ATTRIB_IS_ENC_DEC_SYM;
genTypeDef			return ATTRIB_GEN_TYPEDEF_SYM;
genPrintRoutine		return ATTRIB_GEN_PRINT_ROUTINE_SYM;
genEncodeRoutine	return ATTRIB_GEN_ENCODE_ROUTINE_SYM;
genDecodeRoutine	return ATTRIB_GEN_DECODE_ROUTINE_SYM;
genFreeRoutine		return ATTRIB_GEN_FREE_ROUTINE_SYM;
choiceIdSymbol		return ATTRIB_CHOICE_ID_SYMBOL_SYM;
choiceIdValue		return ATTRIB_CHOICE_ID_VALUE_SYM;
choiceIdEnumName	return ATTRIB_CHOICE_ID_ENUM_NAME_SYM;
choiceIdEnumFieldName	return ATTRIB_CHOICE_ID_ENUM_FIELD_NAME_SYM;
isBigInt			return ATTRIB_IS_BIG_INT_SYM;
namespace			return ATTRIB_NAMESPACE_SYM;

\"TRUE\"		return TRUE_SYM;
\"FALSE\"		return FALSE_SYM;

\"C_CHOICE\"		return C_CHOICE_SYM;
\"C_LIST\"			return C_LIST_SYM;
\"C_ANYDEFINEDBY\"	return C_ANYDEFBY_SYM;
\"C_ANY\"			return C_ANY_SYM;
\"C_LIB\"			return C_LIB_SYM;
\"C_STRUCT\"		return C_STRUCT_SYM;
\"C_TYPEDEF\"		return C_TYPEDEF_SYM;
\"C_TYPEREF\"		return C_TYPEREF_SYM;
\"C_NO_TYPE\"		return C_NO_TYPE_SYM;

\"UNKNOWN\"				return UNKNOWN_SYM;
\"BOOLEAN\"				return BOOLEAN_SYM;
\"INTEGER\"				return INTEGER_SYM;
\"BITSTRING\"			return BITSTRING_SYM;
\"OCTETSTRING\"			return OCTETSTRING_SYM;
\"NULL\"				return NULL_SYM;
\"OID\"					return OBJECT_IDENTIFIER_SYM;
\"RELATIVE-OID\"        return RELATIVE_OID_SYM;
\"REAL\"				return REAL_SYM;
\"ENUMERATED\"			return ENUMERATED_SYM;
\"SEQUENCEOF\"			return SEQUENCE_OF_SYM;
\"SEQUENCE\"			return SEQUENCE_SYM;
\"SETOF\"				return SET_OF_SYM;
\"SET\"					return SET_SYM;
\"CHOICE\"				return CHOICE_SYM;
\"ANYDEFINEDBY\"		return ANY_DEFINED_BY_SYM;
\"ANY\"					return ANY_SYM;
\"LOCALTYPEREF\"		return LOCAL_TYPE_REF_SYM;
\"IMPORTYPEREF\"		return IMPORT_TYPE_REF_SYM;
\"NumericString\"		return NUMERICSTRING_SYM;
\"PrintableString\"		return PRINTABLESTRING_SYM;
\"IA5String\"			return IA5STRING_SYM;
\"BMPString\"			return BMPSTRING_SYM;
\"UniversalString\"		return UNIVERSALSTRING_SYM;
\"UTF8String\"			return UTF8STRING_SYM;
\"TeletexString\"		return TELETEXSTRING_SYM;


\"(0|[1-9][0-9]*)\"	{
						yylval.uintVal = atoi(yytext + 1);
						return NUMBER_SYM; }

\"[^\"\n]*\"	{
					yylval.charPtr = (char*)Malloc (yyleng - 1);
					strncpy (yylval.charPtr, yytext + 1, yyleng - 2);
					yylval.charPtr[yyleng - 2] = '\0';
					return CSTRING_SYM; }

[ \t]*			/* Skip white space */

"--"			{ BEGIN(INITIAL); }

"\n"			{ ++myLineNoG; BEGIN(INITIAL); }

.				return *yytext;		/* Any other character is just returned */

} /* end of SNACC_ATTRIBS start condition */


%%


/*
 * these "LexBegin..." routines are used by yacc for (ack!)
 * lexical tie ins
 */

int
LexBeginMacroDefContext()
{
    BEGIN (MACRO_DEF);
	return 0;
}

int
LexBeginBraceBalContext()
{
    BEGIN (BRACE_BAL);
	return 0;
}

int
LexBeginInitialContext()
{
    BEGIN (INITIAL);
	return 0;
}

/*
 * $Log: lex-asn1.l,v $
 * Revision 1.14  2004/03/25 19:20:16  gronej
 * fixed some linux warnings
 *
 * Revision 1.13  2004/01/29 21:21:45  gronej
 * Took all Init() calls out of Clear() functions in generated code
 *
 * Revision 1.12  2004/01/14 19:07:52  gronej
 * Updated Compiler to accept and process relative-oid's
 *
 * Revision 1.11  2003/12/17 19:05:04  gronej
 * SNACC baseline merged with PER v1_7 tag
 *
 * Revision 1.10.2.4  2003/07/29 14:43:02  nicholar
 * Fixed handling of --snacc compiler directives
 *
 * Revision 1.10.2.3  2003/07/28 11:11:23  colestor
 * Changes to complete handing of the "--snacc namespace" compiler directive.
 * Also, updates to handle ASN.1 constant integer tag designations for C++/C.
 *
 * Revision 1.10.2.2  2003/07/14 21:07:44  nicholar
 * Changed how parser handles --snacc directives.  Added namespace option.
 *
 * Revision 1.10.2.1  2003/07/07 20:55:07  nicholar
 * no message
 *
 * Revision 1.7  2003/04/21 13:04:19  leonberp
 * Fixed typo changed GraphicsString to GraphicString
 *
 * Revision 1.6  2002/10/24 15:13:57  leonberp
 * fixing OCTET STRING CONTAINING
 *
 * Revision 1.5  2002/10/24 14:04:22  leonberp
 * fixing support for OCTET STRING CONTAINING
 *
 * Revision 1.4  2002/10/23 20:03:04  leonberp
 * updated lexer and parser and handle OCTET STRING CONTAINING
 *
 * Revision 1.3  2002/05/15 17:00:59  leonberp
 * added support for new basicTypes to compiler
 *
 * Revision 1.2  2000/10/24 14:54:52  rwc
 * Updated to remove high-level warnings (level 4 on MSVC++) for an easier build.
 * SOME warnings persist due to difficulty in modifying the SNACC compiler to
 * properly build clean source; also some files are built by Lex/Yacc.
 *
 * Revision 1.1.1.1  2000/08/21 20:36:00  leonberp
 * First CVS Version of SNACC.
 *
 * Revision 1.5  1997/08/28 09:46:41  wan
 * Reworked number range checking, only gives warning now.
 *
 * Revision 1.4  1997/06/19 09:17:17  wan
 * Added isPdu flag to tables. Added value range checks during parsing.
 *
 * Revision 1.3  1995/07/25 19:41:30  rj
 * changed `_' to `-' in file names.
 *
 * Revision 1.2  1994/09/01  00:37:12  rj
 * snacc_config.h removed.
 *
 * for a list of changes relative to the 1.1 distribution, please refer to the ChangeLog.
 */
