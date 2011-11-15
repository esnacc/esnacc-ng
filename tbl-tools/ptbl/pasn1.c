/*
 * file: .../tbl-tools/ptbl/pasn1.c - this takes a table and prints a version of
 *           is contents in a rough ASN.1 format.  Its
 *           rough - ie IMPORT/EXPORT info is not re-computed and printed.
 *           Its much easier to understand than the
 *           printout of the table's actual values
 *
 * MS 93
 * Copyright (C) 1993 Michael Sample
 *            and the University of British Columbia
 *
 * $Header: /baseline/SNACC/tbl-tools/ptbl/pasn1.c,v 1.2 2003/12/17 19:05:04 gronej Exp $
 * $Log: pasn1.c,v $
 * Revision 1.2  2003/12/17 19:05:04  gronej
 * SNACC baseline merged with PER v1_7 tag
 *
 * Revision 1.1.2.1  2003/11/05 14:59:00  gronej
 * working PER code merged with esnacc_1_6
 *
 * Revision 1.1.1.1  2000/08/21 20:35:48  leonberp
 * First CVS Version of SNACC.
 *
 * Revision 1.3  1997/06/19 09:17:18  wan
 * Added isPdu flag to tables. Added value range checks during parsing.
 *
 * Revision 1.2  1997/05/07 15:18:36  wan
 * Added (limited) size constraints, bitstring and enumeration names to tables
 *
 * Revision 1.1  1997/02/15 19:26:21  rj
 * first check-in
 *
 */


#include "tbl-incl.h"
#include "pasn1.h"


static void PrintElmts PROTO ((FILE *f, TBL *tbl, TBLTypeDef *td, AsnList *elmts));
static void PrintTag PROTO ((FILE *f, TBLTag *tag));
int UnivTagCodeMatch PROTO ((TBLTypeId tid, TBLTagClass tclass));


void
PrintTblInAsn1 PARAMS ((f, tbl),
    FILE *f _AND_
    TBL *tbl)
{
    TBLModule *m;
    TBLTypeDef *td;

    /* print internal info */
    fprintf (f,"--\n");
    fprintf (f,"-- This printout contains the following modules:\n");
    FOR_EACH_LIST_ELMT (m, tbl->modules)
    {
        fprintf (f,"--   %s \n", m->name.octs);
    }

    fprintf (f,"--\n\n");

    /* print each module */
    FOR_EACH_LIST_ELMT (m, tbl->modules)
    {
        fprintf (f,"--  Definitions for  %s \n", m->name.octs);
        fprintf (f,"%s ",  m->name.octs);
        if (m->id.octs != NULL)
            PrintAsnOid (f, &m->id, 0);
        fprintf (f," DEFINITIONS ::=\n");
        fprintf (f,"BEGIN\n");

        /* print this module's type defs */
        FOR_EACH_LIST_ELMT (td, m->typeDefs)
        {
            PrintTblTypeDefInAsn1 (f, tbl, td);
        }

        fprintf (f,"END\n");
    }
} /* PrintTblInAsn1 */


void
PrintTblTypeDefInAsn1 PARAMS ((f, tbl, td),
    FILE *f _AND_
    TBL *tbl _AND_
    TBLTypeDef *td)
{
    /*
     * I don't think there is a case where no
     * typename is defined for the typedef.
     * (I marked it optional at one point, so ...)
     */
    if (td->typeName.octs != NULL)
        fprintf (f,"%s ::= ", td->typeName.octs);
    else
        fprintf (f,"??? ::= ");
    
    if (td->isPdu)
	fprintf (f,"--snacc isPdu:\"TRUE\" -- ");

    PrintTblTypeInAsn1 (f, tbl, td, td->type);
    fprintf (f,"\n");

}  /* PrintTblTypeDefInAsn1 */

void
PrintTblRange PARAMS ((f, r, s1, s2),
    FILE* f _AND_
    TBLRange* r _AND_
    char* s1 _AND_
    char* s2)
{
    if (r)
    {
	fprintf(f," %s",s1);
	fprintf(f,r->from==r->to?"(%d)":"(%d..%d)",r->from,r->to);
	fprintf(f,"%s",s2);
    }
}

TBLTypeId
RealTypeId PARAMS ((t),
    TBLType* t)
{
    switch (t->typeId)
    {
    case TBL_TYPEREF:
	return RealTypeId (t->content->a.typeRef->typeDefPtr->type);
    default:
	return t->typeId;
    }
}

/*
 * prints "{ field list}" for the given elmts
 * good for SET, SEQ and CHOICE types
 */
static void
PrintTblValues PARAMS ((f, tnnl),
    FILE *f _AND_
    TBLNamedNumberList *tnnl)
{
    TBLNamedNumber *tnn;
    if (!tnnl || LIST_EMPTY(tnnl))
	return;

    fprintf (f,"\n{\n");
    FOR_EACH_LIST_ELMT (tnn, tnnl)
    {
        if (tnn->name.octs != NULL)
            fprintf (f,"    %s ", tnn->name.octs);
        else
            fprintf (f, "    ");
        fprintf(f,"(%d)",tnn->value);
        if (tnn != LAST_LIST_ELMT (tnnl))
            fprintf (f,",\n");
        else
            fprintf (f,"\n");
    }
    fprintf (f,"}");

} /* PrintTblValues */

void
PrintTblTypeInAsn1 PARAMS ((f, tbl, td, t),
    FILE *f _AND_
    TBL *tbl _AND_
    TBLTypeDef *td _AND_
    TBLType *t)
{
    TBLTag *tag;
    TBLTag *lastTag;
    TBLType *elmt;

    /* print tags */
    FOR_EACH_LIST_ELMT (tag, t->tagList)
    {
        /* don't print the tag if it the last one and it has UNIVERSAL class */
        if (!((tag == LAST_LIST_ELMT (t->tagList)) && (tag->tclass == UNIV)))
            PrintTag (f, tag);
    }


    /* figure out if IMPLICIT tagging has been done */
    if (t->tagList != NULL)
        lastTag = LAST_LIST_ELMT (t->tagList);
    else
        lastTag = NULL;

    if (lastTag != NULL)
    {
        if (t->typeId == TBL_TYPEREF)
        {
            if (lastTag->tclass == UNIV) /* weird case - user using univ tags*/
                PrintTag (f, lastTag);

            if (t->content->a.typeRef->implicit)
                fprintf (f,"IMPLICIT ");
        }

        /*
         * non univ tag on builtin/non ref type means orig univ tag
         * has been removed due to implicit tagging
         */
        else if (lastTag->tclass != UNIV)
            fprintf (f,"IMPLICIT ");

        /*
         * last tag is universal - it should match the type then
         * if not, then implicit tagging with universal tags
         * has been done - like useful types
         * ie NumericString ::= [UNIVERSAL 18] IMPLICT OCTET STRING
         */
        else if (!UnivTagCodeMatch (t->typeId, lastTag->code))
        {
            /*
             * last tag was universal but did not match
             * so must print univsal tag (not printed
             * in above tag printing code) as well
             * as the implicit
             */
            PrintTag (f, lastTag);
            fprintf (f,"IMPLICIT ");
        }
    }


    /* print type info */
    switch (t->typeId)
    {
        case TBL_BOOLEAN:
            fprintf (f,"BOOLEAN");
            break;

        case TBL_INTEGER:
            fprintf (f,"INTEGER");
	    PrintTblRange(f,t->constraint,"","");
            break;

        case TBL_BITSTRING:
            fprintf (f,"BIT STRING");
	    PrintTblValues(f,t->values);
            break;

        case TBL_OCTETSTRING:
            fprintf (f,"OCTET STRING");
	    PrintTblRange(f,t->constraint,"(SIZE",")");
            break;

        case TBL_NULL:
            fprintf (f,"NULL");
            break;

        case TBL_OID:
            fprintf (f,"OBJECT IDENTIFIER");
            break;

        case TBL_REAL:
            fprintf (f,"REAL");
            break;

        case TBL_ENUMERATED:
            fprintf (f,"ENUMERATED");
	    PrintTblValues(f,t->values);
            break;

        case TBL_SEQUENCE:
            fprintf (f,"SEQUENCE\n");
            PrintElmts (f, tbl, td, t->content->a.elmts);
            break;

        case TBL_SET:
            fprintf (f,"SET\n");
            PrintElmts (f, tbl, td, t->content->a.elmts);
            break;

        case TBL_SEQUENCEOF:
            fprintf (f,"SEQUENCE");
	    PrintTblRange(f,t->constraint,"SIZE","");
            fprintf (f," OF ");
            PrintTblTypeInAsn1 (f, tbl, td, FIRST_LIST_ELMT (t->content->a.elmts));
            break;

        case TBL_SETOF:
            fprintf (f,"SET OF ");
            PrintTblTypeInAsn1 (f, tbl, td, FIRST_LIST_ELMT (t->content->a.elmts));
            break;

        case TBL_CHOICE:
            fprintf (f,"CHOICE\n");
            PrintElmts (f, tbl, td, t->content->a.elmts);
            break;

        case TBL_TYPEREF:
            fprintf (f,"%s", t->content->a.typeRef->typeDefPtr->typeName.octs);
	    if (RealTypeId(t)==TBL_INTEGER)
		PrintTblRange(f,t->constraint,"","");
	    else
		PrintTblRange(f,t->constraint,"(SIZE",")");
            break;
    }
}  /* PrintTblTypeInAsn1 */





/*
 * prints "{ field list}" for the given elmts
 * good for SET, SEQ and CHOICE types
 */
static void
PrintElmts PARAMS ((f, tbl, td, elmts),
    FILE *f _AND_
    TBL *tbl _AND_
    TBLTypeDef *td _AND_
    AsnList *elmts)
{
    TBLType *elmt;

    fprintf (f,"{\n");
    FOR_EACH_LIST_ELMT (elmt, elmts)
    {
        if (elmt->fieldName.octs != NULL)
            fprintf (f,"    %s ", elmt->fieldName.octs);
        else
            fprintf (f, "    ");
        PrintTblTypeInAsn1 (f, tbl, td, elmt);
        if (elmt != LAST_LIST_ELMT (elmts))
            fprintf (f,",\n");
        else
            fprintf (f,"\n");
    }
    fprintf (f,"}");

} /* PrintElmts */


/*
 * returns true if the given tag class, tclass,
 * is the correct UNIVERSAL code for the type
 * with the given tid.
 */
int
UnivTagCodeMatch PARAMS ((tid, tclass),
    TBLTypeId tid _AND_
    TBLTagClass tclass)
{
    int expectedClass;
    switch (tid)
    {
        case TBL_BOOLEAN:
            expectedClass = 1;
            break;

        case TBL_INTEGER:
            expectedClass = 2;
            break;

        case TBL_BITSTRING:
            expectedClass = 3;
            break;

        case TBL_OCTETSTRING:
            expectedClass = 4;
            break;

        case TBL_NULL:
            expectedClass = 5;
            break;

        case TBL_OID:
            expectedClass = 6;
            break;

        case TBL_REAL:
            expectedClass = 9;
            break;

        case TBL_ENUMERATED:
            expectedClass = 10;
            break;

        case TBL_SEQUENCE:
        case TBL_SEQUENCEOF:
            expectedClass = 16;
            break;

        case TBL_SET:
        case TBL_SETOF:
            expectedClass = 17;
            break;

        case TBL_CHOICE:
        case TBL_TYPEREF:
            expectedClass = -1;
            break;
    }

    return tclass == expectedClass;
} /* UnivTagCodeMatch */


static void
PrintTag PARAMS ((f, tag),
    FILE *f _AND_
    TBLTag *tag)
{
    fprintf (f,"[");
    switch (tag->tclass)
    {
        case UNIVERSAL:
            fprintf (f,"UNIVERSAL ");
        break;
        case APPLICATION:
            fprintf (f,"APPLICATION ");
        break;
        case CONTEXT:
            break;
        case PRIVATE:
            fprintf (f,"PRIVATE ");
        break;
    }
    fprintf (f,"%d",tag->code);
    fprintf (f,"] ");
} /* PrintTag */
