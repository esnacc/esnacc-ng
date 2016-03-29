// file: .../c++-lib/src/asn-tag.C -  ASN.1 tag manipulation routines
//
// MS 92/06/18
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

// $Header: /baseline/SNACC/c++-lib/src/asn-tag.cpp,v 1.15 2004/03/25 19:20:16 gronej Exp $


#include "asn-incl.h"

using namespace SNACC;


/*returns the number of bytes that are in the encoded tag passed in*/
long SNACC::BytesInTag(AsnTag tag)
{
    if( (tag & 0x00FFFFFF) == 0 )
    {
        return 1;
    }
    else if( (tag & 0x0000FFFF) == 0 )
    {
        return 2;
    }
    
    else if ( (tag & 0x000000FF) == 0 )
    {
        return 3;
    }
    else 
    {
        return 4;
    }
}


/*
 * Decode a BER Tag from the given buffer.  Error is
 * flagged if the tag is too long or if a read error occurs.
 */
AsnTag SNACC::BDecTag(const AsnBuf &b, AsnLen &bytesDecoded)
{
	FUNC("BDecTag()");

	// Read first byte and shift it into the AsnTag
	unsigned char tagByte = b.GetUByte();
	AsnTag tagId = tagByte << ((TB - 1) * 8);
	bytesDecoded++;

	// Check if long tag format (last 5 bits all set) */
	if ((tagByte & 0x1f) == 0x1f)
	{
		int i = 1;
		do
		{
			// Read next tag byte
			tagByte = b.GetUByte();
			tagId |= tagByte << ((TB - i - 1) * 8);
			bytesDecoded++;
			i++;
		}
		while ((tagByte & 0x80) && ((unsigned)i < TB));

		// Check that tag isn't too long
		if (tagByte & 0x80)
			throw EXCEPT("Tag value overflow", INVALID_TAG);
	}

	return tagId;
} /* BDecTag */


/*
 * Function:  PDecTag()
 * Decodes a BER Tag from the given PER buffer and updates the
 * number of bits decoded parameter.  Exceptions are thrown if
 * the tag is too long or if a read error occurs.
 */
AsnTag SNACC::PDecTag(AsnBufBits &bufBits, AsnLen &bitsDecoded)
{
	FUNC("PDecTag()");

	// Read first byte and shift it into the AsnTag
	unsigned char tagByte = bufBits.GetByte();
	AsnTag tagId = tagByte << ((TB - 1) * 8);
	bitsDecoded += 8;

	// Check if long tag format (last 5 bits all set) */
	if ((tagByte & 0x1f) == 0x1f)
	{
		int i = 1;
		do
		{
			// Read next tag byte
			tagByte = bufBits.GetByte();
			tagId |= tagByte << ((TB - i - 1) * 8);
			bitsDecoded += 8;
			i++;
		}
		while ((tagByte & 0x80) && ((unsigned)i < TB));

		// Check that tag isn't too long
		if (tagByte & 0x80)
			throw EXCEPT("Tag value overflow", INVALID_TAG);
	}

	return tagId;
} // end of PDecTag()

//
//RWC;  This routine returns the number of bits encoded.
AsnLen SNACC::PEncTag(AsnBufBits &b, unsigned char ucClass, unsigned char form, long code, long lByteCount)
{
    unsigned char ucChar;
    /*#define BEncTag5( b, class, form, code)
    b.PutByteRvs ((code) & 0x7F);
    b.PutByteRvs ((char)(0x80 | (char)((code) >> 7)));
    b.PutByteRvs ((char)(0x80 | (char)((code) >> 14)));
    b.PutByteRvs ((char)(0x80 | (char)((code) >> 21)));
    b.PutByteRvs ((class) | (form) | 31)*/
    if (lByteCount == 1)
    {
        ucChar = (unsigned char)((ucClass) | (form) | (code));
        b.PutBits(&ucChar, 8 );
    }
    else
    {
        ucChar = (unsigned char)(ucClass | form | 31);
        b.PutBits(&ucChar, 8);

        if (lByteCount == 2)
           b.PutBits((unsigned char *)&code, 8);
        else        // MUST be 3, 4, or 5
        {
           
           if (lByteCount >= 5)
           {
               ucChar = (unsigned char)(0x80 | (unsigned char)((code) >> 21));
               b.PutBits(&ucChar, 8);
           }

           if (lByteCount >= 4)
           {
               ucChar = (unsigned char)(0x80 | (char)((code) >> 14));
               b.PutBits(&ucChar, 8);
           }

           ucChar = (unsigned char)(0x80 | (unsigned char)((code) >> 7));
           b.PutBits(&ucChar, 8);

           ucChar = (unsigned char)(code & 0x7F);
           b.PutBits(&ucChar, 8);
                     
        }      // END IF 2
        
    }          // END IF 1

    return(lByteCount*8);
}   /* PEncTag */
