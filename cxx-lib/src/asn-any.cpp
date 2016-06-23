// file: .../c++-lib/src/asn-any.C
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
//

// $Header: /baseline/SNACC/c++-lib/src/asn-any.cpp,v 1.49 2004/03/22 18:30:17 leonberp Exp $
// $Log: asn-any.cpp,v $
// Revision 1.49  2004/03/22 18:30:17  leonberp
// Fixed Linux warnings.
//
// Revision 1.48  2004/03/03 20:19:01  gronej
// took out readLoc from choice's BDecContent generation, and made new
// put-back logic to put back a tag and a length so you can properly decode
// an unkown any
//
// Revision 1.47  2004/03/02 14:34:01  gronej
// added proto for new AsnAny bdeccontent with a tag and a length passed in the parameter list
// (only occurs in presence of extension additions)
//
// Revision 1.46  2004/02/11 19:08:48  nicholar
// Updated Print() function so no longer uses global indent
//
// Revision 1.45  2004/02/04 14:59:27  gronej
// Fixed a TON of memory leaks
//
// Revision 1.44  2003/12/17 19:05:03  gronej
// SNACC baseline merged with PER v1_7 tag
//
// Revision 1.41.2.7  2003/12/04 20:47:10  gronej
// Moved bAlign out of all PEnc calls and into AsnBufBits as a member
// An AsnBufBits is now invoked with a bAlign parameter defaulted to false
//
// Revision 1.41.2.6  2003/12/04 16:47:53  colestor
// Changed AsnBufBits &m_buf to *m_pbuf for Linux builds to compile.
//
// Revision 1.41.2.5  2003/12/04 02:59:27  colestor
// Added PEnc/PDec functionality for both AsnAny ANY and ANYDefinedBy logic.
// Tested!
//
// Revision 1.41.2.4  2003/12/03 14:17:29  colestor
// Added PEnc/PDec for ANYs (untested).
//
// Revision 1.41.2.3  2003/11/05 14:58:54  gronej
// working PER code merged with esnacc_1_6
//
// Revision 1.41.2.2  2003/10/22 12:45:58  gronej
// Updating PER compiler
//
// Revision 1.41.2.1  2003/10/02 17:15:24  gronej
// Updating PER compiler
//
// Revision 1.41  2003/04/08 18:21:34  leonberp
// changed SM_NO_THREADS to NO_THREADS
//
// Revision 1.40  2003/03/12 16:20:23  mcphersc
// Added SM_NO_THREAD around destroy
//
// Revision 1.39  2003/01/06 16:20:07  leonberp
// Changed BDec() and BDecContent() to use const AsnBufs
//
// Revision 1.38  2002/12/22 01:20:52  colestor
// (RWC)Updated PrintXML(...) details to better match specification for SEQUENCE OF and SET OF logic.
//
// Revision 1.37  2002/12/17 20:27:40  leonberp
// made BEnc() and BEncContent() const
//
// Revision 1.36  2002/12/12 16:41:04  leonberp
// added copy constructor to AsnAny
//
// Revision 1.35  2002/12/11 21:39:49  leonberp
// added call to anyBuf->hexDump() so unkown ANYs will be printed
//
// Revision 1.34  2002/12/09 20:24:33  leonberp
// changed BEnc() to not use splice
//
// Revision 1.33  2002/12/06 18:10:27  leonberp
// fixed bug in AsnBuf copy constructor
//
// Revision 1.32  2002/12/03 21:27:35  leonberp
// renamed AsnBuf::insert to AsnBuf::append
//
// Revision 1.31  2002/11/25 18:39:48  colestor
// (RWC) Updates from testing with Pierce.  Specifically, AsnBuf and AsnAny
// length updates.
//
// Revision 1.30  2002/11/25 16:06:27  leonberp
// fixed bug in cleanup (destructor code)
//
// Revision 1.29  2002/11/18 20:58:12  leonberp
// removed old comments
//
// Revision 1.28  2002/11/08 17:28:32  leonberp
// fixed bug.  anyBuf was not checked for NULL prior to dereferencing
//
// Revision 1.27  2002/10/23 21:02:48  leonberp
// fixed AsnBuf references and fixed clock skew problem
//
// Revision 1.26  2002/10/23 10:51:10  mcphersc
// Changed BUF_TYPE to AsnBuf
//
// Revision 1.25  2002/10/09 19:37:55  leonberp
// new AsnBuf integration
//
// Revision 1.24  2002/09/17 19:21:06  vracarl
// AsnAnyBuffer changes
//
// Revision 1.23  2002/06/17 16:19:58  nicholar
// Changed AsnAny to use int's rather than AsnInt's for ANY DEFINED BY INTEGER's.
//
// Revision 1.22  2002/06/17 14:53:06  bodins
// no message
//
// Revision 1.21  2002/05/10 16:39:34  leonberp
// latest changes for release 2.2
// includes integrating asn-useful into C & C++ runtime library, the compiler changes that go along with that, SnaccException changes for C++ runtime and compiler
//
// Revision 1.20  2001/11/21 19:23:12  leonberp
// changed SetTypeByOid to take a const object
//
// Revision 1.19  2001/10/24 17:04:25  rwc
// AsnAny and IndefiniteLength length determination logic was updated.
//
// Revision 1.18  2001/10/16 04:26:49  leonberp
// added a destructor for the any tables
//
// Revision 1.17  2001/10/15 18:00:06  leonberp
// mem leak fixes
//
// Revision 1.16  2001/10/15 17:14:19  leonberp
// mem leak fixes
//
// Revision 1.15  2001/10/11 22:26:31  rwc
// Newly tested memory leak fixes.
//
// Revision 1.14  2001/10/09 13:52:30  rwc
// Memory leak testing updates.
//
// Revision 1.13  2001/09/18 12:59:30  nicholar
// Fixed bug in AsnAny assignment operator.
//
// Revision 1.12  2001/08/29 22:04:17  leonberp
// enchanced Clone() to allocate a new pointe AND COPY the object
//
// Revision 1.11  2001/08/29 17:01:47  leonberp
// no message
//
// Revision 1.10  2001/08/27 21:25:40  leonberp
// I 'const' enchanced CSM_Buffer and update all code that references it
//
// Revision 1.9  2001/07/12 19:33:35  leonberp
// Changed namespace to SNACC and added compiler options: -ns and -nons.  Also removed dead code.
//
// Revision 1.8  2001/07/10 21:03:08  rwc
// Removed previous fix by Tex for ANY DEFINED BY logic.
//
// Revision 1.6  2001/06/28 15:29:48  rwc
// ADDED "SNACCASN" namespace definition to all SNACC data structures.
// This should not affect most applications since we do not have any name
// conflicts.
// ALSO, combined all ASN primitive data type includes into asn-incl.h.
//
// Revision 1.5  2001/06/18 17:47:42  rwc
// Updated to reflect newly added C++ Exception error handling, instead of "C" longjmp and setjmp calls.
// Changes made to both the compiler and the SNACC C++ run-time library.
//
// Revision 1.4  2001/04/23 15:37:04  rwc
// Further SNACC ANY DEFINED BY update testing fixes (for the SFL).
//
// Revision 1.3  2001/04/18 16:28:40  rwc
// Updated test for ANY DEFINED BY updates.  Now supports default CSM_Buffer (AsnAnyBuffer) result if
// OID is not recognized.
//
// Revision 1.2  2001/01/22 20:18:46  rwc
// Updates to test newly added ASN.1 XML output.
//
// Revision 1.1.1.1  2000/08/21 20:36:08  leonberp
// First CVS Version of SNACC.
//
// Revision 1.6  1997/02/28 13:39:43  wan
// Modifications collected for new version 1.3: Bug fixes, tk4.2.
//
// Revision 1.5  1997/02/16 20:26:01  rj
// check-in of a few cosmetic changes
//
// Revision 1.4  1995/07/24  20:12:48  rj
// changed `_' to `-' in file names.
//
// Revision 1.3  1994/10/08  04:18:20  rj
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
// Revision 1.2  1994/08/28  10:01:10  rj
// comment leader fixed.
//
// Revision 1.1  1994/08/28  09:20:55  rj
// first check-in. for a list of changes to the snacc-1.1 distribution please refer to the ChangeLog.

#include "asn-incl.h"

_BEGIN_SNACC_NAMESPACE

SNACCDLL_API Table *AsnAny::oidHashTbl = NULL;
SNACCDLL_API Table *AsnAny::intHashTbl = NULL;
long SM_DetermineLengthBuf(AsnBuf &SNACCinputBuf, AsnLen elmtLen0, 
                           int bFirstTimeFlag=false);
long SM_DetermineLengthBuf(AsnBuf &SNACCinputBuf);

class AnyTableDestructor
{
public:
   ~AnyTableDestructor() { AsnAny::AsnAnyDestroyHashTbls(); }
};

static AnyTableDestructor __anyTableDestructor;


// Define this ANY value's type to the one that the given id hashes
// to in the ANY table.
void
AsnAny::SetTypeByInt (const AsnInt& id) const
{
    Hash hash;
    void *anyInfo;

    /* use int as hash string */
    hash = MakeHash ((const char*)id.c_str(), id.length());
    if (CheckForAndReturnValue (intHashTbl, hash, &anyInfo))
        ai = (AnyInfo*) anyInfo;
    else
        ai = NULL; /* indicates failure */

} /* SetAnyTypeByInt */

// Define this ANY value's type to the one that the given id hashes
// to in the ANY table.
void AsnAny::SetTypeByOid (const AsnOid &id) const
{
    Hash hash;
    void *anyInfo;

    /* use encoded oid as hash string */
    hash = MakeHash (id.Str(), id.Len());
    if (CheckForAndReturnValue (oidHashTbl, hash, &anyInfo))
        ai = (AnyInfo*) anyInfo;
    else
        ai = NULL; /* indicates failure */
    //RWC;4/16/01; lastly, clear ->value in case loaded previously.

} /* SetAnyTypeByOid */



// Given an integer, intId, to hash on, the type and it's anyId
// are installed in the integer id hash tbl
void
AsnAny::InstallAnyByInt (AsnIntType intId, int anyId, AsnType *type)
{
    AnyInfo *a;
    Hash h;

    a = new AnyInfo;
    //  Oid will be NULL and 0 len by default constructor
    a->anyId = anyId;
    a->intId = intId;
    a->typeToClone = type;

    if (AsnAny::intHashTbl == NULL)
        AsnAny::intHashTbl = InitHash();

	 AsnInt intval = intId;
    h = MakeHash ((const char*)intval.c_str(), intval.length());
    if ( ! Insert (AsnAny::intHashTbl, a, h) )
    {
       delete a->typeToClone;
       delete a;
    }

}  /* InstallAnyByInt */


//
//
void AsnAny::AsnAnyDestroyHashTbls() // Added for static call to destroy special; at end.
{
   if (oidHashTbl)
   {
       AsnAnyDestroyHashTbl(oidHashTbl);
   }
   if (intHashTbl)
   {
       AsnAnyDestroyHashTbl(intHashTbl);
   }
}

//
//
void AsnAny::AsnAnyDestroyHashTbl(Table *&pHashTbl) // Added for static call to destroy special; at end.
{
    HashSlot *pHash;
    AnyInfo *pA;
    int i;
    for (i=0; i < TABLESIZE; i++)
         if ((*pHashTbl)[i])
         {
            pHash = (HashSlot *)(*pHashTbl)[i];
            pA = (AnyInfo *)pHash->value;
            if (pHash->table)
              AsnAnyDestroyHashTbl(pHash->table);
            else if (pA->typeToClone)
            {
              delete pA->typeToClone;
              delete pA;
            }
            delete pHash;
         }
      delete[] pHashTbl;
      pHashTbl = NULL;
}
//

// given an OBJECT IDENTIFIER, oid, to hash on, the type and it's anyId
// are installed in the OBJECT IDENTIFIER id hash tbl
void
AsnAny::InstallAnyByOid (AsnOid &oid, int anyId, AsnType *type)
{
    AnyInfo *a;
    Hash h;

    a =  new AnyInfo;
    a->anyId = anyId;
    a->oid = oid;  // copy given oid
    a->typeToClone = type;

    h = MakeHash (oid.Str(), oid.Len());

    if (AsnAny::oidHashTbl == NULL)
        AsnAny::oidHashTbl = InitHash();

    if (! Insert (AsnAny::oidHashTbl, a, h))
    {
       delete a->typeToClone;
       delete a;
    }

}  /* InstallAnyByOid */


//
//
AsnLen AsnAny::PEnc(AsnBufBits &b) const
{
   std::stringbuf *pbufStr=new std::stringbuf;    //MEMORY released by ~AsnBufBits.
   AsnBufBits TmpBufBits(pbufStr);
   AsnOcts tmpAnyLoadOcts;
   unsigned char *pBits;
   long lAnyBitCount, lAnyByteCount;
   unsigned long lLength=0;
   FUNC("AsnAny::PEnc()");

   if (value != NULL)    // HANDLE the case where we know the syntax.
   {
      value->PEnc(TmpBufBits);
      lAnyBitCount = TmpBufBits.length();
      pBits = TmpBufBits.GetBits(lAnyBitCount);
      lAnyByteCount = lAnyBitCount/8;
      if (lAnyByteCount*8 < lAnyBitCount)
          lAnyByteCount++;      // ZERO padded here.
      tmpAnyLoadOcts.Set((const char *)pBits, lAnyByteCount);
      delete[] pBits;
      lLength = tmpAnyLoadOcts.PEnc(b);
   }        // IF value
   else if (anyBuf != NULL)  // HANDLE the case with just a BLOB of data.
   {
       anyBuf->ResetMode();
       lLength = anyBuf->length();
       char *ptr = anyBuf->GetSeg(lLength);
       if (ptr && lLength)
       {
          tmpAnyLoadOcts.Set(ptr, lLength); // BYTE count here.
          lLength = tmpAnyLoadOcts.PEnc(b);  // BIT count returned.
       }    // END IF any data in ANY.
       delete[] ptr;
   }        // IF value/anyBuf
   else
      throw EXCEPT("Unknown any with no value", ENCODE_ERROR);

   if(pbufStr)
        delete pbufStr;

   return(lLength);
}       // END AsnAny::PEnc(...)

//
//
void AsnAny::PDec(AsnBufBits &b, AsnLen &bitsDecoded)
{
   AsnBufBits tmpBufBits;
   AsnOcts tmpAnyLoadOcts;
   AsnLen tmpBitsDecoded=0;
   FUNC("AsnAny::PDec");

   // ai will be NULL if this is an ANY (not an ANY DEFINED BY)
   if (ai != NULL)
   {
      // the type is already known clone it and use it's BDec to decode the 
      // ASN.1
      //
      value = ai->typeToClone->Clone();
      if (value == NULL)
      {
         throw SnaccException(STACK_ENTRY, "typeToClone->Clone() failed", INVALID_ANY);
      }     // IF value == NULL
      else
      {
         tmpAnyLoadOcts.PDec(b, bitsDecoded);  // OUTER OctetString
                                // OUTER "bitsDecoded" returned to caller.
         if (tmpAnyLoadOcts.length())
         {
             tmpBufBits.PutBits((unsigned char *)tmpAnyLoadOcts.c_ustr(), 
                            tmpAnyLoadOcts.length()*8);
             value->PDec(tmpBufBits, tmpBitsDecoded);
                                // DECODE actual known value.
         }      // END IF tmpBitsDecoded
      }     // END IF value == NULL
   }        // IF ai != NULL
   else     // JUST load BLOB of data in "anyBuf"
   {
         tmpAnyLoadOcts.PDec(b, bitsDecoded);  // OUTER OctetString
                                // OUTER "bitsDecoded" returned to caller.
         if (tmpAnyLoadOcts.length())
         {
             if(this->anyBuf)
                 delete this->anyBuf;
             this->anyBuf = new AsnBuf((char *)tmpAnyLoadOcts.c_str(), 
                                               tmpAnyLoadOcts.length());
         }  // END IF any data in ANY.
   }        // END IF ai != NULL
}           // END AsnAny::PDec(...)


// FUNCTION: BEnc()
// PUPROSE: Encode ANY DEFINED BY if "value" is present otherwise encode ANY if 
//          anyBuf is present.  If neither is present an exception is thrown.
//
AsnLen
AsnAny::BEnc (AsnBuf &b) const
{
   FUNC("AsnAny::BEnc()");

   if (value != NULL)
      return value->BEnc(b);
   else if (anyBuf != NULL)
   {
		anyBuf->ResetMode();
		b.insert(*anyBuf);
		return anyBuf->length();

   }
   else
      throw EXCEPT("Unknown any with no value", ENCODE_ERROR);
}



// BDec()
//
// Decoded ANY DEFINED BY or UNKNOWN ANY from 'b'.  If an ANY DEFINED
// BY is found it's will be decoded into value.  If an UNKNOWN ANY is
// found it's binary values will be copied into 'anyBuf'.
//
void AsnAny::BDec (const AsnBuf &b, AsnLen &bytesDecoded)
{
   FUNC("AsnAny::BDec");

   // ai will be NULL if this is an ANY (not an ANY DEFINED BY)
   if (ai == NULL)
   {             
      anyBuf = new AsnBuf;
      b.GrabAny(*anyBuf, bytesDecoded);
   }
   else
   {
      // the type is already known clone it and use it's BDec to decode the 
      // ASN.1
      //
      value = ai->typeToClone->Clone();
      if (value == NULL)
      {
         throw SnaccException(STACK_ENTRY, "typeToClone->Clone() failed", INVALID_ANY);
      }
      else
         value->BDec (b, bytesDecoded);
   }
}

/* JKG -- added 03/03/04 for support of unkown any's in extension additions                   */
/* This BDecContent function was written specifically to handle the decoding of unknown any's */
/* that happened because of extension additions.  It is not intended for use with decoding    */
/* known any's.  ALL extension additions that are not in the root extension list are unkown   */
/* any's and these are the ONLY any's that should call this function.                         */
void AsnAny::BDecContent (const AsnBuf &b, AsnTag tag, AsnLen len, AsnLen &bytesDecoded)
{

    long lBytesToUnget = 0;
    
    lBytesToUnget += BytesInLen(len);
    lBytesToUnget += BytesInTag(tag);
      
    b.UnGetBytes(lBytesToUnget);
    anyBuf = new AsnBuf;
    b.GrabAny(*anyBuf, bytesDecoded);
}

void AsnAny::Print(std::ostream& os, unsigned short indent) const
{
	if (value != NULL)
		value->Print(os, indent);
	else if (anyBuf != NULL)
	{
		os << "UNKNOWN ANY hex dump: ";
		anyBuf->hexDump(os);
	}
}

void AsnAny::PrintXML (std::ostream &os, const char *lpszTitle) const 
{
    const char *tagName = typeName();

    if (lpszTitle) tagName = lpszTitle;

    os << "<" << tagName << ">";
    Print(os);
    os << "</" << tagName << ">\n";
}

AsnAny::~AsnAny()
{
   delete this->value;
   this->value = NULL;
   delete this->anyBuf;
   this->anyBuf = NULL;
}

AsnAny &AsnAny::operator = (const AsnAny &o) 
{ 
   FUNC("AsnAny::operator=");
   ai = o.ai;  // pointer into Any Table

   delete value;
   value = NULL;
   delete anyBuf;
   anyBuf = NULL;

   // NOW, take care of special CSM_Buffer copy, if present.
   if (o.value != NULL)
   {
      value = o.value->Clone();
      if (value == NULL)
         throw SnaccException(STACK_ENTRY,"AsnType::Clone() failed",INVALID_ANY);
   }
   else if (o.anyBuf != NULL)
   {
      anyBuf = new AsnBuf(*o.anyBuf);
   }
   return *this; 
}



extern "C" {
void SNACCDLL_API SNACC_CleanupMemory()
{
#ifndef NO_THREADS
   threadDestroy();  // ONLY necessary if a thread lock is created (and it is 
                     //  important to clear all memory leaks before exiting.
#endif
   AsnAny::AsnAnyDestroyHashTbls();   // FINAL call, to clear static tables, 
                                      //  before exiting.
}
}     // END extern "C"

_END_SNACC_NAMESPACE
