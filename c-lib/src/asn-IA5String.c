/* Include Files */
#include "config.h"
#include "asn-incl.h"


/* Function Prototypes */
static int checkIA5String(IA5String *octs);


AsnLen BEncIA5StringContent(GenBuf *b, IA5String *octs)
{
	if (checkIA5String(octs) != 0)
	{
		Asn1Error ("BEncIA5StringContent: ERROR - Invalid IA5String");
		GenBufSetWriteError (b, TRUE);


	}
	return BEncAsnOctsContent (b, octs);
} /* end of BEncIA5StringContent() */


AsnLen BEncIA5String(GenBuf *b, IA5String *v)
{
    AsnLen l;

    l = BEncIA5StringContent (b, v);
    l += BEncDefLen (b, l);
    l += BEncTag1 (b, UNIV, PRIM, IA5STRING_TAG_CODE);
    return l;
} /* end of BEncBMPString() */


void BDecIA5StringContent(GenBuf *b, AsnTag tagId, AsnLen len,
						  IA5String *result, AsnLen *bytesDecoded, 
						  ENV_TYPE env)
{
    BDecAsnOctsContent (b, tagId, len, result, bytesDecoded, env);
	if (checkIA5String(result) != 0)
    {
        Asn1Error ("BDecIA5StringContent: ERROR - Invalid IA5String");
        longjmp (env, -40);
    }
} /* end of BDecIA5StringContent() */


void BDecIA5String(GenBuf *b, IA5String *result, AsnLen *bytesDecoded, 
				   ENV_TYPE env)
{
    AsnTag tag;
    AsnLen elmtLen1;

    if (((tag = BDecTag (b, bytesDecoded, env)) != 
		MAKE_TAG_ID (UNIV, PRIM, IA5STRING_TAG_CODE)) &&
		(tag != MAKE_TAG_ID (UNIV, CONS, IA5STRING_TAG_CODE)))
    {
        Asn1Error ("BDecIA5String: ERROR - wrong tag\n");
        longjmp (env, -105);
    }
    elmtLen1 = BDecLen (b, bytesDecoded, env);
    BDecIA5StringContent (b, tag, elmtLen1, result, bytesDecoded, env);
 
} /* end of BDecIA5String() */


static int checkIA5String(IA5String *octs)
{
	unsigned int i;

	if (octs == NULL)
		return -1;

	for (i = 0; i < octs->octetLen; i++)
	{
		if ((unsigned char)octs->octs[i] > 0x7F)
			return -1;
	}

	return 0;
}
