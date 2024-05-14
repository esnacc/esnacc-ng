// File: .../c++-lib/src/asn-int.C - methods for AsnInt (ASN.1 INTEGER) class
//
// MS 92/06/16
// Copyright (C) 1992 Michael Sample and the University of British Columbia
// Copyright (C) 2011-2015 Azimuth Systems, Inc.
// Copyright (C) 2016 Red Hat, Inc
//
// This library is free software; you can redistribute it and/or
// modify it provided that this copyright/license information is retained
// in original form.
//
// This source code is distributed in the hope that it will be
// useful, but WITHOUT ANY WARRANTY; without even the implied warranty
// of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
// NOTE: Numerous amounts of history were removed from this.  The complete
//       change is available at https://github.com/esnacc/esnacc-ng

#include "asn-incl.h"

#ifdef WIN32
#if defined(_MSC_VER)
#pragma warning(disable: 4100 4710 4251 4018)
#pragma warning(push,3)
#endif
#include <vector>
#if defined(_MSC_VER)
#pragma warning(pop)
#endif
#else
#include <vector>
#endif

#include <stdlib.h>

_BEGIN_SNACC_NAMESPACE

#if META

const AsnIntTypeDesc AsnInt::_desc (NULL, NULL, false, AsnTypeDesc::INTEGER,
                                    NULL, NULL);

const AsnTypeDesc *AsnInt::_getdesc() const
{
    return &_desc;
}

#if TCL

int AsnInt::TclGetVal (Tcl_Interp *interp) const
{
    char buf[32];
    sprintf (buf, "%d", value);
    Tcl_SetResult (interp, buf, TCL_VOLATILE);
    return TCL_OK;
}

int AsnInt::TclSetVal (Tcl_Interp *interp, const char *valstr)
{
    const AsnNameDesc *n = _getdesc()->getnames();
    if (n)
        for (; n->name; n++)
            if (!strcmp (n->name, valstr)) {
                value = n->value;
                return TCL_OK;
            }

    int valval;
    if (Tcl_GetInt (interp, (char*)valstr, &valval) != TCL_OK)
        return TCL_ERROR;
    value = valval;
    return TCL_OK;
}

#endif /* TCL */
#endif /* META */


AsnLen AsnInt::BEnc (AsnBuf &b) const
{
    FUNC("AsnInt::BEnc");
    if( checkConstraints(NULL) != 0 )
        throw ConstraintException("Integer not within constraints",
                                  STACK_ENTRY);

    AsnLen l=0;
    l = BEncContent (b);
    l += BEncDefLen (b, l);
    l += BEncTag1 (b, UNIV, PRIM, INTEGER_TAG_CODE);
    return l;
}

void AsnInt::BDec (const AsnBuf &b, AsnLen &bytesDecoded)
{
    FUNC("AsnInt::BDec");

    AsnTag tag;
    AsnLen elmtLen1;

    if (((tag = BDecTag (b, bytesDecoded)) !=
         MAKE_TAG_ID (UNIV, PRIM, INTEGER_TAG_CODE))
        && (tag != MAKE_TAG_ID (UNIV, CONS, INTEGER_TAG_CODE))) {
        throw InvalidTagException(typeName(), tag, STACK_ENTRY);
    }
    elmtLen1 = BDecLen (b, bytesDecoded);
    BDecContent (b, tag, elmtLen1, bytesDecoded);
}

AsnLen AsnInt::BEncContent (AsnBuf &b) const
{
    b.PutSegRvs((char *)m_bytes, m_len);
    return m_len;
}

void AsnInt::BDecContent (const AsnBuf &b, AsnTag, AsnLen elmtLen,
                          AsnLen &bytesDecoded)
{
    FUNC("AsnInt::BDecContent()");

    if (elmtLen == INDEFINITE_LEN)
        throw EXCEPT("indefinite length on primitive", DECODE_ERROR);

    unsigned char *bytes = new unsigned char[elmtLen];

    bool isNeg = false;
    b.GetSeg((char *)bytes, elmtLen);
    bytesDecoded += elmtLen;

    if ((bytes[0] & 0x80)) {
        isNeg = true;
    }

    storeDERInteger(bytes, elmtLen, !isNeg);
    delete [] bytes;
}

AsnInt::AsnInt (const AsnInt &that)
    : m_bytes(0), m_len(0)
{
    operator=(that);
}

AsnInt::AsnInt(const char *str, const size_t len, bool unsignedFlag)
    : m_bytes(0), m_len(0)
{
    storeDERInteger((const unsigned char *)str, len, unsignedFlag);
}

AsnInt::AsnInt(const AsnOcts &o, bool unsignedFlag)
    : m_bytes(0), m_len(0)
{
    storeDERInteger(o.c_ustr(), o.Len(), unsignedFlag);
}
   
// Construct an AsnInt from an integer value
//
AsnInt::AsnInt (AsnIntType val)
    : m_bytes(NULL), m_len(0)
{
    if (!val) {
        m_len = 1;
        m_bytes = new unsigned char[1];
        *m_bytes = 0;
    } else {
        Set(val);
    }
}

// Construct an AsnInt from a null terminated character string.
// The string can be in either HEX or decimal format.
//
// HEX format accepted:
//   0xFFFFF
//   'FFFFF'H
//

AsnInt::AsnInt(const char *str, bool unsignedFlag)
    : m_bytes(NULL), m_len(0)
{
   FUNC("AsnInt::AsnInt(str,unsignedFlag)");
   unsigned length = strlen(str);
   if (length == 0)
       return;

   const char *useStr = str;
   char *errstr = NULL;
   int radix = 0;

   if (str[length-1] == 'H' && str[0] == '\'' && str[length-2] == '\'') {
       useStr = str+1;
       radix = 16;
   }


   if (unsignedFlag) {
       unsigned char s[sizeof(AsnUIntType)];
       AsnUIntType t = strtoul(useStr, &errstr, radix);
       size_t l = sizeof(t) - 1;
       for (length = 0; length < sizeof(t); ++length)
           s[l-length] = (unsigned char)((t >> (8*length)) & 0xff);
       storeDERInteger(s, sizeof(s), true);
   } else {
       unsigned char s[sizeof(AsnIntType)];
       AsnIntType t = strtol(useStr, &errstr, radix);
       size_t l = sizeof(t) - 1;
       for (length = 0; length < sizeof(t); ++length)
           s[l-length] = (unsigned char)((t >> (8*length)) & 0xff);
       storeDERInteger(s, sizeof(s), (t >= 0));
   }

   if ((errstr && *errstr == '\0') ||
       !errstr ||
       (*errstr == '\'' && radix == 16)) {
       ; /* the set happens in the prior blocks. */
   } else {
       char s[512];
       snprintf(s, 512, "Invalid bytes: %s", errstr);
       throw EXCEPT(strdup(s), INTEGER_ERROR);
   }
}

AsnInt::~AsnInt()
{
    delete [] m_bytes;
}

void AsnInt::storeDERInteger(const unsigned char *pData, long dataLen, bool unsignedFlag)
{
    m_len = 0;
    delete [] m_bytes;
    m_bytes = 0;

    /* IF the application generates an r,s,p,q,g or y value in which the
     * first 9 bits are all set to 0, then the encoding software deletes the
     * first octet from the octets to be encoded.  This rule is applied
     * repeatedly to the remaining octets until the first 9 bits are not all
     * set to 0.
     */
    if (unsignedFlag) {
        // Check for leading nine bits all zero
        while (dataLen > 1 && (pData[0] & 0xFF) == 0x00 &&
               (pData[1] & 0x80) == 0x0) {
			   ++pData;
			   --dataLen; 
		   }

        m_bytes = new unsigned char[dataLen + 1];
        m_len = dataLen;

        /* If the application generates a r,s,p,q,g, or y value in which the
         * MSB is set to 1, THEN the software prepends a single octet in which
         * all bits are set to 0.
         */
        unsigned char *cwrite = m_bytes;
        if (*pData & 0x80) {
            // Prepend a leading octet
            *m_bytes = 0;
            m_len++;
            cwrite++;
        }
        memcpy(cwrite, pData, dataLen);

    } else if (dataLen > 1 ) {

        /* check for first first 9 bits all ones
         */
	   while ((dataLen > 1) && (pData[0] == 0xFF) && (pData[1] & 0x80)) {
		   ++pData;
		   --dataLen;
	   }

	   /* check for first 9 bits all zeros
        */
	   while ((dataLen > 1) && (pData[0] == 0) && ((pData[1] & 0x80) == 0)) {
           ++pData;
           --dataLen;
       }
       m_bytes = new unsigned char[dataLen + 1];
       m_len = dataLen;
       memcpy(m_bytes, pData, dataLen);
    } else {
        m_bytes = new unsigned char [1];
        m_len = 1;
        m_bytes[0] = *pData;
    }
}

AsnInt &AsnInt::operator =(const AsnInt &that)
{
    if (this != &that) {
        m_len = that.m_len;
        delete [] m_bytes;
        m_bytes = new unsigned char[m_len];
        memcpy(m_bytes, that.m_bytes, m_len);
    }

    return *this;
}

// Set AsnInt from a buffer.  Buffer is assumed to be a proper
// ASN.1 integer.
//
void AsnInt::Set (const unsigned char *pData, size_t len, bool unsignedFlag)
{
    storeDERInteger(pData, len, unsignedFlag);
}

template <class T>
void endswap(T *objp)
{
  unsigned char *memp = reinterpret_cast<unsigned char*>(objp);
  std::reverse(memp, memp + sizeof(T));
}

// Set AsnInt from a AsnIntType
//
void AsnInt::Set(AsnIntType iIn)
{ 
   unsigned char cTmp[sizeof(iIn)];

   for (unsigned long i=0; i < sizeof(iIn); i++)
       cTmp[(sizeof(iIn)-1)-i] = (unsigned char)((iIn >> (8*i)) & 0xff);

   storeDERInteger(cTmp, sizeof(iIn), (iIn >= 0));
}

void AsnInt::getPadded(unsigned char *&bigIntDataOut, size_t &bigIntLen,
                       const size_t padToSize) const
{
   FUNC("AsnInt::GetUnSignedBitExtendedData()");

   bigIntLen = m_len;
   const unsigned char *bigIntData = m_bytes;

   /* This is fix to determine if the r,s,p,q,g, or y value is of the correct
    * length.
    */
   if (padToSize > 0) {
      /* if bigint length is less than the expected number of octets
       * the decoding software ensures that the MSB is 0 and, if so, it
       * prepends the appropriate number of octets in which every bit is
       * set to 0 to the decoded value to obtain the value supplied to
       * Fortezza Card.
       */
      if (bigIntLen < padToSize) {
         long prepend = 0;
         unsigned char *tmpInt;

         prepend = padToSize - bigIntLen;

         tmpInt = (unsigned char *) calloc(1, bigIntLen + prepend);
         memset(tmpInt, 0, prepend);
         memcpy(&tmpInt[prepend], bigIntData , bigIntLen);

         bigIntDataOut = tmpInt;
         bigIntLen += prepend;
      } else if (bigIntLen > padToSize) {
      /* If the encoded values includes an "extra" octet THEN the
       * decoding software ensures that every bit in the initial octets is
       * set to 0 and, if so, deletes the initial octet from the decoded value
       * to obtain the value to be supplied to the Fortezza Card.  If the
       * extra octet contains a bit set to 1, then an error is reported.
       */
          if (bigIntData[0] != 0) {
            throw EXCEPT("Extra octet is not zero.", INTEGER_ERROR);
          }
          bigIntLen--;
          bigIntDataOut = (unsigned char *) calloc(1, bigIntLen);
          memcpy( &bigIntDataOut[0], &bigIntData[1], bigIntLen);
      } else {      // Exact length.
          bigIntDataOut = (unsigned char *) calloc(1, bigIntLen);
          memcpy( &bigIntDataOut[0], &bigIntData[0], bigIntLen);
      }
   }
   // bigIntData AND bigIntLen contain the results.
}


bool AsnInt::operator == (const AsnInt &o) const
{
   if (m_len == o.m_len)
      return (memcmp(m_bytes, o.m_bytes, m_len) == 0);
   else
      return false;
}
bool AsnInt::operator != (const AsnInt &o) const
{ 
    return (!(*this == o));
}

bool AsnInt::operator==(AsnIntType o) const
{
    if (m_len > sizeof(AsnIntType))
        return false;

    // Convert this AsnInt to a normal integer
    AsnIntType result = 0;

    if (m_len > 0) {
        // If the AsnInt is negative initialize the result to -1
        if ((m_bytes[0] & 0x80) != 0)
            result = -1;

        for (unsigned int i = 0; i < m_len; ++i) {
            result <<= 8;
            result |= (unsigned char)m_bytes[i];
        }
    }

    return (result == o);
}

bool AsnInt::operator<(const AsnInt &o) const
{
   if (m_len < o.m_len)
      return true;
   else if (m_len > o.m_len)
      return false;

   if (memcmp(m_bytes, o.m_bytes, m_len) < 0)
      return true;
   else
      return false;
}

void AsnInt::Print(std::ostream& os, unsigned short /*indent*/) const
{
    try {
        long l = toInteger<long>();
        os << l << '\n';
    } catch (SnaccException &e) {
        os << "'";
        std::ios_base::fmtflags flags = os.flags();
        os.setf(std::ios::hex);
        char buf[3] = {0};
        for (unsigned long i = 0; i < m_len; i++) {
            sprintf(buf, "%2.2x", (int)m_bytes[i]);
            os << buf;
        }
        os << "'H  -- \n";
        os.setf(flags);
    }
}

void AsnInt::PrintXML(std::ostream &os, const char *lpszTitle) const
{
   if (lpszTitle) {
       os << "<" << lpszTitle << ">\n";
   } else {
       os << "<INTEGER>\n"; 
   }

   Print(os); 

   if (lpszTitle) 
       os << "</" << lpszTitle << ">\n"; 
   else
       os << "</INTEGER>\n"; 
}

int AsnInt::checkConstraints (ConstraintFailList* pConstraintFails)const
{
    FUNC("AsnInt::checkConstraints");
    int numValueRanges;
    const ValueRange* valueRanges = ValueRanges(numValueRanges);
	int count = 0;
	int failed = 1;
	std::string  ptr;
    long ltemp= 0;
	 const char* tmpptr = NULL;

    if (m_len > sizeof(AsnIntType) && (numValueRanges > 0) ) {
        throw EXCEPT("Integer is out of constraint range", CONSTRAINT_ERROR);
    }
    
	if(valueRanges)
	{
        ltemp = *this;
        
		for(count = 0; count< numValueRanges; count++) {
            tmpptr = NULL;
			if(valueRanges[count].upperBoundExists == 1) {
                if ((ltemp < valueRanges[count].lowerBound) ||
                    (ltemp > valueRanges[count].upperBound)) {
                    tmpptr = ConstraintErrorStringList[INTEGER_VALUE_RANGE];
                }
			} else if(valueRanges[count].upperBoundExists == 2) {
                if (ltemp != valueRanges[count].lowerBound ) {
                    tmpptr = ConstraintErrorStringList[INTEGER_SINGLE_VALUE];
                }
            } else if(valueRanges[count].upperBoundExists == 0) {
                if (ltemp < valueRanges[count].lowerBound) {
                    tmpptr = ConstraintErrorStringList[INTEGER_VALUE_RANGE];
                }
            }

            if(tmpptr) {
                ptr += tmpptr;
            } else {
				failed = 0;
			}
		}
	} else {
        failed = 0;
	}

	if(failed && pConstraintFails != NULL) {
        pConstraintFails->push_back(ptr);
	}

    return failed;
}

void AsnInt::putByte(long offset, unsigned char cByte)
{
    FUNC("AsnInt::putByte");
    if (offset < 0 ||
        (m_len <= (unsigned long)offset)) {
        throw EXCEPT("Invalid Offset", INTEGER_ERROR);
    }
    m_bytes[offset] = cByte;
}

void AsnInt::Allocate(long size)
{
    unsigned char* temp = new unsigned char[m_len + size];
    if (m_len) {
        memcpy(temp, m_bytes, m_len);
	    size += m_len;
    }

	Clear();

	m_len = size;
	m_bytes = new unsigned char[m_len];
    if( m_len )
        memcpy(m_bytes, temp, m_len);
    delete [] temp;
}

void AsnInt::PDec(AsnBufBits &b, AsnLen &bitsDecoded)
{
    int numValueRanges;
    const ValueRange* valueRanges = ValueRanges(numValueRanges);
	int x = 0;
	int upperBoundFound = 0;
	int lowerBound = 0;
	int upperBound = 0;

    Clear();
    
	if(numValueRanges <= 0)
	{
		DecodeGeneral(b, bitsDecoded);
	}
	else
	{
		lowerBound = valueRanges[x].lowerBound;
		upperBound = lowerBound;
		upperBoundFound = valueRanges[x].upperBoundExists;

		for(x = 0; x < numValueRanges; x++)
		{
			if(lowerBound > valueRanges[x].lowerBound)
			{
				lowerBound = valueRanges[x].lowerBound;
			}
		
            if(upperBound < valueRanges[x].lowerBound)
            {
                upperBound = valueRanges[x].lowerBound;
            }

			if(valueRanges[x].upperBoundExists == 1)
			{
				upperBoundFound = 1;

				if(upperBound < valueRanges[x].upperBound)
				{
					upperBound = valueRanges[x].upperBound;
				}
			}
		}
	
        if(upperBound > lowerBound)
            upperBoundFound = 1;

		if(upperBoundFound == 1)
		{
            if(lowerBound != upperBound)
			    PDecFullyConstrained(b, lowerBound, upperBound, bitsDecoded);
		}
		else
		{   
            if(numValueRanges == 1 && valueRanges[0].upperBoundExists == 2 )
            {
                Set(lowerBound);
            }
            else
            {
                PDecSemiConstrained(b, lowerBound, bitsDecoded);
            }
            
		}

	}


}

void AsnInt::PDecSemiConstrained (AsnBufBits &b, long lowerBound, AsnLen &bitsDecoded)
{
	FUNC("AsnInt::PDec(...Semi-Constrained Int...)");

	unsigned char* seg;
	long l_intval;

	m_len = 0;

	seg = (unsigned char*)b.GetBits(8);
	bitsDecoded += 8;

	m_len = (long)seg[0];

	if(m_len > 4)
	{
		throw EXCEPT("integer is too big for decoding from offset",
				INTEGER_ERROR);
	}

    delete [] seg;
	seg = (unsigned char*)b.GetBits(m_len * 8);
	bitsDecoded += (m_len * 8);

	Set(seg, m_len);
	
	l_intval = *this;

	l_intval += lowerBound;

	Set(l_intval);
	
    delete [] seg;
}

void AsnInt::PDecFullyConstrained (AsnBufBits &b, long lowerBound, long upperBound, AsnLen &bitsDecoded)
{
	FUNC("AsnInt::PDec(...Fully-Constrained Int...)");

	unsigned char* seg = NULL;
	long l_intval = 0;
	int oddBits = 0;
	unsigned long range = (upperBound - lowerBound) + 1;
	long tempRange = range;
	long minBitsNeeded = 0;
	long numBytes = 0;
	long count = 0;
	unsigned char pChar[] = {0x00, 0x00};

    Clear();

	m_len = 0;

	if(range != 1)
	{
		tempRange -= 1;
		while(tempRange > 0)
		{
			tempRange -= (long)(1 << minBitsNeeded);
			minBitsNeeded += 1;
		}
	
		if(b.IsAligned())
		{
			if(range <= 255)
			{
				seg = b.GetBits(minBitsNeeded);
				bitsDecoded += minBitsNeeded;

                seg[0] >>= (8 - minBitsNeeded);
				l_intval = (long)seg[0];
			}
			else if(range == 256)
			{
				bitsDecoded += b.OctetAlignRead();
                seg = b.GetBits(8);
                bitsDecoded += 8;

				l_intval = (long)seg[0];
			}
			else if(range > 256 && range < 65536)
			{
				bitsDecoded += b.OctetAlignRead();
				seg = b.GetBits(16);
                bitsDecoded += 16;
				l_intval = (long)seg[0];
				l_intval <<= 8;
				l_intval |= (long)seg[1];
			}
			else if(range >= 65536)
			{
				minBitsNeeded /= 8;

				if( (minBitsNeeded % 8) != 0 )
				{	
					minBitsNeeded += 1;
				}
				
				minBitsNeeded -= 1;

				seg = b.GetBits(minBitsNeeded);
                bitsDecoded += minBitsNeeded;
				
				seg[0] >>= 8 - (minBitsNeeded % 8);
				numBytes = (long)seg[0];
				numBytes += 1;

                delete [] seg;
				if(numBytes > 4)
					throw EXCEPT("integer is too big for decoded", INTEGER_ERROR);
                
				seg = b.GetBits(numBytes * 8);
                bitsDecoded += (numBytes * 8);

				l_intval = seg[0];

				count = 1;
				while(count < numBytes)
				{
					l_intval <<= 8;
					l_intval |= (long)seg[count];
					count++;
				}

			}
		}
		else
		{
			seg = b.GetBits(minBitsNeeded);
			bitsDecoded += minBitsNeeded;
            
			numBytes = minBitsNeeded / 8;
			
			oddBits = (minBitsNeeded % 8);

			if( oddBits == 0)
			{	
				l_intval = (long)seg[0];
				count = 1;
				while(count < numBytes)
				{
					l_intval <<= 8;
					l_intval |= (long)seg[count];
					count++;
				}
 
			}
			else
			{
				m_len = numBytes + 2;
				m_bytes = new unsigned char[m_len];	
				
				count = numBytes;
				while(count > 0)
				{
					seg[count] >>= (8 - oddBits);
					pChar[0] = seg[count - 1];
					pChar[0] <<= oddBits;
					seg[count] |= pChar[0];
					count--;
				}
				seg[count] >>= (8 - oddBits);

				l_intval = (long)seg[0];
				count = 1;
				while(count < numBytes + 1)
				{
					l_intval <<= 8;
					l_intval |= (long)seg[count];
					count++;
				}
			}

		}
	}

	l_intval += lowerBound;
	Set(l_intval);

    delete [] seg;
}


AsnLen AsnInt::Interpret(AsnBufBits &b, long offset)const
{
	AsnLen len = 8;
	unsigned char* pEncodedVal = NULL;
	unsigned char c;
	
	c = getByte(offset);

	pEncodedVal = &c;

	b.PutBits(pEncodedVal, len);
	
	return len;
}

void AsnInt::Deterpret(AsnBufBits &b, AsnLen &bitsDecoded, long offset)
{
	unsigned char* seg;
	seg = b.GetBits(8);
    bitsDecoded += 8;
	
	putByte(offset, seg[0]);
    delete [] seg;
}


AsnLen AsnInt::PEnc(AsnBufBits &b)const
{
    FUNC("AsnInt::PEnc");

    int numValueRanges;
    const ValueRange* valueRanges = ValueRanges(numValueRanges);
	AsnLen len = 0;
	int x = 0;
	int upperBoundFound = 0;
	int lowerBound = 0;
	int upperBound = 0;

    if (checkConstraints(NULL) != 0)
        throw ConstraintException("Integer not within constraints", STACK_ENTRY);

    if(numValueRanges <= 0) {
        len = EncodeGeneral(b);
    } else {
        lowerBound = valueRanges[x].lowerBound;
        upperBound = lowerBound;
        upperBoundFound = valueRanges[x].upperBoundExists;

        for(x = 0; x < numValueRanges; x++) {
            if(lowerBound > valueRanges[x].lowerBound) {
                lowerBound = valueRanges[x].lowerBound;
            }

            if(upperBound < valueRanges[x].lowerBound) {
                upperBound = valueRanges[x].lowerBound;
            }

            if(valueRanges[x].upperBoundExists == 1) {
                upperBoundFound = 1;

                if(upperBound < valueRanges[x].upperBound) {
                    upperBound = valueRanges[x].upperBound;
                }
            }
        }

        if(upperBound > lowerBound)
            upperBoundFound = 1;

        if(upperBoundFound == 1) {
            if(lowerBound != upperBound)
                len = PEncFullyConstrained(b, lowerBound, upperBound);
        } else {
            if(numValueRanges == 1 && valueRanges[0].upperBoundExists == 2 ) {
                if((long)*this != lowerBound) {
                    throw EXCEPT("integer does not match singlevalue size constraint",
                                 INTEGER_ERROR);
                }
            } else {
                len = PEncSemiConstrained(b, lowerBound);
            }
        }

    }

    return len;
}

/*PER encoding of an semi-constrained integer*/
AsnLen AsnInt::PEncSemiConstrained (AsnBufBits &b, long lowerBound)const
{

	FUNC("AsnInt::PEnc(...Semi-Constrained Int...)");

	AsnLen len = 0;
	long tempval;
	AsnInt tempInt = AsnInt();
	
	if(m_len>4)
	{
		throw EXCEPT("integer is too big for encoding from offset",
				INTEGER_ERROR);
	}

	tempval = *this;

	tempval -= lowerBound;

	tempInt = tempval;

	if((tempInt.m_len) > 1 && (tempInt.m_bytes[0]) == 0x00)
	{
		memmove(tempInt.m_bytes, tempInt.m_bytes + 1, tempInt.m_len - 1);
		tempInt.m_len--;
	}

	if(tempInt.m_len > 4)
	{
		throw EXCEPT("offset from lower bound too large to be encoded",
				INTEGER_ERROR);
	}

	len = tempInt.EncodeGeneral(b);

	return len;
}

/*PER encoding of constrained integer types*/ 
AsnLen AsnInt::PEncFullyConstrained(AsnBufBits &b, long lowerBound, long upperBound)const
{
	FUNC("AsnInt::PEnc(...Constrained Int...)");
	unsigned long range = ((upperBound - lowerBound) + 1);
	AsnLen len = 0;
	long tempval;
	AsnInt tempInt;
	AsnInt tempInt2;
	int minBitsNeeded = 0;
	long tempRange = range;
	int oddBits = 0;
	unsigned char pChar[] = {0x00, 0x00, 0x00, 0x00};

	if(m_len>4)
	{
		throw EXCEPT("decimal string is too big to convert to an integer",
				INTEGER_ERROR);
	}

	tempval = *this;
	
	if( tempval < lowerBound || tempval > upperBound )
	{
		throw EXCEPT("Integer is out of range",
				INTEGER_ERROR);
	}
	
	tempval -= lowerBound;

	tempInt.Set(tempval);

	if((tempInt.m_len) > 1 && (tempInt.m_bytes[0]) == 0x00)
	{
		memmove(tempInt.m_bytes, tempInt.m_bytes + 1, tempInt.m_len - 1);
		tempInt.m_len--;
	}

	if( range <= 0 )
	{
		throw EXCEPT("upperBound cannot be smaller than lowerBound",
				INTEGER_ERROR);
	}

	if(tempInt.m_len > 4)
	{
		throw EXCEPT("offset from lower bound too large to be encoded",
				INTEGER_ERROR);
	}

	if(range != 1)
	{
		tempRange -= 1;
		while(tempRange > 0)
		{
			tempRange -= (long)(1 << minBitsNeeded);
			minBitsNeeded += 1;
		}
	
		if(b.IsAligned())
		{
			if(range <= 255)
			{
				pChar[0] = tempInt.m_bytes[0];
				pChar[0] <<=  8 - minBitsNeeded;
				len += b.PutBits(pChar, minBitsNeeded);
			}
			else if(range == 256)
			{
				len += b.OctetAlignWrite();
				pChar[0] = tempInt.m_bytes[0];
				len += b.PutBits(pChar, 8);
			}
			else if(range > 256 && range < 65536)
			{
				len += b.OctetAlignWrite();
				if(tempInt.m_len < 2)
				{
					pChar[0] = 0x00;
					len += b.PutBits(pChar, 8);
				}

				len += b.PutBits(tempInt.m_bytes, tempInt.m_len * 8);
			}
			else if(range >= 65536)
			{
				minBitsNeeded /= 8;

				if( (minBitsNeeded % 8) != 0 )
				{	
					minBitsNeeded += 1;
				}
				
				minBitsNeeded -= 1;

				tempval = tempInt.m_len - 1;

				tempInt2.Set(tempval);

				if((tempInt2.m_len) > 1 && (tempInt2.m_bytes[0]) == 0x00)
				{
					memmove(tempInt2.m_bytes, tempInt2.m_bytes + 1, tempInt2.m_len - 1);
					tempInt2.m_len--;
				}

				if((unsigned)minBitsNeeded > (tempInt2.m_len * 8))
				{
					len += b.PutBits(pChar, minBitsNeeded - (tempInt2.m_len * 8)); 
					minBitsNeeded -= (minBitsNeeded - (tempInt2.m_len * 8));
				}

                //TBD, NOT SURE IF THIS LOGIC WORKS CORRECTLY
				oddBits = (minBitsNeeded % 8);
				if( oddBits== 0)
				{
					len += b.PutBits(tempInt2.m_bytes, tempInt2.m_len * 8);
				}
				else
				{
					pChar[0] = tempInt2.m_bytes[0];
					pChar[0] <<=  8 - oddBits;
					len += b.PutBits(pChar, oddBits);
					
					if(tempInt2.m_len > 1)
					{
						len += b.PutBits(&tempInt2.m_bytes[1], (tempInt2.m_len - 1) * 8);
					}
				}
				
				len += b.PutBits(tempInt.m_bytes, tempInt.m_len * 8);
			}
		}
		else
		{
			if((unsigned)minBitsNeeded > (tempInt.m_len * 8))
			{
				len += b.PutBits(pChar, minBitsNeeded - (tempInt.m_len * 8)); 
				minBitsNeeded -= (minBitsNeeded - (tempInt.m_len * 8));
			}
			
			oddBits = (minBitsNeeded % 8);
			if( oddBits== 0)
			{
				len += b.PutBits(tempInt.m_bytes, tempInt.m_len * 8);
			}
			else
			{
				pChar[0] = tempInt.m_bytes[0];
				pChar[0] <<=  8 - oddBits;
				len += b.PutBits(pChar, oddBits);
				
				if(tempInt.m_len > 1)
				{
					len += b.PutBits(&tempInt.m_bytes[1], (tempInt.m_len - 1) * 8);
				}
			}
			
		}
	}

	return len;
}

_END_SNACC_NAMESPACE
