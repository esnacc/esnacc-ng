// file: .../c++-lib/src/asn-relativeoid.cpp   - RELATIVE OBJECT IDENTIFIER
//
//  Joseph Grone
//  14/01/04
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

#include "asn-incl.h"

#if defined(_MSC_VER)
#pragma warning(push, 3)
#endif

#include <vector>

#if defined(_MSC_VER)
#pragma warning(pop)
#endif

#include <stdlib.h>

_BEGIN_SNACC_NAMESPACE


AsnRelativeOid::~AsnRelativeOid()
{
	if (oid != NULL)
		delete[] oid;
	if (m_lpszOidString != NULL)
		delete[] m_lpszOidString;
}

AsnRelativeOid::operator const char*() const
{
	if (m_lpszOidString == NULL)
		createDottedOidStr();

	return m_lpszOidString;
}


bool AsnRelativeOid::operator==(const char* o) const
{
	if (o == NULL)
		return false;

	if (m_lpszOidString == NULL)
		createDottedOidStr();

	if (strcmp(m_lpszOidString, o) == 0)
		return true; 
	else
		return false;
}

bool AsnRelativeOid::operator<(const AsnRelativeOid& o) const
{
	if (octetLen < o.octetLen)
		return true;
	else if (octetLen > o.octetLen)
		return false;
	else
		return (memcmp(oid, o.oid, octetLen) < 0);
}

unsigned long int AsnRelativeOid::NumArcs() const
{
	unsigned long numArcs = 0;

	// For each arc...
	for (size_t i = 0; i < octetLen; ++i)
	{
		// Skip octets in this arc number with the 'more' bit set
		while ((i < octetLen) && (oid[i] & 0x80))
			++i;

		++numArcs;
	}

	// If this is not a relative OID, add one to the number of arcs because the
	// first two arcs are munged together
	if (!m_isRelative && (numArcs > 0))
		++numArcs;

	return numArcs;
}

void AsnRelativeOid::GetOidArray(unsigned long oidArray[]) const
{
	FUNC("AsnRelativeOid::GetOidArray()");

	unsigned long iArc = 0;
	for (size_t i = 0; i < octetLen; ++i)
	{
		while ((i < octetLen) && (oid[i] & 0x80))
		{
			oidArray[iArc] <<= 7;
			oidArray[iArc] |= oid[i++] & 0x7F;
		}

		if (i == octetLen)
			throw OidException("Invalid encoded OID", STACK_ENTRY);

		oidArray[iArc] <<= 7;
		oidArray[iArc] |= oid[i] & 0x7F;

		// If this is not a relative OID and this is the first arc number,
		// unmunge this arc number
		if (!m_isRelative && (iArc == 0))
		{
			oidArray[1] = oidArray[0];
			oidArray[0] /= 40;
			if (oidArray[0] > 2)
				oidArray[0] = 2;
			oidArray[1] -= oidArray[0] * 40;
			++iArc;
		}

		++iArc;
	}
}

void AsnRelativeOid::Set(const char* szOidCopy)
{
	FUNC("AsnRelativeOid::Set()");

	if (szOidCopy == NULL)
		throw ParameterException("szOidCopy == NULL", STACK_ENTRY);

	// Check that the string doesn't begin with a period
	if (*szOidCopy == '.')
		throw OidException("Invalid OID string format", STACK_ENTRY);

	// Parse the string into a vector of long integers
	std::vector<unsigned long> intArray;
	const char* pIntStr = szOidCopy;
	while (*pIntStr != '\0')
	{
		if (*pIntStr == '.')
			++pIntStr;

		const char* pEnd = pIntStr;
		while ((*pEnd != '\0') && (*pEnd != '.'))
		{
			if ((*pEnd < '0') || (*pEnd > '9'))
			{
				throw OidException("Invalid character in OID string.",
					STACK_ENTRY);
			}
			++pEnd;
		}

		if (pEnd == pIntStr)
			throw OidException("Invalid OID string format", STACK_ENTRY);

		intArray.push_back((unsigned long)atol(pIntStr));
		pIntStr = pEnd;
	}

	// Create a temporary array from the vector
	unsigned long* pTempArray = new unsigned long[intArray.size()];
	if (pTempArray == NULL)
		throw SNACC_MEMORY_EXCEPT(intArray.size() * sizeof(long), "pTempArray");

	// Copy the arc numbers into the temporary array
	for (unsigned int i = 0; i < intArray.size(); ++i)
		pTempArray[i] = intArray[i];

	Set(pTempArray, intArray.size());
	delete[] pTempArray;

	// Copy the string value
	m_lpszOidString = new char[strlen(szOidCopy) + 1];
	if (m_lpszOidString == NULL)
		throw SNACC_MEMORY_EXCEPT(strlen(szOidCopy) + 1, "m_lpszOidString");
	strcpy(m_lpszOidString, szOidCopy);
}

void AsnRelativeOid::Set(const char* encOid, size_t len)
{
	FUNC("AsnRelativeOid::Set()");

	delete[] oid;
	delete[] m_lpszOidString;
	m_lpszOidString = NULL;

	octetLen = len;
	oid = new char[octetLen];
	if (oid == NULL)
		throw SNACC_MEMORY_EXCEPT(octetLen, "AsnRelativeOid::oid");
	memcpy(oid, encOid, octetLen);
}

void AsnRelativeOid::Set(const AsnRelativeOid& o)
{
	FUNC("AsnRelativeOid::Set()");

	if (this != &o)
	{
		Set(o.oid, o.octetLen);
		if (o.m_lpszOidString != NULL)
		{
			m_lpszOidString = new char[strlen(o.m_lpszOidString) + 1];
			if (m_lpszOidString == NULL)
			{
				throw SNACC_MEMORY_EXCEPT(strlen(o.m_lpszOidString) + 1,
					"AsnRelativeOid::m_lpszOidString");
			}
			strcpy(m_lpszOidString, o.m_lpszOidString);
		}
	}
}

void AsnRelativeOid::Set (unsigned long arcNumArr[], unsigned long arrLength)
{
	FUNC("AsnRelativeOid::Set()");

	if ((arcNumArr == NULL) || (arrLength < 1))
	{
		throw ParameterException("Invalid arguments in AsnRelativeOid::Set()",
			STACK_ENTRY);
	}

	char *buf = new char[arrLength * 5];	// Sized according to length
	char *tmpBuf = buf;

	// For each arc number...
	unsigned int totalLen = 0;
	unsigned int i;
	for (i = 0; (i < arrLength) && (arcNumArr[i] != (unsigned long)-1); ++i)
	{
		unsigned long tmpArcNum = arcNumArr[i];
		if ((i == 0) && !m_isRelative)
		{
			// If not a relative OID, munge together first oid arc numbers
			if ((arrLength < 2) || (arcNumArr[1] == (unsigned long)-1))
			{
				throw ParameterException("Invalid parameter to AsnRelativeOid::Set()",
					STACK_ENTRY);
			}
			if (tmpArcNum > 2)
			{
				throw OidException("First arc number must be 0, 1, or 2",
					STACK_ENTRY);
			}
			tmpArcNum *= 40;
			tmpArcNum += arcNumArr[++i];
		}
		
		// Calculate encoded length for this arc number
		unsigned long tmpNum = tmpArcNum;
		unsigned int arcLen = 0;
		do
		{
			++arcLen;
			tmpNum >>= 7;
		} while (tmpNum > 0);

		// Write the encoded bytes in reverse order -- all bytes except last
		// have MSB set
		bool isLast = true;
		unsigned int j = arcLen;
		do
		{
			tmpBuf[--j] = (char)(tmpArcNum & 0x7F);
            tmpArcNum >>= 7;
			if (!isLast)
				tmpBuf[j] |= 0x80;
			else
				isLast = false;
		} while (j > 0);

		// Update the total encoded length and the tmpBuf pointer
		totalLen += arcLen;
		tmpBuf += arcLen;

	} // end of for each arc number loop

	// Set this encoded OID value as the new value
	Set(buf, totalLen);
	delete[] buf;

}	// end of AsnRelativeOid::Set()

AsnLen AsnRelativeOid::BEnc(AsnBuf& b) const
{
	AsnLen l = BEncContent(b);
	l += BEncDefLen(b, l);

	if (m_isRelative)
	{
		l += BEncTag1(b, UNIV, PRIM, RELATIVE_OID_TAG_CODE);
	}
	else
	{
		l += BEncTag1(b, UNIV, PRIM, OID_TAG_CODE);
	}
    return l;
}

void AsnRelativeOid::BDec(const AsnBuf& b, AsnLen& bytesDecoded)
{
	FUNC("AsnRelativeOid::BDec()");

	// Decode the tag
	AsnTag tagId = BDecTag(b, bytesDecoded);
	if ((m_isRelative && (tagId != MAKE_TAG_ID(UNIV, PRIM, RELATIVE_OID_TAG_CODE))) ||
		(!m_isRelative && (tagId != MAKE_TAG_ID(UNIV, PRIM, OID_TAG_CODE))))
	{
		throw InvalidTagException(typeName(), tagId, STACK_ENTRY);
	}
	AsnLen elmtLen = BDecLen(b, bytesDecoded);
	BDecContent(b, tagId, elmtLen, bytesDecoded);
}

AsnLen AsnRelativeOid::BEncContent(AsnBuf& b) const
{
	b.PutSegRvs(oid, octetLen);
	return octetLen;
}

void AsnRelativeOid::BDecContent(const AsnBuf& b, AsnTag /* tagId */,
								 AsnLen elmtLen, AsnLen& bytesDecoded)
{
	FUNC("AsnRelativeOid::BDecContent()");

	if (elmtLen == INDEFINITE_LEN)
		throw BoundsException("indefinite length on primitive", STACK_ENTRY);

	if (elmtLen < 1)
		throw OidException("invalid length of OID", STACK_ENTRY);

	delete[] oid;
	delete[] m_lpszOidString;
	m_lpszOidString = NULL;

	octetLen = elmtLen;
	oid = new char[octetLen];
	if (oid == NULL)
		throw MemoryException(octetLen, "AsnRelativeOid::oid", STACK_ENTRY);
    b.GetSeg(oid, octetLen);
	bytesDecoded += elmtLen;
} // end of AsnRelativeOid::BDecContent()

AsnLen AsnRelativeOid::PEnc(AsnBufBits& b) const
{
	AsnLen len = PEncDefLenTo127(b, octetLen);

	len += b.OctetAlignWrite();
	b.PutBits((unsigned char*)oid, octetLen * 8);
	len += octetLen * 8;
	return len;
}

void AsnRelativeOid::PDec (AsnBufBits &b, AsnLen &bitsDecoded)
{
	unsigned char* seg = b.GetBits(8);
	bitsDecoded += 8;
	unsigned long lseg = (unsigned long)seg[0];
    delete [] seg;
	
	if (lseg > 0)
	{
		bitsDecoded += b.OctetAlignRead();
        
		seg = b.GetBits(lseg * 8);
		bitsDecoded += lseg * 8;

		Set((const char*)seg, lseg);
        delete [] seg;
	}
}

// Prints an AsnRelativeOid in ASN.1 Value Notation.
// Decodes the oid to get the individual arc numbers
void AsnRelativeOid::Print(std::ostream& os, unsigned short /*indent*/) const
{
	unsigned long numArcs = NumArcs();

	os << "{";

	if (numArcs > 0)
	{
		unsigned long* arcArray = new unsigned long[numArcs];
        for(unsigned long n = 0; n < numArcs; n++)
            arcArray[n] = 0;
		
        GetOidArray(arcArray);

		for (unsigned long i = 0; i < numArcs; ++i)
			os << " " << arcArray[i];

		delete[] arcArray;
	}
	else
		os << "-- void --";

	os << "}";
} // AsnRelativeOid::Print

void AsnRelativeOid::PrintXML (std::ostream &os, const char *lpszTitle) const
{
	const char* xmlTag;
	if (m_isRelative)
		xmlTag = "RELATIVE-OID";
	else
		xmlTag = "OID";

	if (lpszTitle)
		os << "<" << lpszTitle << " type=\"" << xmlTag << "\">";
	else
		os << '<' << xmlTag << ">\n";
	Print(os);
	if (lpszTitle)
		os << "</" << lpszTitle << ">";
	else
		os << "</" << xmlTag << ">\n";
}


#if META
const AsnRelativeOidTypeDesc AsnRelativeOid::_desc(NULL, NULL, false,
												   AsnTypeDesc::RELATIVE_OID,
												   NULL);
const AsnTypeDesc *AsnRelativeOid::_getdesc() const
{
	return &_desc;
}

#if TCL

int AsnRelativeOid::TclGetVal (Tcl_Interp *interp) const
{
  if (oid)
  {
    strstream buf;
    buf << *this;
    buf.str()[strlen(buf.str())-1] = '\0';			// chop the trailing '}'
    Tcl_SetResult (interp, buf.str()+1, TCL_VOLATILE);	// copy without leading '{'
  }
  return TCL_OK;
}

int AsnRelativeOid::TclSetVal (Tcl_Interp *interp, const char *valstr)
{
  if (!*valstr)
  {
    delete[] oid;
    oid = NULL;
    octetLen = 0;
	delete[] m_lpszOidString;
	m_lpszOidString = NULL;
    return TCL_OK;
  }

  Args arc;
  if (Tcl_SplitList (interp, (char*)valstr, &arc.c, &arc.v) != TCL_OK)
    return TCL_ERROR;
  if (m_isRelative && (arc.c < 1))
  {
    Tcl_AppendResult (interp, "relative oid must contain at least one number", NULL);
    Tcl_SetErrorCode (interp, "SNACC", "ILLARC", "<1", NULL);
    return TCL_ERROR;
  }
  if (!m_isRelative && (arc.c < 2))
  {
    Tcl_AppendResult (interp, "oid must contain at least two numbers", NULL);
    Tcl_SetErrorCode (interp, "SNACC", "ILLARC", "<2", NULL);
    return TCL_ERROR;
  }

  unsigned long* pLongArray = new unsigned long[arc.c];
  for (int i=0; i<arc.c; i++)
    if (Tcl_GetInt (interp, arc.v[i], pLongArray[i]) != TCL_OK)
    {
        delete [] pLongArray;
        return TCL_ERROR;
    }
  Set(pLongArray, arc.c);
  delete[] pLongArray;
  return TCL_OK;
}

#endif /* TCL */
#endif /* META */


bool AsnRelativeOid::OidEquiv(const AsnRelativeOid& o) const
{
	return ((octetLen == o.octetLen) && (memcmp(oid, o.oid, octetLen) == 0));
}

// FUNCTION: createDottedOidStr()
//
// PURPOSE: Populate null terrminated dotted notation string.  This function
//          will always re-create m_lpszOidString.
//
void AsnRelativeOid::createDottedOidStr() const
{
    FUNC("AsnRelativeOid::createDottedOidStr()");

    if (oid == NULL)
        throw OidException("NULL pointer in AsnRelativeOid", STACK_ENTRY);

	if (m_lpszOidString != NULL)
	{
		delete[] m_lpszOidString;
		m_lpszOidString = NULL;
	}
    
	bool isFirst = true;
	std::string tempBuf;
	char tempArcStr[40];
	for (unsigned long i = 0; i < octetLen;)
	{
		// Get the next arc number
		unsigned long arcNum = 0;
		do
		{
			arcNum <<= 7;
			arcNum += oid[i] & 0x7F;
			i++;
		} while ((i < octetLen) && (oid[i - 1] & 0x80));

		if (isFirst)
		{
			if (!m_isRelative)
			{
				// Unmunge the first arc number
				unsigned long firstNum = arcNum / 40;
				if (firstNum > 2)
					firstNum = 2;
				arcNum -= firstNum * 40;

				sprintf(tempArcStr, "%ld.%ld", firstNum, arcNum);
			}
			else
				sprintf(tempArcStr, "%ld", arcNum);
			isFirst = false;
		}
		else
			sprintf(tempArcStr, ".%ld", arcNum);

		// Append the temporary arc string to the temporary std::string
		tempBuf.append(tempArcStr);
	}

	m_lpszOidString = new char[tempBuf.length() + 1];
	strcpy(m_lpszOidString, tempBuf.c_str());
}


_END_SNACC_NAMESPACE
