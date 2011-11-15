#include "asn-incl.h"

#ifdef WIN32
#pragma warning(disable: 4100 4710 4251 4018)
#pragma warning(push,3)
#include <vector>
#pragma warning(pop)
#else
#include <vector>
#endif


_BEGIN_SNACC_NAMESPACE


AsnLen PERGeneral::EncodeGeneral(AsnBufBits &b)const
{
	AsnLen len = 0;
	unsigned long l_64kFrag = l_16k * 4;
	unsigned long count = 0;
	unsigned long x = 0;
	unsigned long y = 0;
	unsigned long tempLen = lEncLen();
	unsigned char ch = 0x00;
	unsigned char *c = NULL;
	long offset = 0;
	
	

	if(tempLen >= l_16k)
	{	
		/*there is more than 16k bytes of data*/
		count = (tempLen / l_64kFrag);
		
		for(x=0; x < count; x++)
		{  
			len += b.OctetAlignWrite();
			
			len += PEncLen_16kFragment(b, 4);
			
			len += b.OctetAlignWrite();

			for(y = 0; y < l_64kFrag; y++)
			{
				len += Interpret(b, offset);
				offset++;
			}
			
		}

		tempLen -= count * l_64kFrag;

		count = tempLen / l_16k;

		if(count != 0)
		{  
			len += b.OctetAlignWrite();
			
			len += PEncLen_16kFragment(b, count);
		
			len += b.OctetAlignWrite();

			for(y = 0; y < (l_16k * count); y++)
			{
				len += Interpret(b, offset);
				offset++;
			}
		
		}
		
		tempLen -=  (l_16k * count);

		if(tempLen == 0)
		{
			ch = 0x00;
			c = &ch;
			
			len += b.OctetAlignWrite();

			len += b.PutBits(c, 8);
			
			return len;
		}

	}

	/*if there are less than 128 bytes of data*/
	if(tempLen < 128)
	{
		len += b.OctetAlignWrite();

		len += PEncDefLenTo127(b, tempLen);
		
		len += b.OctetAlignWrite();

		for(y = 0; y < tempLen; y++)
		{
			len += Interpret(b, offset);
			offset++;
		}
		
	}
	else if(tempLen >= 128 && tempLen < l_16k)
	{
		len += b.OctetAlignWrite();
		/*if there is less than 16k bytes of data*/
		/*and more than 127 bytes of data*/
		len += PEncLen_1to16k(b, tempLen);
		
		len += b.OctetAlignWrite();

	
		for(y = 0; y < tempLen; y++)
		{
			len += Interpret(b, offset);
			offset++;
		}
		
	}

	return len;
}


void PERGeneral::DecodeGeneral(AsnBufBits &b, AsnLen &bitsDecoded)
{
	unsigned char* seg;
	unsigned long templen = 0;
	long offset = 0;
	
	Clear();

	bitsDecoded += b.OctetAlignRead();
	
	seg = (unsigned char*)b.GetBits(8);
    bitsDecoded += 8;
	
	while((seg[0] & 0xC0) == 0xC0)
	{
		seg[0] &= 0x3F;
		templen = (unsigned long)seg[0];
		templen *= l_16k;
		Allocate(templen);

		bitsDecoded += b.OctetAlignRead();

		
		while(templen)
		{
			Deterpret(b, bitsDecoded, offset);
			offset++;
			templen--;
		}
		

		bitsDecoded += b.OctetAlignRead();

        free(seg);
		seg = (unsigned char*)b.GetBits(8);
		bitsDecoded += 8;
	}
	
	
	if((seg[0] & 0xC0) == 0x80)
	{
		seg[0] &= 0x3F;
		templen = (unsigned long)seg[0];
		templen <<= 8;
        free(seg);
		seg = (unsigned char*)b.GetBits(8);
        bitsDecoded += 8;
		templen |= (unsigned long)seg[0];
		Allocate(templen);

		bitsDecoded += b.OctetAlignRead();

	
		while(templen)
		{
			Deterpret(b, bitsDecoded, offset);
			offset++;
			templen--;
		}
		
	}
	else if((seg[0] & 0x80) == 0x00)
	{
		seg[0] &= 0x7F;
		templen = (unsigned long)seg[0];
		Allocate(templen);

		bitsDecoded += b.OctetAlignRead();

	
		while(templen)
		{
			Deterpret(b, bitsDecoded, offset);
			offset++;
			templen--;
		}
					
	}

    free(seg);
}


_END_SNACC_NAMESPACE

