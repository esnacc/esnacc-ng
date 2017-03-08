/*
 * compiler/back_ends/c_gen/tag_util.c  - utilities for dealing with tags
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
 * $Header: /baseline/SNACC/compiler/back-ends/tag-util.c,v 1.7 2003/07/28 11:13:51 colestor Exp $
 * $Log: tag-util.c,v $
 * Revision 1.7  2003/07/28 11:13:51  colestor
 * Changes to complete handing of the "--snacc namespace" compiler directive.
 * Also, updates to handle ASN.1 constant integer tag designations for C++/C.
 *
 * Revision 1.6  2003/07/07 14:54:24  nicholar
 * Eliminated headers and cleaned up include references
 *
 * Revision 1.5  2002/10/23 10:21:23  mcphersc
 * changed BUF_TYPE to GenBuf
 *
 * Revision 1.4  2002/09/16 16:56:35  mcphersc
 * Fixed warnings
 *
 * Revision 1.3  2002/05/15 14:53:10  leonberp
 * added support for new basicTypes to compiler
 *
 * Revision 1.2  2000/10/24 14:54:43  rwc
 * Updated to remove high-level warnings (level 4 on MSVC++) for an easier build.
 * SOME warnings persist due to difficulty in modifying the SNACC compiler to
 * properly build clean source; also some files are built by Lex/Yacc.
 *
 * Revision 1.1.1.1  2000/08/21 20:36:04  leonberp
 * First CVS Version of SNACC.
 *
 * Revision 1.3  1995/07/25  18:15:28  rj
 * changed `_' to `-' in file names.
 *
 * Revision 1.2  1994/09/01  00:26:07  rj
 * snacc_config.h and other superfluous .h files removed.
 *
 * Revision 1.1  1994/08/28  09:48:39  rj
 * first check-in. for a list of changes to the snacc-1.1 distribution please refer to the ChangeLog.
 *
 */

#include "config.h"
#include <stdio.h>

#include "asn-incl.h"
#include "asn1module.h"
#include "mem.h"
#include "define.h"
#include "lib-types.h"
#include "c-gen/rules.h"
#include "c-gen/type-info.h"
#include "str-util.h"
#include "snacc-util.h"
#include "c-gen/util.h"
#include "tag-util.h"
#include "print.h"
#include <memory.h>
#ifdef WIN32
#pragma  warning( disable : 4706 )  // IGNORE assign w/in conditional expression.
#endif



/*
 * returns the tags for the given type (stops at next type definition).
 * if no tags have been grabbed yet and an untagged CHOICE is encountered,
 * all of the CHOICE's top level tags are returned and the stoleChoiceTags
 * flag is set.  If the type has no tags an empty list is returned, not
 * NULL.
 *
 * ASSUMES: tag list's and implicit flags have been adjusted according
 *          to module level IMPLICIT/EXPLICIT-TAGS and type level
 *          IMPLICIT/EXPLICIT tagging.
 *
 * EXAMPLE:
 *
 *   typeX ::= SEQUENCE                         SomeChoice ::= CHOICE
 *   {                                          {
 *         foo [0] INTEGER,                          [0] INTEGER,
 *         bar SomeChoice,                           [1] BOOLEAN,
 *         bell [1] IMPLICIT BOOLEAN,                [2] IA5String
 *         gumby [2] SomeChoice,                }
           poki SomeOtherChoice
 *   }
 *
 *  SomeOtherChoice ::= [APPLICATION 99] CHOICE { ....}
 *
 *  GetTags (foo's type) -->  CNTX 0, UNIV INTEGER_TAG_CODE   stoleChoiceTags = FALSE
 *  GetTags (bar)        -->  CNTX 0, CNTX 1, CNTX 2   (SomeChoice Elmt's first Tags)
 *                                                      stoleChoiceTags = TRUE
 *  GetTags (bell)       -->  CNTX 1    stoleChoiceTags = FALSE
 *  GetTags (gumby)      -->  CNTX 2    stoleChoiceTags = FALSE
 *  GetTags (poki)       -->  APPLICATION 99    stoleChoiceTags = FALSE
 *
 * MS 92/03/04 Added tag form information
 */
TagList*
GetTags PARAMS ((t, stoleChoiceTags),
    Type *t _AND_
    int *stoleChoiceTags)
{
    Tag *tag;
    TagList *tl;
    TagList *retVal;
    Tag *last;
    Tag  *tagCopy;
    Tag **tagHndl;
    int implicitRef;
    int stoleChoicesAgain;
    NamedType *e;

    tl = t->tags;
    if (tl != NULL)
        AsnListFirst (tl);

    retVal = (TagList*) AsnListNew (sizeof (void*));
    implicitRef = FALSE;
    *stoleChoiceTags = FALSE;

    for (;;) {
         /*
          * go through tag list local to this type if any
          */

        FOR_REST_LIST_ELMT (tag, tl) {
            tagCopy = (Tag*)Malloc (sizeof (Tag));
            memcpy (tagCopy, tag, sizeof (Tag));
            tagHndl = (Tag**)AsnListAppend (retVal);
            *tagHndl = tagCopy;
        }

        /*
         * follow tags of referenced types
         */

        if ((t->basicType->choiceId == BASICTYPE_LOCALTYPEREF) ||
             (t->basicType->choiceId == BASICTYPE_IMPORTTYPEREF)) {
            if (!implicitRef)
                implicitRef = t->implicit;

            if (t->basicType->a.localTypeRef->link == NULL) {
                fprintf(errFileG,
                        "ERROR - unresolved type ref, cannot get tags for decoding>\n");
                break;
            }

            t = t->basicType->a.localTypeRef->link->type;
            tl = t->tags;

            if (tl != NULL) {
                AsnListFirst (tl); /* set curr ptr to first node */
                if ((!LIST_EMPTY (tl)) && implicitRef) {
                    AsnListNext (tl);
                    implicitRef = FALSE;
                }
            }
        } else if ((t->basicType->choiceId == BASICTYPE_CHOICE) &&
                   (LIST_EMPTY (retVal))) {
            /* untagged choice and no tags found yet */

            /*
             * Return list of top level tags from this choice
             * and set "stoleChoiceTags" bool param
             */
            if (implicitRef)
                fprintf(errFileG, "ERROR - IMPLICITLY Tagged CHOICE\n");

            *stoleChoiceTags = TRUE;

            FOR_EACH_LIST_ELMT (e, t->basicType->a.choice) {
                stoleChoicesAgain = FALSE;
                tl = GetTags (e->type, &stoleChoicesAgain);

                if (tl == NULL)
                    break;

                if (e->type->basicType->choiceId == BASICTYPE_EXTENSION)
                    continue;

                AsnListFirst (tl);
                if (stoleChoicesAgain) {
                    FOR_EACH_LIST_ELMT (tag, tl) {
                        tagCopy = (Tag*)Malloc (sizeof (Tag));
                        memcpy (tagCopy, tag, sizeof (Tag));
			               tagHndl = (Tag**)AsnListAppend (retVal);
		               	*tagHndl = tagCopy;
                    }
                } else {
                    tag = (Tag*)FIRST_LIST_ELMT (tl);
                    tagCopy = (Tag*)Malloc (sizeof (Tag));
                    memcpy (tagCopy, tag, sizeof (Tag));
                    tagHndl = (Tag**)AsnListAppend (retVal);
                    *tagHndl = tagCopy;
                }
                FreeTags (tl);
            }
            break;  /* exit for loop */
        } else
            break; /* exit for loop */
    }

    if (!*stoleChoiceTags && (retVal != NULL) && !LIST_EMPTY (retVal)) {
        last = (Tag*)LAST_LIST_ELMT (retVal);
        FOR_EACH_LIST_ELMT (tag, retVal) {
            tag->form = CONS;
        }
        last->form = LIBTYPE_GET_TAG_FORM (GetBuiltinType (t));
    }

    AsnListFirst (retVal);
    return retVal;

}  /* GetTags */


void
FreeTags PARAMS ((tl),
    TagList *tl)
{
    Tag *tag;
    AsnListNode *listNode;
    AsnListNode *ln;

    /* free tags */
    FOR_EACH_LIST_ELMT (tag, tl)
    {
        Free (tag);
    }

    /* free list nodes */
    for (ln = FIRST_LIST_NODE (tl); ln != NULL; )
    {
        listNode = ln;
	ln = ln->next;
        Free (listNode);
    }

    /* free list head */
    Free (tl);

}  /* FreeTags */

/*
 * Returns the number of tags that GetTags would return for
 * the same type.
 */
int
CountTags PARAMS ((t),
    Type *t)
{
    int tagCount;
    Tag *tag;
    TagList *tl;
    int implicitRef;

    tl = t->tags;
    if (tl != NULL)
        AsnListFirst (tl);

    tagCount = 0;
    implicitRef = FALSE;

    for (;;)
    {
         /*
          * go through tag list local to this type if any
          */

        FOR_REST_LIST_ELMT (tag, tl)
        {
	    tagCount++;
        }

        /*
         * follow tags of referenced types
         */

        if ((t->basicType->choiceId == BASICTYPE_LOCALTYPEREF) ||
             (t->basicType->choiceId == BASICTYPE_IMPORTTYPEREF))
        {
            if (!implicitRef)
                implicitRef = t->implicit;


            if (t->basicType->a.localTypeRef->link == NULL)
            {
                fprintf (errFileG,"ERROR - unresolved type ref, cannot get tags for decoding>\n");
                break;
            }
            t = t->basicType->a.localTypeRef->link->type;
            tl = t->tags;

            if (tl != NULL)
            {
                AsnListFirst (tl); /* set curr ptr to first node */
                if ((!LIST_EMPTY (tl)) && implicitRef)
                {
                    AsnListNext (tl);
                    implicitRef = FALSE;
                }
            }

        }
        else
            break;
    }

    return tagCount;

}  /* CountTags */


unsigned long
TagByteLen PARAMS ((tagCode),
    unsigned long tagCode)
{
    unsigned long tagLen;

    if (tagCode < 31)
        tagLen = 1;
    else if (tagCode < 128)
        tagLen = 2;
    else if (tagCode < 16384)
        tagLen = 3;
    else if (tagCode < 2097152)
        tagLen = 4;
    else
        tagLen = 5;

    return tagLen;
} /* TagByteLen */



char*
Class2ClassStr PARAMS ((class),
    int class)
{
    switch (class)
    {
        case UNIV:
            return "UNIV";
            break;

        case APPL:
            return "APPL";
            break;

        case CNTX:
            return "CNTX";
            break;

        case PRIV:
            return "PRIV";
            break;

        default:
            return "UNKNOWN";
            break;
    }
} /* Class2ClassStr */



char*
Form2FormStr PARAMS ((form),
    BER_FORM form)
{
    switch (form)
    {
        case PRIM:
            return "PRIM";
            break;

        case CONS:
            return "CONS";
            break;

        default:
            return "UNKNOWN";
            break;
    }
} /* Form2FormStr */



char*
Code2UnivCodeStr PARAMS ((code),
    BER_UNIV_CODE code)
{
    switch (code)
    {
    case BOOLEAN_TAG_CODE:
        return "BOOLEAN_TAG_CODE";

    case INTEGER_TAG_CODE:
        return "INTEGER_TAG_CODE";

    case BITSTRING_TAG_CODE:
        return "BITSTRING_TAG_CODE";

    case OCTETSTRING_TAG_CODE:
        return "OCTETSTRING_TAG_CODE";

    case NULLTYPE_TAG_CODE:
        return "NULLTYPE_TAG_CODE";

    case OID_TAG_CODE:
        return "OID_TAG_CODE";

    case OD_TAG_CODE:
        return "OD_TAG_CODE";

    case EXTERNAL_TAG_CODE:
        return "EXTERNAL_TAG_CODE";

    case REAL_TAG_CODE:
        return "REAL_TAG_CODE";

    case ENUM_TAG_CODE:
        return "ENUM_TAG_CODE";

    case SEQ_TAG_CODE:
        return "SEQ_TAG_CODE";

    case SET_TAG_CODE:
        return "SET_TAG_CODE";
        
    case NUMERICSTRING_TAG_CODE:
        return "NUMERICSTRING_TAG_CODE";

    case PRINTABLESTRING_TAG_CODE:
        return "PRINTABLESTRING_TAG_CODE";

    case TELETEXSTRING_TAG_CODE:
        return "TELETEXSTRING_TAG_CODE";

    case VIDEOTEXSTRING_TAG_CODE:
        return "VIDEOTEXSTRING_TAG_CODE";

    case IA5STRING_TAG_CODE:
        return "IA5STRING_TAG_CODE";

    case UTCTIME_TAG_CODE:
        return "UTCTIME_TAG_CODE";

    case GENERALIZEDTIME_TAG_CODE:
        return "GENERALIZEDTIME_TAG_CODE";

    case GRAPHICSTRING_TAG_CODE:
        return "GRAPHICSTRING_TAG_CODE";

    case VISIBLESTRING_TAG_CODE:
        return "VISIBLESTRING_TAG_CODE";

    case GENERALSTRING_TAG_CODE:
        return "GENERALSTRING_TAG_CODE";

    case UNIVERSALSTRING_TAG_CODE:
        return "UNIVERSALSTRING_TAG_CODE";

    case BMPSTRING_TAG_CODE:
        return "BMPSTRING_TAG_CODE";

    case UTF8STRING_TAG_CODE:
        return "UTF8STRING_TAG_CODE";

    default:
        { 
            /* if the universal type is not known then just return the
             * unvisersal tag code.  This is useful for defining new types
             * in local modules w/o having to modify the compiler.
             */
            static char retstring[256];
            snacc_snprintf(retstring, 256, "%d", code); 
            return retstring; 
        }
    }
} /* TagId2FormStr */

/* Compare the tags of two types */
int CmpTags PARAMS((a, b),
    Type *a _AND_ 
    Type *b)
{
  TagList *t1;
  TagList *t2;
  Tag *tg1;
  Tag *tg2;
  int stoleChoiceTags;
  unsigned tagBuf1[256];
  unsigned tagBuf2[256];
  int len1;
  int len2;
  int cmp;
  int min;

  t1 = GetTags(a, &stoleChoiceTags);
  t2 = GetTags(b, &stoleChoiceTags);
  len1 = 0;
  len2 = 0;

  /* Get tags for item 1 */
  FOR_EACH_LIST_ELMT(tg1, t1) {
    tagBuf1[len1++] = MAKE_TAG_ID(tg1->tclass, tg1->form, tg1->code);
    if (len1 == 256) {
      /* XXX Can do better than this */
      fprintf(errFileG, "CmpTags: Tag length too long");
      abort();
    }
  }

  /* Get tags for item2 */
  FOR_EACH_LIST_ELMT(tg2, t2) {
    tagBuf2[len2++] = MAKE_TAG_ID(tg2->tclass, tg2->form, tg2->code);
    if (len2 == 256) {
      /* XXX Can do better than this */
      fprintf(errFileG, "CmpTags: Tag length too long");
      abort();
    }
  }

  /* Compare results */
  min = (len1 > len2)?len2:len1;
  cmp = memcmp(tagBuf1, tagBuf2, min * sizeof(unsigned));
  if (cmp != 0) {
    return cmp;
  } else {
    return len1 - len2;
  }
  
}


//
//
char *DetermineCode(Tag *tag, int *ptagLen, int bJustIntegerFlag)
{
    static char retstring[256];
    char *codeStr=NULL;
    int iValue=500;     // WILL indicate a problem on source creation...
    memset(retstring, 0, sizeof(retstring));
    if (tag->valueRef == NULL)
    {
        if (!bJustIntegerFlag)
        {
            codeStr = Code2UnivCodeStr (tag->code);
        }
        else
        {
            sprintf(retstring, "%d", tag->code); 
            codeStr = retstring;
        }       // END IF bJustIntegerFlag
        if (ptagLen) {
            *ptagLen = TagByteLen(tag->code);
        }
    }
    else
    {
        if (tag->valueRef && tag->valueRef->basicValue &&
            tag->valueRef->basicValue->choiceId == BASICVALUE_LOCALVALUEREF &&
            tag->valueRef->basicValue->a.localValueRef &&
            tag->valueRef->basicValue->a.localValueRef->link &&
            tag->valueRef->basicValue->a.localValueRef->link->value &&
            tag->valueRef->basicValue->a.localValueRef->link->value->basicValue)
        {
            if (tag->valueRef->basicValue->a.localValueRef->link->value->basicValue->choiceId == 
                        BASICVALUE_INTEGER)
            {
                iValue = tag->valueRef->basicValue->a.localValueRef->link->
                                                value->basicValue->a.integer;
            }       // IF Integer
            else if (tag->valueRef->basicValue->a.localValueRef->link->value->basicValue->choiceId == 
                        BASICVALUE_LOCALVALUEREF)
            {
                ValueRef *pvalueRef=NULL;
                if (tag->valueRef->basicValue->a.localValueRef->link->value->basicValue->choiceId == BASICVALUE_LOCALVALUEREF)
                {
                    pvalueRef = tag->valueRef->basicValue->a.localValueRef->link->value->basicValue->a.localValueRef;
                    if (pvalueRef->link->value && pvalueRef->link->value->basicValue &&
                        pvalueRef->link->value->basicValue->choiceId == BASICVALUE_INTEGER)
                        iValue = pvalueRef->link->value->basicValue->a.integer;
                }
            }       // END IF Integer/Import
            else
            {
                printf("Tag value type NOT RECOGNIZED; COULD NOT RESOLVE tag integer!\n");
            }
        }
        else if (tag->valueRef->basicValue->choiceId == BASICVALUE_IMPORTVALUEREF &&
            tag->valueRef->basicValue->a.importValueRef &&
            tag->valueRef->basicValue->a.importValueRef->link &&
            tag->valueRef->basicValue->a.importValueRef->link->value &&
            tag->valueRef->basicValue->a.importValueRef->link->value->
                   basicValue && 
            tag->valueRef->basicValue->a.importValueRef->link->value->
                   basicValue->choiceId == BASICVALUE_INTEGER)
        {
            iValue = tag->valueRef->basicValue->a.importValueRef->link->
                        value->basicValue->a.integer;
        }
        sprintf(retstring, "%d", iValue); 
        codeStr = retstring;
        if (ptagLen) {
            *ptagLen = TagByteLen(iValue);
        }
    }
    return(codeStr);
}

