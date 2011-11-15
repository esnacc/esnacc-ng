// file: .../c++-lib/src/asn-enum.C - methods for AsnEnum (ASN.1 ENUMERATED) class
//
// MS 92/06/16
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
// $Header: /baseline/SNACC/c++-lib/src/asn-enum.cpp,v 1.13 2003/12/17 19:05:03 gronej Exp $
// $Log: asn-enum.cpp,v $
// Revision 1.13  2003/12/17 19:05:03  gronej
// SNACC baseline merged with PER v1_7 tag
//
// Revision 1.12.2.3  2003/12/02 18:33:43  gronej
// Updated and debugged PER constraint logic for primitives
//
// Revision 1.12.2.2  2003/10/22 12:45:58  gronej
// Updating PER compiler
//
// Revision 1.12.2.1  2003/10/02 17:15:24  gronej
// Updating PER compiler
//
// Revision 1.12  2003/01/06 16:20:07  leonberp
// Changed BDec() and BDecContent() to use const AsnBufs
//
// Revision 1.11  2002/12/17 20:27:40  leonberp
// made BEnc() and BEncContent() const
//
// Revision 1.10  2002/10/23 21:02:48  leonberp
// fixed AsnBuf references and fixed clock skew problem
//
// Revision 1.9  2002/10/23 10:51:10  mcphersc
// Changed BUF_TYPE to AsnBuf
//
// Revision 1.8  2002/05/10 16:39:34  leonberp
// latest changes for release 2.2
// includes integrating asn-useful into C & C++ runtime library, the compiler changes that go along with that, SnaccException changes for C++ runtime and compiler
//
// Revision 1.7  2001/08/29 22:04:18  leonberp
// enchanced Clone() to allocate a new pointe AND COPY the object
//
// Revision 1.6  2001/07/12 19:33:37  leonberp
// Changed namespace to SNACC and added compiler options: -ns and -nons.  Also removed dead code.
//
// Revision 1.5  2001/06/28 15:29:47  rwc
// ADDED "SNACCASN" namespace definition to all SNACC data structures.
// This should not affect most applications since we do not have any name
// conflicts.
// ALSO, combined all ASN primitive data type includes into asn-incl.h.
//
// Revision 1.4  2001/06/26 15:03:48  mcphersc
// Tex - Added logic to accept a clone of the enum.
//
// Revision 1.3  2001/06/18 17:47:43  rwc
// Updated to reflect newly added C++ Exception error handling, instead of "C" longjmp and setjmp calls.
// Changes made to both the compiler and the SNACC C++ run-time library.
//
// Revision 1.2  2000/10/24 14:54:40  rwc
// Updated to remove high-level warnings (level 4 on MSVC++) for an easier build.
// SOME warnings persist due to difficulty in modifying the SNACC compiler to
// properly build clean source; also some files are built by Lex/Yacc.
//
// Revision 1.1.1.1  2000/08/21 20:36:08  leonberp
// First CVS Version of SNACC.
//
// Revision 1.5  1997/02/28 13:39:44  wan
// Modifications collected for new version 1.3: Bug fixes, tk4.2.
//
// Revision 1.4  1995/08/17 15:19:52  rj
// AsnEnumTypeDesc gets its own TclGetVal and TclSetVal functions.
//
// Revision 1.3  1995/07/24  20:14:49  rj
// Clone() added, or else the _desc would be wrong (and the wrong BEnc etc... would get called) for Clone-d objects.
//
// call constructor with additional pdu and create arguments.
//
// changed `_' to `-' in file names.
//
// Revision 1.2  1994/10/08  05:26:37  rj
// comment leader fixed.
//
// Revision 1.1  1994/10/08  05:24:03  rj
// functions extracted from ../inc/asn_enum.h

#include "asn-incl.h"

_BEGIN_SNACC_NAMESPACE

AsnLen AsnEnum::BEnc (AsnBuf &b) const
{
    AsnLen l;
    l = BEncContent (b);
    BEncDefLenTo127 (b, l);
    l++;
    l += BEncTag1 (b, UNIV, PRIM, ENUM_TAG_CODE);
    return l;
}

/*Takes the enumerated list, sorts them, and returns the index of the */
/*number that matches the value                                       */
long AsnEnum::IndexedVal(long *penumList, long numVals)const
{
    FUNC("AsnEnum::IndexedVal");
    long * indexedList = penumList;
    long temp = 0;
    long count1 = 0;
    long count2 = 0;
    bool bValueNotInList = true;
    
    if(m_len > 4)
        throw EXCEPT("enumerated value is too big", INTEGER_ERROR);

    for(count1 = 0; count1 < numVals; count1++)
    {
        for(count2 = 0; count2 < numVals; count2++)
        {
            if(indexedList[count2] < indexedList[count1])
            {
                temp = indexedList[count2];
                indexedList[count2] = indexedList[count1];
                indexedList[count1] = temp;
            }
        }
    }
    
    temp = (long)*this;
    for(count1 = 0; count1 < numVals; count1++)
    {
        if(temp == indexedList[count1])
        {
            temp = count1;
            count1 = numVals;
            bValueNotInList = false;
        }
    }

    if(bValueNotInList)
        throw EXCEPT("value is not in enumerated List", INTEGER_ERROR);

    return temp;
}

/*sorts the enumerated list and matches the decoded number with */
/*the associated indexed number in the list                     */
void AsnEnum::SetIndex(long *penumList, long numVals, long index)
{
    long * indexedList = penumList;
    long temp = 0;
    long count1 = 0;
    long count2 = 0; 
    
    for(count1 = 0; count1 < numVals; count1++)
    {
        for(count2 = 0; count2 < numVals; count2++)
        {
            if(indexedList[count2] < indexedList[count1])
            {
                temp = indexedList[count2];
                indexedList[count2] = indexedList[count1];
                indexedList[count1] = temp;
            }
        }
    }

    Set(indexedList[index]);
}

void AsnEnum::BDec (const AsnBuf &b, AsnLen &bytesDecoded)
{
   FUNC("AsnEnum::BDec");

   AsnLen elmtLen;
   AsnTag tagId;

   tagId = BDecTag (b, bytesDecoded);
   if (tagId != MAKE_TAG_ID (UNIV, PRIM, ENUM_TAG_CODE))
   {
    throw InvalidTagException(typeName(), tagId, STACK_ENTRY);
   }

   elmtLen = BDecLen (b, bytesDecoded);
   BDecContent (b, MAKE_TAG_ID (UNIV, PRIM, ENUM_TAG_CODE), elmtLen, bytesDecoded);
}

#if META

const AsnEnumTypeDesc AsnEnum::_desc (NULL, NULL, false, AsnTypeDesc::ENUMERATED, NULL, NULL);

const AsnTypeDesc *AsnEnum::_getdesc() const
{
  return &_desc;
}

#if TCL

int AsnEnum::TclGetVal (Tcl_Interp *interp) const
{
  const AsnNameDesc *n = _getdesc()->getnames();
  if (n)
  {
    for (; n->name; n++)
      if (n->value == value)
      {
	Tcl_SetResult (interp, (char*)n->name, TCL_STATIC);
	return TCL_OK;
      }
  }
  char valstr[80];
  sprintf (valstr, "%d", value);
  Tcl_AppendResult (interp, "illegal numeric enumeration value ", valstr, " for type ", _getdesc()->getmodule()->name, ".", _getdesc()->getname(), NULL);
  Tcl_SetErrorCode (interp, "SNACC", "ILLENUM", NULL);
  return TCL_ERROR;
}

int AsnEnum::TclSetVal (Tcl_Interp *interp, const char *valstr)
{
  const AsnNameDesc *n = _getdesc()->getnames();
  if (n)
  {
    for (; n->name; n++)
      if (!strcmp (n->name, valstr))
      {
	value = n->value;
	return TCL_OK;
      }
  }
  Tcl_SetErrorCode (interp, "SNACC", "ILLENUM", NULL);
  Tcl_AppendResult (interp, "illegal symbolic enumeration value \"", valstr, "\" for type ", _getdesc()->getmodule()->name, ".", _getdesc()->getname(), NULL);
  return TCL_ERROR;
}

#endif /* TCL */
#endif /* META */

_END_SNACC_NAMESPACE
