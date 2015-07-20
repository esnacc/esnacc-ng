// file: .../c++-lib/src/asn-bits.C - AsnBits (ASN.1 BIT STRING) Type
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


/*#include <string>*/
#include "asn-incl.h"

#include <stdlib.h>

_BEGIN_SNACC_NAMESPACE

char SNACCDLL_API numToHexCharTblG[16] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f'};


AsnBits::AsnBits(const char *stringForm)
{
   bits=NULL;
   nblFlag = false; 
   bitLen = 0;
   if (stringForm)
      operator=(stringForm);
}

AsnBits::~AsnBits()
{
  delete[] bits;
}


// Initializes the bits string with a bit string numBits in length.
// All bits are zeroed.
void AsnBits::Set (size_t numBits)
{
  if (numBits > 0)
  {
     bitLen = numBits;
     size_t octetLen = (bitLen+7)/8;

     if (bits)
         delete[] bits;
     bits = new unsigned char[octetLen];
     memset (bits, 0, octetLen); // init to zeros
  }
}

// initializes a BIT STRING with the given string and bit length
// Copies the bits from bitsOcts.
void AsnBits::Set (const unsigned char *bitOcts, size_t numBits)
{
    if (bitOcts != bits)
    {
        if (bits)
            delete[] bits;
        bitLen = numBits;
        size_t octetLen = (bitLen+7)/8;
        bits = new unsigned char[octetLen];
        memcpy (bits, bitOcts, octetLen);
    }
}

// initializes a BIT STRING by copying another BIT STRING's bits
void AsnBits::Set (const AsnBits &b)
{
	if (&b != this)
	{
		delete[] bits;
		bits = NULL;

		bitLen = b.bitLen;
		nblFlag = b.nblFlag;

		size_t octetLen = (bitLen+7)/8;
		if (octetLen)
		{
			bits = new unsigned char[octetLen];
			memcpy (bits, b.bits, octetLen);
		}
	}
}

// PIERCE 8-28-2001 removed ReSet members
//
#if 0
// Initializes the bits string with a bit string numBits in length.
// All bits are zeroed.
void AsnBits::ReSet (size_t numBits)
{
  delete[] bits;
  Set (numBits);
}

// frees old bits value and then re-initializes the
// BIT STRING with the given string and bit length
// Copies the bitOcts into bits.
void AsnBits::ReSet (const char *bitOcts, size_t numBits)
{
  if (bitOcts != bits)
  {
    delete[] bits;
    Set (bitOcts, numBits);
  }
}

// frees old bits value and then re-initializes the
// BIT STRING by copying another BIT STRING's bits
void AsnBits::ReSet (const AsnBits &b)
{
  if (&b != this)  // avoid b = b; probs
  {
    delete[] bits;
    Set (b);
  }
}
#endif

// Returns true if the given BIT STRING is the same as this one
bool AsnBits::BitsEquiv (const AsnBits &ab) const
{
  size_t octetsLessOne = (bitLen-1)/8;
  size_t unusedBits;
  
  unusedBits = bitLen % 8;
  if (unusedBits != 0)
     unusedBits = 8 - unusedBits;

  if (!bitLen && !ab.bitLen)
    return true;

  // trailing bits may not be significant
  return bitLen == ab.bitLen
    && !memcmp (bits, ab.bits, octetsLessOne)
    && (bits[octetsLessOne] & (0xFF << unusedBits)) == (ab.bits[octetsLessOne] & (0xFF << unusedBits));
}  /* AsnBits::BitsEquiv */

void AsnBits::SetSize(size_t newsize)
{

    if(bitLen < newsize)
    {
        size_t newLength = (newsize + 7) / 8;
        unsigned char *tmpBits = new unsigned char[ newLength ];
        memset(tmpBits, 0, newLength);
        memcpy((char*)tmpBits, (const char*)bits, std::min(length(), newLength));
        delete [] bits;
        bits = new unsigned char[ newLength ];
        memcpy((char*)bits, (const char*)tmpBits, newLength);
        delete [] tmpBits;
    }
    
    bitLen = newsize;
}

// set given bit to 1. Most signif. bit is bit 0, least signif bit is bitLen-1
void AsnBits::SetBit (size_t bit)
{
    FUNC("AsnBits::SetBit");

  if (bit < bitLen)
  {
    size_t octet = bit/8;
    size_t octetsBit = 7 - (bit % 8);	// bit zero is first/most sig bit in octet
    bits[octet] |= 1 << octetsBit;
  }
  else
  {
    throw ParameterException("Parameter is larger than BIT STRING size", 
         STACK_ENTRY);
  }
} /* AsnBits::SetBit */

// Clr bit. Most signif. bit is bit 0, least signif bit is bitLen-1
void AsnBits::ClrBit (size_t bit)
{
    FUNC("AsnBits::ClrBit");

  if (bit < bitLen)
  {
    size_t octet = bit/8;
    size_t octetsBit = 7 - (bit % 8);	// bit zero is first/most sig bit in octet
    bits[octet] &= ~(1 << octetsBit);
  }
  else
  {
    throw ParameterException("Parameter is larger than BIT STRING size", 
         STACK_ENTRY);
  }
} /* AsnBits::ClrBit */

// returns given bit. Most signif. bit is bit 0, least signif bit is bitLen-1.
// Returns false if the given bit index is out of range.
bool AsnBits::GetBit (size_t bit) const
{
  if (bit < bitLen)
  {
    size_t octet = bit/8;
    size_t octetsBit = 7 - (bit % 8);	// bit zero is first/most sig bit in octet
    return !!(bits[octet] & (1 << octetsBit));
  }
  return false;
}  /* AsnBits::GetBit */


// Returns true if the bit string is empty
bool AsnBits::IsEmpty () const
{
  size_t i = 0;

  if (bits == NULL)
      return true;
  if (bitLen <= 0)
      return true;
  for (i = 0; i < bitLen; i++)
  {
     if (GetBit(i) == 1)
         return false;
  }
  return true;
}  /* AsnBits::IsEmpty */


// Encodes the content (included unused bits octet) of the BIT STRING
// to the given buffer.
AsnLen AsnBits::BEncContent (AsnBuf &b) const
{
    size_t unusedBits;
    size_t byteLen;

    unsigned int i;
    //bool bStop;
    
    // IF bitstring is a NamedBitList 
    if (nblFlag)
    {

       // Calculate last octet.  
       // 
       size_t finalOctet;
       if (bitLen <= 8)
          finalOctet = 0;
       else if (bitLen % 8 == 0)
          finalOctet = ((bitLen / 8) - 1);
       else 
          finalOctet = bitLen / 8;

       // The final octet is the last octet which has at least 
       // one bit set.  Loop backwards starting with the
       // last octet (calculated above) until you find an
       // that has at least one bit set (it's value is not 0).
       //
       if (bits != NULL)
          for (; finalOctet > 0 && bits[finalOctet] == 0; finalOctet--);

       // Calculate unsigned bits in final octet
       //
       if (bits == NULL || (finalOctet == 0 && bits[0] == 0))
       {
          byteLen = 0;
          unusedBits = 0;
       }
       else
       {
          // Calculate how many trailing bits are unset in the
          // last octet.
          unusedBits=0;
          for (i = 0; i < 8 && ! (bits[finalOctet] & (0x01 << i)); i++)
              unusedBits++;
          byteLen = finalOctet + 1;
       }
    } 
    else 
    {
       // If this is not a NamedBitList Calculate the unusedBits
       // as ( (BitLen() / 8) + 1) * 8 ) - BitLen();
       //
       // In other words it's the number of bits not used by
       // the BIT STRING specification, not the number of unset 
       // bits in the the final subsequent octet.
       unusedBits = bitLen % 8;
       if (unusedBits != 0)
          unusedBits = 8 - unusedBits;
       
       byteLen = (bitLen+7)/8;
    }
    b.PutSegRvs (bits, byteLen);

//    unusedBits = (bitLen % 8);
//    if (unusedBits != 0)
//        unusedBits = 8 - unusedBits;
    b.PutByteRvs ((unsigned char)unusedBits);

    return byteLen + 1;

} /* AsnBits::BEncContent */


// Decodes a BER BIT STRING from the given buffer and stores
// the value in this object.
void AsnBits::BDecContent (const AsnBuf &b, AsnTag tagId, AsnLen elmtLen, AsnLen &bytesDecoded)
{
   FUNC("AsnBits::BDecContent");

	if (elmtLen == INDEFINITE_LEN || elmtLen > b.length())
	{
	   throw MemoryException(elmtLen, "elmtLen requests for too much data", STACK_ENTRY);
	}

   /*
    * tagId is encoded tag shifted into long int.
    * if CONS bit is set then constructed bit string
    */
   if (tagId & 0x20000000)
     BDecConsBits (b, elmtLen, bytesDecoded);

   else /* primitive octet string */
   {
     if (elmtLen == INDEFINITE_LEN)
        throw BoundsException("indefinite length on primitive", STACK_ENTRY);

	 if (elmtLen > b.length() || elmtLen <= 0)
        throw BoundsException("length problem on decoding content", STACK_ENTRY);

     bytesDecoded += elmtLen;
     elmtLen--;

	 unsigned int iUnusedBitLen= (unsigned int)b.GetByte();
	 if (iUnusedBitLen > 7)
        throw BoundsException("Length problem - Unused bits > 7", STACK_ENTRY);

     bitLen = (elmtLen * 8) - iUnusedBitLen;
     bits =  new unsigned char[elmtLen];
     b.GetUSeg (bits, elmtLen);
   }

} /* AsnBits::BDecContent */

size_t AsnBits::encLen() const
{
	size_t unusedBits;
    size_t byteLen;

    unsigned int i;
    //bool bStop;
    
    // IF bitstring is a NamedBitList 
    if (nblFlag)
    {

       // Calculate last octet.  
       // 
       size_t finalOctet;
       if (bitLen <= 8)
          finalOctet = 0;
       else if (bitLen % 8 == 0)
          finalOctet = ((bitLen / 8) - 1);
       else 
          finalOctet = bitLen / 8;

       // The final octet is the last octet which has at least 
       // one bit set.  Loop backwards starting with the
       // last octet (calculated above) until you find an
       // that has at least one bit set (it's value is not 0).
       //
       if (bits != NULL)
          for (; finalOctet > 0 && bits[finalOctet] == 0; finalOctet--);

       // Calculate unsigned bits in final octet
       //
       if (bits == NULL || (finalOctet == 0 && bits[0] == 0))
       {
          byteLen = 0;
          unusedBits = 0;
       }
       else
       {
          // Calculate how many trailing bits are unset in the
          // last octet.
          unusedBits=0;
          for (i = 0; i < 8 && ! (bits[finalOctet] & (0x01 << i)); i++)
              unusedBits++;

          byteLen = finalOctet + 1;
       }
	}
	else
	{
		return BitLen();
	}
	
	return ( (byteLen * 8) - unusedBits);
}

AsnLen AsnBits::EncodeGeneral(AsnBufBits &b)const
{
	AsnLen len = 0;
	unsigned long l_64kFrag = l_16k * 4;
	unsigned long count = 0;
	unsigned long x = 0;
	unsigned long tempLen = encLen();
	unsigned char ch = 0x00;
	unsigned char *c = NULL;
	long offset = 0;

	if(tempLen >= l_16k)
	{	
		/*there is more than 16k bits of data*/
		count = (tempLen / l_64kFrag);
		
		for(x=0; x < count; x++)
		{  
			len += b.OctetAlignWrite();
			
			len += PEncLen_16kFragment(b, 4);
				
			len += b.OctetAlignWrite();
			
			len += b.PutBits((unsigned char*)&bits[offset / 8], l_64kFrag);
			
			offset += l_64kFrag;
		}

		tempLen -= count * l_64kFrag;

		count = tempLen / l_16k;

		if(count != 0)
		{  
			len += b.OctetAlignWrite();
			
			len += PEncLen_16kFragment(b, count);
		
			len += b.OctetAlignWrite();
						
			len += b.PutBits((unsigned char*)&bits[offset / 8], (count * l_16k) );

			offset += (count * l_16k);
		}
		
		tempLen -=  (l_16k * count);

		if(tempLen == 0)
		{
			ch = 0x00;
			c = &ch;
			
			len += b.OctetAlignWrite();

			len += b.PutBits(c, 8);
			
			return len;
		}
	}

	/*if there are less than 128 bits of data*/
	if(tempLen < 128)
	{
		len += b.OctetAlignWrite();

		len += PEncDefLenTo127(b, tempLen);
		
		len += b.OctetAlignWrite();

		len += b.PutBits((unsigned char*)&bits[offset / 8], tempLen);

		offset += tempLen;
	}
	else if(tempLen >= 128 && tempLen < l_16k)
	{
		len += b.OctetAlignWrite();
		/*if there is less than 16k bits of data*/
		/*and more than 127 bits of data*/
		len += PEncLen_1to16k(b, tempLen);
		
		len += b.OctetAlignWrite();
		
		len += b.PutBits((unsigned char*)&bits[offset / 8], tempLen);

		offset += tempLen;
	}

	return len;
}

void AsnBits::Allocate(long size)
{
	char* temp = new char[(((bitLen + size)/ 8) + 1)];
	memcpy(temp, bits, length());
	size += bitLen;
	
	clear();

	bitLen = size;
	bits = new unsigned char[((bitLen / 8) + 1)];
	memcpy(bits, temp, length());
    delete [] temp;
}

void AsnBits::DecodeGeneral(AsnBufBits &b, AsnLen &bitsDecoded)
{
	unsigned char* seg;
	unsigned long templen = 0;
	long offset = 0;
	
	clear();

	bitsDecoded += b.OctetAlignRead();
	
	seg = (unsigned char*)b.GetBits(8);
	bitsDecoded += 8;

	while((seg[0] & 0xC0) == 0xC0)
	{
		seg[0] &= 0x3F;
		templen = (unsigned long)seg[0];
		templen *= l_16k;
	
		b.OctetAlignRead();
		
		Allocate(templen);

        delete [] seg;
        seg = b.GetBits(templen);
		memcpy(&bits[offset / 8], seg, ((templen + 7) / 8));
		bitsDecoded += templen;

		bitsDecoded += b.OctetAlignRead();

		offset += templen;
        delete [] seg;
		seg = (unsigned char*)b.GetBits(8);
		bitsDecoded += 8;
	}
	
	if((seg[0] & 0xC0) == 0x80)
	{
		seg[0] &= 0x3F;
		templen = (unsigned long)seg[0];
		templen <<= 8;
        delete [] seg;
		seg = (unsigned char*)b.GetBits(8);
        bitsDecoded += 8;
		templen |= (unsigned long)seg[0];
		
		bitsDecoded += b.OctetAlignRead();

		Allocate(templen);

        delete [] seg;
        seg = b.GetBits(templen);
		memcpy(&bits[offset / 8], seg, ((templen + 7) / 8));
        bitsDecoded += templen;

		offset += templen;
	}
	else if((seg[0] & 0x80) == 0x00)
	{
		seg[0] &= 0x7F;
		templen = (unsigned long)seg[0];
		
		bitsDecoded += b.OctetAlignRead();

		Allocate(templen);
		
        delete [] seg;
        seg = b.GetBits(templen);
		memcpy(&bits[offset / 8], seg, ((templen + 7) / 8));
		bitsDecoded += templen;

		offset += templen;
	}

    delete [] seg;
}

long AsnBits::FindSizeConstraintBounds(int &iSCLowerBound, int &iSCUpperBound)const
{
	int count = 0;
    int numsizeconstraints;
    const SizeConstraint* sizeConstraints = SizeConstraints(numsizeconstraints);

	while(count < numsizeconstraints)
	{	
		if((unsigned)iSCUpperBound < sizeConstraints[count].lowerBound)
		{
			iSCUpperBound = sizeConstraints[count].lowerBound;
		}

		if( sizeConstraints[count].upperBoundExists == 1 && 
			(unsigned)iSCUpperBound < sizeConstraints[count].upperBound)
		{
			iSCUpperBound = sizeConstraints[count].upperBound;
		}

		if( (unsigned)iSCLowerBound > sizeConstraints[count].lowerBound )
		{
			iSCLowerBound = sizeConstraints[count].lowerBound;
		}

		count++;
	}

	return ( (iSCUpperBound - iSCLowerBound) + 1);
}

AsnLen AsnBits::EncodeWithSizeConstraint (AsnBufBits &b)const
{
	AsnLen len = 0;
    int numSizeConstraints;
    const SizeConstraint* sizeConstraints = SizeConstraints(numSizeConstraints);
	int iSCLowerBound = sizeConstraints[0].lowerBound;
	int iSCUpperBound = iSCLowerBound;
	int minBitsNeeded = 0;
	int minBytesNeeded = 0;
	long Range = FindSizeConstraintBounds(iSCLowerBound, iSCUpperBound);
	long tempRange = Range - 1;
	long size = bitLen;
	unsigned char* temp = NULL;
	long tempLength = 0;
	unsigned char* pStr = new unsigned char[1];

	while(tempRange > 0)
	{
		tempRange -= (long)(1 << minBitsNeeded);
		minBitsNeeded += 1;
	}

	if(Range > 1)
	{
		if( (iSCUpperBound > 16) && b.IsAligned())
		{
			len += b.OctetAlignWrite();
		}

        if(size > iSCUpperBound)
        {
            size = iSCUpperBound;
        }

        if(size < iSCLowerBound)
        {
            size = iSCLowerBound;
        }

		minBytesNeeded = minBitsNeeded / 8;
		minBitsNeeded = minBitsNeeded % 8;
		size -= iSCLowerBound;

		if(minBytesNeeded > 0)
		{
			pStr[0] = (unsigned char)(size >> minBitsNeeded);
			len += b.PutBits(pStr, 8);
		}

		pStr[0] = (unsigned char)size;
		pStr[0] <<= 8 - minBitsNeeded;
		len += b.PutBits(pStr, minBitsNeeded);
	}
	
	if( (iSCUpperBound > 16) && b.IsAligned())
	{
		len += b.OctetAlignWrite();
	}
	
	if((unsigned)bitLen < (unsigned)iSCLowerBound)
	{
		tempLength = iSCLowerBound - bitLen;
		len += b.PutBits((unsigned char*)bits, bitLen);
		temp = new unsigned char[(tempLength + 7) / 8];
		memset(temp, 0, (tempLength + 7) / 8);
		len += b.PutBits(temp, tempLength);
	}
	else if((unsigned)bitLen > (unsigned)iSCUpperBound)
	{
		len += b.PutBits((unsigned char*)bits, iSCUpperBound);
	}
	else
	{
		len += b.PutBits((unsigned char*)bits, bitLen);
	}

    delete [] pStr;
	return len;
}

void AsnBits::DecodeWithSizeConstraint(AsnBufBits &b, AsnLen &bitsDecoded)
{
	FUNC("AsnBits::DecodeWithSizeConstraint");
    
    int numSizeConstraints;
    const SizeConstraint* sizeConstraints = SizeConstraints(numSizeConstraints);
	int iSCLowerBound = sizeConstraints[0].lowerBound;
	int iSCUpperBound = iSCLowerBound;
	int minBitsNeeded = 0;
	int minBytesNeeded = 0;
	long Range = FindSizeConstraintBounds(iSCLowerBound, iSCUpperBound);
	long tempRange = Range - 1;
	long decodeSize = 0;
    unsigned char* seg;
	unsigned char* pStr = new unsigned char[1];

	clear();

	while(tempRange > 0)
	{
		tempRange -= (long)(1 << minBitsNeeded);
		minBitsNeeded += 1;
	}
	
	
	if(Range > 1)
	{
		if( (iSCUpperBound > 16) && b.IsAligned())
		{
			bitsDecoded += b.OctetAlignRead();
		}

		minBytesNeeded = minBitsNeeded / 8;
		minBitsNeeded = minBitsNeeded % 8;

		if(minBytesNeeded > 0)
		{
            delete [] pStr;
			pStr = b.GetBits(8);
            bitsDecoded += 8;
			decodeSize <<= 8;
			decodeSize |= (long)pStr[0];
		}

        delete [] pStr;
		pStr = b.GetBits(minBitsNeeded);
        bitsDecoded += minBitsNeeded;

		if(minBitsNeeded > 0)
		{
			decodeSize <<= minBitsNeeded;
			pStr[0] >>= (8 - minBitsNeeded);
			decodeSize |= (long)pStr[0];
		}
	}

	decodeSize += iSCLowerBound;

	if(decodeSize > iSCUpperBound)
	{
        delete [] pStr;
		throw EXCEPT("String size not withing restricted bounds", RESTRICTED_TYPE_ERROR);
	}


	if( (iSCUpperBound > 16) && b.IsAligned())
	{
		bitsDecoded += b.OctetAlignRead();
	}
	
	bitLen = decodeSize;
	bits = new unsigned char[((decodeSize + 7) /8)];
    seg = b.GetBits(decodeSize);
	memcpy(bits, seg, ((decodeSize + 7) / 8));
    bitsDecoded += decodeSize;
    delete [] seg;
    delete [] pStr;
    
}

AsnLen AsnBits::PEnc (AsnBufBits &b) const
{
	AsnLen len = 0;
    int numSizeConstraints;
    const SizeConstraint* sizeConstraints = SizeConstraints(numSizeConstraints);
	if(sizeConstraints == NULL && numSizeConstraints == 0 )
	{
		len = EncodeGeneral(b);
	}
	else
	{
		len = EncodeWithSizeConstraint(b);
	}

	return len;
}

void AsnBits::PDec (AsnBufBits &b, AsnLen &bitsDecoded) 
{
    int numSizeConstraints;
    const SizeConstraint* sizeConstraints = SizeConstraints(numSizeConstraints);

    if(sizeConstraints == NULL && numSizeConstraints == 0 )
	{
		DecodeGeneral(b, bitsDecoded);
	}
	else
	{
		DecodeWithSizeConstraint(b, bitsDecoded);
	}

}

AsnLen AsnBits::BEnc (AsnBuf &b) const
{
    AsnLen l;
    l =  BEncContent (b);
    l += BEncDefLen (b, l);
    l += BEncTag1 (b, UNIV, PRIM, BITSTRING_TAG_CODE);
    return l;
}

void AsnBits::BDec (const AsnBuf &b, AsnLen &bytesDecoded)
{
   FUNC("AsnBits::BDec");

   AsnLen elmtLen;
   AsnTag tag;

   tag = BDecTag (b, bytesDecoded);
   if ((tag != MAKE_TAG_ID (UNIV, PRIM, BITSTRING_TAG_CODE))
   && (tag != MAKE_TAG_ID (UNIV, CONS, BITSTRING_TAG_CODE)))
   {
      throw InvalidTagException(typeName(), tag, STACK_ENTRY);
   }
   elmtLen = BDecLen (b, bytesDecoded);
   BDecContent (b, tag, elmtLen, bytesDecoded);
}

/*
 * decodes a seq of universally tagged bits until either EOC is
 * encountered or the given len decoded.  Return them in a
 * single concatenated bit string
 */
void AsnBits::BDecConsBits (const AsnBuf &b, AsnLen elmtLen, AsnLen &bytesDecoded)
{
   FUNC("AsnBits::BDecConsBits ()");

   ConsStringDeck deck(BITSTRING_TAG_CODE);
   ConsStringDeck::iterator i;

   deck.Fill(b, elmtLen, bytesDecoded);
   
   // Check all but the last card to make sure it's set to zero
   // 
   // This enforces the constructed BIT STRING rule which states
   // the unused bits must 0 except for the last component.
   //
   i = deck.begin();
   for (; i < (deck.end() - 1); i++)
   {
      if (i->first[0] != 0)
         throw EXCEPT("Constructed BIT STRING contains a component with a unused bits value other than 0", DECODE_ERROR);
   }
}  /* BDecConsBits */

// prints the BIT STRING to the given ostream.
void AsnBits::Print(std::ostream& os, unsigned short /*indent*/) const
{
    size_t octetLen = (bitLen+7)/8;

    os << "'";
    for (unsigned int i = 0; i < octetLen; i++)
        os << TO_HEX (bits[i] >> 4) << (TO_HEX (bits[i]));
    os << "'H  -- BIT STRING bitlen = " << bitLen << " --";
}

void AsnBits::PrintXML(std::ostream &os, const char *lpszTitle) const
{
   os << "<BIT_STRING>";
   if (lpszTitle)
      os << lpszTitle;
   os << "-";
   Print(os);
   os << "</BIT_STRING>\n";
}


AsnBits & AsnBits::operator=(const char *stringForm)
{
	return SetEqual(stringForm);
}

// operator=(const char *)
//
// create AsnBits object from the BINARY string form
//
// example:
//     '1001'B
//
AsnBits & AsnBits::SetEqual(const char *stringForm)
{
   FUNC("AsnBits::operator=()");

   char *pend = NULL;
   char *pbegin = NULL;
   
   pend = (char *)strstr(stringForm, "'B");

   pbegin = (char *)strstr(stringForm,"'");

   if (pend == NULL || pbegin == NULL)
   {
      throw ParameterException("Invalid string form for BIT STRING", 
         STACK_ENTRY);
   }

   pbegin++;

   // Set length
   Set(pend - pbegin);

   for (int i=0; (pbegin + i) != pend; i++)
   {
      // if ASCII 0
      if (pbegin[i] == '0')
         ClrBit(i);
      // else if ASCII 1
      else if (pbegin[i] == '1')
         SetBit(i);
      else
         // PIERCE THROW
       throw ParameterException("Invalid string form for BIT STRING", 
         STACK_ENTRY);
   }

   nblFlag = true;
   return *this;
}

// soloBitCheck()
//
// determines if the only bit set is 'b'.  This is used to determine whether a
// BIT STRING contains only the default bit set.  If it does then it is not encoded.
//
bool AsnBits::soloBitCheck(size_t b)
{
   bool result = false;

   if (GetBit(b))
   {
      // Clear bit b and check if bit string is empty
      ClrBit(b);
      if (IsEmpty())
      {
         result = true;
      }
      SetBit(b);
   }
   return result;
}

#if META

const AsnBitsTypeDesc AsnBits::_desc (NULL, NULL, false, AsnTypeDesc::BIT_STRING, NULL, NULL);

const AsnTypeDesc *AsnBits::_getdesc() const
{
  return &_desc;
}

#if TCL

int AsnBits::TclGetVal (Tcl_Interp *interp) const
{
  Tcl_ResetResult(interp);
  for (int i=0; i<bitLen; i++)
    Tcl_AppendResult(interp,GetBit (i) ? "1" : "0",NULL);
  return TCL_OK;
}

int AsnBits::TclSetVal (Tcl_Interp *interp, const char *valstr)
{
  int		i;
  const char	*p;

  for (i=0, p=valstr; *p; i++, p++)
    switch (*p)
    {
      case '0':
      case '1':
	break;
      default:
	const char c[2] = { *p, '\0' };
	Tcl_AppendResult (interp, "illegal character '", c, "' for bit in type ", _getdesc()->getmodule()->name, ".", _getdesc()->getname(), NULL);
	Tcl_SetErrorCode (interp, "SNACC", "ILLBIT", NULL);
	return TCL_ERROR;
    }

  ReSet (i);

  for (i=0, p=valstr; i<bitLen; i++, p++)
    *p == '0' ? ClrBit(i) : SetBit(i);

  return TCL_OK;
}

#endif /* TCL */
#endif /* META */

_END_SNACC_NAMESPACE
