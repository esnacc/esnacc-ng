
/* Include Files */
#include "asn-incl.h"


/* Function Prototypes */
static int chkPrintableString (PrintableString *checkBuf);


AsnLen BEncPrintableStringContent(GenBuf *b, PrintableString *octs)
{
	if (chkPrintableString (octs) < 0)
	{
        Asn1Error ("BEncPrintableStringContent: ERROR - Format Error");
		GenBufSetWriteError (b, TRUE);

	}
	return BEncAsnOctsContent(b, octs);
} /* end of BEncPrintableStringContent() */


AsnLen BEncPrintableString(GenBuf *b, PrintableString *v)
{
    AsnLen l;
    l = BEncPrintableStringContent (b, v);
    l += BEncDefLen (b, l);
    l += BEncTag1 (b, UNIV, PRIM, PRINTABLESTRING_TAG_CODE);
    return l;
} /* end of BEncPrintableString() */


void BDecPrintableStringContent(GenBuf *b, AsnTag tagId, AsnLen len,
								PrintableString *result, AsnLen *bytesDecoded,
								ENV_TYPE env)
{
	BDecAsnOctsContent (b, tagId, len, result, bytesDecoded, env);
	if (chkPrintableString (result) < 0)
	{
        Asn1Error ("BDecPrintableStringContent: ERROR - Format Error");
        longjmp (env, -40);
	}
} /* end of BDecPrintableStringContent() */


void BDecPrintableString(GenBuf *b, PrintableString *result,
						 AsnLen *bytesDecoded, ENV_TYPE env)
{
    AsnTag tag;
    AsnLen elmtLen1;

    if (((tag = BDecTag (b, bytesDecoded, env)) != 
		MAKE_TAG_ID (UNIV, PRIM, PRINTABLESTRING_TAG_CODE)) &&
		(tag != MAKE_TAG_ID (UNIV, CONS, PRINTABLESTRING_TAG_CODE)))
    {
        Asn1Error ("BDecPrintableString: ERROR - wrong tag\n");
        longjmp (env, -101);
    }
    elmtLen1 = BDecLen (b, bytesDecoded, env);
    BDecPrintableStringContent (b, tag, elmtLen1, result, bytesDecoded, env);
}  /* end of BDecPrintableString() */


static int chkPrintableString(PrintableString *checkBuf)
{
	unsigned int i;
	char temp;

	if (checkBuf == NULL)
		return -1;
	
	for (i = 0; i < checkBuf->octetLen; i++)
	{
		temp = checkBuf->octs[i];
		/* Check A-Z */
		if ((temp < 'A') || (temp > 'Z'))
		{
			/* Check for a-z */
			if ((temp < 'a') || (temp > 'z'))
			{
				/* Check for 0-9 */
				if ((temp < '0') || (temp > '9'))
				{
					switch (temp)
					{
					case ' ':		/* space */
					case '\'':		/* apostrophe */
					case '(':		/* left parenthesis */
					case ')':		/* right parenthesis */
					case '+':		/* plus sign */
					case ',':		/* comma */
					case '-':		/* hyphen */
					case '.':		/* full stop (period) */
					case '/':		/* solidus */
					case ':':		/* colon */
					case '=':		/* equal sign */
					case '?':		/* question mark */
						break;

					default:
						return -1;
					}
				}
			}
		}
	}

	return 0;
} /* end of chkPrintableString() */

