#ifndef _asn_VisibleString_h_
#define _asn_VisibleString_h_

#ifdef __cplusplus
extern "C" {
#endif


typedef AsnOcts VisibleString; /* [UNIVERSAL 26] IMPLICIT OCTET STRING */

AsnLen BEncVisibleString PROTO ((GenBuf *b, VisibleString *v));
AsnLen BEncVisibleStringContent PROTO ((GenBuf *b, VisibleString *octs));

void BDecVisibleString PROTO ((GenBuf *b, VisibleString *result, AsnLen *bytesDecoded, ENV_TYPE env));
void BDecVisibleStringContent PROTO ((GenBuf *b, AsnTag tagId, AsnLen len, VisibleString *result, AsnLen *bytesDecoded, ENV_TYPE env));

#define PrintVisibleString PrintAsnOcts

#define FreeVisibleString FreeAsnOcts 

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif


