// file: .../c++-lib/src/asn-oid.C   - OBJECT IDENTIFIER
//
//  Mike Sample
//  92/07/02
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
// $Header: /baseline/SNACC/c++-lib/src/asn-oid.cpp,v 1.59 2004/02/09 20:38:50 nicholar Exp $
//

#include "asn-incl.h"


_BEGIN_SNACC_NAMESPACE


AsnOid::AsnOid() : AsnRelativeOid()
{
	m_isRelative = false;
	Set("0.0");
}

AsnOid::AsnOid(const char* pszOID) : AsnRelativeOid()
{
	m_isRelative = false;
	Set(pszOID);
}

AsnOid AsnOid::operator+(const AsnRelativeOid& ro) const
{
	AsnOid tempOid(*this);
	tempOid += ro;
	return tempOid;
}

AsnOid& AsnOid::operator+=(const AsnRelativeOid& ro)
{
	FUNC("AsnOid::operator+=()");

	// Create a temporary encoded OID and concatenate the RelativeOID to it
	char *tmp = new char[octetLen + ro.Len()];
	if (tmp == NULL)
		throw SNACC_MEMORY_EXCEPT(octetLen + ro.Len(), "tmp");
	memcpy(tmp, oid, octetLen);
	memcpy(&tmp[octetLen], ro.Str(), ro.Len());

	// Delete existing members
	delete[] oid;
	delete[] m_lpszOidString;
	m_lpszOidString = NULL;

	// Set this OID's encoded value to the temporary one
	octetLen += ro.Len();
	oid = tmp;

	return *this;
}


#if META
const AsnOidTypeDesc AsnOid::_desc (NULL, NULL, false, AsnTypeDesc::OBJECT_IDENTIFIER, NULL);
const AsnTypeDesc *AsnOid::_getdesc() const
{
	return &_desc;
}
#endif /* META */


_END_SNACC_NAMESPACE
