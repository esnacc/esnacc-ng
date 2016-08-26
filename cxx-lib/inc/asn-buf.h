#ifndef _ASN_BUF_H
#define _ASN_BUF_H 1


#if defined(_MSC_VER)
#pragma warning(disable: 4100 4702 4710 4514 4786 4251 4018 4146 4284)
#pragma warning(push,3)
#endif

#include <deque>
#include <fstream>
#include <list>
#include <ostream>
#include <sstream>
#include <streambuf>
#include <vector>

#if defined(_MSC_VER)
#pragma warning(pop)
#endif

#if !defined(WIN32)
#include <utility>
#endif // WIN32


#ifndef SNACCDLL_API
#ifdef WIN32
#ifdef SNACCDLL_EXPORTS
#define SNACCDLL_API __declspec(dllexport)
#else
#ifdef SNACCDLL_NONE
#define SNACCDLL_API 
#else
#define SNACCDLL_API __declspec(dllimport)
#endif
#endif
#else
#define SNACCDLL_API 
#endif
#endif

#define _SEG_SIZE 4096



namespace SNACC
{

// Forward declarations
class Card;
class AsnFileSeg;
class AsnBits;

typedef unsigned long      AsnLen;

typedef std::deque<Card *> Deck;

class SNACCDLL_API AsnFDBuf : public std::streambuf
{
    int fd;
    const bool sock;
    const size_t putsz;
    const bool close_on_fail;
    std::vector<char> buffer;

protected:
    virtual void extra_reset();
    std::streambuf::int_type underflow();
    std::streambuf::int_type overflow(std::streambuf::int_type c);
    std::streamsize xsputn (const char* s, std::streamsize num);

public:
    explicit AsnFDBuf(int descriptor, bool socket = false, bool failure = true,
                      size_t buffersz = 256, size_t putsize = 8)
        : fd(descriptor), sock(socket), putsz(std::max<int>(putsize,1)),
        close_on_fail(failure), buffer(std::max<int>(buffersz,putsz) + putsz)
    {
        char *end = &buffer.front() + buffer.size();
        setg(end, end, end);
    }
};

struct SNACCDLL_API AsnBufLoc
{
   Deck::iterator m_card;
   long           m_offset;
};

class SNACCDLL_API AsnBuf
{
public:
	AsnBuf();
	AsnBuf(const char* seg, size_t segLen);
	AsnBuf(const std::stringstream& ss);
	AsnBuf(std::streambuf* sb);
	AsnBuf(const AsnBuf& o);
	AsnBuf(const char* pFilename);
	~AsnBuf()									{ clear(); }

	AsnBuf& operator=(const AsnBuf& o);
	bool operator==(const AsnBuf& b) const;
	bool operator<(const AsnBuf& rhs) const;

	void PutByteRvs(char byte);
	void PutSegRvs(const char* seg, size_t segLen);
	void PutSegRvs(const unsigned char *seg, size_t segLen) {
		PutSegRvs((const char*)seg, segLen); }
	void PutStream(std::streambuf *sb);

	void skip(size_t skipBytes);
	char PeekByte() const;
	char GetByte() const;
	unsigned char GetUByte() const		{ return (unsigned char)GetByte();}
//	unsigned long GetSeg(char* seg, long segLen) const;
	void GetSeg(char* seg, long segLen) const;
	void GetUSeg(unsigned char* seg, long segLen) const {
		GetSeg((char*)seg, segLen); }
	char* GetSeg(long segLen) const;
	void GetSeg(std::string& str, long segLen = 0) const;
	AsnFileSeg* GetFileSeg(long segLen) const;
	void PutFileSeg(AsnFileSeg* fs);

    void UnGetBytes(long lBytesToPutBack) const;

	void GrabAny(AsnBuf& anyBuf, SNACC::AsnLen& bytesDecoded) const;
	const Deck& deck() const							{ return m_deck; }
	const Card& card() const							{ return **m_card; }
	unsigned long length() const;

	void insert(const AsnBuf& that);
	long splice(AsnBuf& b);
	void hexDump(std::ostream& os) const;
   
#ifdef _DEBUG
	void status(std::ostream& os);
#endif

	AsnBufLoc GetReadLoc() const;
	void SetReadLoc(const AsnBufLoc& bl) const;
	void ResetMode(std::ios_base::openmode mode = std::ios_base::in) const;

private:
	void clear();

protected:
	mutable SNACC::Deck::iterator m_card;
	mutable SNACC::Deck m_deck;
};

class SNACCDLL_API AsnBufBits 
{
private:
	std::streambuf* m_pbuf;				// Stream buffer containing the bits
	bool			m_isInternalBuf;	// Indicates if streambuf is internal
    bool            bAlign;
	unsigned char   m_ucWriteCharBuf[2];
	int             m_iWriteBitPos;
	unsigned char   m_ucReadCharBuf[2];	// Read character buffer
	int             m_iReadBitPos;		// Number of bits read from char buffer
	unsigned long   m_ulNumBits;		// Number of bits in the streambuf?
	unsigned long   m_ulBitsLeft;		// Number of bits in the streambuf

public:
	AsnBufBits(std::streambuf *pbuf, bool bAligned = false)
	{
        m_pbuf = pbuf;   // APPLICATION relinquishes memory.
		m_isInternalBuf = false;
        bAlign = bAligned;
        Clear();
    }
	AsnBufBits(bool bAligned = false)
	{
        //m_pbuf = new std::strstreambuf;
        m_pbuf = new std::stringbuf;
		  m_isInternalBuf = true;
        bAlign = bAligned;
        Clear();
    }
    void Clear()
    {
    	m_ucWriteCharBuf[0] = 0x00;
		m_iWriteBitPos      = 8;
		m_ucReadCharBuf[0]  = 0x00;
		m_iReadBitPos       = 8;
		m_ulNumBits         = 0;
		m_ulBitsLeft        = 0;
    }
    
    ~AsnBufBits(){ if (m_isInternalBuf && m_pbuf) delete m_pbuf; }

    
    bool IsAligned()			{ return bAlign; }

    
/*
	AsnBufBits & operator=(const AsnBufBits &buf)
	{
		if(this != &buf)
		{
			m_pbuf = buf.m_pbuf;
		    m_ucWriteCharBuf[0] = buf.m_ucWriteCharBuf[0];
		    m_iWriteBitPos      = buf.m_iWriteBitPos;
		    m_ucReadCharBuf[0]  = buf.m_ucReadCharBuf[0];
		    m_iReadBitPos       = buf.m_iReadBitPos;
		    m_ulNumBits         = buf.m_ulNumBits;
		    m_ulBitsLeft        = buf.m_ulBitsLeft;
		}
		return *this;
	}
*/

    bool operator<(AsnBufBits &rhs);

	unsigned char MaskBits(unsigned char cCharToMask, int iBitsToMask);
	unsigned long PutBits(const unsigned char* seg, unsigned long numBits);
	unsigned char* GetBits(unsigned long numBits);
	unsigned long GetBits(AsnBits& bits, unsigned long numBits);
	bool GetBit();
	unsigned char GetByte();
	
	unsigned long  length(){ return m_ulBitsLeft; }
	int 		   OctetAlignWrite();
	int 		   OctetAlignRead();

	void          hexDump(std::ostream &os);
    void          AppendTo(AsnBufBits &bufBitsOut);

    AsnBufBits(const AsnBufBits& buf);    
    AsnBufBits& operator=(const AsnBufBits& buf);

private:
	unsigned char ReadByte();	// Reads next byte from the stream
	    
};


// Class for handling memory based encodings.  It
// writes into the stream in reverse.
//
class SNACCDLL_API AsnRvsBuf : public std::streambuf
{

public:
   AsnRvsBuf(char *preFilled, size_t segSize);
   AsnRvsBuf(const char *seg, size_t segSize);
   AsnRvsBuf(const AsnBuf& otherBuf);
   explicit AsnRvsBuf(long segSize = _SEG_SIZE);
   virtual ~AsnRvsBuf();

   const char *str()  { return m_pStart; }
   int         pcount() { return ( (m_buf + m_segSize) - m_pStart ); }
 
   long max_size() const {return m_segSize;}

   friend class AsnBuf;

protected:
   virtual int_type        underflow();
   virtual int_type        overflow(int c = EOF);   
   virtual std::streamsize xsputn(const char *s, std::streamsize n);
	virtual std::streambuf::pos_type        seekoff(std::streambuf::off_type off, std::ios_base::seekdir way,
                                   std::ios_base::openmode which = std::ios_base::in | std::ios_base::out);
	virtual std::streambuf::pos_type        seekpos(std::streambuf::pos_type sp,
                                   std::ios_base::openmode which = std::ios_base::in | std::ios_base::out);

private:

   char *m_buf;
   char *m_pStart;
   char *m_pReadLoc;
   long  m_segSize;
   bool m_bDeleteable;
};


// Used to store the location of a large object
// (OCTET STRING or ANY) within an AsnBuf card that
// contains a file (i.e. filebuf).
//
class SNACCDLL_API AsnFileSeg : public std::streambuf
{
public:
   explicit AsnFileSeg(const char *pFilename);
   explicit AsnFileSeg(AsnFileSeg *afs, unsigned long segLen);
   AsnFileSeg(const AsnFileSeg &o);
   ~AsnFileSeg();
   unsigned long size() { return m_segSize; }

protected:
   virtual int_type        underflow();
   virtual int_type        uflow();

   virtual int_type        overflow(int = EOF) { return EOF; }
   virtual std::streamsize xsgetn(char *s, std::streamsize n);
   virtual std::streamsize xsputn(const char *, std::streamsize ) { return overflow(); }
   virtual pos_type        seekoff(off_type off, std::ios_base::seekdir way,
                                   std::ios_base::openmode which = std::ios_base::in | std::ios_base::out);
   virtual pos_type        seekpos(pos_type sp,
                                   std::ios_base::openmode which = std::ios_base::in | std::ios_base::out);
private:
   long            m_offset;
   long            m_segSize;
   char           *m_filename;
   std::filebuf   *m_fb;
};

// Expanding buffer which contains a deque of streambufs.
// This allows for non-contiguous memory to be used for
// encoding. Each element (or card) in the deque (deck)
// can be a file or memory based.
//

// Deck contains a streambuf * and a bool flag that indicates
// whether or not the streambuf can be deleted. 
//
enum AsnBufType { FILE_TYPE=0, RVS_BUF_TYPE, IN_MEM_TYPE, EXT_MEM_TYPE } ;

class Card : public std::pair<std::streambuf *, AsnBufType>
{
public:
#if 0
   Card()
   {  m_cDataR = 0x00;
	  m_cDataW = 0x00;
	  m_iBitPosW = 0;
      m_iBitPosR = 0;
	  first = NULL; 
	  second = EXT_MEM_TYPE;
   }
#endif

   Card(std::streambuf *sb)
   {  m_cDataR = 0x00;
	  m_cDataW = 0x00;
	  m_iBitPosW = 0;
      m_iBitPosR = 0;
      first = sb; 
	  second = EXT_MEM_TYPE; 
   }

   Card(const std::stringstream &ss)
   {  m_cDataR = 0x00;
	  m_cDataW = 0x00;
	  m_iBitPosW = 0;
      m_iBitPosR = 0;
	  first = ss.rdbuf(); 
	  second = EXT_MEM_TYPE;
   }

   Card(AsnRvsBuf *pRvsBuf)
   {  m_cDataR = 0x00;
	  m_cDataW = 0x00;
	  m_iBitPosW = 0;
      m_iBitPosR = 0;
	  first = pRvsBuf; 
	  second = RVS_BUF_TYPE;
   }
  
   Card(AsnFileSeg *pFs)
   {  m_cDataR = 0x00;
	  m_cDataW = 0x00;
	  m_iBitPosW = 0;
      m_iBitPosR = 0;
	  first = pFs; 
	  second = FILE_TYPE;
   }

 Card(const Card &o) : std::pair<std::streambuf *, AsnBufType>(o)
   {  m_cDataR = 0x00;
	  m_cDataW = 0x00;
	  m_iBitPosW = 0;
      m_iBitPosR = 0;
      first = o.first;
      second = o.second;
   }
   
   virtual ~Card()
   {
      if (second != EXT_MEM_TYPE)
         delete first;
   }

   int		BitPosW(){return m_iBitPosW;}
   int      SetBitPosW(int newBitPos){m_iBitPosW = newBitPos; return m_iBitPosW;}   
   
   int		BitPosR(){return m_iBitPosR;}
   int		SetBitPosR(int newBitPos){m_iBitPosR = newBitPos; return m_iBitPosR;}

   char     cDataW(){return m_cDataW;}
   char     cDataW(char chr){m_cDataW=chr; return m_cDataW;}
   
   char     cDataR(){return m_cDataR;}
   char     cDataR(char chr){m_cDataR=chr; return m_cDataR;}

   long size();    // number of bytes stored in card
   long length();  // number of bytes not read from card

#ifdef _DEBUG
   const char * bufTypeStr(); // return a string discribing the bufType
#endif

   void resetType(AsnBufType type) {second = type;}
   std::streambuf * rdbuf()  {return first;}
   const std::streambuf * rdbuf() const  {return first;}
   enum AsnBufType bufType() const {return second;}

private:
	int m_iBitPosW;
	int m_iBitPosR;
	char m_cDataW;
	char m_cDataR;

};

} // end namespace SNACC

SNACCDLL_API void sortSet(std::list<SNACC::AsnBuf> &bufList);

SNACCDLL_API std::ostream & operator<<(std::ostream &os, const SNACC::AsnBuf &b);



#endif // _ASN_BUF_H
