#ifndef _asn_PrintableString_h_
#define _asn_PrintableString_h_

#ifdef __cplusplus
extern "C" {
#endif


typedef AsnOcts PrintableString; /* [UNIVERSAL 19] IMPLICIT OCTET STRING */

AsnLen BEncPrintableString PROTO ((GenBuf *b, PrintableString *v));
AsnLen BEncPrintableStringContent PROTO ((GenBuf *b, PrintableString *octs));

void BDecPrintableString PROTO ((GenBuf *b, PrintableString *result, AsnLen *bytesDecoded, ENV_TYPE env));
void BDecPrintableStringContent PROTO ((GenBuf *b, AsnTag tagId, AsnLen len, PrintableString *result, AsnLen *bytesDecoded, ENV_TYPE env));

#define PrintPrintableString PrintAsnOcts

#define FreePrintableString FreeAsnOcts 

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif
