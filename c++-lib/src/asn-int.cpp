// file: .../c++-lib/src/asn-int.C - methods for AsnInt (ASN.1 INTEGER) class
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

// $Log: asn-int.cpp,v $
// Revision 1.66  2004/03/25 19:20:16  gronej
// fixed some linux warnings
//
// Revision 1.65  2004/03/22 19:38:13  leonberp
// changed ConstraintErrorStringList to be const to avoid possible memory corruption
//
// Revision 1.64  2004/03/09 16:48:45  gronej
// Updated c++ constraint checking capability and constraint error handling
//
// Revision 1.63  2004/03/01 15:50:45  gronej
// Took out SetConstraints() and member variables for constraints, replaced with functions that return constraint lists
//
// Revision 1.62  2004/02/12 18:55:09  gronej
// Stable SNACC
// non optional choices, sets and sequences are now not pointers
// merged with list code
// all memory leaks within SNACC fixed
//
// Revision 1.61  2004/02/11 19:08:48  nicholar
// Updated Print() function so no longer uses global indent
//
// Revision 1.60  2004/02/06 00:39:13  nicholar
// Changed AsnList to use std::list<T> instead of List<T>
//
// Revision 1.59  2004/02/04 14:59:27  gronej
// Fixed a TON of memory leaks
//
// Revision 1.58  2004/02/03 14:44:32  gronej
// made all constraint lists static to avoid memory issues
//
// Revision 1.57  2003/12/17 19:05:03  gronej
// SNACC baseline merged with PER v1_7 tag
//

// Revision 1.54.2.14  2003/12/15 18:43:50  gronej
// EKMS PER Beta Release Files
//
// Revision 1.54.2.13  2003/12/09 21:20:50  gronej
// Updated to pull working EKMS test space
//
// Revision 1.54.2.12  2003/12/04 20:47:10  gronej
// Moved bAlign out of all PEnc calls and into AsnBufBits as a member
// An AsnBufBits is now invoked with a bAlign parameter defaulted to false
//
// Revision 1.54.2.11  2003/12/03 19:48:08  gronej
// Fixed bitsDecoded to return correct value on decode operations
//
// Revision 1.54.2.10  2003/12/02 18:33:43  gronej
// Updated and debugged PER constraint logic for primitives
//
// Revision 1.54.2.9  2003/12/02 17:00:09  colestor
// Fixed unsetf(...) of HEX setting when printing.
//
// Revision 1.54.2.8  2003/11/26 14:23:17  colestor
// Fixed AsnInt initialization.
//
// Revision 1.54.2.7  2003/11/06 20:23:25  gronej
// Updated PER compiler with working Interpret() and Deterpret() functionality for AsnInt and String Types
//
// Revision 1.54.2.6  2003/11/05 14:58:54  gronej
// working PER code merged with esnacc_1_6
//

// Revision 1.54.2.5  2003/11/04 18:04:08  gronej
// Update PER compiler with some PERGeneral functionality  11/04/03
//
// Revision 1.54.2.4  2003/11/04 14:21:21  gronej
// Update PER compiler with some PERGeneral functionality  11/04/03
//
// Revision 1.54.2.3  2003/10/22 12:45:58  gronej
// Updating PER compiler
//
// Revision 1.54.2.2  2003/10/02 17:15:24  gronej
// Updating PER compiler
//
// Revision 1.54.2.1  2003/08/12 20:04:38  gronej
// no message
//


// Revision 1.54  2003/03/28 14:10:16  leonberp
// added pragmas to get rid of warnings
//
// Revision 1.53  2003/03/17 17:35:16  leonberp
// fixed compile errors for HPUX
//
// Revision 1.52  2003/01/28 17:30:44  leonberp
// added AsnInt copy constructor and operator=
//
// Revision 1.51  2003/01/28 14:23:22  leonberp
// replaced memmove with memcpy
//
// Revision 1.50  2003/01/28 14:10:13  leonberp
// Added AsnInt destructor
//
// Revision 1.49  2003/01/27 20:58:06  leonberp
// enhanced to use dynamically allocated unsigned char[] and length instead of basic_string<> for performance reasons also removed obsolete code
//
// Revision 1.48  2003/01/06 16:20:07  leonberp
// Changed BDec() and BDecContent() to use const AsnBufs
//
// Revision 1.47  2002/12/22 01:20:52  colestor
// (RWC)Updated PrintXML(...) details to better match specification for SEQUENCE OF and SET OF logic.
//
// Revision 1.46  2002/12/19 18:17:54  colestor
// (RWC)Updated to allow integer output as ASCII.
//
// Revision 1.45  2002/12/17 20:27:40  leonberp
// made BEnc() and BEncContent() const
//
// Revision 1.44  2002/11/25 20:21:19  leonberp
// added AsnBuf copy constructor
//
// Revision 1.43  2002/10/23 21:02:48  leonberp
// fixed AsnBuf references and fixed clock skew problem
//
// Revision 1.42  2002/10/23 10:51:10  mcphersc
// Changed BUF_TYPE to AsnBuf
//
// Revision 1.41  2002/10/21 19:49:50  leonberp
// changes for Linux and GCC 3.2
//
// Revision 1.40  2002/10/01 19:43:06  vracarl
// now using c_ustr
//
// Revision 1.39  2002/09/25 13:54:54  vracarl
// made asn_buf fixes
//
// Revision 1.38  2002/09/16 20:06:47  vracarl
// new asn-octs changes
//
// Revision 1.37  2002/08/21 16:48:07  vracarl
// added a FUNC and throw and checks to make sure the decimal number isn't too large
//
// Revision 1.36  2002/08/19 18:14:43  vracarl
// added code to construct an int from decimal format
//
// Revision 1.35  2002/07/17 17:30:09  leonberp
// fixed for unix
//
// Revision 1.34  2002/07/17 14:29:47  vracarl
// removed #include<string.h>
//
// Revision 1.33  2002/07/15 17:04:14  vracarl
// moved an indefinite len check
//
// Revision 1.32  2002/07/12 19:59:21  nicholar
// Fixed bug in AsnInt:storeDERInteger.
// Enhanced AsnInt::operator==(AsnIntType i) function.
//
// Revision 1.31  2002/06/17 18:42:14  leonberp
// yet another bug fix
//
// Revision 1.30  2002/06/17 18:25:50  leonberp
// fixed bug again
//
// Revision 1.29  2002/06/17 17:20:46  leonberp
// fixed AsnInt bug
//
// Revision 1.28  2002/06/17 16:37:58  leonberp
// fixed AsnInt conversion operator code
//
// Revision 1.27  2002/06/13 15:47:03  leonberp
// added c_str() and length() dumped get()
//
// Revision 1.26  2002/06/13 15:06:26  leonberp
// added get() to AsnInt
//
// Revision 1.25  2002/06/12 21:29:56  leonberp
// fixed Linux compile error
//
// Revision 1.24  2002/06/12 20:43:26  leonberp
// new AsnInt class
//
// Revision 1.23  2002/05/21 14:07:02  nicholar
// Removed warnings
//
// Revision 1.22  2002/05/10 16:39:35  leonberp
// latest changes for release 2.2
// includes integrating asn-useful into C & C++ runtime library, the compiler changes that go along with that, SnaccException changes for C++ runtime and compiler
//
// Revision 1.21  2002/03/25 19:32:16  vracarl
// added an AsnInt constructor to handle str to hex conversions - still need to add binary
//
// Revision 1.20  2002/03/01 14:07:40  vracarl
// typo
//
// Revision 1.19  2002/02/28 21:47:27  vracarl
// added an INDEFINATE_LEN check on the primitive
//
// Revision 1.18  2002/02/12 14:42:34  rwc
// Updates to fix the BigInteger Get Signed call to properly pre-load 0xff.  (Thank you james.bishop@jrc.it).
// Also some Linux/Unix fixes.
//
// Revision 1.17  2002/02/11 15:54:04  leonberp
// changed the conversion and comparison operators so that they'll work with const objects
//
// Revision 1.16  2001/11/14 22:39:21  sfl
// Updated logic to accept BigIntegerStr with 1 in upper-most bit on decode.
// This allows our lib to be more forgiving on older messages, not throw
// an exception when all else would succeed..
//
// Revision 1.15  2001/10/29 12:04:56  nicholar
// Replaced memcpy with memmove where needed
//
// Revision 1.14  2001/10/09 13:52:30  rwc
// Memory leak testing updates.
//
// Revision 1.13  2001/09/21 20:16:20  rwc
// Partial updates to start integration of Sign/Encrypt/Sign message processing.
//
// Revision 1.12  2001/09/21 18:01:08  rwc
// Updated to properly return BigIntegerString processed buffer if count matches
// precisely to expected value.  Previously, it simply returned, without copying
// the input to the output as expected.
//
// Revision 1.11  2001/08/29 22:04:18  leonberp
// enchanced Clone() to allocate a new pointe AND COPY the object
//
// Revision 1.10  2001/08/27 21:25:41  leonberp
// I 'const' enchanced CSM_Buffer and update all code that references it
//
// Revision 1.9  2001/08/24 15:39:24  leonberp
// Enchanced Print() and PrintXML()  #if'd out PrintXMLSupport()
//
// Revision 1.8  2001/07/19 16:02:51  sfl
// Yet more string.h updates.
//
// Revision 1.7  2001/07/12 19:33:37  leonberp
// Changed namespace to SNACC and added compiler options: -ns and -nons.  Also removed dead code.
//
// Revision 1.6  2001/06/28 21:38:28  rwc
// Updated to remove referneces to vdacerr, which originally replaced the cerr standard error output.
// Updated all references in macros and source that printed to vdacerr.  All code now performs an
// ASN_THROW(...) exception.
//
// Revision 1.5  2001/06/28 15:29:47  rwc
// ADDED "SNACCASN" namespace definition to all SNACC data structures.
// This should not affect most applications since we do not have any name
// conflicts.
// ALSO, combined all ASN primitive data type includes into asn-incl.h.
//
// Revision 1.4  2001/06/18 17:47:43  rwc
// Updated to reflect newly added C++ Exception error handling, instead of "C" longjmp and setjmp calls.
// Changes made to both the compiler and the SNACC C++ run-time library.
//
// Revision 1.3  2001/06/12 14:40:57  rwc
// Updates to add the SFL CSM_BigIntegerStr multi-byte integer logic to
// AsnInt for SNACC primitive support.  The code has been tested for full
// backward compatibility for integer assign/compare, etc.
//
// Revision 1.2  2000/10/16 18:10:37  rwc
// removed most warnings from C++-lib, some C-lib.
//
// Revision 1.1.1.1  2000/08/21 20:36:08  leonberp
// First CVS Version of SNACC.
//
// Revision 1.7  1997/02/28 13:39:45  wan
// Modifications collected for new version 1.3: Bug fixes, tk4.2.
//
// Revision 1.6  1995/09/07 18:55:50  rj
// (unsigned) long int replaced by newly introduced Asn(U)IntType at a lot of places.
// they shall provide 32 bit integer types on all platforms.
//
// Revision 1.5  1995/07/24  20:17:32  rj
// #if TCL ... #endif wrapped into #if META ... #endif
//
// call constructor with additional pdu and create arguments.
//
// changed `_' to `-' in file names.
//
// Revision 1.4  1995/02/18  16:48:05  rj
// denote a long if we want a long
//
// Revision 1.3  1994/10/08  04:18:23  rj
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
// Revision 1.2  1994/08/28  10:01:12  rj
// comment leader fixed.
//
// Revision 1.1  1994/08/28  09:20:59  rj
// first check-in. for a list of changes to the snacc-1.1 distribution please refer to the ChangeLog.

// Revision 2.0  6/4/01 RWC; VDA 
// Added logic to accommodate real ASN.1 multi-byte integers, not just 4 byte.

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

const AsnIntTypeDesc AsnInt::_desc (NULL, NULL, false, AsnTypeDesc::INTEGER, NULL, NULL);

const AsnTypeDesc *AsnInt::_getdesc() const
{
  return &_desc;
}

#if TCL

#define RETURN_NAME_INSTEAD_OF_VALUE	0

int AsnInt::TclGetVal (Tcl_Interp *interp) const
{
#if RETURN_NAME_INSTEAD_OF_VALUE
  const AsnNameDesc *n = _getdesc()->getnames();
  if (n)
    for (; n->name; n++)
      if (n->value == value)
      {
	Tcl_SetResult (interp, n->name, TCL_STATIC);
	return TCL_OK;
      }
#endif

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
      if (!strcmp (n->name, valstr))
      {
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


//RWC;6/4/01; newly added functionality (along with changes above).

//------------------------------------------------------------------------------
// class member definitions:

//
//
AsnLen AsnInt::BEnc (AsnBuf &b) const
{
    FUNC("AsnInt::BEnc");
    if( checkConstraints(NULL) != 0 )
        throw ConstraintException("Integer not within constraints", STACK_ENTRY);

    AsnLen l=0;
    l = BEncContent (b);
    l += BEncDefLen (b, l);

    l += BEncTag1 (b, UNIV, PRIM, INTEGER_TAG_CODE);
    return l;
}

//
//
void AsnInt::BDec (const AsnBuf &b, AsnLen &bytesDecoded)
{
   FUNC("AsnInt::BDec");

   AsnTag tag;
   AsnLen elmtLen1;

   if (((tag = BDecTag (b, bytesDecoded)) != MAKE_TAG_ID (UNIV, PRIM, INTEGER_TAG_CODE))
     && (tag != MAKE_TAG_ID (UNIV, CONS, INTEGER_TAG_CODE)))
   {
    throw InvalidTagException(typeName(), tag, STACK_ENTRY);
   }
   elmtLen1 = BDecLen (b, bytesDecoded);
   BDecContent (b, tag, elmtLen1, bytesDecoded);
}

AsnLen  AsnInt::BEncContent (AsnBuf &b) const
{
   b.PutSegRvs((char *)m_bytes, m_len);
   return m_len;
}

void AsnInt::BDecContent (const AsnBuf &b, AsnTag, AsnLen elmtLen, AsnLen &bytesDecoded)
{
   FUNC("AsnInt::BDecContent()");

   if (elmtLen == INDEFINITE_LEN)
      throw EXCEPT("indefinite length on primitive", DECODE_ERROR);

   delete[] m_bytes;
   m_bytes = new unsigned char[elmtLen + 1];
   m_len = elmtLen;
   b.GetSeg((char *)m_bytes, elmtLen);
   bytesDecoded += elmtLen;
}

AsnInt::AsnInt (const AsnInt &that)
{
   m_len = 0;
   m_bytes = NULL;
   operator=(that);
}

AsnInt::AsnInt(const char *str, const size_t len, bool unsignedFlag) 
{
   m_len = 0;
   m_bytes = NULL;
   storeDERInteger((const unsigned char *)str, len, unsignedFlag); 
}

AsnInt::AsnInt(const AsnOcts &o, bool unsignedFlag) 
{
   m_len = 0;
   m_bytes = NULL;
   storeDERInteger(o.c_ustr(), o.Len(), unsignedFlag); 
}
   
// Construct an AsnInt from an integer value
//
AsnInt::AsnInt (AsnIntType val)
{
   if (val == 0)
   {
      m_len = 1;
      m_bytes = new unsigned char[1];
      *m_bytes = 0;
   }
   else
   {
      m_len = 0;
      m_bytes = NULL;
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
{
 
   char radix=0;
   unsigned length = strlen(str);
   unsigned i = 0;

   //std::basic_string<unsigned char> localBytes;
   std::vector<unsigned char> localBytes;

   AsnIntType l = 0;

   FUNC("AsnInt::AsnInt");

   m_len = 0;
   m_bytes = NULL;

   if (length == 0)
	   return;

	switch (str[length-1])
	{
	case 'h':
	case 'H':
		radix=16;
		break;
	}

   if (radix == 16)
   {
      if (str[0] != '\'' && str[length-2] != '\'')
         return;
      length -= 2;
      i = 1;
   }
   else
   {
      if (strncmp("0x", str, 2) == 0)
      {
		 radix = 16;
         i = 2;
      }
	  else  // assume it's a decimal
	  {
         // make sure the number isn't out of range
		 if (((strlen(str) >= 11) && (str[0] != '-')) ||
		    ((strlen(str) == 11) && (str[0] == '-') && (str[1] >= '2')&& (str[2] >= '1')) ||
			((strlen(str) == 10) && (str[0] >= '2') && (str[1] >= '1')))
		 {
			throw EXCEPT("decimal string is too big to convert to an integer",
				INTEGER_ERROR);
		 }

		 l = atol(str);
		 Set(l);
	  }
   }
   
   if (radix == 16)
   {
		bool flag = false;
		unsigned char prevDigit = 0;
		for (; i<length; i++)
		{
			unsigned char digit;

			if (str[i] >= '0' && str[i] <= '9')
				digit = (unsigned char)(str[i] - '0');
			else if (str[i] >= 'A' && str[i] <= 'F')
				digit = (unsigned char)(str[i] - 'A' + 10);
			else if (str[i] >= 'a' && str[i] <= 'f')
				digit = (unsigned char)(str[i] - 'a' + 10);
			else
				return;

		  if (!flag)
		  {
			  prevDigit = digit;
			  flag = true;
		  }
		  else
		  {
			  prevDigit <<= 4;
			  prevDigit |= digit;

           //localBytes += prevDigit;
			  localBytes.push_back(prevDigit);

			  flag = false;
		  }
		}
	   storeDERInteger(&localBytes[0], localBytes.size(), unsignedFlag);
   }
}

AsnInt::~AsnInt()
{
    if(m_bytes)
        delete[] m_bytes;
}

void AsnInt::storeDERInteger(const unsigned char *pData, long dataLen, bool unsignedFlag)
{
   
   m_len = 0;
   delete[] m_bytes;

   /* IF the application generates an r,s,p,q,g or y value in which the
    * first 9 bits are all set to 0, then the encoding software deletes the
    * first octet from the octets to be encoded.  This rule is applied
    * repeatedly to the remaining octets until the first 9 bits are not all
    * set to 0.
    */
   if (unsignedFlag)
   {
	   // Check for leading nine bits all zero
	   if (dataLen > 1)
	   {
		   while (dataLen > 1 &&  !( (pData[0] & 0xFF) || (pData[1] & 0x80)) )
		   {
			   ++pData;
			   --dataLen; 
		   }
	   }
	   
      m_bytes = new unsigned char[dataLen + 1];
      m_len = dataLen;

	   /* If the application generates a r,s,p,q,g, or y value in which the
       * MSB is set to 1, THEN the software prepends a single octet in which
       * all bits are set to 0.
       */
	   if (*pData & 0x80)
	   {
		   // Prepend a leading octet
         memcpy(m_bytes + 1, pData, dataLen);
         *m_bytes = '\0';
         m_len++;
	   }
      else
         memcpy(m_bytes, pData, dataLen);

   }
   /*
    * ASN.1 rules state that the first 9 bits of an integer encoding can
    * not be all ones or all zeros.
    */
   else if (dataLen > 1 )
   {
   /* check for first first 9 bits all ones
       */
	   while ((dataLen > 1) && (pData[0] == 0xFF) && (pData[1] & 0x80))
	   {
		   ++pData;
		   --dataLen;
	   }
	   
	   /* check for first 9 bits all zeros
       */
	   while ((dataLen > 1) && (pData[0] == 0) && ((pData[1] & 0x80) == 0))
	   {
		   ++pData;
		   --dataLen;
	   }
      m_bytes = new unsigned char[dataLen + 1];
      m_len = dataLen;
      memcpy(m_bytes, pData, dataLen);
   }
}

AsnInt & AsnInt::operator =(const AsnInt &that)
{
   if (this != &that)
   {
      m_len = that.m_len;
      delete[] m_bytes;
      m_bytes = new unsigned char[m_len];
      memcpy(m_bytes, that.m_bytes, m_len);
   }

   return *this;
}

// Conversion operator for converting an AsnInt to an AsnIntType
//
AsnInt::operator AsnIntType() const
{
   FUNC("AsnInt::operator AsnIntType");

   AsnIntType iResult=0;

   if (m_len > sizeof(AsnIntType))
   {
    throw EXCEPT("integer is too big for conversion to AsnIntType", INTEGER_ERROR);
   }

   // If big int is negative initialize result to -1
   //
   if ( (m_bytes[0] >> 7 == 1) )
   {
      iResult = -1;
   }

   if (m_len > 0)
   {
     /*
      * write from buffer into AsnIntType
      */
     for (unsigned int i = 0; i < m_len; i++)
         iResult = (iResult << 8) | (AsnUIntType)(m_bytes[i]);
   }
   else
   {
     iResult = 0;
   }

   return iResult; 
}

// Set AsnInt from a buffer.  Buffer is assumed to be a proper
// ASN.1 integer.
//
void AsnInt::Set (const unsigned char *pData, size_t len, bool unsignedFlag)
{
   storeDERInteger(pData, len, unsignedFlag);
}

// Set AsnInt from a AsnIntType
//
void AsnInt::Set(AsnIntType iIn)
{ 
   AsnIntType iTmp;
   unsigned char cTmp[sizeof(iIn)];

   iTmp = iIn;
   for (unsigned long i=0; i < sizeof(iIn); i++)
     cTmp[3-i] = (unsigned char)((iTmp >> (8*i)) & 0xff);

   storeDERInteger(cTmp, sizeof(iIn), (iIn >= 0));
}

void AsnInt::getPadded(unsigned char *&bigIntDataOut, size_t &bigIntLen, const size_t padToSize) const
{
   FUNC("AsnInt::GetUnSignedBitExtendedData()");   

   bigIntLen = m_len;
   const unsigned char *bigIntData = m_bytes;

   /* This is fix to determine if the r,s,p,q,g, or y value is of the correct
    * length.
    */
   if (padToSize > 0)
   {
      /* if bigint length is less than the expected number of octets
       * the decoding software ensures that the MSB is 0 and, if so, it
       * prepends the appropriate number of octets in which every bit is
       * set to 0 to the decoded value to obtain the value supplied to
       * Fortezza Card.
       */
      if ( bigIntLen < padToSize )
      {
         long prepend = 0;
         unsigned char *tmpInt;

         prepend = padToSize - bigIntLen;

         tmpInt = (unsigned char *) calloc(1, bigIntLen + prepend);
         memset( tmpInt, 0, prepend);
         memcpy( &tmpInt[prepend], bigIntData , bigIntLen);

         bigIntDataOut = tmpInt;
         bigIntLen += prepend;
      }
      /* If the encoded values includes an "extra" octet THEN the
       * decoding software ensures that every bit in the initial octets is
       * set to 0 and, if so, deletes the initial octet from the decoded value
       * to obtain the value to be supplied to the Fortezza Card.  If the
       * extra octet contains a bit set to 1, then an error is reported.
       */
      else if (bigIntLen > padToSize)
      {
         if (bigIntData[0] != 0)
         {
            throw EXCEPT("Extra octet is not zero.", INTEGER_ERROR);
         }
         bigIntLen--;
         bigIntDataOut = (unsigned char *) calloc(1, bigIntLen);
         memcpy( &bigIntDataOut[0], &bigIntData[1], bigIntLen);
      }
      else      // Exact length.
      {
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
	
   if (m_len > 0)
	{
		// If the AsnInt is negative initialize the result to -1
		if ((m_bytes[0] & 0x80) != 0)
			result = -1;

      for (unsigned int i = 0; i < m_len; ++i)
		{
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
   os << "'";
   os.setf(std::ios::hex);
   char buf[3];
   buf[2] = '\0';
   for (unsigned long i = 0; i < m_len; i++)
   {
        sprintf(buf, "%2.2x", (int)m_bytes[i]);
        os << buf;
   }        //END for all data.
   os << "'H  -- \n";

   os.unsetf(std::ios::hex);

}

void AsnInt::PrintXML(std::ostream &os, const char *lpszTitle) const
{
   if (lpszTitle)
   {
       os << "<" << lpszTitle; 
       os << " type=\"INTEGER\">\n"; 
   }
   else
   {
       os << "<INTEGER>\n"; 
   }
   //RWC:os << "-"; 
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

    if (m_len > sizeof(AsnIntType) && (numValueRanges > 0) )
    {
        throw EXCEPT("Integer is out of constraint range", CONSTRAINT_ERROR);
    }
    
	if(valueRanges)
	{
        ltemp = *this;
        
		for(count = 0; count< numValueRanges; count++)
		{
            tmpptr = NULL;
			if(valueRanges[count].upperBoundExists == 1)
			{
                if( ( ltemp < valueRanges[count].lowerBound ) || ( ltemp > valueRanges[count].upperBound ) )
                {
                    tmpptr = ConstraintErrorStringList[ INTEGER_VALUE_RANGE ];
                }
			}
			else if(valueRanges[count].upperBoundExists == 2)
			{
                if( ltemp != valueRanges[count].lowerBound )
                {
                    tmpptr = ConstraintErrorStringList[ INTEGER_SINGLE_VALUE ];
                }
            }
            else if(valueRanges[count].upperBoundExists == 0)
			{
                if( ltemp < valueRanges[count].lowerBound )
                {
                    tmpptr = ConstraintErrorStringList[ INTEGER_VALUE_RANGE ];
                }
            }

			if(tmpptr)
			{
				ptr += tmpptr;
            }
			else
			{
				failed = 0;
			}
		}
	}
	else
	{
		failed = 0;
	}

	if(failed)
	{
		if(pConstraintFails!=NULL)
			pConstraintFails->push_back(ptr);
	}	  
    return failed;
}

void AsnInt::putByte(long offset, unsigned char cByte)
{
		m_bytes[offset] = cByte;
} 

void AsnInt::Allocate(long size)
{
	unsigned char* temp = new unsigned char[m_len + size];
    if (m_len)          // RWC;
    {                   // RWC;
	    memcpy(temp, m_bytes, m_len);
	    size += m_len;
    }                   //RWC;

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

                seg[0] >>= 8 - minBitsNeeded;
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

    if( checkConstraints(NULL) != 0 )
        throw ConstraintException("Integer not within constraints", STACK_ENTRY);

 	if(numValueRanges <= 0)
	{
		len = EncodeGeneral(b);
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
    			len = PEncFullyConstrained(b, lowerBound, upperBound);
		}
		else
        { 
            if(numValueRanges == 1 && valueRanges[0].upperBoundExists == 2 )
            {
                if((long)*this != lowerBound)
                {
                    throw EXCEPT("integer does not match singlevalue size constraint",
				        INTEGER_ERROR);
                }
            }
            else
            {
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
		memcpy(tempInt.m_bytes, tempInt.m_bytes + 1, tempInt.m_len - 1);
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
		memcpy(tempInt.m_bytes, tempInt.m_bytes + 1, tempInt.m_len - 1);
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
					memcpy(tempInt2.m_bytes, tempInt2.m_bytes + 1, tempInt2.m_len - 1);
					tempInt2.m_len--;
				}

				if((unsigned)minBitsNeeded > (tempInt.m_len * 8))
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
