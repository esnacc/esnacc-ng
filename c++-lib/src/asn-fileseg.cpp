#include "asn-buf.h"
#include "snaccexcept.h"

#include <string.h>

using namespace SNACC;

AsnFileSeg::AsnFileSeg(const char *pFilename)
{
   FUNC("AsnFileSeg::AsnFileSeg()");

   m_filename = strdup(pFilename);

   m_fb = new std::filebuf;
   if (m_fb->open(m_filename, std::ios_base::in | std::ios_base::binary) == NULL)
      throw FileException(m_filename, FileException::OPEN, STACK_ENTRY);

   m_offset = 0;
   
   m_segSize = m_fb->pubseekoff(0, std::ios_base::end, std::ios_base::in);
   if (m_segSize == -1)
   {
      throw FileException(m_filename, FileException::READ, STACK_ENTRY);
   }
   
   m_fb->pubseekpos(0, std::ios_base::in);
}

AsnFileSeg::AsnFileSeg(AsnFileSeg *afs, unsigned long segLen)
{
   FUNC("AsnFileSeg::CopyConstructor()");

   m_offset = afs->pubseekoff(0, std::ios_base::cur, std::ios_base::in);
   m_segSize = segLen;

   m_filename = strdup(afs->m_filename);
   m_fb = new std::filebuf;
   if (m_fb->open(m_filename, std::ios_base::in | std::ios_base::binary) == NULL)
      throw FileException(m_filename, FileException::OPEN, STACK_ENTRY);
} 

// Copy constructor
// 
// assign offset and segSize from o and copy filename, and construct new 
// std::filebuf from that filename
//
AsnFileSeg::AsnFileSeg(const AsnFileSeg &o)
{
   FUNC("AsnFileSeg::CopyConstructor()");

   m_offset = o.m_offset;
   m_segSize = o.m_segSize;

   m_filename = strdup(o.m_filename);

   m_fb = new std::filebuf;
   if (m_fb->open(m_filename, std::ios_base::in | std::ios_base::binary) == NULL)
      throw FileException(m_filename, FileException::OPEN, STACK_ENTRY);
}

// Destructor
//
AsnFileSeg::~AsnFileSeg()
{
   free(m_filename);
   delete m_fb;
}

std::streambuf::int_type AsnFileSeg::underflow()
{
   // check current position to determine if we've read segSize yet
   //
   long currPos = m_fb->pubseekoff(0, std::ios_base::cur);
   if ( (currPos - m_offset) < (long) m_segSize)
   {
      return m_fb->sgetc();
   }
   else
      return EOF;
}

std::streambuf::int_type AsnFileSeg::uflow()
{
   // check current position to determine if we've read segSize yet
   //
   long currPos = m_fb->pubseekoff(0, std::ios_base::cur);
   if ( (currPos - m_offset) < (long) m_segSize)
   {
      return m_fb->sbumpc();
   }
   else
      return EOF;
}

  
std::streamsize AsnFileSeg::xsgetn(char *s, std::streamsize n)
{
   long currPos = m_fb->pubseekoff(0, std::ios_base::cur);

   // if the file segment contains the number of bytes requested
   // then grab it.
   if (n <= (m_segSize - (currPos - m_offset)) )
   {
      return m_fb->sgetn(s, n);
   }
   else
   {
      return EOF;
   }
}

std::streambuf::pos_type AsnFileSeg::seekoff(off_type off, std::ios_base::seekdir way,
                                             std::ios_base::openmode which)
{
   return (m_fb->pubseekoff((m_offset + off), way, which));
}

std::streambuf::pos_type AsnFileSeg::seekpos(std::streambuf::pos_type sp,
                                             std::ios_base::openmode which)
{
   return (seekoff(sp, std::ios_base::beg, which));
}
