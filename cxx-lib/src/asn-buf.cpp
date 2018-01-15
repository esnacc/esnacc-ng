#include <config.h>

#include <fcntl.h>
#include <stdio.h>
#include <sys/types.h>

#ifndef WIN32
#include <sys/socket.h>
#include <unistd.h>
#else
#define _POSIX_
#include <io.h>
#include <winsock2.h>
#include <windows.h>
#ifndef MSG_NOSIGNAL
#define MSG_NOSIGNAL 0
#endif
#endif

// MSG_NOSIGNAL does not exist under OS X
#if defined(__APPLE__) || defined(__MACH__)
# ifndef MSG_NOSIGNAL
#  define MSG_NOSIGNAL SO_NOSIGPIPE
# endif
#endif


#include "asn-incl.h"

using namespace SNACC;


// AsnBuf.cpp
AsnBuf::AsnBuf()
{
   m_card = m_deck.begin();
   //m_card = m_deck.insert(m_deck.begin(), new Card());
}

AsnBuf::AsnBuf(const char *seg, size_t segLen)
{ 
   m_card = m_deck.insert(m_deck.begin(), new Card(new AsnRvsBuf(seg, segLen))); 
}

AsnBuf::AsnBuf(const std::stringstream &ss)
{
   m_card = m_deck.insert(m_deck.begin(), new Card(ss));
}

AsnBuf::AsnBuf(std::streambuf *sb)
{
   m_card = m_deck.insert(m_deck.begin(), new Card(sb));
}

// Copy constructor
AsnBuf::AsnBuf(const AsnBuf &o)
{
   operator=(o);
}

void AsnBuf::insert(const AsnBuf &that)
{
    if (!m_deck.empty() && m_card != m_deck.end()
        && (*m_card)->length() == 0) {
        delete *m_card;
        m_card = m_deck.erase(m_card);
    }

    AsnRvsBuf *rvsBuf = new AsnRvsBuf(that);
    m_card = m_deck.insert(m_deck.begin(), new Card(rvsBuf));
}

AsnBuf & AsnBuf::operator=(const AsnBuf &o)
{
   if (this != &o)
   {
      clear();
		insert(o);
   }
   return *this;
}

AsnBuf::AsnBuf(const char *pFilename)
{
   m_card = m_deck.insert(m_deck.begin(), new Card(new AsnFileSeg(pFilename)));
}

void AsnBuf::clear()
{
   for (m_card = m_deck.begin(); m_card != m_deck.end(); m_card++)
   {
      delete *m_card;
   }
   m_deck.clear();
}

void AsnBuf::PutByteRvs(char byte)
{

   // if empty add an AsnRvsBuf card
   //
   if (m_deck.empty())
   {
      m_card = m_deck.insert(m_deck.begin(), new Card(new AsnRvsBuf));
   }

   if ((*m_card)->rdbuf()->sputc(byte) == EOF)
   {
      m_card = m_deck.insert(m_deck.begin(), new Card(new AsnRvsBuf));
      (*m_card)->rdbuf()->sputc(byte);
   }
}

void AsnBuf::PutSegRvs(const char *seg, size_t segLen)
{
   // if empty add an AsnRvsBuf card
   //
   if (m_deck.empty())
   {
      m_card = m_deck.insert(m_deck.begin(), new Card(new AsnRvsBuf));
   }

   while (segLen > 0)
   {
      segLen -= (*m_card)->rdbuf()->sputn(seg, segLen);
      if (segLen > 0)
      {
         // reuse any existing card(s)
         if (m_card == m_deck.begin()) 
            m_card = m_deck.insert(m_deck.begin(), new Card(new AsnRvsBuf));
         else
            --m_card;
      }
   }
}

//#ifdef WIN32
//void AsnBuf::ResetMode(std::ios_base::open_mode mode) const
//#else
void AsnBuf::ResetMode(std::ios_base::openmode mode) const
//#endif
{
   AsnRvsBuf tmp;
   Deck::iterator i;
   i = m_deck.begin();

   while (i != m_deck.end())
   {
      if (mode & std::ios_base::out)
      {
         // dump all cards that are not AsnRvsBuf's because
         // those are the only cards that can be encoded into.
         //
         if ((*i)->bufType() != RVS_BUF_TYPE) {
             delete *i;
             i = m_deck.erase(i);
             continue;
         }
      }
      (*i)->rdbuf()->pubseekpos(0, mode);
      i++;
   }

   if (mode == std::ios_base::in)
      m_card = m_deck.begin();
   else 
   {
      m_card = m_deck.end();
      if ( ! m_deck.empty())
         --m_card;
   }
}

// GetFileSeg()
//
// Create a AsnFileSeg object from the current card in the deck
// for the length "segLen".
//
AsnFileSeg * AsnBuf::GetFileSeg(long segLen) const
{
   FUNC("AsnBuf::GetFileSeg()");


   // If the bufType == FILE_TYPE then the card is a AsnFileSeg
   // cast it and pass it to the AsnFileSeg constructor
   //
   if ((*m_card)->bufType() == FILE_TYPE)
      return (new AsnFileSeg((AsnFileSeg *)(*m_card)->rdbuf(), segLen));
   else
      throw BufferException("GetFileSeg called with non file card", STACK_ENTRY);
}

// PutFileSeg()
//
// Copy AsnFileSeg pointer into this AsnBuf's deck.  AsnBuf assumes
// responsibility of cleanup for it.
//
void AsnBuf::PutFileSeg(AsnFileSeg *pFs)
{
   m_card = m_deck.insert(m_deck.begin(), new Card(pFs));
}

char * AsnBuf::GetSeg(long segLen) const
{
    char *seg = new (std::nothrow) char[segLen];
    if (!seg)
        return NULL;

    try {
        GetSeg(seg, segLen);
    } catch (BufferException &) {
        delete [] seg;
        seg = NULL;
    }
    return seg;
}


//unsigned long AsnBuf::GetSeg(char *seg, long segLen) const
void AsnBuf::GetSeg(char *seg, long segLen) const
{
	FUNC("AsnBuf::GetSeg()");
   long bytesRead = 0;
   long lTmp;
   
   while (segLen > 0 && m_card != m_deck.end() )
   {
      lTmp = (*m_card)->rdbuf()->sgetn(&seg[bytesRead], segLen);
   
      bytesRead += lTmp;
      if (lTmp != segLen)
         m_card++;

      segLen -= lTmp;
   }
	if (segLen > 0)
		throw BufferException("Read past end of data", STACK_ENTRY);

   //return bytesRead;
}
// FUNCTION: GetSeg()
// PURPOSE: Retrieve the contents of the AsnBuf into a std::string.
//          segLen is defaulted 0.  If defaulted to 0 then the entire
//          buffer is returned.
//
void AsnBuf::GetSeg(std::string &seg, long segLen) const
{
   long i;

   FUNC("AsnBuf::GetSeg(std::string &seg, long segLen)");

   if (segLen == 0)
      segLen = length();

   if ((unsigned long)segLen > this->length()) //RWC; TOO MUCH DATA requested...
   {
       throw BufferException("GetSeg attempt to read past end of data", STACK_ENTRY);
   }
   seg.resize(segLen);
   for (i = 0; i < segLen; i++)
   {
      seg[i] = GetByte();
   }
}

bool AsnBuf::operator<(const AsnBuf &rhs) const
{
   bool lessThan = true;
   bool firstTime = true;
   ResetMode();
   rhs.ResetMode();
   std::streambuf::int_type ch1;
   std::streambuf::int_type ch2;
   unsigned char Ch1;
   unsigned char Ch2;

   while ( lessThan )
   {
      if (GetUByteNoEx(Ch1))
      {
         ch1 = Ch1;
      }
      else
      {
         ch1 = EOF;
      }

      if (rhs.GetUByteNoEx(Ch2))
      {
         ch2 = Ch2;
      }
      else
      {
         ch2 = EOF;
      }

      if ((ch1 == EOF) && (ch2 == EOF))
      {
         if (firstTime)
            lessThan = false;
         break;
      }
      else if (ch2 == EOF)
      {
         lessThan = false;
         break;
      }
      else if (ch1 == EOF)
      {
         break;
      }

      if (ch1 > ch2)
         lessThan = false;
      else if (ch1 < ch2)
         break;

      firstTime = false;
   }
   ResetMode();
   rhs.ResetMode();
   return lessThan;
}

// FUNCTION: length()
// PURPOSE;  Traverse all REMAINING cards in deck and calculate the overall length
//           of the AsnBuf.
//
unsigned long AsnBuf::length() const
{
   unsigned long bytesRemaining = 0;
   Deck::iterator tmpCard = m_card;
   int currPos, endPos;

   if (! m_deck.empty())
   {
      //AsnBufLoc readLoc = GetReadLoc();

      // ResetMode();    //RWC;

      while (tmpCard != m_deck.end())
      {
         //(*tmpCard)->rdbuf()->sgetc();
         currPos = (*tmpCard)->rdbuf()->pubseekoff(0, std::ios_base::cur, std::ios_base::in);
         if (currPos == -1)
            currPos = 0;
         endPos = (*tmpCard)->rdbuf()->pubseekoff(0, std::ios_base::end, std::ios_base::in);
         (*tmpCard)->rdbuf()->pubseekpos(currPos, std::ios_base::in);
         bytesRemaining += endPos - currPos;
            
         tmpCard++;
      }
   
      //SetReadLoc(readLoc);
   }

   return bytesRemaining;
}

/* JKG -- added 03/03/04 for support of unkown any's in extension additions                   */
/* For unkown any's we need to unget the tag and length bytes to be able to properly decode   */
/* unkown any's in extension additions, but outside the extension addition root list          */
void AsnBuf::UnGetBytes(long lBytesToPutBack) const
{
    FUNC("AsnBuf::UnGetByte()");

    while( lBytesToPutBack )
    {
        if( ((*m_card)->rdbuf()->sungetc()) == EOF )
        {
            if( m_card != m_deck.begin() )
            {    
                m_card--;                
            }
            else
            {
                throw BufferException("Failed putting bytes back", STACK_ENTRY);
            }
        }
        else
        {
            lBytesToPutBack--;    
        }
    }
}


char AsnBuf::GetByte() const
{
   FUNC("AsnBuf::GetByte()");

   std::streambuf::int_type ch;

   if (m_card != m_deck.end()) {
#ifdef _DEBUG
       Card *tmpCard = *m_card;
#endif
       while ((ch = (*m_card)->rdbuf()->sbumpc()) == EOF)
       {
           m_card++;

           if ((m_card == m_deck.end())) {
               throw BufferException("Read past end of data", STACK_ENTRY);
           }
       }
   } else
       throw BufferException("Read past end of data", STACK_ENTRY);

   return (char)ch;
}

bool AsnBuf::GetByteNoEx(char &ret) const
{
    std::streambuf::int_type ch = EOF;

    while (ch == EOF && m_card != m_deck.end())
    {
	ch = (*m_card)->rdbuf()->sbumpc();
	if (ch == EOF)
	{
	    ++m_card;
	}
    }

    ret = static_cast<char>(ch);
    return ch != EOF && m_card != m_deck.end();
}

bool AsnBuf::GetUByteNoEx(unsigned char &ret) const
{
    char val;
    bool tmp;
    tmp = GetByteNoEx(val);
    ret = static_cast<unsigned char>(val);
    return tmp;
}

// FUNCTION: GrabAny()
// PURPOSE : copy the current sequence of bytes (i.e. Tag Length and associated data)
//           into AsnBuf that was passed in.  This is useful for copying the raw encoding
//           of any ANY out of an AsnBuf.
//
void AsnBuf::GrabAny(AsnBuf &anyBuf, AsnLen &bytesDecoded) const
{
    FUNC("AsnBuf::GrabAny");

   AsnLen len;
   AsnLen tmpLen = bytesDecoded;
   AsnBufLoc readLoc = GetReadLoc();
   AsnLen lTmpbytesDecoded=0;
   
   // Decode tag of the ANY.  This will be encoded into AnyBuf after the length
   //
   // @todo: decode the tag using an ASN.1 TagId structure, and check
   // validity
   //
   (void)BDecTag(*this, bytesDecoded);
   
   // Decode length of the ANY.  This will be encoded into anyBuf after the data.
   len = BDecLen(*this, bytesDecoded);

   if (len == INDEFINITE_LEN)
   {
      ConsStringDeck deck(0);
      try {
         deck.Fill(*this, len, lTmpbytesDecoded);
      }
      catch (... /*std::exception &e*/)
      {
        throw BufferException("deck.Fill(...) failed, unexpected exception (STACK?)", STACK_ENTRY);
      }
      len = lTmpbytesDecoded;
   }
	else if (len > this->length())
   {
      throw BufferException("len error from BDecLen call", STACK_ENTRY);
   }
   
   SetReadLoc(readLoc);
   
   // length is greater than the magic size and 
   // the m_card contains a file then store a
   // AsnFileSeg object in output buf.
   //
   tmpLen = (bytesDecoded - tmpLen) + len;

	/** PIERCE: Commented out until further testing is done
   if (tmpLen > _MAGIC_SIZE && card().bufType() == FILE_TYPE)
   {
      anyBuf.PutFileSeg(GetFileSeg(tmpLen));
   }
   else
   {
      AsnRvsBuf *pRvsBuf = new AsnRvsBuf(tmpLen);
      GetSeg(pRvsBuf->m_buf, tmpLen);
      pRvsBuf->m_pStart = pRvsBuf->m_buf;
      anyBuf.m_card =
		  anyBuf.m_deck.insert(anyBuf.m_deck.begin(), new Card(pRvsBuf));
   }
	**/
	AsnRvsBuf *pRvsBuf = new AsnRvsBuf(tmpLen);
   GetSeg(pRvsBuf->m_buf, tmpLen);

   pRvsBuf->m_pStart = pRvsBuf->m_buf;
   anyBuf.m_card =
		  anyBuf.m_deck.insert(anyBuf.m_deck.begin(), new Card(pRvsBuf));
   bytesDecoded += len;

#ifdef _DEBUG
   if (! anyBuf.m_deck.empty())
      Card *tmpCard = *(anyBuf.m_deck.begin());
#endif  // _DEBUG
}

char AsnBuf::PeekByte() const
{
   FUNC("AsnBuf::PeekByte()");

   std::streambuf::int_type ch;
   if (m_card != m_deck.end())
   {
      if ( (ch = (*m_card)->rdbuf()->sgetc()) == EOF )
      {
         m_card++;
         if ( (m_card == m_deck.end()) ||
              (ch = (*m_card)->rdbuf()->sgetc()) == EOF)
            throw BufferException("Read past end of data", STACK_ENTRY);
      }
   }
   else
      throw BufferException("Read past end of data", STACK_ENTRY);

   return (unsigned char) ch;
}


AsnBufLoc AsnBuf::GetReadLoc() const
{
   AsnBufLoc bl;
   bl.m_card = m_card;
   bl.m_offset = (*m_card)->rdbuf()->pubseekoff(0, std::ios_base::cur, std::ios_base::in);
#ifdef _DEBUG
   Card *pCard=*m_card;
#endif
   if (bl.m_offset == -1) 
       bl.m_offset = 0;
   return bl;
}

void AsnBuf::SetReadLoc(const AsnBufLoc &bl) const
{
   FUNC("AsnBuf::setReadLoc()");

   Deck::iterator i = m_deck.begin();
   
   // first make sure interator bl.m_card is between current card
   // start of the deck.
   //
   
   while (i != bl.m_card && i != m_deck.end())
   {
      i++;
   }

   if ( i == bl.m_card )
   {
		ResetMode();
      m_card = bl.m_card;
      (*m_card)->rdbuf()->pubseekpos(bl.m_offset, std::ios_base::in);
   }
   else
   {
      throw BufferException("Invalid AsnBufLoc", STACK_ENTRY);
   }
}

// skip()
//
// skips ahead by the number of "skipBytes"
//
void AsnBuf::skip(size_t skipBytes)
{
   FUNC("AsnBuf::skip()");

   size_t nCardLen;
   while (skipBytes > 0 && m_card != m_deck.end() )
   {
      nCardLen = (*m_card)->length();

      if (skipBytes > nCardLen)
      {
         skipBytes -= nCardLen;
         (*m_card)->rdbuf()->pubseekoff(0, std::ios_base::end, std::ios_base::in);
         m_card++;
      }
      else
      {
         (*m_card)->rdbuf()->pubseekoff(skipBytes, std::ios_base::cur, std::ios_base::in);
         skipBytes = 0;
      }
   }
   if (skipBytes > 0)
   {
      throw BufferException("Skipped past end of buffer", STACK_ENTRY);
   }
}

// insert()
//
// insert cards from b into this AsnBuf
//
// TBD: is it necessary to return the length?
//
long AsnBuf::splice(AsnBuf &b)
{
    if (m_card != m_deck.end() &&
        ((*m_card == NULL) || (*m_card)->length() == 0)) {
        delete *m_card;
        m_card = m_deck.erase(m_card);
    }

    long length = b.length();
    Deck::reverse_iterator ib;
    for (ib = b.m_deck.rbegin(); ib != b.m_deck.rend(); ++ib) {
        m_card = m_deck.insert(m_deck.begin(), *ib);
    }
    b.m_deck.clear();

    return length;
}

void BDEC_2ND_EOC_OCTET(const SNACC::AsnBuf &b, SNACC::AsnLen &bytesDecoded)
{
   FUNC("BDEC_2ND_EOC_OCTET");

   if ((b.GetByte() != 0))
      throw EXCEPT("second octet of EOC not zero", DECODE_ERROR);
   
   bytesDecoded++;
}


// Sort by encoding included tag, length, and data
//
bool asnbuf_greater(const SNACC::AsnBuf &x, const SNACC::AsnBuf &y)
{
    AsnLen len(0);
    AsnTag xTag = (BDecTag(x, len) & 0xDFFFFFFF);
    AsnTag yTag = (BDecTag(y, len) & 0xDFFFFFFF);

    x.ResetMode();
    y.ResetMode();

    return (xTag > yTag);
}

void sortSet(std::list<SNACC::AsnBuf> &bufList)
{
    for (std::list<SNACC::AsnBuf>::iterator j = bufList.begin();
         j != bufList.end(); ++j)
        j->ResetMode();

    bufList.sort(asnbuf_greater);
}

#define ASN_UNIVERSAL   0x00
#define ASN_APPLICATION 0x40
#define ASN_CONTEXT     0x80
#define ASN_PRIVATE     0xC0

bool AsnBuf::operator == (const AsnBuf &b) const
{
   bool equal = true;
   ResetMode();
   b.ResetMode();
   std::streambuf::int_type ch1;
   std::streambuf::int_type ch2;
   unsigned char Ch1;
   unsigned char Ch2;

   while ( equal )
   {
      if (GetUByteNoEx(Ch1))
      {
         ch1 = Ch1;
      }
      else
      {
         ch1 = EOF;
      }

      if (b.GetUByteNoEx(Ch2))
      {
         ch2 = Ch2;
      }
      else
      {
         ch2 = EOF;
      }

      if ((ch1 == EOF) && (ch1 == EOF))
         break;

      if (ch1 != ch2)
         equal = false;
   }
   ResetMode();
   b.ResetMode();
   return equal;
}

void AsnBuf::hexDump(std::ostream &os) const
{
   bool done = false;
   int ch;
   
   ResetMode();

   std::hex(os);

   while (! done)
   {
	 
     try
      {
		 ch = GetUByte();
		 os << "0x";
		 os << ch;
		 os << "   ";
	  }
      catch (...)
      {

		 os.unsetf(std::ios_base::hex);
         os.unsetf(std::ios_base::hex);
         done = true;
      }
 
   }

  
}



std::ostream & operator<<(std::ostream &os, const SNACC::AsnBuf &b)
{ 
    SNACC::Deck::const_iterator card = b.deck().begin();
    while (card != b.deck().end())
    {
		 if ((*card)->length() > 0)
			os << (*card)->rdbuf();
       card++;
    }
    os.flush();
    return os; 
}

long Card::size()
{
  long currPos, endPos;

  currPos = first->pubseekoff(0, std::ios_base::cur, std::ios_base::in);
  
  endPos = first->pubseekoff(0, std::ios_base::end, std::ios_base::in);
 
  if (currPos != -1)
      first->pubseekpos(currPos, std::ios_base::in);
 
  if (endPos == -1)
     endPos = 0;
     
  return endPos;
}

long Card::length()
{
  long currPos, endPos;

  currPos = first->pubseekoff(0, std::ios_base::cur, std::ios_base::in);
  
  if (currPos == -1)
     currPos = 0;
  
  endPos = first->pubseekoff(0, std::ios_base::end, std::ios_base::in);
 
  first->pubseekpos(currPos, std::ios_base::in);
 
  if (endPos == -1)
     endPos = 0;

  return endPos - currPos;
}

#ifdef _DEBUG
void AsnBuf::status(std::ostream &os)
{
   std::cout << "**** AsnBuf Status ****\n";
   std::cout << "Card  Written   Bytes TBR    Type    Max Size\n";
   std::cout << "----------------------------------------------------------------------\n";

   Deck::iterator i;
   int j;
   for (j=0,i = m_deck.begin(); i != m_deck.end(); i++,j++)
   {
      std::cout << j << "\t" << (*i)->size() << "\t" << (*i)->length() << "\t" << (*i)->bufTypeStr() << "\t";
      if ((*i)->bufType() == RVS_BUF_TYPE)
      {
         const AsnRvsBuf *pRvsBuf = (const AsnRvsBuf *)(*i)->rdbuf();
         std::cout << pRvsBuf->max_size() << "\n";
      }
      else
         std::cout << "?\n";
   }
   std::cout << "**** AsnBuf Status ****\n";
}


// AsnBufType { FILE_TYPE=0, RVS_BUF_TYPE, IN_MEM_TYPE, EXT_MEM_TYPE} ;

const char * Card::bufTypeStr()
{
   switch (this->bufType())
   {
      case RVS_BUF_TYPE:
         return "ASN_RVS_BUF";
      case FILE_TYPE:
         return "FILE_TYPE";
      case IN_MEM_TYPE:
         return "IN_MEM_TYPE";
      case EXT_MEM_TYPE:
         return "EXT_MEM_TYPE";
   }
   return NULL;
}
#endif

std::streambuf::int_type
AsnFDBuf::underflow()
{
    if (fd == -1)
        return std::streambuf::traits_type::eof();

    if (gptr() < egptr())
        return std::streambuf::traits_type::to_int_type(*gptr());

    char *base = &buffer.front();
    char *start = base;

    if (eback() == base) {
        // Make arrangements for putback characters
        ::memmove(base, egptr() - putsz, putsz);
        start += putsz;
    }

    int n = 0;
    if (sock)
        n = recv(fd, start, buffer.size() - (start - base), 0);
    else
        n = ::read(fd, start, buffer.size() - (start - base));

    if(n == 0 || n < 0) {
        extra_reset();
        return std::streambuf::traits_type::eof();
    }

    setg(base, start, start+n);
    return std::streambuf::traits_type::to_int_type(*gptr());
}

std::streambuf::int_type
AsnFDBuf::overflow(std::streambuf::int_type c)
{
    if( fd == -1 || c == std::streambuf::traits_type::eof() )
        return std::streambuf::traits_type::eof();

    char a = c;
    int n = 0;
    if (sock)
        n = send(fd, &a, 1, MSG_NOSIGNAL);
    else
        n = ::write(fd, &a, 1);

    if (n != 1) {
        extra_reset();
        return std::streambuf::traits_type::eof();
    }
    return n;
}

std::streamsize
AsnFDBuf::xsputn(const char* s, std::streamsize num)
{
    if( fd == -1 )
        return std::streambuf::traits_type::eof();

    int result;
    if (sock)
        result = send(fd, s, num, MSG_NOSIGNAL);
    else
        result = ::write(fd, s, num);

    if (result < 0) {
        extra_reset();
        return std::streambuf::traits_type::eof();
    }
    return result;
}

void
AsnFDBuf::extra_reset()
{
    if (close_on_fail) {
        if (!sock) {
            close(fd);
        } else {
#ifndef WIN32
            ::close(fd);
#else
            ::closesocket(fd);
#endif
        }
        fd = -1;
    }
}

#ifndef WIN32
#include <sys/ioctl.h>
#else
#include <winsock2.h>
#include <windows.h>
#include <ws2tcpip.h>
#endif

#ifndef WIN32
static int ioctlsocket(int fd, long request, void *data )
{
	return ioctl(fd, request, data);
}
#define CAST void *
#else
#define CAST u_long *
#endif

std::streamsize
AsnFDBuf::showmanyc()
{
    if (sock) {
        fd_set fds;
        struct timeval tv;

        tv.tv_sec = 0;
        tv.tv_usec = 0;

        FD_ZERO(&fds);
#ifdef WIN32 
        FD_SET((unsigned int) fd, &fds);
#else
        FD_SET(fd, &fds);
#endif

        if (select(fd+1, &fds, NULL, NULL, &tv) > 0) {
            return 1; // at least 1 available...
        }

        // black arts... try an ioctl
        size_t numBytes = 0;
        int i = ioctlsocket(fd, FIONREAD, (CAST) &numBytes);
        if (i > 0) {
            return numBytes;
        }
    } else {
#ifndef WIN32
        // we can check for file length
        size_t numBytes = 0;
        int i = ioctl(fd, FIONREAD, &numBytes);
        if (i > 0) {
            return i;
        }
#else
        return 0;
#endif
    }
    return 0;
}
