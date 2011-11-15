// file: .../c++-lib/src/asn-type.C  - Abstract class that all ASN.1 types are derived from
//               Design motivated by ANY type.
//
//               Runtime cost in speed and space for virtual fcns will
//               hopefully not be too bad
//
//               If your ASN.1 code does not use ANY or ANY DEFIND BY
//               types then you could  make the BEnc, BDec and Clone
//               non-virtual in the AsnType to improve performance.
//               (undef SUPPORT_ANY_TYPE)
//
//               NOTE: The virtual encode/decode/print etc fcns
//               could be purely virtual (= 0) creating an abstract class
//               but the ANY handling code needs to instantiate the AsnType
//               base class (via Cloning).   Also it allows for default
//               error reporting for ANY types that have not been
//               instantiated properly.
//
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
// MS 92
//
// $Header: /baseline/SNACC/c++-lib/src/asn-type.cpp,v 1.17 2004/02/11 19:08:48 nicholar Exp $
//

#include "asn-incl.h"

_BEGIN_SNACC_NAMESPACE

AsnType::~AsnType()
{
}

bool AsnType::BEncPdu (AsnBuf &b, AsnLen &bytesEncoded) const
{
   try 
   {
   	bytesEncoded = BEnc (b);
  	return true;
   }
   catch (...)
   {
      return false;
   }
}

bool AsnType::BDecPdu (const AsnBuf &b, AsnLen &bytesDecoded)
{
	try
   {
      bytesDecoded = 0;
      {		  
         BDec (b, bytesDecoded);
	     return true;
      }
   }
   catch (...)
   {
      return false;
   }
}

#if META

const AsnTypeDesc AsnType::_desc (NULL, NULL, false, AsnTypeDesc::VOID, NULL);

const AsnTypeDesc *AsnType::_getdesc() const
{
  return &_desc;
}

AsnType	*AsnType::_getref (const char *membername, bool create)
{
  return NULL;
}

const char *AsnType::_typename() const
{
  return _desc.typenames[_getdesc()->type];
}

#if TCL

int AsnType::TclGetDesc (Tcl_DString *valstr) const
{
  return TCL_OK;
}

int AsnType::TclGetVal (Tcl_Interp *interp) const
{
  Tcl_AppendResult (interp, "can't get value from ", _typename(), NULL);
  return TCL_ERROR;
}

int AsnType::TclSetVal (Tcl_Interp *interp, const char *)
{
  Tcl_AppendResult (interp, "can't set value in ", _typename(), NULL);
  return TCL_ERROR;
}

int AsnType::TclUnsetVal (Tcl_Interp *interp, const char *)
{
  Tcl_AppendResult (interp, "can't unset member(s) in ", _typename(), NULL);
  return TCL_ERROR;
}

#endif /* TCL */
#endif /* META */

_END_SNACC_NAMESPACE
