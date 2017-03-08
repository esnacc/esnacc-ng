/* Include Files */
#include "config.h"
#include "asn-incl.h"
#include "gen-buf.h"

AsnLen BEncUniversalStringContent(GenBuf *b, UniversalString *octs)
{
	if ((octs->octetLen % 4) != 0)
	{	
		Asn1Error ("BEncUniversalStringContent: ERROR - Invalid UniversalString Format");
		GenBufSetWriteError (b, TRUE);

	}
	return BEncAsnOctsContent(b, octs);
} /* end of BEncUniversalStringContent() */


AsnLen BEncUniversalString(GenBuf *b, UniversalString *v)
{
    AsnLen l;

    l = BEncUniversalStringContent (b, v);
    l += BEncDefLen (b, l);
    l += BEncTag1 (b, UNIV, PRIM, UNIVERSALSTRING_TAG_CODE);
    return l;
} /* end of BEncUniversalString() */


void BDecUniversalStringContent(GenBuf *b, AsnTag tagId, AsnLen len,
								UniversalString *result, AsnLen *bytesDecoded,
								ENV_TYPE env)
{
	BDecAsnOctsContent (b, tagId, len, result, bytesDecoded, env);
	if ((result->octetLen % 4) != 0)
	{
        Asn1Error ("BDecUniversalStringContent: ERROR - Invalid UniversalString Format");
        longjmp (env, -40);
    }
} /* end of BDecUniversalStringContent() */


void BDecUniversalString(GenBuf *b, UniversalString *result,
						 AsnLen *bytesDecoded, ENV_TYPE env)
{
	AsnTag tag;
	AsnLen elmtLen1;

	if (((tag = BDecTag (b, bytesDecoded, env)) != 
		MAKE_TAG_ID (UNIV, PRIM, UNIVERSALSTRING_TAG_CODE)) &&
		(tag != MAKE_TAG_ID (UNIV, CONS, UNIVERSALSTRING_TAG_CODE)))
	{
		Asn1Error ("BDecUniversalString: ERROR - wrong tag\n");
		longjmp (env, -112);
	}

    elmtLen1 = BDecLen (b, bytesDecoded, env);
    BDecUniversalStringContent (b, tag, elmtLen1, result, bytesDecoded, env);

}  /* BDecUniversalString */


/* Convert a UniversalString to a wide character string */
int CvtUniversalString2wchar(UniversalString *inOcts, wchar_t **outStr)
{
	unsigned int i, j;
	size_t wchar_size = sizeof(wchar_t);

	if ((inOcts == NULL) || (outStr == NULL))
		return -1;
	if ((inOcts->octetLen % 4) != 0)
		return -2;
	if ((wchar_size != 2) && (wchar_size < 4))
		return -3;

	*outStr = (wchar_t*)calloc(inOcts->octetLen / 4 + 1, wchar_size);
	if (*outStr == NULL)
		return -4;

  	/* Transform the UniversalString into wchar_t array */
	for (i = 0, j = 0; i < (inOcts->octetLen / 4); i++, j += 4)
	{
		if (wchar_size == 2)
		{
			/* Check that the first two bytes of the UniversalString character
			are zero */
			if ((inOcts->octs[j] != 0) || (inOcts->octs[j + 1] != 0))
			{
				free(*outStr);
				*outStr = NULL;
				return -5;
			}
		}
		else  /* (wchar_size >= 4) */
			(*outStr)[i] |= (inOcts->octs[j] << 24) | (inOcts->octs[j + 1] << 16);

		(*outStr)[i] |= (inOcts->octs[j + 2] << 8) | inOcts->octs[j + 3];
	}

	return 0;
} /* end of CvtUniversalAsnOcts2wchar() */
