#include "asn-buf.h"

#include <string.h>

using namespace SNACC;

AsnRvsBuf::AsnRvsBuf(char *preFilled, size_t segSize)
{

	m_segSize = segSize;
	m_buf = preFilled;
	m_bDeleteable = false;
	m_pReadLoc = m_buf;
	m_pStart = m_buf;

}

AsnRvsBuf::AsnRvsBuf(const char *seg, size_t segSize)
{

	m_segSize = segSize;
	m_buf = new char[segSize];
	m_bDeleteable = true;
	memcpy(m_buf, seg, segSize);
	m_pReadLoc = m_buf;
	m_pStart = m_buf;
}

AsnRvsBuf::AsnRvsBuf(const AsnBuf& otherBuf)
{
	otherBuf.ResetMode();
	size_t len = otherBuf.length();
	if (len > 0)
		m_segSize = len;
	else
		m_segSize = _SEG_SIZE;

	m_buf = new char[m_segSize];
	m_bDeleteable = true;
	m_pReadLoc = NULL; 
	m_pStart = (m_buf + m_segSize);

	if (len > 0)
	{
		otherBuf.GetSeg(m_buf, len);
		otherBuf.ResetMode();
		m_pReadLoc = m_buf;
		m_pStart = m_buf;
	}
}

AsnRvsBuf::AsnRvsBuf(long segSize) 
{

	m_segSize = segSize;
	m_buf = new char[segSize];
	m_bDeleteable = true;
	m_pReadLoc = NULL; 
	m_pStart = (m_buf + segSize);
} 

AsnRvsBuf::~AsnRvsBuf()
{
   if (m_bDeleteable)
      delete[] m_buf;
}

std::streambuf::int_type AsnRvsBuf::underflow() 
{ 
   if (eback() == m_pStart)
      return EOF;
   setg(m_pStart, m_pStart, &m_buf[m_segSize]);
   return (int_type) (unsigned char) *m_pStart; 
}

std::streambuf::int_type AsnRvsBuf::overflow(int c)
{
   if (m_pStart > m_buf && c <= 255 && c >= 0)
   {
      *(--m_pStart) = (char)c;
   }
   else
      return EOF;
   return c;
}
   
std::streamsize AsnRvsBuf::xsputn(const char *s, std::streamsize n)
{
   std::streamsize written;
   if (n <= (m_pStart - m_buf))
   {
      m_pStart -= n;
      memcpy(m_pStart, s, n);
      written = n;
   }
   else
   {
      written = m_pStart - m_buf;
      memcpy(m_buf, &s[n - written], written);
      
      m_pStart = m_buf;
   }
   return written;
}

std::streambuf::pos_type AsnRvsBuf::seekoff(std::streambuf::off_type off, std::ios_base::seekdir way,
                                            std::ios_base::openmode which)
{
   char *pNext = NULL;

   if (which & std::ios_base::out)
   {
		m_pStart = &m_buf[m_segSize];
      setg(0,0,0);
   }

   if (which & std::ios_base::in)
   {
      switch (way)
      {
         case std::ios_base::beg:
                pNext = m_pStart + off;
                break;
         case std::ios_base::end:
                pNext = m_buf + m_segSize + off;
                break;
         case std::ios_base::cur:
                if (gptr() == NULL)
                {
                   pNext = m_pStart;
                }
                else
                   pNext = gptr() + off;
                break;
      default:
	break;
      }

      if ((pNext <= &m_buf[m_segSize]) && (pNext >= m_pStart))
      {
         setg(m_pStart, pNext, &m_buf[m_segSize]);
         return (pNext - m_pStart);
      }
      else
         return (-1);
   }
   return (-1);
}

std::streambuf::pos_type AsnRvsBuf::seekpos(std::streambuf::pos_type sp,
                                            std::ios_base::openmode which)
{
#ifdef _MSVC_6
   return (seekoff(sp, std::ios_base::seekdir::beg, which));
#else
   return (seekoff(sp, std::ios_base::beg, which));
#endif
}

