// file: .../c++-lib/inc/asn-incl.h - C++ class SNACCDLL_API
//
//
//  NEW HISTORY started once all SNACC class definitions move to a single file.
//

#ifndef SNACC_CXX_ASN_INCL_H
#define SNACC_CXX_ASN_INCL_H


#include "asn-config.h"
#include "asn-buf.h"
#include "snaccexcept.h"

#ifdef WIN32
#if defined(_MSC_VER)
	#pragma warning(push,3)
#endif
	#include <string>
    #include <string.h>
#if defined(_MSC_VER)
	#pragma warning(pop)
#endif
#else // WIN32
	#include <iostream>
	#include <string>
	#include "asn-chartraits.h" 

	#if defined(SunOS) || defined(SCO_SV) || defined(HPUX) || defined(HPUX32)
	namespace std
	{
		typedef basic_string<wchar_t> wstring;
	}
	#endif // SunOS
#endif //WIN32


#if TCL
#include <tcl.h>
#undef VOID
#endif

#if META
#include "meta.h"
#endif

void SNACCDLL_API BDEC_2ND_EOC_OCTET(const SNACC::AsnBuf& b,
									 SNACC::AsnLen& bytesDecoded);

_BEGIN_SNACC_NAMESPACE

typedef std::list<std::string> ConstraintFailList;


//########################################################################

#if SIZEOF_INT == 4
	#define UL		unsigned int
#elif SIZEOF_LONG == 4
	#define UL		unsigned long
#elif SIZEOF_SHORT == 4
	#define UL		unsigned short
#else
	#define UL		unsigned int
#endif
typedef UL AsnTag;

// Tag Id's byte len
#define TB			sizeof(SNACC::AsnTag)

// The MAKE_TAG_ID macro generates the TAG_ID rep for the
// the given class/form/code (rep'd in long integer form)
// if the class/form/code are constants the compiler (should)
// calculate the tag completely --> zero runtime overhead.
// This is good for efficiently comparing tags in switch statements
// (decoding) etc.  because run-time bit fiddling (eliminated) minimized
#define MAKE_TAG_ID( cl, fm, cd)\
	((((UL)(cl)) << ((TB-1)*8)) | (((UL)(fm)) << ((TB-1)*8)) | (MAKE_TAG_ID_CODE(((UL)(cd)))))

#define MAKE_TAG_ID_CODE( cd)\
	((cd < 31) ?	(MAKE_TAG_ID_CODE1 (cd)):\
	((cd < 128) ?	(MAKE_TAG_ID_CODE2 (cd)):\
	((cd < 16384) ?	(MAKE_TAG_ID_CODE3 (cd)):\
					(MAKE_TAG_ID_CODE4 (cd)))))

#define MAKE_TAG_ID_CODE1(cd)	((UL)cd << ((TB-1)*8))
#define MAKE_TAG_ID_CODE2(cd)	((31l << ((TB-1)*8)) | (cd << ((TB-2)*8)))
#define MAKE_TAG_ID_CODE3(cd)	((31l << ((TB-1)*8))\
								| ((cd & 0x3f80) << 9)\
								| (0x0080 << ((TB-2)*8))\
								| ((cd & 0x007F) << ((TB-3)*8)))

#define MAKE_TAG_ID_CODE4(cd)	((31l << ((TB-1)*8))\
								| ((cd & 0x1fc000) << 2)\
								| (0x0080 << ((TB-2)*8))\
								| ((cd & 0x3f80) << 1)\
								| (0x0080 << ((TB-3)*8))\
								| ((cd & 0x007F) << ((TB-4)*8)))

typedef enum BER_CLASS
{
	ANY_CLASS	= -2,
	NULL_CLASS	= -1,
	UNIV		= 0,
	APPL		= (1 << 6),
	CNTX		= (2 << 6),
	PRIV		= (3 << 6)
} BER_CLASS;

typedef enum BER_FORM
{
	ANY_FORM	= -2,
	NULL_FORM	= -1,
	PRIM		= 0,
	CONS		= (1 << 5)
} BER_FORM;

typedef enum BER_UNIV_CODE
{
	NO_TAG_CODE					= 0,
	BOOLEAN_TAG_CODE			= 1,
	INTEGER_TAG_CODE,
	BITSTRING_TAG_CODE,
	OCTETSTRING_TAG_CODE,
	NULLTYPE_TAG_CODE,
	OID_TAG_CODE,
	OD_TAG_CODE,
	EXTERNAL_TAG_CODE,
	REAL_TAG_CODE,
	ENUM_TAG_CODE,
	UTF8STRING_TAG_CODE			= 12,
	RELATIVE_OID_TAG_CODE		= 13,
	SEQ_TAG_CODE				= 16,
	SET_TAG_CODE,
	NUMERICSTRING_TAG_CODE,
	PRINTABLESTRING_TAG_CODE,
	TELETEXSTRING_TAG_CODE,
	VIDEOTEXSTRING_TAG_CODE,
	IA5STRING_TAG_CODE,
	UTCTIME_TAG_CODE,
	GENERALIZEDTIME_TAG_CODE,
	GRAPHICSTRING_TAG_CODE,
	VISIBLESTRING_TAG_CODE,
	GENERALSTRING_TAG_CODE,
	UNIVERSALSTRING_TAG_CODE	= 28,
	BMPSTRING_TAG_CODE			= 30
} BER_UNIV_CODE;

#define TT61STRING_TAG_CODE		TELETEXSTRING_TAG_CODE
#define ISO646STRING_TAG_CODE	VISIBLESTRING_TAG_CODE

/*
 * the TAG_ID_[CLASS/FORM/CODE] macros are not
 * super fast - try not to use during encoding/decoding
 */
#define TAG_ID_CLASS(tid)	( (tid & (0xC0 << ((TB-1)*8))) >> ((TB-1)*8))
#define TAG_ID_FORM(tid)	( (tid & (0x20 << ((TB-1)*8))) >> ((TB-1)*8))

/*
 * TAG_IS_CONS evaluates to true if the given AsnTag type
 * tag has the constructed bit set.
 */
#define TAG_IS_CONS(tag)	((tag) & (CONS << ((TB-1)*8)))

#define EOC_TAG_ID	0

/*
 * tag encoders.  given constant exprs for class SNACCDLL_API form & code in the
 * source, these can be optimized by the compiler (eg
 * do the shifts and bitwise ors etc)
 */
#define BEncTag1( b, class, form, code)\
	1;\
	b.PutByteRvs((unsigned char)((class) | (form) | (code)))

#define BEncTag2( b, class, form, code)\
	2;\
	b.PutByteRvs(code);\
	b.PutByteRvs((char)((class) | (form) | 31))

#define BEncTag3( b, class, form, code)\
	3;\
	b.PutByteRvs((code) & 0x7F);\
	b.PutByteRvs((char)(0x80 | (char)((code) >> 7)));\
	b.PutByteRvs((class) | (form) | 31)

#define BEncTag4( b, class, form, code)\
	4;\
	b.PutByteRvs((code) & 0x7F);\
	b.PutByteRvs((char)(0x80 | (char)((code) >> 7)));\
	b.PutByteRvs((char)(0x80 | (char)((code) >> 14)));\
	b.PutByteRvs((class) | (form) | 31)

#define BEncTag5( b, class, form, code)\
	5;\
	b.PutByteRvs((code) & 0x7F);\
	b.PutByteRvs((char)(0x80 | (char)((code) >> 7)));\
	b.PutByteRvs((char)(0x80 | (char)((code) >> 14)));\
	b.PutByteRvs((char)(0x80 | (char)((code) >> 21)));\
	b.PutByteRvs((class) | (form) | 31)

AsnLen SNACCDLL_API PEncTag(AsnBufBits& b, unsigned char ucClass,
							unsigned char form, long code, long lByteCount);
AsnTag SNACCDLL_API BDecTag(const AsnBuf& b, AsnLen& bytesDecoded);
AsnTag SNACCDLL_API PDecTag(AsnBufBits& bufBits, AsnLen& bitsDecoded);
long   SNACCDLL_API BytesInTag(AsnTag tag);
long   SNACCDLL_API BytesInLen(AsnLen len);

//########################################################################

#define INDEFINITE_LEN		~0UL	// max unsigned value used for indef rep
#define MAX_OCTS			1048576	// 1 meg
#define l_16k				16384

#ifdef USE_INDEF_LEN

#define BEncEocIfNec(b)\
	BEncEoc (b)
#define BEncConsLen(b, len)\
	2 + BEncIndefLen (b)		// include len for EOC */

#else	// default -- use definite length -- usually faster
		// and smaller encodings

#define BEncEocIfNec(b)		0;	// do nothing

#define BEncConsLen(b, len)\
	BEncDefLen (b, len)

#endif

#define BEncIndefLen( b)\
	1;\
	b.PutByteRvs (0x80)

/* 
 * use if you know the encoded length will be 0 >= len <= 127
 * Eg for booleans, nulls, any resonable integers and reals
 *
 * NOTE: this particular Encode Routine does NOT return the length
 * encoded (1).  The length counter must be explicity incremented
 */
#define BEncDefLenTo127( b, len)\
	b.PutByteRvs ((unsigned char) len)

AsnLen SNACCDLL_API BDecLen(const AsnBuf& b, AsnLen& bytesDecoded);
AsnLen SNACCDLL_API BEncDefLen(AsnBuf& b, AsnLen len);

AsnLen SNACCDLL_API BEncEoc(AsnBuf& b);
void SNACCDLL_API   BDecEoc(const AsnBuf& b, AsnLen& bytesDecoded);

AsnLen SNACCDLL_API PEncDefLenTo127(AsnBufBits& b, int len);
AsnLen SNACCDLL_API PEncLen_16kFragment(AsnBufBits& b, int len);		
AsnLen SNACCDLL_API PEncLen_1to16k(AsnBufBits& b, int len);


// Indent function used in Print() functions
void SNACCDLL_API Indent(std::ostream& os, unsigned short i);

//########################################################################

AsnLen SNACCDLL_API PEncLen_16kFragment(AsnBufBits &b, int len);		

AsnLen SNACCDLL_API PEncLen_1to16k(AsnBufBits &b, int len);

//Forward definition
class AsnType;

typedef struct SNACCDLL_API ContainedSubtype{
	AsnType *containedSubtype;
}ContainedSubtype;

typedef struct SNACCDLL_API SizeConstraint{
	unsigned long lowerBound;
	unsigned long upperBound;
	int  upperBoundExists;
}SizeConstraint;

typedef struct SNACCDLL_API ValueRange{
	long lowerBound;
	long upperBound;
	int  upperBoundExists;
}ValueRange;

typedef struct SNACCDLL_API PermittedAlphabet{
	unsigned char* ucApha; 
}PermittedAlphabet;


class SNACCDLL_API PERGeneral
{
protected:
	virtual AsnLen EncodeGeneral(AsnBufBits& b) const;
	virtual AsnLen Interpret(AsnBufBits& b, long offset) const = 0;
	
	virtual long lEncLen() const						{ return 0; }
	virtual void Clear() = 0;

	virtual void DecodeGeneral(AsnBufBits& b, AsnLen& bitsDecoded);
	virtual void Deterpret(AsnBufBits&b, AsnLen& bitsDecoded, long offset) = 0;
	virtual void Allocate(long size) = 0;
};


class SNACCDLL_API AsnType
{
protected:
	ContainedSubtype  *containedSubtypes;

public:

	virtual ~AsnType();

	virtual AsnType* Clone() const = 0;
	virtual const char* typeName() const = 0;

    /* JKG: virtual constraint check function added 7/29/03 */
	/* function returns 1 on error and 0 on success        */
	virtual int checkConstraints(ConstraintFailList* ) const	{ return 0; }

	virtual void   BDec(const AsnBuf &b, AsnLen &bytesDecoded) = 0;
	virtual AsnLen BEnc(AsnBuf &b) const = 0;

	virtual void   PDec(AsnBufBits &, AsnLen &)		{}
	virtual AsnLen PEnc(AsnBufBits &) const					{ return 0; }

	bool BEncPdu(AsnBuf &b, AsnLen &bytesEncoded) const;
	bool BDecPdu(const AsnBuf &b, AsnLen &bytesDecoded);

    virtual void Print(std::ostream& os, unsigned short indent = 0) const = 0;

#if META
	static const AsnTypeDesc	 _desc;
	virtual const AsnTypeDesc* _getdesc() const;
	virtual AsnType* _getref(const char* membername, bool create = false);

private:
	const char* _typename() const;

#if TCL
public:
	virtual int TclGetDesc(Tcl_DString* ) const;
	virtual int TclGetVal(Tcl_Interp* ) const;
	virtual int TclSetVal(Tcl_Interp* , const char* val);
	virtual int TclUnsetVal(Tcl_Interp* , const char* membernames);
#endif // TCL
#endif // META
};


//########################################################################
//########################################################################

typedef std::pair<unsigned char *, unsigned long> StringPair;

class ConsStringDeck : public std::deque< StringPair >
{
public:
   ConsStringDeck(AsnTag baseTag = OCTETSTRING_TAG_CODE) {m_baseTag = baseTag;}
   
   virtual ~ConsStringDeck(); 
   void Fill(const AsnBuf &b, AsnLen elmtLen, AsnLen &bytesDecoded);
   void Collapse(std::string &str);

private:
   AsnTag m_baseTag;
};

//########################################################################

class SNACCDLL_API AsnNull: public AsnType
{
public:

  virtual AsnType* Clone() const			{ return new AsnNull(*this); }
  virtual const char* typeName() const		{ return "AsnNull"; }

  AsnLen         BEncContent (AsnBuf &) const      { return 0; }
  void           BDecContent (const AsnBuf &b, AsnTag tagId, AsnLen elmtLen, AsnLen &bytesDecoded);
  AsnLen         BEnc (AsnBuf &b) const;
  void           BDec (const AsnBuf &b, AsnLen &bytesDecoded);
	
  AsnLen	     PEnc (AsnBufBits &b) const;
  void  		 PDec (AsnBufBits &b, AsnLen &bitsDecoded);

  void           Print(std::ostream& os, unsigned short indent = 0) const;
  void           PrintXML (std::ostream &os, const char *lpszTitle=NULL) const;

#if META
  static const AsnNullTypeDesc   _desc;
  const AsnTypeDesc      *_getdesc() const;

#if TCL
  int            TclGetVal (Tcl_Interp *) const;
  int            TclSetVal (Tcl_Interp *, const char *val);
#endif /* TCL */
#endif /* META */
};

//########################################################################

class SNACCDLL_API AsnBool: public AsnType
{
protected:
	bool value;

public:
	AsnBool (const bool val = false)			{ value = val; }

	virtual AsnType* Clone() const				{ return new AsnBool(*this);}
	virtual const char* typeName() const		{ return "AsnBool"; }

	operator bool() const						{ return value; }

   AsnBool         &operator = (bool newvalue)      { value = newvalue; return *this; }

   AsnLen         BEnc (AsnBuf &b) const;
   void           BDec (const AsnBuf &b, AsnLen &bytesDecoded);
   AsnLen         BEncContent (AsnBuf &b) const;
   void           BDecContent (const AsnBuf &b, AsnTag tagId, AsnLen elmtLen, AsnLen &bytesDecoded);

   AsnLen         PEnc (AsnBufBits &b) const;	
   void		      PDec (AsnBufBits &b, AsnLen &bitsDecoded);
  

   void           Print(std::ostream& os, unsigned short indent = 0) const;
   void           PrintXML (std::ostream &os, const char *lpszTitle=NULL) const; 
   char* checkBoolSingleVal(const bool m_SingleVal)const;

#if META
  static const AsnBoolTypeDesc   _desc;

  const AsnTypeDesc      *_getdesc() const;

#if TCL
  int            TclGetVal (Tcl_Interp *) const;
  int            TclSetVal (Tcl_Interp *, const char *val);
#endif // TCL
#endif // META
};

//########################################################################

class SNACCDLL_API AsnOcts: public AsnType
{
public:
   // constructor always copies strings so destructor can always delete
   AsnOcts() {m_pFileSeg = NULL; }
   AsnOcts (const char *str) { m_pFileSeg = NULL; m_str.assign(str); }
   AsnOcts (const char *str, const size_t len) { m_pFileSeg = NULL; m_str.assign(str, len); }
   AsnOcts (const AsnOcts &o);
//   AsnOcts (const std::filebuf &fb);
   virtual ~AsnOcts();

   virtual const SizeConstraint* SizeConstraints(int &sizeList)const { sizeList = 0; return NULL;}

   virtual AsnType* Clone() const				{ return new AsnOcts(*this); }
   virtual const char* typeName() const			{ return "AsnOcts"; }

   AsnOcts		&operator = (const AsnOcts &o)	{ return SetEqual(o); }
   AsnOcts		&operator = (const char *str)	{ return SetEqual(str); }

   AsnOcts      &SetEqual(const AsnOcts &o)  { m_str = o.m_str; return *this; }
   AsnOcts      &SetEqual(const char *str)   { m_str.assign(str); return *this; }


   void         Set (const char *str, size_t len);

   size_t       Len() const;
   long			length(){return (long)Len();}
   long			length()const{return (long)Len();}

   void			clear(){m_str.erase();}

   const std::string &   data() const;
   const char *          c_str() const;
   const unsigned char * c_ustr() const;

   bool operator == (const AsnOcts &o) const;
   bool operator != (const AsnOcts &o) const;

   AsnLen         BEncContent (AsnBuf &b) const;
   void           BDecContent (const AsnBuf &b, AsnTag tagId, AsnLen elmtLen, AsnLen &bytesDecoded);
   AsnLen         BEnc (AsnBuf &b) const ;
   void           BDec (const AsnBuf &b, AsnLen &bytesDecoded);

   AsnLen		  PEnc (AsnBufBits &b)const;
   void			  PDec (AsnBufBits &b, AsnLen &bitsDecoded);

   void           Print(std::ostream& os, unsigned short indent = 0) const;
   void           PrintXML (std::ostream &os, const char *lpszTitle=NULL,
                     const char *lpszType=NULL) const;

protected:
	AsnLen EncodeGeneral(AsnBufBits &b)const;
	void DecodeGeneral(AsnBufBits &b, AsnLen &bitsDecoded);
	AsnLen	EncodeWithSizeConstraint(AsnBufBits &b)const;
	void	DecodeWithSizeConstraint(AsnBufBits &b, AsnLen &bitsDecoded);
	long	FindSizeConstraintBounds(int &iSCLowerBound, int &iSCUpperBound)const;

private:
  void           BDecConsOcts (const AsnBuf &b, AsnLen elmtLen, AsnLen &bytesDecoded);
  void           FillStringDeck(AsnBuf &b, AsnLen elmtLen, AsnLen &bytesDecoded);

  
  // IF AsnOcts length is < MAX_OCTS, store in AsnString base
  mutable std::string  m_str;

  // IF AsnOcts length is > MAX_OCTS, store in m_FileBuf;
  mutable AsnFileSeg     *m_pFileSeg;
};

//########################################################################

class SNACCDLL_API AsnBits: public AsnType
{
public:

  AsnBits(const char *stringForm=NULL);
  AsnBits (size_t numBits) 
          { bits=NULL;nblFlag = false; Set (numBits);  }
  AsnBits (const unsigned char *bitOcts, size_t numBits)
          {bits=NULL; nblFlag = false; Set (bitOcts, numBits); }
  AsnBits (const AsnBits &b)      { bits=NULL; Set (b); }
  ~AsnBits();

  virtual const SizeConstraint* SizeConstraints(int &sizeList)const { sizeList = 0; return NULL;}

  virtual AsnType* Clone() const				{ return new AsnBits(*this); }
  virtual const char* typeName() const			{ return "AsnBits"; }

  AsnBits         &operator = (const AsnBits &b)      { Set (b); return *this; }

  // overwrite existing bits and bitLen values
  void            Set (size_t numBits);
  void            Set (const unsigned char *bitOcts, size_t numBits);
  void            Set (const AsnBits &b);

  AsnBits &		  SetEqual(const char *stringForm);

  AsnBits &       operator = (const char *stringForm);
  bool            operator == (const AsnBits &ab) const   { return BitsEquiv (ab); }
  bool            operator != (const AsnBits &ab) const   { return !BitsEquiv (ab); }

  void            SetSize(size_t);
  bool            soloBitCheck(size_t);
  void            SetBit (size_t);
  void            ClrBit (size_t);
  bool            GetBit (size_t) const;
  bool            IsEmpty() const;
  long			  FindSizeConstraintBounds(int &iSCLowerBound, int &iSCUpperBound)const;


  void            UseNamedBitListRules(bool flag) { nblFlag = flag; }

  void			 clear(){if(bits) delete[] bits; bits = NULL; bitLen = 0;}

  size_t         BitLen() const            { return bitLen; }
  const unsigned char* data() const { return bits; }
  size_t         length() const { return ((bitLen+7)/8); }
  size_t		 encLen() const;
  virtual void	 Allocate(long size);


  AsnLen         BEncContent (AsnBuf &b) const;
  void           BDecContent (const AsnBuf &b, AsnTag tagId, AsnLen elmtLen, AsnLen &bytesDecoded);
  AsnLen         BEnc (AsnBuf &b) const;
  void           BDec (const AsnBuf &b, AsnLen &bytesDecoded);

  AsnLen		 EncodeGeneral (AsnBufBits &b)const;
  void			 DecodeGeneral (AsnBufBits &b, AsnLen &bitsDecoded);
  AsnLen	     EncodeWithSizeConstraint (AsnBufBits &b)const;
  void			 DecodeWithSizeConstraint (AsnBufBits &b, AsnLen &bitsDecoded);
  AsnLen		 PEnc (AsnBufBits &b) const;
  void			 PDec (AsnBufBits &b, AsnLen &bitsDecoded);
  

  void            Print(std::ostream& os, unsigned short indent = 0) const;
  void            PrintXML (std::ostream &os, const char *lpszTitle=NULL) const;

#if META
  static const AsnBitsTypeDesc   _desc;

  const AsnTypeDesc      *_getdesc() const;

#if TCL
  int            TclGetVal (Tcl_Interp *) const;
  int            TclSetVal (Tcl_Interp *, const char *val);
#endif /* TCL */
#endif /* META */

private:
  bool            BitsEquiv (const AsnBits &ab) const;
  void            BDecConsBits (const AsnBuf &b, AsnLen elmtLen, AsnLen &bytesDecoded);

protected:
  size_t         bitLen;
  unsigned char* bits;
  bool           nblFlag;
};

extern SNACCDLL_API char	numToHexCharTblG[];

#define TO_HEX(fourBits)	(numToHexCharTblG[(fourBits) & 0x0F])

//########################################################################

class SNACCDLL_API AsnInt : public AsnType, protected PERGeneral
{
 protected:

    unsigned char *m_bytes;
    unsigned long  m_len;
   
    void storeDERInteger(const unsigned char *pDataCopy, long dataLen,
                         bool unsignedFlag);
    void Clear() { delete [] m_bytes; m_bytes = NULL; m_len = 0; }
    long lEncLen() const { return length(); }
    char getByte(long offset) const { return m_bytes[offset]; }
    void putByte(long offset, unsigned char cByte);

    virtual AsnLen Interpret(AsnBufBits &b,long offset)const;
    virtual void Deterpret(AsnBufBits &b, AsnLen &bitsDecoded, long offset);
    virtual void Allocate(long size);

public:
   AsnInt (AsnIntType val=0);
   AsnInt (const char *str, bool unsignedFlag = true);
   AsnInt (const AsnOcts &o, bool unsignedFlag = true);
   AsnInt (const char *str, const size_t len, bool unsignedFlag = true);
   AsnInt (const AsnInt &that);
   virtual ~AsnInt ();

   virtual const ValueRange* ValueRanges(int &sizeVRList) const
   { sizeVRList = 0; return NULL; }
      
   virtual AsnType* Clone() const { return new AsnInt(*this); }
   virtual const char* typeName() const { return "AsnInt"; }

   template <typename int_t>
   int_t toInteger() const
   {
       //FUNC("AsnInt::toInteger");

       if (!m_bytes || m_len <= 0)
           return 0;

       int_t iResult = 0;

       if (m_len > sizeof(int_t)) {
           throw SNACCDLL_API
               SnaccException("integer is too big for conversion to type",
                              DECODE_ERROR);
       }

       // If big int is negative initialize result to -1
       if ((m_bytes[0] >> 7 == 1)) {
           iResult = -1;
       }

       for (unsigned int i = 0; i < m_len; i++)
           iResult = (iResult << 8) | (AsnUIntType)(m_bytes[i]);
       return iResult;
   }

   operator AsnIntType() const { return toInteger<AsnIntType>(); }
   bool operator == (AsnIntType o) const;
   bool operator != (AsnIntType o) const { return !operator==(o);}
   bool operator == (const AsnInt &o) const;
   bool operator != (const AsnInt &o) const;
   bool operator < (const AsnInt &o) const;
   AsnInt &operator = (const AsnInt &o);

   long length() const { return m_len; }
   long length(void) { return m_len; }
   const unsigned char *c_str(void) const { return m_bytes; }
   void getPadded(unsigned char *&data, size_t &len,
                  const size_t padToSize=0) const;

   int checkConstraints (ConstraintFailList* pConstraintFails) const;

   void Set(const unsigned char *str, size_t len, bool unsignedFlag=true);
   void Set(AsnIntType i);

   AsnLen         BEnc (AsnBuf &b) const;
   void           BDec (const AsnBuf &b, AsnLen &bytesDecoded);
   AsnLen         BEncContent (AsnBuf &b) const;
   void           BDecContent (const AsnBuf &b, AsnTag tagId, AsnLen elmtLen,
                               AsnLen &bytesDecoded);

   virtual AsnLen PEnc (AsnBufBits &b) const;
   AsnLen         PEncSemiConstrained(AsnBufBits &b, long lowerBound ) const;
   AsnLen         PEncFullyConstrained(AsnBufBits &b, long lowerBound,
                                       long upperBound) const;

   void           PDecSemiConstrained(AsnBufBits &b, long lowerBound,
                                      AsnLen &bitsDecoded);
   void           PDecFullyConstrained(AsnBufBits &b, long lowerBound,
                                       long upperBound, AsnLen &bitsDecoded);
   void	          PDec(AsnBufBits &b, AsnLen &bitsDecoded);


   void      Print(std::ostream& os, unsigned short indent = 0) const;
   void      PrintXML (std::ostream &os, const char *lpszTitle=NULL) const; 

#if META
  static const AsnIntTypeDesc   _desc;

  const AsnTypeDesc      *_getdesc() const;

#if TCL
  int            TclGetVal (Tcl_Interp *) const;
  int            TclSetVal (Tcl_Interp *, const char *val);
#endif /* TCL */
#endif /* META */
};

//########################################################################

class SNACCDLL_API AsnEnum : public AsnInt
{
public:
	AsnEnum(int val = 0) : AsnInt(val)						{}

	AsnLen BEnc(AsnBuf& b) const;
	void   BDec(const AsnBuf& b, AsnLen& bytesDecoded);

	virtual AsnType *Clone() const					{ return new AsnEnum(*this); }
    virtual const char* typeName() const			{ return "AsnEnum"; }

	long IndexedVal(long* penumList, long numVals) const;
	void SetIndex(long* penumList, long numVals, long index);

#if META
	static const AsnEnumTypeDesc _desc;
	const AsnTypeDesc* _getdesc() const;

#if TCL
	int TclGetVal(Tcl_Interp * ) const;
	int TclSetVal(Tcl_Interp * , const char* val);
#endif /* TCL */
#endif /* META */
};

//########################################################################

class SNACCDLL_API AsnReal : public AsnType
{
protected:
  double         value;

public:
   AsnReal():value (0.0){}
   AsnReal (double val):value (val){}

   virtual AsnType *Clone() const				{ return new AsnReal(*this); }
   virtual const char* typeName() const			{ return "AsnReal"; }

                  operator double() const         { return value; }
   AsnReal       &operator = (double newvalue)    { value = newvalue; return *this; }

   AsnLen         BEncContent (AsnBuf &b) const;
   void           BDecContent (const AsnBuf &b, AsnTag tagId, AsnLen elmtLen, AsnLen &bytesDecoded);
   AsnLen         BEnc (AsnBuf &b) const;
   void           BDec (const AsnBuf &b, AsnLen &bytesDecoded);

   AsnLen		  PEnc (AsnBufBits &b)const;
   void			  PDec (AsnBufBits &b, AsnLen &bitsDecoded);
   
   void           Print(std::ostream& os, unsigned short indent = 0) const;
   void           PrintXML (std::ostream &os, const char *lpszTitle=NULL) const; 

   char* checkRealSingleVal(const double m_SingleVal)const;
   char* checkRealValRange(const double m_Lower, const double m_Upper) const;
 

#if META
  static const AsnRealTypeDesc   _desc;

  const AsnTypeDesc      *_getdesc() const;

#if TCL
  int            TclGetVal (Tcl_Interp *) const;
  int            TclSetVal (Tcl_Interp *, const char *val);
#endif /* TCL */
#endif /* META */
};

extern const AsnReal      PLUS_INFINITY;
extern const AsnReal      MINUS_INFINITY;

//########################################################################

class SNACCDLL_API AsnRelativeOid : public AsnType
{
public:
	AsnRelativeOid()							{ Init(); }
	AsnRelativeOid(const AsnRelativeOid& o)		{ Init(); Set(o); }
	AsnRelativeOid(const char* pszOID)			{ Init(); Set(pszOID); }
	virtual ~AsnRelativeOid();

	AsnRelativeOid& operator=(const AsnRelativeOid& o)	{ Set(o); return *this; }
	operator const char*() const;

	virtual AsnType* Clone() const			{ return new AsnRelativeOid(*this); }
	virtual const char* typeName() const	{ return "AsnRelativeOid"; }
	size_t Len() const						{ return octetLen; }
	const char* Str() const					{ return oid; }
   
	bool operator==(const AsnRelativeOid& o) const	{ return OidEquiv(o); }
	bool operator==(const char* o) const;
	bool operator!=(const AsnRelativeOid& o) const	{ return !operator==(o); }
	bool operator!=(const char* o) const			{ return !operator==(o); }
	bool operator<(const AsnRelativeOid& o) const;

	unsigned long NumArcs() const;
	void GetOidArray(unsigned long oidArray[]) const;

	void Set(const char* szOidCopy);
	void Set(const char* encOid, size_t len);
	void Set(const AsnRelativeOid& o);
	void Set(unsigned long arcNumArr[], unsigned long arrLength);

	AsnLen BEnc(AsnBuf& b) const;
	void   BDec(const AsnBuf& b, AsnLen& bytesDecoded);
	AsnLen BEncContent(AsnBuf& b) const;
	void   BDecContent(const AsnBuf& b, AsnTag tagId, AsnLen elmtLen,
		AsnLen& bytesDecoded);

	AsnLen PEnc(AsnBufBits& b) const;
	void   PDec(AsnBufBits& b, AsnLen& bitsDecoded);

	void   Print(std::ostream& os, unsigned short indent = 0) const;
	void   PrintXML(std::ostream& os, const char* lpszTitle = NULL) const;

#if META
	static const AsnRelativeOidTypeDesc _desc;
	const AsnTypeDesc* _getdesc() const;

#if TCL
	int TclGetVal(Tcl_Interp* ) const;
	int TclSetVal(Tcl_Interp* , const char* val);
#endif /* TCL */
#endif /* META */

protected:
	size_t			octetLen;
	char*			oid;
	mutable char*	m_lpszOidString;
	bool			m_isRelative;

private:
	void Init()			{ octetLen = 0; oid = NULL; m_lpszOidString = NULL;
		m_isRelative = true; }
	bool OidEquiv(const AsnRelativeOid& o) const;
	void createDottedOidStr() const;
};

//########################################################################

class SNACCDLL_API AsnOid : public AsnRelativeOid
{
public:
	AsnOid();
	AsnOid(const char* pszOID);
	AsnOid(const AsnOid &that):AsnRelativeOid(that) {m_isRelative = false;}

	virtual AsnType* Clone() const				{ return new AsnOid(*this); }
	virtual const char* typeName() const		{ return "AsnOid"; }

	// get a copy of the OID's NULL-terminated dotted string notation
	char* GetChar() const		{ return strdup(operator const char*()); }

	// set from a number-dot null term string
	void PutChar(const char* szOidCopy)					{ Set(szOidCopy); }
   
	AsnOid operator+(const AsnRelativeOid& ro) const;
	AsnOid& operator+=(const AsnRelativeOid& ro);

#if META
	static const AsnOidTypeDesc _desc;
	const AsnTypeDesc* _getdesc() const;
#endif /* META */
};

//########################################################################

//hash.h
#define TABLESIZE 256
#define INDEXMASK 0xFF
#define INDEXSHIFT 8

typedef void *Table[TABLESIZE];

typedef unsigned int Hash;

typedef struct HashSlot
{
  int    leaf;
  Hash   hash;
  void  *value;
  Table *table;
} HashSlot;

Hash MakeHash (const char *str, size_t len);

Table *InitHash();

int Insert (Table *table, void *element, Hash hash);

int CheckFor (Table *table, Hash hash);

int CheckForAndReturnValue (Table *table, Hash hash, void **value);

//########################################################################

/* this is put into the hash table with the int or oid as the key */
class SNACCDLL_API AnyInfo
{
public:
  int            anyId;   // will be a value from the AnyId enum
  AsnOid         oid;   // will be zero len/null if intId is valid
  AsnIntType     intId;
  AsnType        *typeToClone;
};

class SNACCDLL_API AsnAny: public AsnType
{
public:
   static Table         *oidHashTbl;   // all AsnAny class SNACCDLL_API instances
   static Table         *intHashTbl;   // share these tables
   mutable AnyInfo      *ai;      // points to entry in hash tbl for this type
   AsnType              *value;
   AsnBuf               *anyBuf; // used if ai == null

   AsnAny()            { ai = NULL; value = NULL; anyBuf=NULL;}
   AsnAny(const AsnAny &o) { ai = NULL; value = NULL; anyBuf = NULL; *this = o; }
   virtual ~AsnAny();

   AsnAny &operator = (const AsnAny &o);

   virtual AsnType* Clone() const				{ return new AsnAny(*this); }
   virtual const char* typeName() const			{ return "AsnAny"; }

   static void AsnAnyDestroyHashTbls(); 
   
   // Recursive call to destroy table during destruction
   static void AsnAnyDestroyHashTbl(Table *&pHashTbl);

   // class SNACCDLL_API level methods
   static void         InstallAnyByInt (AsnIntType intId, int anyId, AsnType *type);
   static void         InstallAnyByOid (AsnOid &oid,  int anyId, AsnType *type);

   int            GetId()   const            { return ai ? ai->anyId : -1; }
   void           SetTypeByInt (const AsnInt& id) const;
   void           SetTypeByOid (const AsnOid &id) const;

   AsnLen         BEnc (AsnBuf &b) const;
   void           BDec (const AsnBuf &b, AsnLen &bytesDecoded);

   void           BDecContent (const AsnBuf &b, AsnTag tag, AsnLen len, AsnLen &bytesDecoded);

   AsnLen	      PEnc (AsnBufBits &b) const;
   void           PDec (AsnBufBits &b, AsnLen &bitsDecoded);

   void           Print(std::ostream& os, unsigned short indent = 0) const;
   void           PrintXML(std::ostream &os, const char *lpszTitle=NULL) const; 

#if TCL
   int            TclGetDesc (Tcl_DString *) const;
   int            TclGetVal (Tcl_DString *) const;
   int            TclSetVal (Tcl_Interp *, const char *val);
   int            TclUnSetVal (Tcl_Interp *, const char *member);
#endif /* TCL */

};

//########################################################################
// SNACC ASN.1 String Types
//
class SNACCDLL_API AsnString : public std::string, public AsnType, protected PERGeneral
{
public:
	AsnString(const char* str = NULL)				 { operator=(str); }
	AsnString(const std::string& str)				 { operator=(str); }
    AsnString(const AsnString& aStr) : std::string() { operator=(aStr.c_str()); }     

	AsnString& operator=(const char* str);
	AsnString& operator=(const std::string& str)	{ assign(str); return *this; }
	
	void Set(const char* str){operator=(str);}
	 
	virtual const SizeConstraint* SizeConstraints(int &sizeList)const { sizeList = 0; return NULL;}
	virtual const char* PermittedAlphabet(int &sizeAlpha) const;
	
	int cvt_LDAPtoStr  (char *in_string, char  **char_ptr);
	int cvt_StrtoLDAP  (wchar_t *in_string, char  **char_ptr);
	
	AsnLen BEnc(AsnBuf &b) const;
	void   BDec(const AsnBuf &b, AsnLen& bytesDecoded);
	AsnLen BEncContent(AsnBuf &b) const;
	void   BDecContent(const AsnBuf &b, AsnTag tagId, AsnLen elmtLen,
		AsnLen& bytesDecoded);

	AsnLen PEnc(AsnBufBits &b)const;
	void   PDec(AsnBufBits &b, AsnLen &bitsDecoded);

	int checkConstraints (ConstraintFailList* pConstraintFails)const;
	const char* checkStringTypPermittedAlpha(const char* m_Alpha, long m_AlphaSize)const;

    // each string type must implement these methods
	// 
	// tagCode should be implemented to return the ASN.1 tag corresponding to the 
	// character string type.
	//
	// the check method should be implemented to enforce any rules imposed on the 
	// character string type.
	virtual BER_UNIV_CODE tagCode(void) const = 0;
	virtual bool check() const						{ return true; }
	
	

	void Print(std::ostream& os, unsigned short indent = 0) const;
	void PrintXML(std::ostream& os, const char *lpszTitle = NULL) const;

protected:
	void	Clear()									{ erase(); }
    char*	getChar(long offset)const;
	int		numBits()const;
	void	putChar(char* seg){append(seg, 1);}
	int		findB2(int B)const;

	AsnLen	EncodeWithSizeConstraint(AsnBufBits &b)const;
	void	DecodeWithSizeConstraint(AsnBufBits &b, AsnLen &bitsDecoded);
	long	FindSizeConstraintBounds(int &iSCLowerBound, int &iSCUpperBound)const;

	virtual AsnLen	Interpret(AsnBufBits &b, long offset)const;  
	virtual	long	lEncLen()const{return length();}
	virtual void	Deterpret(AsnBufBits &b, AsnLen &bitsDecoded, long offset);
	virtual void	Allocate(long){/*std::string automatically allocates mem*/};
		
private:
	void BDecConsString(const AsnBuf &b, AsnLen elmtLen, AsnLen &bytesDecoded);
};


class SNACCDLL_API NumericString : public AsnString
{
public:
	NumericString(const char* str = NULL) : AsnString(str)	{ }
	NumericString(const std::string& str) : AsnString(str)	{ }
	
	NumericString& operator=(const char* str)
		{ AsnString::operator=(str); return *this;}
	NumericString& operator=(const std::string& str)
		{ AsnString::operator=(str); return *this; }

	AsnType* Clone() const				{ return new NumericString(*this); }
	const char* typeName() const		{ return "NumericString"; }
	BER_UNIV_CODE tagCode() const		{ return NUMERICSTRING_TAG_CODE; }

    virtual const SizeConstraint* SizeConstraints(int &sizeList)const;

	const char* PermittedAlphabet(int &alphaSize) const;

	bool check() const;				// Enforce string rules
};


class SNACCDLL_API PrintableString : public AsnString
{
public:
	PrintableString(const char* str = NULL) : AsnString(str) { }
	PrintableString(const std::string& str) : AsnString(str) { }
	
	PrintableString& operator=(const char* str)
		{ AsnString::operator=(str); return *this;}
	PrintableString& operator=(const std::string& str)
		{ AsnString::operator=(str); return *this; }

	AsnType* Clone() const				{ return new PrintableString(*this); }
	const char* typeName() const		{ return "PrintableString"; }
	BER_UNIV_CODE tagCode() const		{ return PRINTABLESTRING_TAG_CODE; }
	
    virtual const SizeConstraint* SizeConstraints(int &sizeList)const { sizeList = 0; return NULL;}

	const char* PermittedAlphabet(int &sizeAlpha) const;

	bool check() const;	// Enforce string rules
};


class SNACCDLL_API TeletexString : public AsnString
{
public:
	TeletexString(const char* str = NULL) : AsnString(str)		{}
	TeletexString(const std::string& str) : AsnString(str)		{}
	
	TeletexString& operator=(const char* str)
		{ AsnString::operator=(str); return *this;}
	TeletexString& operator=(const std::string& str)
		{ AsnString::operator=(str); return *this; }

	AsnType* Clone() const				{ return new TeletexString(*this); }
	const char* typeName() const		{ return "TeletexString"; }
	BER_UNIV_CODE tagCode() const		{ return TELETEXSTRING_TAG_CODE; }
};

// T61String -- Alternate name for TeletexString
typedef TeletexString T61String;


class SNACCDLL_API VideotexString : public AsnString
{
public:
	VideotexString(const char* str = NULL) : AsnString(str)		{}
	VideotexString(const std::string& str) : AsnString(str)		{}
	
	VideotexString& operator=(const char* str)
		{ AsnString::operator=(str); return *this;}
	VideotexString& operator=(const std::string& str)
		{ AsnString::operator=(str); return *this; }

	AsnType* Clone() const				{ return new VideotexString(*this); }
	const char* typeName() const		{ return "VideotexString"; }
	BER_UNIV_CODE tagCode() const		{ return VIDEOTEXSTRING_TAG_CODE; }
};

class SNACCDLL_API IA5String : public AsnString
{
public:
	IA5String(const char* str = NULL) : AsnString(str)	{ }
	IA5String(const std::string& str) : AsnString(str)	{ }
	
	IA5String& operator=(const char* str)
		{ AsnString::operator=(str); return *this;}
	IA5String& operator=(const std::string& str)
		{ AsnString::operator=(str); return *this;}

	AsnType* Clone() const					{ return new IA5String(*this); }
	const char* typeName() const			{ return "IA5String"; }
	BER_UNIV_CODE tagCode() const			{ return IA5STRING_TAG_CODE; }

	virtual const SizeConstraint* SizeConstraints(int &sizeList)const { sizeList = 0; return NULL;}
	const char* PermittedAlphabet(int &sizeAlpha) const;
	
	bool check() const;				// Enforce string rules
};


class SNACCDLL_API GraphicString : public AsnString
{
public:
	GraphicString(const char* str = NULL) : AsnString(str)		{}
	GraphicString(const std::string& str) : AsnString(str)		{}
	
	GraphicString& operator=(const char* str)
		{ AsnString::operator=(str); return *this;}
	GraphicString& operator=(const std::string& str)
		{ AsnString::operator=(str); return *this; }

	AsnType* Clone() const				{ return new GraphicString(*this); }
	const char* typeName() const		{ return "GraphicString"; }
	BER_UNIV_CODE tagCode() const		{ return GRAPHICSTRING_TAG_CODE; }
};


class SNACCDLL_API VisibleString : public AsnString
{
public:
	VisibleString(const char* str = NULL) : AsnString(str){ }
	VisibleString(const std::string& str) : AsnString(str){ }
	
	VisibleString& operator=(const char* str)
		{ AsnString::operator=(str); return *this;}
	VisibleString& operator=(const std::string& str)
		{ AsnString::operator=(str); return *this; }

	AsnType* Clone() const				{ return new VisibleString(*this); }
	const char* typeName() const		{ return "VisibleString"; }
	BER_UNIV_CODE tagCode() const		{ return VISIBLESTRING_TAG_CODE; }
	
	virtual const SizeConstraint* SizeConstraints(int &sizeList)const { sizeList = 0; return NULL;}
    const char* PermittedAlphabet(int &sizeAlpha) const;

	bool check() const;				// Enforce string rules
};

// ISO646String -- Alternate name for VisibleString
typedef VisibleString ISO646String;


class SNACCDLL_API GeneralString : public AsnString
{
public:
	GeneralString(const char* str = NULL) : AsnString(str)		{}
	GeneralString(const std::string& str) : AsnString(str)		{}
	
	GeneralString& operator=(const char* str)
		{ AsnString::operator=(str); return *this;}
	GeneralString& operator=(const std::string& str)
		{ AsnString::operator=(str); return *this; }

	AsnType* Clone() const				{ return new GeneralString(*this); }
	const char* typeName() const		{ return "GeneralString"; }
	BER_UNIV_CODE tagCode() const		{ return GENERALSTRING_TAG_CODE; }
};


// Multi-byte character based definitions... BMP, Universal, UTF8.
class SNACCDLL_API WideAsnString : public std::wstring, public AsnType, protected PERGeneral
{
public:
	WideAsnString(const char* str = NULL)				{ set(str); }
	WideAsnString(const std::string& str)				{ set(str.c_str()); }
	WideAsnString(const std::wstring& wstr) 			{ assign(wstr); }


	// each string type must implement these methods
	// 
	// tagCode should be implemented to return the ASN.1 tag corresponding to the 
	// character string type.
	//
	// the check method should be implemented to enforce any rules imposed on the 
	// character string type.
	virtual BER_UNIV_CODE tagCode() const = 0;
//	virtual bool check() const = 0;

	virtual const SizeConstraint* SizeConstraints(int &sizeList)const { sizeList = 0; return NULL;}

	AsnLen BEnc(AsnBuf &b) const;
	void   BDec(const AsnBuf &b, AsnLen &bytesDecoded);
	virtual AsnLen BEncContent(AsnBuf &b) const = 0;
	virtual void   BDecContent(const AsnBuf &b, AsnTag tagId, AsnLen elmtLen,
		AsnLen &bytesDecoded) = 0;
	
	AsnLen PEnc(AsnBufBits &b)const;
	
	void   PDec(AsnBufBits &b, AsnLen &bitsDecoded);

	void Print(std::ostream& os, unsigned short indent = 0) const;
	void PrintXML(std::ostream &os, const char *lpszTitle) const;

	int checkConstraints (ConstraintFailList* pConstraintFails)const;
	const char* checkStringTypPermittedAlpha(const char* m_Alpha, long m_AlphaSize)const;

	virtual bool check() const						{ return true; }
    virtual const char* PermittedAlphabet(int &sizeAlpha)const { sizeAlpha = 0; return NULL;};

	void set(const char* str);

	void getAsUTF8(std::string &utf8String) const;
	char* getAsUTF8() const;
	
protected:
	void		Clear()								{ erase(); }
	void		putWideChar(wchar_t* seg){append(seg, 1);}
	long		lEncLen()const{return length();}
	wchar_t*	getWideChar(long offset)const;
	virtual		AsnLen Interpret(AsnBufBits &b, long offset)const;

	virtual void	Deterpret(AsnBufBits &b, AsnLen &bitsDecoded, long offset);
	virtual void	Allocate(long){/*std::string automatically allocates mem*/};

	AsnLen CombineConsString(const AsnBuf &b, AsnLen elmtLen, std::string& encStr);
};

class SNACCDLL_API BMPString : public WideAsnString
{
public:
	BMPString(const char* str = NULL) : WideAsnString(str)		{}
	BMPString(const std::string& str) : WideAsnString(str)		{}
	BMPString(const std::wstring& wstr) : WideAsnString(wstr)	{}
	
	BMPString& operator=(const char* str)
		{ set(str); return *this;}
	BMPString& operator=(const std::string& str)
		{ set(str.c_str()); return *this; }
	BMPString& operator=(const std::wstring& wstr)
		{ assign(wstr); return *this; }
	
	AsnType* Clone() const					{ return new BMPString(*this); }
	const char* typeName() const			{ return "BMPString"; }
	BER_UNIV_CODE tagCode() const			{ return BMPSTRING_TAG_CODE; }
	
	AsnLen BEncContent(AsnBuf &b) const;
	void   BDecContent(const AsnBuf &b, AsnTag tagId, AsnLen elmtLen,
		AsnLen &bytesDecoded);
};


class SNACCDLL_API UniversalString : public WideAsnString
{
public:
	UniversalString(const char* str = NULL) : WideAsnString(str)		{}
	UniversalString(const std::string& str) : WideAsnString(str)		{}
	UniversalString(const std::wstring& wstr) : WideAsnString(wstr) 	{}
	
	UniversalString& operator=(const char* str)
		{ set(str); return *this;}
	UniversalString& operator=(const std::string& str)
		{ set(str.c_str()); return *this; }
	UniversalString& operator=(const std::wstring& wstr)
		{ assign(wstr); return *this; }

	AsnType* Clone() const				{ return new UniversalString(*this); }
	const char* typeName() const		{ return "UniversalString"; }
	BER_UNIV_CODE tagCode() const		{ return UNIVERSALSTRING_TAG_CODE; }

	AsnLen BEncContent(AsnBuf &b) const;
	void   BDecContent(const AsnBuf &b, AsnTag tagId, AsnLen elmtLen,
		AsnLen &bytesDecoded);

	/*void   PDecConstent(const AsnBuf&b, AsnTag tagId, AsnLen elmtLen, 
	*	AsnLen &bytesDecoded);
	*/
};


class SNACCDLL_API UTF8String : public WideAsnString
{
public:
	UTF8String(const char* str = NULL) : WideAsnString(str) 		{}
	UTF8String(const std::string& str) : WideAsnString(str) 		{}
	UTF8String(const std::wstring& wstr) : WideAsnString(wstr) 		{}
	
	UTF8String& operator=(const char* str)
		{ set(str); return *this;}
	UTF8String& operator=(const std::string& str)
		{ set(str.c_str()); return *this; }
	UTF8String& operator=(const std::wstring& wstr)
		{ assign(wstr); return *this; }

	AsnType* Clone() const				{ return new UTF8String(*this); }
	const char* typeName() const		{ return "UTF8String"; }
	BER_UNIV_CODE tagCode() const		{ return UTF8STRING_TAG_CODE; }

	AsnLen BEncContent(AsnBuf &b) const;
	void   BDecContent(const AsnBuf &b, AsnTag tagId, AsnLen elmtLen,
		AsnLen &bytesDecoded);
};

//########################################################################

// Time Classes
//
class SNACCDLL_API UTCTime : public VisibleString
{
public:
	UTCTime(const char* str = NULL) : VisibleString(str)	{}
	UTCTime(const std::string& str) : VisibleString(str)	{}
	
	UTCTime& operator=(const char* str)
		{ VisibleString::operator=(str); return *this;}
	UTCTime& operator=(const std::string& str)
		{ VisibleString::operator=(str); return *this; }

	AsnType* Clone() const				{ return new UTCTime(*this); }
	const char* typeName() const		{ return "UTCTime"; }
	
	// ASN.1 tag for the character string
	BER_UNIV_CODE tagCode() const		{ return UTCTIME_TAG_CODE; }
};


class SNACCDLL_API GeneralizedTime : public VisibleString
{
public:
	GeneralizedTime(const char* str = NULL) : VisibleString(str)	{}
	GeneralizedTime(const std::string& str) : VisibleString(str)	{}
	
	GeneralizedTime& operator=(const char* str)
		{ VisibleString::operator=(str); return *this;}
	GeneralizedTime& operator=(const std::string& str)
		{ VisibleString::operator=(str); return *this; }

	AsnType* Clone() const				{ return new GeneralizedTime(*this); }
	const char* typeName() const		{ return "GeneralizedTime"; }
	BER_UNIV_CODE tagCode() const		{ return GENERALIZEDTIME_TAG_CODE; }
};

// Object Descriptor class
class SNACCDLL_API ObjectDescriptor : public GraphicString
{
public:
	ObjectDescriptor(const char* str = NULL) : GraphicString(str)	{}
	ObjectDescriptor(const std::string& str) : GraphicString(str)	{}

	ObjectDescriptor& operator=(const char* str)
		{ GraphicString::operator=(str); return *this;}
	ObjectDescriptor& operator=(const std::string& str)
		{ GraphicString::operator=(str); return *this; }

	AsnType* Clone() const			{ return new ObjectDescriptor(*this); }
	const char* typeName() const	{ return "ObjectDescriptor"; }
	BER_UNIV_CODE tagCode() const	{ return OD_TAG_CODE; }
};

//########################################################################

// AnyDefinedBy is currently the same as AsnAny:
typedef AsnAny         AsnAnyDefinedBy;

/* JKG -- 01/27/2004
   AsnExtension class added in support for ASN.1 Extensibility syntax */
class SNACCDLL_API AsnExtension : public AsnType
{
public:
	std::list<AsnAny> extList;

	AsnExtension() {}
    AsnExtension(const AsnExtension &ext) { operator=(ext);}     
	~AsnExtension()	{}

	AsnType* Clone() const			{ return new AsnExtension(*this); }
	const char* typeName() const	{ return "AsnExtension"; }

    void operator=(const AsnExtension& ext){extList = ext.extList;}

	int checkConstraints(ConstraintFailList* ) const	{ return 0; }

	void   BDec(const AsnBuf&, AsnLen&) {/* Implemented in generated code */}
	void   PDec(AsnBufBits&, AsnLen&)   {/* Implemented in generated code */}
	AsnLen BEnc(AsnBuf& b) const;
	AsnLen PEnc(AsnBufBits& b) const;

	void   Print (std::ostream& os, unsigned short indent = 0) const;
    void   PrintXML (std::ostream &, const char * = NULL) const {} 
};


void SNACCDLL_API threadLock();
void SNACCDLL_API threadUnlock();
void SNACCDLL_API threadDestroy();
extern "C" {
void SNACCDLL_API SNACC_CleanupMemory();
}

//########################################################################
//########################################################################


enum SNACCEncodeDecodeRules {
    SNACC_ASCII,
    BER,
    PER
};

_END_SNACC_NAMESPACE

#include "asn-iomanip.h"
#include "snaccexcept.h"
#include "asn-usefultypes.h"

#endif
