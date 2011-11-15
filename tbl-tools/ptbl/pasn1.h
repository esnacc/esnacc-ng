/*
 * file: .../tbl-tools/ptbl/pasn1.h
 *
 * $Header: /baseline/SNACC/tbl-tools/ptbl/pasn1.h,v 1.2 2003/12/17 19:05:04 gronej Exp $
 * $Log: pasn1.h,v $
 * Revision 1.2  2003/12/17 19:05:04  gronej
 * SNACC baseline merged with PER v1_7 tag
 *
 * Revision 1.1.2.1  2003/11/05 14:59:00  gronej
 * working PER code merged with esnacc_1_6
 *
 * Revision 1.1.1.1  2000/08/21 20:35:48  leonberp
 * First CVS Version of SNACC.
 *
 * Revision 1.1  1997/02/15 19:26:22  rj
 * first check-in
 *
 */


void PrintTblTypeDefInAsn1 PROTO ((FILE *f, TBL *tbl, TBLTypeDef *td));

void PrintTblTypeInAsn1 PROTO ((FILE *f, TBL *tbl, TBLTypeDef *td, TBLType *t));

void PrintTblInAsn1 PROTO ((FILE *f, TBL *tbl));
