#include "asn-incl.h"
#include <math.h>
#include <stdlib.h>

using namespace SNACC;

#define _MAGIC_SIZE 99999999

//RWC;This class was created to allow a reset of the protected inherited elements
//RWC;  for a non-destructive read of the contents.  It is used in several 
//RWC;  places in this source file.
class VDAstreambuf: public std::streambuf
{
public:
    //char* pptr() const (return std::streambuf::pptr());
    char* Vgptr() const { return (this->gptr()); }
    char* Vegptr() const { return this->egptr(); }
    void Vsetg( char* peb, char* pg, char* peg )
    { this->setg(peb, pg, peg); }
    char* Veback() const { return this->eback(); }
    //std::streambuf & operator = ( const std::streambuf &A) 
    //{ (std::streambuf &) *this = A; return *this; }
};

AsnBufBits::AsnBufBits(const AsnBufBits& buf)
{
	m_isInternalBuf = false;
	m_pbuf = NULL;
    operator=(buf);	
}


AsnBufBits& AsnBufBits::operator=(const AsnBufBits& buf)
{
	// Destroy the existing buffer
	if (m_isInternalBuf && (m_pbuf != NULL))
		delete m_pbuf;

    if (buf.m_isInternalBuf)
    {
       m_pbuf = new std::stringbuf;


	   // Save current read position
	   std::streambuf::pos_type origPos = buf.m_pbuf->pubseekoff(0,
		   std::ios_base::cur, std::ios_base::in);

	   // Copy each byte from buf to this new streambuf
       std::streambuf::int_type ch;
       while((ch = buf.m_pbuf->sbumpc()) != EOF)
       {
            m_pbuf->sputc((char)ch);
       }

	   // Reset read position to original value
	   buf.m_pbuf->pubseekpos(origPos, std::ios_base::in);

       //m_pbuf = new std::stringbuf(buf.pbuf_str(), buf.pbuf_length());
//       VDAstreambuf *pVDAbufref = (VDAstreambuf *)buf.m_pbuf;
//     char* pg=pVDAbufref->Vgptr();
//       char* peg=pVDAbufref->Vegptr();
//       char* peb=pVDAbufref->Veback();
       
       

//       pVDAbufref->Vsetg( peb, pg, peg );
      
    }
    else
    {
        m_pbuf = buf.m_pbuf;
    }

     m_isInternalBuf     = buf.m_isInternalBuf;
     m_ucWriteCharBuf[0] = buf.m_ucWriteCharBuf[0];
	 m_iWriteBitPos      = buf.m_iWriteBitPos;
	 m_ucReadCharBuf[0]  = buf.m_ucReadCharBuf[0];
	 m_iReadBitPos       = buf.m_iReadBitPos;
	 m_ulNumBits         = buf.m_ulNumBits;
	 m_ulBitsLeft        = buf.m_ulBitsLeft;
	 bAlign				 = buf.bAlign;

    return *this;
}

/*
AsnBufBits& AsnBufBits::operator=(const AsnBufBits& buf)
{
	if (this != &buf)
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

//
//
void AsnBufBits::hexDump(std::ostream &os)
{
	FUNC("AsnBufBits::hexDump()");

	// Check that the buffer is present
	if (m_pbuf == NULL)
		throw BufferException("NULL internal m_pbuf pointer", STACK_ENTRY);

	;

/*   bool done = false;
   int ch;
   unsigned char* c = NULL;
   if (m_pbuf == NULL)
        m_pbuf = new std::stringbuf;  // DELETED in this class.
   AsnBufBits tmpBuf(*this);
   VDAstreambuf *pVDAbufref = (VDAstreambuf *)tmpBuf.m_pbuf;
   char* pg=pVDAbufref->Vgptr();
   char* peg=pVDAbufref->Vegptr();
   char* peb=pVDAbufref->Veback();
*/
    AsnBufBits tmpbuf = *this;
    int ch;
	std::hex(os);
	os << "0x";

	bool done = false;
	unsigned int i = 1;
	while (!done)
	{
        ch = tmpbuf.m_pbuf->sbumpc();
		//std::streambuf::int_type ch = tmpbuf.m_pbuf->sbumpc();
		if (ch == EOF)
			done = true;
		else
			os << ch;
		
		if ((i % 8) == 0)
			os << "  ";
		else if ((i % 16) == 0)
			os << std::endl;

        i++;
	}

    if(tmpbuf.m_iWriteBitPos < 8)
    {
        ch = tmpbuf.m_ucWriteCharBuf[0];
        os << ch;
    }

	os << std::endl;
	os.unsetf(std::ios_base::hex);

	//m_pbuf->pubseekpos(0 /* REN -- 12/30/03, ios_base::in */);
 
/*
	while (! done)
	{
		try
		{
		 ch = tmpBuf.ReadByte();
		 os << "0x";
		 os << ch;
		 os << "   ";
	  }
      catch (...)
      {		
         os.unsetf(std::ios_base::hex);
         done = true;
      }
 
   }

   pVDAbufref->Vsetg( peb, pg, peg );   //RE-SET referenced shared stream pointer.

   tmpBuf.m_pbuf = NULL; //Be sure not to delete original buffer on a delete
*/
}	// END AsnBufBits::hexDump(...)


//
//
void AsnBufBits::AppendTo(AsnBufBits &bufBitsOut)
{
	FUNC("AsnBufBits::AppendTo()");

	unsigned char ch, *p_ch;
	AsnBufBits tmpBuf(*this);
	VDAstreambuf *pVDAbufref = (VDAstreambuf *)this->m_pbuf;
	char* pg=pVDAbufref->Vgptr();
	char* peg=pVDAbufref->Vegptr();
	char* peb=pVDAbufref->Veback();
	int iThisCount=this->length();
	int iLeft;
	
	// Check that buffer is valid
	if (m_pbuf == NULL)
		throw BufferException("NULL internal m_pbuf pointer", STACK_ENTRY);
	
	try
	{
		for (int ii=0; ii < iThisCount/8; ii++)
		{
			ch = ReadByte();
			bufBitsOut.PutBits(&ch, 8);
		}
		iLeft = iThisCount - 8*(iThisCount/8);
		if (iLeft)
		{
			p_ch = this->GetBits(iLeft);
			if (p_ch)
			{
				bufBitsOut.PutBits(&ch, iLeft);
				free(p_ch);
			}  // END IF p_ch
		}	 // END IF iLeft
	}		 // END try
	catch (...)
	{		
        // @todo: really handle this properly.
	}
	
	
	pVDAbufref->Vsetg( peb, pg, peg );	 //RE-SET referenced shared stream pointer.

}		// END Append(...)


/*****RWC;PAINFUL DANG THING...
AsnBufBits::operator AsnBuf *()
{
   AsnBuf *pResultBuf=NULL;
   bool done = false;
   unsigned char* c = NULL;
   AsnBufBits tmpBuf(*this);
   VDAstreambuf *pVDAbufref = (VDAstreambuf *)&tmpBuf.m_buf;
   char* pg=pVDAbufref->Vgptr();
   char* peg=pVDAbufref->Vegptr();
   char* peb=pVDAbufref->Veback();
   int iBufLength, iBufLength2;
   char *pch;

   FUNC("AsnBufBits::operator AsnBuf *()");
   try
   {
       //tmpBuf.
       //int sgetn( char* pch, int nCount );
       iBufLength = pVDAbufref->Vblen();//in_avail();
       if (iBufLength)
       {
           pch = (char *)calloc(1, iBufLength);
           iBufLength2 = pVDAbufref->sgetn(pch, iBufLength);
           if (iBufLength2 != iBufLength)
                throw BufferException("NOT ENOUGH chars from buffer", STACK_ENTRY);
           pResultBuf = new AsnBuf(pch, iBufLength);
           free(pch);
       }        // END IF iBufLength
       //RWC;NOT DEFINED YET;pResultBuf->PutStream(pVDAbufref);
   }
   catch (...)
   {
       if (pResultBuf)
          delete pResultBuf;
       pResultBuf = NULL;
       done = true;
   }
 

   pVDAbufref->Vsetg( peb, pg, peg );   //RE-SET referenced shared stream pointer.

   return pResultBuf;

}       // END AsnBufBits::operator *AsnBuf()
***/

//
//
int AsnBufBits::OctetAlignWrite()
{
	FUNC("AsnBufBits::OctetAlignWrite()");
	int returnVal = 0;
    if(bAlign)
    {
        if(m_iWriteBitPos < 8)
	    {
		    if( m_pbuf->sputc( m_ucWriteCharBuf[0] ) == EOF )
			    throw BufferException("Ran out of room in the designated buffer", STACK_ENTRY);
		    
            returnVal = m_iWriteBitPos;
		    m_ulNumBits += m_iWriteBitPos;
		    m_ulBitsLeft += m_iWriteBitPos;
		    m_ucWriteCharBuf[0] = 0x00;
		    m_iWriteBitPos = 8;
        }
    }    
    return returnVal;
}

int AsnBufBits::OctetAlignRead()
{
    int returnVal = 0;
 
    if(bAlign)
    {
        returnVal = (8 - m_iReadBitPos);
	    m_ulBitsLeft -= returnVal;
	    m_iReadBitPos = 8;
	    m_ucReadCharBuf[0] = 0x00;
    }
    return returnVal;
}


unsigned char AsnBufBits::ReadByte()
{
	FUNC("AsnBufBits::ReadByte()");

	// Check that buffer is valid
	if (m_pbuf == NULL)
		throw BufferException("NULL internal m_pbuf pointer", STACK_ENTRY);
	
	std::streambuf::int_type ch = m_pbuf->sbumpc();
	if (ch == EOF)
	{
		if (m_iWriteBitPos < 8)
		{
			ch = m_ucWriteCharBuf[0];
			m_iWriteBitPos = 8;
			m_ucWriteCharBuf[0] = 0x00;
		}
		else if (m_ulBitsLeft > 0)
		{
			ch = m_ucReadCharBuf[0];
			m_ulBitsLeft = 0;
		}
		else 
			throw BufferException("Read Past End of Buffer", STACK_ENTRY);
	}
	return (unsigned char)ch;
}



unsigned char* AsnBufBits::GetBits(unsigned long numBits)
{
	FUNC("AsnBufBits::GetBits()");
	
	unsigned char* seg = NULL;
	unsigned long count = 0;
	unsigned long ulNumBytes = (numBits / 8);
	int 		  iExtraBits = (numBits % 8);
	unsigned char ucTempChar = 0x00;
	
	// Check that buffer is valid and contains enough bits
	if (m_pbuf == NULL)
		throw BufferException("NULL internal m_pbuf pointer", STACK_ENTRY);
	if (m_ulBitsLeft < numBits)
	{
		throw BufferException("Trying to retrieve more bits than in the buffer",
			STACK_ENTRY);
	}
	
	if(iExtraBits)
	{
		seg = new unsigned char[ulNumBytes + 2];
	}
	else
	{
		seg = new unsigned char[ulNumBytes + 1];
	}
	
	while(count < ulNumBytes)
	{
		if( m_iReadBitPos == 8)
		{
			seg[count] = ReadByte();
		}
		else
		{
			m_ucReadCharBuf[1] = ReadByte();
			ucTempChar = m_ucReadCharBuf[1];
			m_ucReadCharBuf[1] = MaskBits(m_ucReadCharBuf[1], (m_iReadBitPos));
			m_ucReadCharBuf[1] >>= (8 - m_iReadBitPos);
			seg[count] = m_ucReadCharBuf[0] |= m_ucReadCharBuf[1];
			
			m_ucReadCharBuf[0] = (unsigned char)(ucTempChar << (m_iReadBitPos) );
		}
		
		count++;
		
	}
	
	if(iExtraBits > 0 )
	{
		if(iExtraBits <= (8 - m_iReadBitPos))
		{
			seg[count] = MaskBits(m_ucReadCharBuf[0], iExtraBits);
			m_ucReadCharBuf[0] <<= iExtraBits;
			m_iReadBitPos += iExtraBits;
		}
		else
		{
			m_ucReadCharBuf[1] = ReadByte();
			ucTempChar = m_ucReadCharBuf[1];
			
			ucTempChar = MaskBits(ucTempChar, (iExtraBits - (8 - m_iReadBitPos)) );
			
			if(m_iReadBitPos != 8)
			{
				ucTempChar >>= (8 - m_iReadBitPos);
			}
			
			seg[count] = m_ucReadCharBuf[0] |= ucTempChar;
			
			m_ucReadCharBuf[0] = (unsigned char)(m_ucReadCharBuf[1] << (iExtraBits - (8 - m_iReadBitPos)) );
			m_iReadBitPos = (iExtraBits - (8 - m_iReadBitPos));
			
		}
		count++;
	}
	
	seg[count] = 0x00;
	m_ulBitsLeft -= numBits;
	
	return seg;
}


bool AsnBufBits::GetBit()
{
 	FUNC("AsnBufBits::GetBit()");

	// Check that the buffer is valid and contains at least one bit
	if (m_pbuf == NULL)
		throw BufferException("NULL internal m_pbuf pointer", STACK_ENTRY);
	if (m_ulBitsLeft == 0)
		throw BufferException("No more bits in the buffer", STACK_ENTRY);
	
	// Read another byte from the stream if necessary
	if (m_iReadBitPos == 8)
	{
		m_ucReadCharBuf[0] = ReadByte();
		m_iReadBitPos = 0;
	}

	// Calculate the return value
	bool bitRead = (m_ucReadCharBuf[0] & 0x80);

	// Update the read position and read buffer and decrement the bits left
	m_iReadBitPos++;
	m_ucReadCharBuf[0] <<= 1;
	--m_ulBitsLeft;

	// Return the bit read
	return bitRead;
}


unsigned char AsnBufBits::GetByte()
{
 	FUNC("AsnBufBits::GetByte()");

	// Check that the buffer is valid and contains at least eight bits
	if (m_pbuf == NULL)
		throw BufferException("NULL internal m_pbuf pointer", STACK_ENTRY);
	if (m_ulBitsLeft < 8)
	{
		throw BufferException("Trying to retrieve more bits than in the buffer",
			STACK_ENTRY);
	}
	
	// If the read buffer is empty, just read the next byte in the stream
	unsigned char byte;
	if (m_iReadBitPos == 8)
		byte = ReadByte();
	else
	{
		// Set the resulting byte to the remaining bits in the read buffer
		byte = m_ucReadCharBuf[0];
		
		// Read the next byte from the buffer
		m_ucReadCharBuf[0] = ReadByte();
		
		// Mask out the unneeded bits from the next byte
		unsigned char nextBits = m_ucReadCharBuf[0];
		nextBits >>= 8 - m_iReadBitPos;

		// Combine the byte with the bits from the next byte in the stream
		byte |= nextBits;

		// Update the read buffer
		m_ucReadCharBuf[0] <<= m_iReadBitPos;
	}

	// Decrement the number of bits left
	m_ulBitsLeft -= 8;

	// Return the byte
	return byte;
}


unsigned long AsnBufBits::GetBits(AsnBits& bits, unsigned long numBits)
{
	FUNC("AsnBufBits::GetBits()");

	// Check that the buffer contains the required number of bits
	if (m_ulBitsLeft < numBits)
	{
		throw BufferException("Trying to retrieve more bits than in the buffer",
			STACK_ENTRY);
	}

	// Set the size of the bitstring
	bits.Set(numBits);

	// Read each bit from the buffer
	for (unsigned long i = 0; i < numBits; ++i)
	{
		if (GetBit())
			bits.SetBit(i);
	}

	return numBits;
} // end of AsnBufBits::GetBits()


unsigned long AsnBufBits::PutBits(const unsigned char* seg,
								  unsigned long numBits)
{
	FUNC("AsnBufBits::PutBits()");
	
	unsigned long totalBitsWrote = numBits;
	unsigned long ulBitsWrote = 0;
	unsigned long ulNumBytes  = 0;
	int 		  iExtraBits  = 0;
	unsigned long count = 0;
	
	// Check that buffer is valid
	if (m_pbuf == NULL)
		throw BufferException("NULL internal m_pbuf pointer", STACK_ENTRY);
	
	iExtraBits = numBits % 8;
	ulNumBytes = numBits / 8;
	
	while( count < ulNumBytes )
	{
		m_ucWriteCharBuf[1] = seg[count];
		m_ucWriteCharBuf[1] = MaskBits(m_ucWriteCharBuf[1], m_iWriteBitPos);
		m_ucWriteCharBuf[1] >>= ( 8 - m_iWriteBitPos );
		
		m_ucWriteCharBuf[0] |= m_ucWriteCharBuf[1];
		
		if( m_pbuf->sputc( m_ucWriteCharBuf[0] ) == EOF )
			throw BufferException("Ran out of room in the designated buffer", STACK_ENTRY);
		
		m_ucWriteCharBuf[0] = seg[count];
		m_ucWriteCharBuf[0] <<= m_iWriteBitPos;
		
		m_ulBitsLeft += 8;
		m_ulNumBits += 8;
		ulBitsWrote += 8;
		count++;
	}
	
	if( iExtraBits )
	{
		m_ulBitsLeft += iExtraBits;
		m_ulNumBits += iExtraBits;
		m_ucWriteCharBuf[1] = seg[count];
		m_ucWriteCharBuf[1] >>= (8 - m_iWriteBitPos);
		
		m_ucWriteCharBuf[0] |= m_ucWriteCharBuf[1];
		
		if(iExtraBits == m_iWriteBitPos)
		{
			if( m_pbuf->sputc( m_ucWriteCharBuf[0] ) == EOF )
				throw BufferException("Ran out of room in the designated buffer", STACK_ENTRY);
			
			m_iWriteBitPos = 8;
			m_ucWriteCharBuf[0] = 0x00;
		}
		else if(iExtraBits < m_iWriteBitPos)
		{
			m_iWriteBitPos -= iExtraBits;
			m_ucWriteCharBuf[0] = MaskBits(m_ucWriteCharBuf[0], 8 - m_iWriteBitPos); 
		}
		else if(iExtraBits > m_iWriteBitPos)
		{
			if( m_pbuf->sputc( m_ucWriteCharBuf[0] ) == EOF )
				throw BufferException("Ran out of room in the designated buffer", STACK_ENTRY);
			
			m_ucWriteCharBuf[0] = seg[count];
			m_ucWriteCharBuf[0] >>= ( 8 - iExtraBits );
			
			iExtraBits -= m_iWriteBitPos;
			m_iWriteBitPos = ( 8 - iExtraBits );
			
			m_ucWriteCharBuf[0] <<= m_iWriteBitPos;
		}
		
	}
	
	return totalBitsWrote;
}

unsigned char  AsnBufBits::MaskBits(unsigned char cCharToMask, int iBitsToMask)
{
	unsigned char cReturnChar = 0x00;

	cReturnChar = cCharToMask;

	if(iBitsToMask < 8 && iBitsToMask > 0)
	{
		cReturnChar >>= 8 - iBitsToMask;
		cReturnChar <<= 8 - iBitsToMask;
	}

	return cReturnChar;
}



//RWC;TBD; FIX THIS TO WORK PROPERLY.....
bool AsnBufBits::operator<(AsnBufBits &rhs)
{
	FUNC("AsnBufBits::operator<()");

	bool lessThan = true;
	bool firstTime = true;
	//ResetMode();
	//rhs.ResetMode();
	
	// Check that both buffers are valid
	if ((m_pbuf == NULL) || (rhs.m_pbuf == NULL))
		throw BufferException("NULL internal m_pbuf pointer", STACK_ENTRY);
	
	AsnBufBits tmpBuf1(*this);
	VDAstreambuf *pVDAbufref1 = (VDAstreambuf *)tmpBuf1.m_pbuf;
	char* pg1=pVDAbufref1->Vgptr();
	char* peg1=pVDAbufref1->Vegptr();
	char* peb1=pVDAbufref1->Veback();
	
	AsnBufBits tmpBuf2(rhs);
	VDAstreambuf *pVDAbufref2 = (VDAstreambuf *)rhs.m_pbuf;
	char* pg2=pVDAbufref2->Vgptr();
	char* peg2=pVDAbufref2->Vegptr();
	char* peb2=pVDAbufref2->Veback();
	
	std::streambuf::int_type ch1;
	std::streambuf::int_type ch2;
	
	while ( lessThan )
	{
		try
		{
			ch1 = tmpBuf1.ReadByte();
		}
		catch (BufferException &)
		{
			ch1 = EOF;
		}
		
		try
		{
			ch2 = tmpBuf2.ReadByte();
		}
		catch (BufferException &)
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
	//ResetMode();
	//rhs.ResetMode();
	pVDAbufref1->Vsetg( peb1, pg1, peg1);
	pVDAbufref2->Vsetg( peb2, pg2, peg2 );	 //RE-SET referenced shared stream pointer.
	
	return lessThan;
}		// END AsnBufBits::operator<(AsnBufBits &rhs)

