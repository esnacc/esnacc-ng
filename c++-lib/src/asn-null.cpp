// file: .../c++-lib/src/asn-null.C
//
// MS 92
// Copyright (C) 1992 Michael Sample and the University of British Columbia
//
// This library is free software; you can redistribute it and/or
// modify it provided that this copyright/license information is retained
// in original form.
//
// If you modify this file, you must clearly indicate your changes.
//
// This source code is distributed in the hope that it will be
// useful, but WITHOUT ANY WARRANTY; without even the implied warranty
// of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
//
// $Header: /baseline/SNACC/c++-lib/src/asn-null.cpp,v 1.13 2004/02/11 19:08:48 nicholar Exp $
// $Log: asn-null.cpp,v $
// Revision 1.13  2004/02/11 19:08:48  nicholar
// Updated Print() function so no longer uses global indent
//
// Revision 1.12  2003/12/17 19:05:03  gronej
// SNACC baseline merged with PER v1_7 tag
//
// Revision 1.11.2.4  2003/12/04 20:47:10  gronej
// Moved bAlign out of all PEnc calls and into AsnBufBits as a member
// An AsnBufBits is now invoked with a bAlign parameter defaulted to false
//
// Revision 1.11.2.3  2003/11/04 14:21:21  gronej
// Update PER compiler with some PERGeneral functionality  11/04/03
//
// Revision 1.11.2.2  2003/10/22 12:45:58  gronej
// Updating PER compiler
//
// Revision 1.11.2.1  2003/10/02 17:15:24  gronej
// Updating PER compiler
//
// Revision 1.11  2003/01/06 16:20:07  leonberp
// Changed BDec() and BDecContent() to use const AsnBufs
//
// Revision 1.10  2002/12/17 20:27:40  leonberp
// made BEnc() and BEncContent() const
//
// Revision 1.9  2002/10/23 21:02:48  leonberp
// fixed AsnBuf references and fixed clock skew problem
//
// Revision 1.8  2002/10/23 10:51:10  mcphersc
// Changed BUF_TYPE to AsnBuf
//
// Revision 1.7  2002/05/10 16:39:35  leonberp
// latest changes for release 2.2
// includes integrating asn-useful into C & C++ runtime library, the compiler changes that go along with that, SnaccException changes for C++ runtime and compiler
//
// Revision 1.6  2001/08/29 22:04:18  leonberp
// enchanced Clone() to allocate a new pointe AND COPY the object
//
// Revision 1.5  2001/07/12 19:33:38  leonberp
// Changed namespace to SNACC and added compiler options: -ns and -nons.  Also removed dead code.
//
// Revision 1.4  2001/06/28 15:29:47  rwc
// ADDED "SNACCASN" namespace definition to all SNACC data structures.
// This should not affect most applications since we do not have any name
// conflicts.
// ALSO, combined all ASN primitive data type includes into asn-incl.h.
//
// Revision 1.3  2001/06/18 17:47:43  rwc
// Updated to reflect newly added C++ Exception error handling, instead of "C" longjmp and setjmp calls.
// Changes made to both the compiler and the SNACC C++ run-time library.
//
// Revision 1.2  2000/10/16 18:10:37  rwc
// removed most warnings from C++-lib, some C-lib.
//
// Revision 1.1.1.1  2000/08/21 20:36:08  leonberp
// First CVS Version of SNACC.
//
// Revision 1.5  1995/08/17 15:38:19  rj
// set Tcl's errorCode variable
//
// Revision 1.4  1995/07/24  20:18:27  rj
// #if TCL ... #endif wrapped into #if META ... #endif
//
// call constructor with additional pdu and create arguments.
//
// changed `_' to `-' in file names.
//
// Revision 1.3  1994/10/08  04:18:26  rj
// code for meta structures added (provides information about the generated code itself).
//
// code for Tcl interface added (makes use of the above mentioned meta code).
//
// virtual inline functions (the destructor, the Clone() function, BEnc(), BDec() and Print()) moved from inc/*.h to src/*.C because g++ turns every one of them into a static non-inline function in every file where the .h file gets included.
//
// made Print() const (and some other, mainly comparison functions).
//
// several `unsigned long int' turned into `size_t'.
//
// Revision 1.2  1994/08/28  10:01:15  rj
// comment leader fixed.
//
// Revision 1.1  1994/08/28  09:21:04  rj
// first check-in. for a list of changes to the snacc-1.1 distribution please refer to the ChangeLog.

#include "asn-incl.h"

_BEGIN_SNACC_NAMESPACE

void AsnNull::BDecContent (const AsnBuf &/*b*/, AsnTag /*tagId*/, AsnLen elmtLen, AsnLen &/*bytesDecoded*/)
{
   FUNC("AsnNull::BDecContent()");

   if (elmtLen != 0)
   {
    throw BoundsException("AsnNull has non-zero length", STACK_ENTRY);
   }
} /* AsnNull::BDecContent */

AsnLen AsnNull::PEnc (AsnBufBits &b) const
{
	AsnLen len = 0;

	return len;
}

void AsnNull::PDec (AsnBufBits &b, AsnLen &bitsDecoded)
{
	bitsDecoded = bitsDecoded;
}

AsnLen AsnNull::BEnc (AsnBuf &b) const
{
    AsnLen l;
    l = BEncContent (b);
    BEncDefLenTo127 (b, l);
    l++;
    l += BEncTag1 (b, UNIV, PRIM, NULLTYPE_TAG_CODE);
    return l;
}

void AsnNull::BDec (const AsnBuf &b, AsnLen &bytesDecoded)
{
    FUNC("AsnNull::BDec()");

    AsnLen elmtLen;
    AsnTag tagId;
    tagId = BDecTag (b, bytesDecoded); 
    if (tagId != MAKE_TAG_ID (UNIV, PRIM, NULLTYPE_TAG_CODE))
    {
       throw InvalidTagException(typeName(), tagId, STACK_ENTRY);
    }

    elmtLen = BDecLen (b, bytesDecoded);
    BDecContent (b, MAKE_TAG_ID (UNIV, PRIM, NULLTYPE_TAG_CODE), elmtLen, bytesDecoded);
}

void AsnNull::Print(std::ostream& os, unsigned short /*indent*/) const
{
	os << "NULL";
}

void AsnNull::PrintXML (std::ostream &os, const char *lpszTitle) const
{
   os << "<NULL>"; 
   if (lpszTitle) os << lpszTitle; 
   os << "-"; 
   Print(os); os << "</NULL>\n"; 
}

#if META

const AsnNullTypeDesc AsnNull::_desc (NULL, NULL, false, AsnTypeDesc::NUL_, NULL);

const AsnTypeDesc *AsnNull::_getdesc() const
{
  return &_desc;
}

#if TCL

int AsnNull::TclGetVal (Tcl_Interp *interp) const
{
  return TCL_OK;
}

int AsnNull::TclSetVal (Tcl_Interp *interp, const char *valstr)
{
  if (*valstr)
  {
    Tcl_AppendResult (interp, "illegal non-null value `", valstr, "' for type ", _getdesc()->getmodule()->name, ".", _getdesc()->getname(), NULL);
    Tcl_SetErrorCode (interp, "SNACC", "ILLNULL", NULL);
    return TCL_ERROR;
  }

  return TCL_OK;
}

#endif /* TCL */
#endif /* META */

_END_SNACC_NAMESPACE
