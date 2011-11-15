
_BEGIN_SNACC_NAMESPACE

class SNACCDLL_API EXTERNALChoice: public AsnType
{
public:
  enum ChoiceIdEnum
  {
     NoChoiceCid=-1,
     single_ASN1_typeCid = 0,
     octet_alignedCid = 1,
     arbitraryCid = 2
  };

  enum ChoiceIdEnum	choiceId;
  union
  {
     void * NoChoice;
     AsnOcts		*single_ASN1_type;
     AsnOcts		*octet_aligned;
     AsnBits		*arbitrary;
  };


   EXTERNALChoice() {Init();}
   EXTERNALChoice(const EXTERNALChoice& that);
public:
   void Init(void);
   virtual ~EXTERNALChoice() {Clear();}

   void Clear();

	AsnType* Clone() const				{ return new EXTERNALChoice(*this); }
	const char* typeName() const		{ return "EXTERNALChoice"; }

  EXTERNALChoice		&operator = (const EXTERNALChoice &that);
  AsnLen		BEncContent (AsnBuf &b) const;
  void			BDecContent (const AsnBuf &b, AsnTag tag, AsnLen elmtLen, AsnLen &bytesDecoded/*, s env*/);
  AsnLen		BEnc (AsnBuf &b) const;
  void			BDec (const AsnBuf &b, AsnLen &bytesDecoded/*, s env*/);

  AsnLen	    PEnc (AsnBufBits &b) const;

  void			Print(std::ostream& os, unsigned short indent = 0) const;
  void			PrintXML (std::ostream &os, const char *lpszTitle=NULL) const;
};


class SNACCDLL_API EXTERNAL: public AsnType
{
public:
  AsnOid		*direct_reference;
  AsnInt		*indirect_reference;
  ObjectDescriptor		*data_value_descriptor;
  EXTERNALChoice		*encoding;

   EXTERNAL() {Init();}
   void Init(void);
   virtual ~EXTERNAL() {Clear();}
   void Clear();
   EXTERNAL(const EXTERNAL& that);
public:
	AsnType* Clone() const					{ return new EXTERNAL(*this); }
	const char* typeName() const			{ return "EXTERNAL"; }

  EXTERNAL		&operator = (const EXTERNAL &that);
  AsnLen		BEncContent (AsnBuf &b) const;
  void	   BDecContent (const AsnBuf &b, AsnTag tag, AsnLen elmtLen, AsnLen &bytesDecoded/*, s env*/);

  AsnLen		BEnc (AsnBuf &b) const;
  void		BDec (const AsnBuf &b, AsnLen &bytesDecoded/*, s env*/);

  AsnLen    PEnc (AsnBufBits &b) const;
  
  void		Print(std::ostream& os, unsigned short indent = 0) const;
  void		PrintXML (std::ostream &os, const char *lpszTitle=NULL) const;
};

_END_SNACC_NAMESPACE
