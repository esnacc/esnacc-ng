
/* Include Files */
#ifndef WIN32
#include <wchar.h>
#endif
#include <string.h>
#include "asn-incl.h"

/* Type Definitions */
#ifndef bool
	typedef char bool;	
#endif

#ifndef true
	#define true    1
#endif
#ifndef false
	#define false   0
#endif

typedef struct
{
	unsigned char mask;
	unsigned char value;
	unsigned long maxCharValue;
} MaskValue;

/* Global Values */
#define MAX_UTF8_OCTS_PER_CHAR		6

const MaskValue gUTF8Masks[MAX_UTF8_OCTS_PER_CHAR] = {
	{ 0x80, 0x00, 0x0000007F },		/* one-byte encoding */
	{ 0xE0, 0xC0, 0x000007FF },		/* two-byte encoding */
	{ 0xF0, 0xE0, 0x0000FFFF },		/* three-byte encoding */
	{ 0xF8, 0xF0, 0x0001FFFF },		/* four-byte encoding */
	{ 0xFC, 0xF8, 0x03FFFFFF },		/* five-byte encoding */
	{ 0xFE, 0xFC, 0x07FFFFFF }		/* six-byte encoding */
};

/* Constants */
#define FOUR_BYTE_ENCODING    0xf0
#define THREE_BYTE_ENCODING   0xe0 
#define TWO_BYTE_ENCODING     0xc0


/* Function Prototypes */
static bool IsValidUTF8String(UTF8String* octs);


AsnLen BEncUTF8StringContent(GenBuf *b, UTF8String *octs)
{
	if (IsValidUTF8String(octs) == false)
	{
		Asn1Error ("BEncUTF8StringContent: ERROR - Invalid UTF-8 Encoding");
		GenBufSetWriteError (b, TRUE);
	}
	return (BEncAsnOctsContent (b, octs));
} /* end of BEncUTF8StringContent() */


AsnLen BEncUTF8String(GenBuf *b, UTF8String *v)
{
    AsnLen l;

	l = BEncUTF8StringContent (b, v);
	l += BEncDefLen (b, l);
	l += BEncTag1 (b, UNIV, PRIM, UTF8STRING_TAG_CODE);

	return l;
} /* end of BEncUTF8String() */


void BDecUTF8StringContent(GenBuf *b, AsnTag tagId, AsnLen len,
						   UTF8String *result, AsnLen *bytesDecoded,
						   ENV_TYPE env)
{
	BDecAsnOctsContent (b, tagId, len, result, bytesDecoded, env);
	if (IsValidUTF8String(result) == false)
	{
		Asn1Error ("BDecUTF8StringContent: ERROR - Invalid UTF-8 Encoding");
        longjmp (env, -40);
	}
} /* end of BDecUTF8StringContent() */


void BDecUTF8String(GenBuf *b, UTF8String *result, AsnLen *bytesDecoded,
					ENV_TYPE env)
{
	AsnTag tag;
	AsnLen elmtLen1;

	if (((tag = BDecTag (b, bytesDecoded, env)) != 
		MAKE_TAG_ID (UNIV, PRIM, UTF8STRING_TAG_CODE)) &&
		(tag != MAKE_TAG_ID (UNIV, CONS, UTF8STRING_TAG_CODE)))
	{
		Asn1Error ("BDecUTF8String: ERROR - wrong tag\n");
		longjmp (env, -113);
	}

	elmtLen1 = BDecLen (b, bytesDecoded, env);
	BDecUTF8StringContent (b, tag, elmtLen1, result, bytesDecoded, env);
}


static bool IsValidUTF8String(UTF8String* octs)
{
	unsigned long i;
	unsigned int j;

	if (octs == NULL)
		return false;

	i = 0;
	while (i < octs->octetLen)
	{
		/* Determine the number of UTF-8 octets that follow the first */
		for (j = 0; (j < MAX_UTF8_OCTS_PER_CHAR) && 
			((gUTF8Masks[j].mask & octs->octs[i]) != gUTF8Masks[j].value); j++)
			;

		/* Return false if the first octet was invalid or if the number of 
		subsequent octets exceeds the UTF8String length */
		if ((j == MAX_UTF8_OCTS_PER_CHAR) || ((i + j) >= octs->octetLen))
			return false;

		/* Skip over first octet */
		i++;

		/* Check that each subsequent octet is properly formatted */
		for (; j > 0; j--)
		{
			if ((octs->octs[i++] & 0xC0) != 0x80)
				return false;
		}
	}

	return true;
}


int CvtUTF8String2wchar(UTF8String *inOcts, wchar_t **outStr)
{
	if ((inOcts == NULL) || (outStr == NULL))
		return -1;

	if (inOcts->octetLen == 0)
	{
		*outStr = (wchar_t*)calloc(1, sizeof(wchar_t));
		if (*outStr == NULL)
			return -2;
		return 0;
	}
	else
		return CvtUTF8towchar(inOcts->octs, outStr);
}


int CvtUTF8towchar(char *utf8Str, wchar_t **outStr)
{
	unsigned int len, i, j, x;
	size_t wchar_size = sizeof(wchar_t);

	if ((utf8Str == NULL) || (outStr == NULL))
		return -1;

	len = strlen(utf8Str);
	
	/* Allocate and clear the memory for a worst case result wchar_t string */
	*outStr = (wchar_t*)calloc(len + 1, sizeof(wchar_t));
	if (*outStr == NULL)
		return -2;

	/* Convert the UTF-8 string to a wchar_t string */
	i = 0;
	x = 0;
	while (i < len)
	{
		/* Determine the number of UTF-8 octets that follow the first */
		for (j = 0; (j < MAX_UTF8_OCTS_PER_CHAR) && 
			((gUTF8Masks[j].mask & utf8Str[i]) != gUTF8Masks[j].value); j++)
			;

		/* Return an error if the first octet was invalid or if the number of
		subsequent octets exceeds the UTF-8 string length */
		if ((j == MAX_UTF8_OCTS_PER_CHAR) || ((i + j) >= len))
		{
			free(*outStr);
			*outStr = NULL;
			return -3;
		}

		/* Return an error if the size of the wchar_t doesn't support the
		size of this UTF-8 character */
		if ((j > 2) && (wchar_size < 4))
		{
			free(*outStr);
			*outStr = NULL;
			return -4;
		}

		/* Copy the bits from the first octet into the wide character */
		(*outStr)[x] = (char)(~gUTF8Masks[j].mask & utf8Str[i++]);

		/* Add in the bits from each subsequent octet */
		for (; j > 0; j--)
		{
			/* Return an error if a subsequent octet isn't properly formatted */
			if ((utf8Str[i] & 0xC0) != 0x80)
			{
				free(*outStr);
				*outStr = NULL;
				return -3;
			}

			(*outStr)[x] <<= 6;
			(*outStr)[x] |= utf8Str[i++] & 0x3F;
		}
		x++;
	}

	/* Reallocate the wchar string memory to its correct size */
	if (x < len)
	{
		*outStr = (wchar_t*)realloc(*outStr, (x + 1) * sizeof(wchar_t));
		if (*outStr == NULL)
			return -2;
	}

	return 0;
}


int CvtWchar2UTF8(wchar_t *inStr, char **utf8Str)
{
	size_t wLen;
	unsigned int i, j, x, y;
	size_t wchar_size = sizeof(wchar_t);
	wchar_t temp_wchar;

	/* Check parameters */
	if ((inStr == NULL) || (utf8Str == NULL))
		return -1;

	wLen = wcslen(inStr);

	/* Allocate and clear memory for a worst case UTF-8 string */
	*utf8Str = (char*)calloc(wLen * (wchar_size / 2 * 3) + 1, sizeof(char));
	if (*utf8Str == NULL)
		return -2;

	/* Convert each wide character into a UTF-8 char sequence */
	for (i = 0, x = 0; i < wLen; i++)
	{
		temp_wchar = inStr[i];

		/* Return an error if the wide character is invalid */
		if (temp_wchar < 0)
		{
			free(*utf8Str);
			*utf8Str = NULL;
			return -3;
		}

		/* Determine the number of characters required to encode this wide
		character */
		for (j = 0; (j < MAX_UTF8_OCTS_PER_CHAR) &&
			(temp_wchar > gUTF8Masks[j].maxCharValue); j++)
			;

		/* Return an error if the wide character is invalid */
		if (j == MAX_UTF8_OCTS_PER_CHAR)
		{
			free(*utf8Str);
			*utf8Str = NULL;
			return -3;
		}

		/* Skip over the first UTF-8 octet and encode the remaining octets
		(if any) from right-to-left.  Fill in the least significant six bits
		of each octet with the low-order bits from the wide character value */
		for (y = j; y > 0; y--)
		{
			(*utf8Str)[x + y] = (char)(0x80 | (temp_wchar & 0x3F));
			temp_wchar >>= 6;
		}

		/* Encode the first UTF-8 octet */
		(*utf8Str)[x] = gUTF8Masks[j].value;
		(*utf8Str)[x++] |= ~gUTF8Masks[j].mask & temp_wchar;

		/* Update the UTF-8 string index (skipping over the subsequent octets
		already encoded */
		x += j;
	}

	return 0;
} /* end of CvtWchar2UTF8() */





#ifdef OLD_CODE_
	/*
	 * Convert a ISO10646 stream to UTF-8 wide character
	 * Used only by UniversalStrings, BMPStrings, etc
	 */
	int CvtToUTFStr(AsnOcts *Octsinputstr, wchar_t  **wchar_ptr)
	{
	int encodelen = Octsinputstr->octetLen;
	int newlen = 0;
	int flip = 1;
	wchar_t c = 0;
	char *inputstr = (char *)Octsinputstr->octs;
	int pos = encodelen;
	wchar_t *newbuf = (wchar_t *)calloc (1, encodelen*4);
	
		memset (newbuf, 0, encodelen*4);
		while (pos > 0) {
			memcpy ((wchar_t *)&c, inputstr, sizeof (wchar_t));
		   if(*(char *)&flip == 1)
		   {
			   c = (c << 8) | (c >> 8);
		   }
	
			if (c < 0x80)
			{
				newbuf[newlen++] = c;
			}
			else if (c < 0x800)
			{
				/* Produce the 2 byte encoding */
	            newbuf[newlen++] = ((0xc0 | (c >>6))<<8) | 
									(0x80 | (c & 0x3f));
			}
			else if (c < 0x10000)
			{
				/* Produce the 3 byte encoding */
				newbuf[newlen++] = ((0xe0 | (c >>12)) <<8) |
									(0x80 | ((c >>6) & 0x3f));
				newbuf[newlen++] = (0x80 | (c & 0x3f));
	
			}
			else if (c < 0x200000)
			{
				/* Not currently supported */
				return (-1);
			}
			pos -= sizeof (wchar_t);
			inputstr += sizeof (wchar_t);
	    }
		*wchar_ptr = (wchar_t *)newbuf;
		return (newlen);
	
	}
	
	/*
	 * Converts a UTF-8 String into a ISO-10646 string
	 */
	int  CvtUTFToISO (AsnOcts *utf_stringa, wchar_t  **wchar_ptr)
	{
	int count;
	char *utf_string = (char *)utf_stringa->octs; 
	int UTF8Len = utf_stringa->octetLen;
	wchar_t *work_buffer  = (wchar_t *)calloc (1, UTF8Len*sizeof(wchar_t));
	int newlen = 0;
	unsigned short enc_sequence;
	wchar_t enc_value = 0;
	int encoded_length = 0;
	/*	memset (work_buffer, 0, (UTF8Len*sizeof(wchar_t))); */
		count = UTF8Len;
	    while (count > 0)
	    {
	                /* Analyze the UTF encodings */
	                enc_sequence = (unsigned char)(*utf_string);
	                if (enc_sequence >= FOUR_BYTE_ENCODING)
	                { 
						free (work_buffer);
						return (-1);
	                } 
	                else if (enc_sequence >= THREE_BYTE_ENCODING) 
	                { 
	                       /* three bytes encoded, 16 bits */
	                        enc_value = ((utf_string[0] & 0x3f)<<12) | 
										((utf_string[1] & 0x3f)<<6)  | 
										 (utf_string[2] & 0x3f);
	                        utf_string += 3;
	                        count -= 3;
							encoded_length = 2; 
	                }
	                else if (enc_sequence >= TWO_BYTE_ENCODING) 
	                { 
							/* two bytes encoded, 11 bits */
	                        enc_value = ((utf_string[0] & 0x3f)<<6) | 
										(utf_string[1] & 0x3f);
	
	                        utf_string += 2;
	                        count -= 2;
							encoded_length = 2;
	                } 
	                else 
	                {
	                       /* Simple - no encoding needed */
							enc_value = enc_sequence;
	                        utf_string++;
	                        count--;
							encoded_length = 1;
	                 }
	
					work_buffer[newlen] = enc_value;
					newlen ++;
	          }
	         /* NULL Terminate it */\
		
	         work_buffer[newlen] = (wchar_t)NULL;
	         /* Update the caller's pointer (may want to realloc it) */
			*wchar_ptr = (wchar_t *)work_buffer;
			
	        /* Return the length */
	         return (newlen);
	}
	/* Format char pointer to RFC 2253
	 * The flip argument specifies if you want to check for Endian-ness
	 * such as on BMPStrings they are already checked for Endian-ness
	 */
	int cvt_WCStrtoLDAP  (wchar_t *in_string, char  **char_ptr, int flip)
	{
	
		bool quoted = false;	  /* Flag telling us if we are in a quoted string */
		int hex_val = 0;	 	  /* Temporary hex value returned from sprintf */
		int quote_count = 0;	  /* Quote counter */
		int nullcount = 0;		  /* The terminating NULL Counter (related to the sizeof wchar_t) */
		int count = 0;			  /* The input byte counter */
		int to_count = 0;		  /* The destination counter */
		int buf_len = 0;
		char *wrkbf = (char *)NULL;
		wchar_t *lptr = in_string;
		for (buf_len=0; in_string[buf_len] != (wchar_t)NULL; buf_len++);
	
		wrkbf = (char *)calloc (1, buf_len*6);
		while ((wchar_t)lptr[count] != (wchar_t)NULL)
		{
			wchar_t the_string = (wchar_t)lptr[count];
			/* Make platform independent */
		   if(*(char *)&flip == 1)
		   {
			  the_string = (the_string << 8) | (the_string >> 8);
		   }
	
			if (the_string != 0)
			{
				nullcount = 0;
				if ((to_count == 0) && (the_string == '#') ||
					(the_string == ' '))
				{
					/* A # or a Space in the first position must be escaped */
					wrkbf[to_count] = '\\';
				}
				/* Check the character for Quote */
				if ((the_string == '\"') && 
					(!quoted))
				{
					memcpy ((char *)&wrkbf[to_count], (char *)"\\", 1);
					to_count += 1;
					quoted = true;
				}
				else if ((the_string == '\"') && (quoted))
				{
					/* End the Quoted */
					memcpy ((char *)&wrkbf[to_count], (char *)"\\", 1);
					to_count += 1;
					quoted = false;
				}
				/* Check non printable characters */
				if ((the_string < ' ') ||
					(the_string > 0x7E))
				{
					int len = 0;
					int hex_val = 0;  /* Temporary hex value returned from sprintf */
					char csprintf[4]; /* Sprintf Buffer */
	
					/* Check for non-ascii values SPACE and DEL */
	 				/* Escape the value */
					wrkbf[to_count] = '\\';  /* Backslash */
					to_count++;
					hex_val = the_string;
					len = sprintf (csprintf,"%2x", hex_val);
					memcpy ((char *)&wrkbf[to_count], &csprintf[0], 2);
					to_count += 2;
					if (len == 4)
					{
						/* Check for "00" */
						if (memcmp (&csprintf[2], "00", 2) != 0)
						{
							wrkbf[to_count] = '\\';  /* Backslash */
							to_count++;
							memcpy ((char *)&wrkbf[to_count], &csprintf[2], 2);
							to_count += 2;
						}
					}
				}
				else 
				{
					if (!quoted)
					{
						/*
						 * Escape the following characters if not quoted
						 */
						if ((the_string == ',') ||
							(the_string == '=') ||
							(the_string == '+') ||
							(the_string == '<') ||
							(the_string == '>') ||
							(the_string == '#') ||
							(the_string == ';'))
						{
							/* Add the escape character to the work buffer */
							wrkbf[to_count] = '\\';
							to_count ++;
						}
					}
					wrkbf[to_count] = (char)the_string;
					to_count++;
				}
			}
			else
			{
				nullcount ++;
			}
		
			count ++;
		}
			/* Check the last character for a SPACE */
		if (wrkbf[to_count-1] == ' ')
		{
			/* Must be escaped */
			wrkbf[to_count] = wrkbf[to_count-1];
			wrkbf[to_count-1] = '\\';
		}
		*char_ptr = wrkbf;
		return (to_count);
	}
	
	/* Format char pointer with RFC 2253 to a char pointer */
	int cvt_LDAPtoStr  (char *in_string, char  **char_ptr)
	{
	
		bool quoted = false;	  /* Flag telling us if we are in a quoted string */
		int hex_val = 0;	 	  /* Temporary hex value returned from sprintf */
		int quote_count = 0;	  /* Quote counter */
		char hex_str[4];		  /* Sprintf Buffer */
		char *the_string;		  /* The character to parse	*/
		char *wrkbf, *buf_ptr;	  /* Our work buffer */
		int nullcount = 0;		  /* The terminating NULL Counter (related to the sizeof wchar_t) */
		int count = 0;			  /* The input byte counter */
		int to_count = 0;		  /* The destination counter */
		int len = 0;
		int buf_len = 0;
	
		char *lptr = (char *)in_string;
		for (buf_len=0; in_string[buf_len] != (wchar_t)NULL; buf_len++);
		buf_ptr = (char *)calloc (1, buf_len*6);
		wrkbf = buf_ptr;
		the_string = lptr;
		while (nullcount < sizeof (wchar_t))
		{
			if (the_string != NULL)
			{
				if (the_string == (char *)'\\')
				{
					if ((the_string >= (char *)'0') && (the_string <= (char *)'9'))
					{
						memset (&hex_str, 0, 4);
						memcpy (hex_str, (char *)&the_string+1, 2);
						/* Escaped hex value - convert */
						hex_val = strtol (hex_str, NULL, 10);
						/* Copy then to the buffer */
						memcpy ((char *)wrkbf[to_count], (char *)&hex_val, 2);
						the_string++;
					}
					else
					{
						/* Just skip over the backslash and add the data to the buffer */
						wrkbf = (char *)&the_string+1;
						memcpy ((char *)wrkbf[to_count], (char *)the_string+1, 1);
					the_string ++;
				}
			}
			else
			{
				wrkbf = the_string;
			}
			the_string ++;
			wrkbf++;
		}
	}
    if (to_count < buf_len*6)
		realloc (wrkbf, to_count+1);
	*char_ptr = wrkbf;
	return (to_count);
}


#endif /* OLD_CODE_ */
