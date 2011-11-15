
/* Include Files */
#include "asn-incl.h"


/* Function Prototypes */
static int chkVisibleString (VisibleString *checkBuf);


AsnLen BEncVisibleStringContent(GenBuf *b, VisibleString *octs)
{
	if (chkVisibleString (octs) < 0)
	{
        Asn1Error ("BEncVisibleStringContent: ERROR - Format Error");
		GenBufSetWriteError (b, TRUE);

	}
	return BEncAsnOctsContent(b, octs);
} /* end of BEncVisibleStringContent() */


AsnLen BEncVisibleString(GenBuf *b, VisibleString *v)
{
    AsnLen l;
    l = BEncVisibleStringContent (b, v);
    l += BEncDefLen (b, l);
    l += BEncTag1 (b, UNIV, PRIM, VISIBLESTRING_TAG_CODE);
    return l;
} /* end of BEncVisibleString() */


void BDecVisibleStringContent(GenBuf *b, AsnTag tagId, AsnLen len,
								VisibleString *result, AsnLen *bytesDecoded,
								ENV_TYPE env)
{
	BDecAsnOctsContent (b, tagId, len, result, bytesDecoded, env);
	if (chkVisibleString (result) < 0)
	{
        Asn1Error ("BDecVisibleStringContent: ERROR - Format Error");
        longjmp (env, -40);
	}
} /* end of BDecVisibleStringContent() */


void BDecVisibleString(GenBuf *b, VisibleString *result,
						 AsnLen *bytesDecoded, ENV_TYPE env)
{
    AsnTag tag;
    AsnLen elmtLen1;

    if (((tag = BDecTag (b, bytesDecoded, env)) != 
		MAKE_TAG_ID (UNIV, PRIM, VISIBLESTRING_TAG_CODE)) &&
		(tag != MAKE_TAG_ID (UNIV, CONS, VISIBLESTRING_TAG_CODE)))
    {
        Asn1Error ("BDecVisibleString: ERROR - wrong tag\n");
        longjmp (env, -101);
    }
    elmtLen1 = BDecLen (b, bytesDecoded, env);
    BDecVisibleStringContent (b, tag, elmtLen1, result, bytesDecoded, env);
}  /* end of BDecVisibleString() */


static int chkVisibleString(VisibleString *checkBuf)
{
	unsigned int i;
	char temp;

	if (checkBuf == NULL)
		return -1;
	
	for (i = 0; i < checkBuf->octetLen; i++)
	{
		temp = checkBuf->octs[i];
		/* Check A-Z */
		
        if((unsigned int)temp > 128)
        {
            return -1;
        }
	}

	return 0;
} /* end of chkVisibleString() */

