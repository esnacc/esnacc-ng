
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
