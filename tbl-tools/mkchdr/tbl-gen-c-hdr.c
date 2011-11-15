/*
 * file: .../tbl-tools/mkchdr/tbl_gen_c_hdr.c - prints C type defs for
 *	vals tbl decoder will return for the given type table.
 *
 *
 * Mike Sample
 *
 * Copyright (C) 1993 Michael Sample
 *            and the University of British Columbia
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program and the associated libraries are distributed in the hope
 * that they will be useful, but WITHOUT ANY WARRANTY; without even the
 * implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 * PURPOSE.  See the GNU General Public License and GNU Library General
 * Public License for more details.
 *
 * $Header: /baseline/SNACC/tbl-tools/mkchdr/tbl-gen-c-hdr.c,v 1.2 2003/12/17 19:05:04 gronej Exp $
 * $Log: tbl-gen-c-hdr.c,v $
 * Revision 1.2  2003/12/17 19:05:04  gronej
 * SNACC baseline merged with PER v1_7 tag
 *
 * Revision 1.1.2.1  2003/11/05 14:59:00  gronej
 * working PER code merged with esnacc_1_6
 *
 * Revision 1.1.1.1  2000/08/21 20:35:48  leonberp
 * First CVS Version of SNACC.
 *
 * Revision 1.1  1997/02/15 19:05:31  rj
 * first check-in
 *
 */


#include "tbl-incl.h"

static int indentIncrG = 4;



void TblPrintCType PROTO ((TBLTypeDef *tblTd, TBLType *tblT, FILE *f, unsigned short int indent));

static char *genericFieldNameG = "field";

/*
 * Print C type info for the values that the tbl decode will return (and
 * tbl encoder will expect) for the given tbl
 */
void
TblPrintCHdr PARAMS ((tbl, f),
    TBL *tbl _AND_
    FILE *f)
{
    TBLTypeDef *tblTd;
    TBLModule *tblMod;

    /* print file comment */
    fprintf (f,"/*\n");
    fprintf (f," *  type definitions for the values that the snacc table\n");
    fprintf (f," *  decoder will return and snacc table encoder expects\n");
    fprintf (f," *  for the type table that contains the following modules:\n");
    fprintf (f," *  \n");
    FOR_EACH_LIST_ELMT (tblMod, tbl->modules)
    {
        if (!tblMod->isUseful)
        {
            if (tblMod->name.octs != NULL)
                fprintf (f," *      %s\n",tblMod->name.octs);
            else
            {
                fprintf (f,"      <no name> id = ");
                PrintAsnOid (f, &tblMod->id, 0);
                fprintf (f,"\n");
            }
        }
    }
    fprintf (f," *\n");
    fprintf (f," *  UBC snacc written by Mike Sample\n");
    fprintf (f," *  NOTE: This is a machine generated file \n");
    fprintf (f," *  NOTE2: Table routines don't use this so feel free to make changes\n");
    fprintf (f," *       that do not affect the structure. Changing field \n");
    fprintf (f," *       and type names is fine.\n");
    fprintf (f," */\n\n\n");

    /* print C type defs */
    FOR_EACH_LIST_ELMT (tblMod, tbl->modules)
    {
        if (!tblMod->isUseful) /* don't print useful types */
        {
            fprintf (f,"/* start of module %s's definitions */\n\n", tblMod->name.octs);
            FOR_EACH_LIST_ELMT (tblTd, tblMod->typeDefs)
            {
                fprintf (f,"typedef ");
                TblPrintCType (tblTd, tblTd->type, f, 0);
                fprintf (f," %s;\n\n", tblTd->typeName.octs);
            }
        }
    }
}  /* TblPrintCHdr */


/*
 * simple routine to print the C version of the type
 * This routine only indents directly after printing a newline
 * (ie no indent is done for the first text written)
 */
void
TblPrintCType PARAMS ((tblTd, tblT, f, indent),
    TBLTypeDef *tblTd _AND_
    TBLType *tblT _AND_
    FILE *f _AND_
    unsigned short int indent)
{
    TBLType *structElmtType;
    TBLType *choiceElmtType;
    void *tmp;
    int currElmt;
    int currFieldNo;


    switch (tblT->typeId)
    {
      case TBL_TYPEREF:
          fprintf (f, "%s",tblT->content->a.typeRef->typeDefPtr->typeName.octs);
          break;

      case TBL_SEQUENCE:
      case TBL_SET:
          fprintf (f,"struct %s\n", tblTd->typeName.octs);
          Indent (f, indent);
          fprintf (f,"{\n");
          currFieldNo = 0;
          tmp = CURR_LIST_NODE (tblT->content->a.elmts);
          FOR_EACH_LIST_ELMT (structElmtType, tblT->content->a.elmts)
          {
              Indent (f, indent+indentIncrG);
              TblPrintCType (tblTd, structElmtType, f, indent+indentIncrG);
              if (structElmtType->fieldName.octs != NULL)
                  fprintf (f," %s;", structElmtType->fieldName.octs);
              else
                  fprintf (f," %s%d;", genericFieldNameG, currFieldNo++);

              fprintf (f,"\n");
          }

          SET_CURR_LIST_NODE (tblT->content->a.elmts, tmp);
          fprintf (f,"}");
          break;

      case TBL_SEQUENCEOF:
      case TBL_SETOF:
          fprintf (f,"AsnList");
        break;

      case TBL_CHOICE:
          fprintf (f,"struct %s\n", tblTd->typeName.octs);
          Indent (f, indent);
          fprintf (f,"{\n");
          /* print enum */
          Indent (f, indent+indentIncrG);
          fprintf (f,"enum\n");
          Indent (f, indent+indentIncrG);
          fprintf (f,"{\n");
          currElmt = 0;
          currFieldNo = 0;
          tmp = CURR_LIST_NODE (tblT->content->a.elmts);
          FOR_EACH_LIST_ELMT (choiceElmtType, tblT->content->a.elmts)
          {
              Indent (f, indent+indentIncrG+indentIncrG);
              if (choiceElmtType->fieldName.octs != NULL)
                  fprintf (f,"%s", choiceElmtType->fieldName.octs);
              else
                  fprintf (f," %s%d", genericFieldNameG, currFieldNo++);
              fprintf (f,"_ID = %d", currElmt++);
              if (choiceElmtType != LAST_LIST_ELMT (tblT->content->a.elmts))
                  fprintf (f,",");
              fprintf (f,"\n");
          }
          Indent (f, indent);
          fprintf (f,"    } choiceId;\n");

          /* print elmt union */
          Indent (f, indent+indentIncrG);
          fprintf (f,"union\n");
          Indent (f, indent+indentIncrG);
          fprintf (f,"{\n");
          currFieldNo = 0;
          FOR_EACH_LIST_ELMT (choiceElmtType, tblT->content->a.elmts)
          {
              Indent (f, indent+indentIncrG+indentIncrG);
              TblPrintCType (tblTd, choiceElmtType, f, indent+indentIncrG+indentIncrG);
              if (choiceElmtType->fieldName.octs != NULL)
                  fprintf (f," %s;\n", choiceElmtType->fieldName.octs);
              else
                  fprintf (f," %s%d;", genericFieldNameG, currFieldNo++);
          }
          SET_CURR_LIST_NODE (tblT->content->a.elmts, tmp);
          Indent (f, indent+indentIncrG);
          fprintf (f,"} a;\n");
          Indent (f, indent);
          fprintf (f,"}");
        break;

      case TBL_BOOLEAN:
          fprintf (f,"AsnBool");
        break;

      case TBL_INTEGER:
      case TBL_ENUMERATED:
          fprintf (f,"AsnInt");
        break;

      case TBL_BITSTRING:
          fprintf (f,"AsnBits");
        break;

      case TBL_OCTETSTRING:
          fprintf (f,"AsnOcts");
        break;

      case TBL_NULL:
          fprintf (f,"AsnNull");
        break;

      case TBL_OID:
          fprintf (f,"AsnOid");
        break;

      case TBL_REAL:
          fprintf (f,"AsnReal");
        break;

      default:
         fprintf (f, "<ERROR - unknown type!>");
    }

    /*
     * types defined directly from another typedef are
     * the same type as the other typedef (so not a pointer).
     * All other type refs are done by pointer
     */
    if (tblT != tblTd->type)
        fprintf (f,"*");


}  /* TblPrintCType */
